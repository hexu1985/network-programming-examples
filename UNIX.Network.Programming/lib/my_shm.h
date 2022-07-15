#ifndef __unp_my_shm_h
#define __unp_my_shm_h

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "error.h"

/* Devices */                                                                                                                                                           
#define HAVE_DEV_ZERO 1 

void *my_shm(size_t);
void *My_shm(size_t);

#endif

