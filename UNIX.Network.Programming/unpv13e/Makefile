SUBDIRS = advio bcast icmpd inetd intro ioctl ipopts libgai mcast mysdr names nonblock oob rtt select server sigio sockopt ssntp tcpcliserv threads udpcliserv unixdomain

.PHONY: all everything debug clean

all: everything

everything:
	@for dir in $(SUBDIRS); do \
		$(MAKE) $(T) --directory=$$dir || exit 1; \
	done

debug: T := debug
debug: all

profile: T := profile
profile: all

clean:	T := clean
clean: everything



