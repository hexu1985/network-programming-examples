#include "unpsctp.h"

int 
sctp_get_no_strms(int sock_fd,struct sockaddr *to, socklen_t tolen, struct sctp_sndrcvinfo *sri)
{
	int retsz;
	struct sctp_status status;
	retsz = sizeof(status);	
	bzero(&status,sizeof(status));

    status.sstat_assoc_id = sri->sinfo_assoc_id;
    Getsockopt(sock_fd,IPPROTO_SCTP, SCTP_STATUS,&status, &retsz);
	return(status.sstat_outstrms);
}
