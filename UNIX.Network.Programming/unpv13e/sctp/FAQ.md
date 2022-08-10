getsockopt error: Invalid argument
跟TCP和UDP的套路一样，我们先在一个终端运行server，再在另一个终端运行client，按照UNP的内容进行调试：

```
//client shell
./sctpclient01 127.0.0.1
//block here
[0]hello
```

在客户端终端没有回射回来任何内容，反而在运行服务器的终端打印出消息：

```
getsockopt error: Invalid argument
```
服务器进程异常退出。

解决办法
参考(StackOverFlow)[https://stackoverflow.com/questions/23897781/getsockopt-invalid-argument-for-ipproto-sctp-sctp-status]，按照里面的描述，需要对源代码进行修改，需要修改的两个文件是sctpserv01.c和sctp_getnostrm.c。依然以diff -u的形式给出。

1.修改sctpserv01.c

```
--- sctpserv01_origin.c 2016-06-22 16:09:43.750557685 +0800
+++ sctpserv01.c    2016-06-22 11:42:19.254845011 +0800
@@ -35,7 +35,7 @@
                 &sri,&msg_flags);
        if(stream_increment) {
            sri.sinfo_stream++;
-           if(sri.sinfo_stream >= sctp_get_no_strms(sock_fd,(SA *)&cliaddr, len)) 
+           if(sri.sinfo_stream >= sctp_get_no_strms(sock_fd,(SA *)&cliaddr, len,sri)) 
                sri.sinfo_stream = 0;
        }
        Sctp_sendmsg(sock_fd, readbuf, rd_sz, 
```        

2.修改sctp_getnostrm.c

```
--- sctp_getnostrm_origin.c 2016-06-22 16:13:38.654553478 +0800
+++ sctp_getnostrm.c    2016-06-22 11:45:34.070841522 +0800
@@ -1,15 +1,14 @@
 #include   "unp.h"

 int 
-sctp_get_no_strms(int sock_fd,struct sockaddr *to, socklen_t tolen)
+sctp_get_no_strms(int sock_fd,struct sockaddr *to, socklen_t tolen, struct sctp_sndrcvinfo sri)
 {
    int retsz;
    struct sctp_status status;
    retsz = sizeof(status); 
    bzero(&status,sizeof(status));

-   status.sstat_assoc_id = sctp_address_to_associd(sock_fd,to,tolen);
-   Getsockopt(sock_fd,IPPROTO_SCTP, SCTP_STATUS,
-          &status, &retsz);
+   status.sstat_assoc_id = sri.sinfo_assoc_id;
+   Getsockopt(sock_fd,IPPROTO_SCTP, SCTP_STATUS,&status, &retsz);
    return(status.sstat_outstrms);
 }
```

重新编译并运行sctpserv01和sctpclient01，工作正常。
