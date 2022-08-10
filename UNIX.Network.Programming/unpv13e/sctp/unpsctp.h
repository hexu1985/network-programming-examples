#ifndef	__unp_sctp_h
#define	__unp_sctp_h


#include "unp.h"
#include <netinet/sctp.h>

#define Sctp_sendmsg sctp_sendmsg
#define Sctp_recvmsg sctp_recvmsg

int sctp_get_no_strms(int sock_fd, struct sockaddr *to, socklen_t tolen);

sctp_assoc_t sctp_address_to_associd(int sock_fd, struct sockaddr *sa, socklen_t salen);

#endif

