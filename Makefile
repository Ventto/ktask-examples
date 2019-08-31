ifeq ($(KDIR),)
$(error KDIR variable is not set)
endif

.PHONY: all
all:
	make -C "$(KDIR)" M="$(PWD)" $(filter-out all,$(MAKECMDGOALS))

.PHONY: clean
clean:
	make -C "$(KDIR)" M="$(PWD)" clean

obj-m := my_deferred_work.o my_kthread.o
