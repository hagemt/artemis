#include <linux/kernel.h>
#include <linux/module.h>

#include "magic.h"

int LART_MOD_PRIVATE
init_module(void)
{
	printk(KERN_INFO "Hello, World!\n");
	return 0;
}

