ifneq ($(KERNELRELEASE),)
include Kbuild
else

KDIR ?= /lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KDIR) M=$$PWD

.PHONY: all

%.o: %.c
	@echo "CC $< --> $@ ..."
	@$(CC) $(CFLAGS) "$<" -o "$@"

endif # kbuild
