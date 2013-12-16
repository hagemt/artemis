#include <linux/kernel.h>
#include <linux/module.h>

#include "macro.h"

void LART_MOD_PRIVATE
cleanup_module(void)
{
	printk(KERN_INFO "Goodbye, World!\n");
}
