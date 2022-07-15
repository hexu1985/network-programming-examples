#ifndef __unp_unp_in_pktinfo_h 
#define __unp_unp_in_pktinfo_h 

#include <netinet/in.h>

/* The structure returned by recvfrom_flags() */
struct unp_in_pktinfo {
  struct in_addr	ipi_addr;	/* dst IPv4 address */
  int				ipi_ifindex;/* received interface index */
};

#endif
