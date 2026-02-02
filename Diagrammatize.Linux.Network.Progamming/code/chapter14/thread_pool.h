#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

struct list_head {
    struct list_head *prev, *next;
};

static inline void list_head_init(struct list_head *head) {
    head->prev = head;
    head->next = head;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
    struct list_head *next = head->next;
    head->next = new;
    new->prev = head;
    new->next = next;
    next->prev = new;
}

static inline struct list_head* list_del(struct list_head *head) {
    if (head->prev == head) return NULL;
    struct list_head *prev = head->prev;
    prev->prev->next = head;
    head->prev = prev->prev;
    return prev;
}

static inline bool is_empty(struct list_head *head) {
    return head->next == head;
}

typedef void (*func)(void *arg);
struct task {
    struct list_head list;
    func handler;
    void *arg;
};

static inline struct task* task_new(void) {
    return (struct task *)malloc(sizeof(struct task));
}

static inline void task_free(void *p) {
    free(p);
}

struct thread_pool {
    struct list_head head;
    int pool_size;
    int task_num;
    pthread_t *th;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool shutdown;
};

static inline void *work(void *arg);
static inline void thread_pool_init(struct thread_pool *pool, int pool_size) {
    list_head_init(&pool->head);
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->pool_size = pool_size;
    pool->th = (pthread_t *)malloc(pool_size * sizeof(pthread_t));
    for (int i = 0; i < pool->pool_size; i++) {
        pthread_create(&pool->th[i], NULL, work, (void *)pool); 
    }
    pool->shutdown = false;
}

static inline void thread_pool_destroy(struct thread_pool *pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    for (int i = 0; i < pool->task_num; i++) {
        pthread_join(pool->th[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}

static inline void thread_pool_add(struct thread_pool *pool, struct task *t) {
    pthread_mutex_lock(&pool->mutex);
    list_add(&t->list, &pool->head);
    pool->task_num++;
    if (pool->task_num % 1000 == 0) 
        printf("task num:%d\n", pool->task_num); 
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
}

static inline struct task *thread_pool_get(struct thread_pool *pool) {
    pthread_mutex_lock(&pool->mutex);
    while(is_empty(&pool->head) && !pool->shutdown) {
        struct timeval now;
        gettimeofday(&now, NULL);
        struct timespec outtime;
        outtime.tv_sec = now.tv_sec + 1;
        outtime.tv_nsec = now.tv_usec * 1000;
        pthread_cond_timedwait(&pool->cond, &pool->mutex, &outtime);
    }
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->mutex);
        return NULL;
    }

    struct task *t = (struct task *)list_del(&pool->head);
    pool->task_num--;
    pthread_mutex_unlock(&pool->mutex);
    return t;
}

static inline void *work(void *arg) {
    struct thread_pool *pool = (struct thread_pool *)arg;
    while(!pool->shutdown) {
        struct task *t = thread_pool_get(pool);
        if (!t) {
            continue;
        }
        //printf("work thread get task\n");
        t->handler(t->arg);
        task_free(t);
    }
    return NULL;
}

#endif
