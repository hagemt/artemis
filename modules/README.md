Building Artemis as a Kernel Module
===================================

First, make sure you do not have $KERNELRELEASE set. (kbuild will do that)

Next, know you will be expected to have:

* Module root: /lib/modules/`uname -r`/build (for KDIR)
* Kernel headers: /usr/src/linux/include (for Makefile)

To build, check Makefile and Kbuild are correct, then:

> make # all this does is $(MAKE) -C $(KDIR) M=$$PWD

To install (to /usr/local/lib/modules/$KERNELRELEASE/artemis) after building:

> export INSTALL_MOD_PATH=/usr/local
> export INSTALL_MOD_DIR=artemis
> sudo make modules_install

You will then have to insmod, etc.
You probably know what you're doing.
If not, read Linux's Documentation.
