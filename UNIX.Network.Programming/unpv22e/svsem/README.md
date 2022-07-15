#### 查看系统V的IPC
```
ipcs -s
```

#### 创建信号量

```
# semcreate -e /xxx/xxx/xxx 4
# /xxx/xxx/xxx是系统中存在的某个路径
# 4是信号量初始化个数
semcreate -e ${PWD} 1
```

#### 删除信号量

```
# semrmid /xxx/xxx/xxx
# semcreate时传入的路径
semrmid ${PWD}
```

#### 设置信号量的值

```
# semsetvalue /xxx/xxx/xxx 2 3 4 5
# semcreate时传入的路径
# 2 3 4 5是信号量的值，跟信号量的个数得匹配
semsetvalue ${PWD} 5
```

#### 获取信号量的值

```
# semgetvalue /xxx/xxx/xxx
# semcreate时传入的路径
semgetvalue ${PWD}
```

#### 操作信号量

- 信号量wait

```
# semops -- /xxx/xxx/xxx -1 -1 -1 -1
# semcreate时传入的路径
# 每个信号量的值都减1，跟信号量的个数得匹配
semsetvalue -- ${PWD} -1
```

- 信号量post

```
# semops -- /xxx/xxx/xxx 1 1 1 1
# semcreate时传入的路径
# 每个信号量的值都加1，跟信号量的个数得匹配
semsetvalue -- ${PWD} 1
```

