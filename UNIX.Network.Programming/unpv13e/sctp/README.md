### linux主机安装sctp协议栈

1、检查主机是否支持sctp

```
$ lsmod | grep sctp
```

如果输出什么都没有，表示当前主机不支持sctp

2、安装sctp

ubuntu下, 执行:

```
$ sudo apt install libsctp-dev lksctp-tools
```

3、检查sctp是否安装成功

```
$ lsmod |grep sctp
```

如果依然什么都没有，那就执行：`modprobe sctp`  这一步很重要（实际操作之后发现的）

```
$ modprobe sctp
```
然后再检查.

