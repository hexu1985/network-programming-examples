
SUBDIRS = bench fifocliserv lock mutex my_pxmsg_mmap my_pxsem_fifo my_pxsem_mmap my_pxsem_svsem my_rwlock pipe pxmsg pxsem pxshm rwlock shm svipc svmsg svmsgcliserv svmsgmpx1q svmsgmpxnq svsem svshm

all:
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

clean:
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

distclean:
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

common:
	@for dir in $(SUBDIRS); do \
		echo "Making $(WHAT_TO_MAKE) in $$dir..."; \
		(cd $$dir && $(MAKE) $(MFLAGS) $(WHAT_TO_MAKE)) || exit 1; \
	done
