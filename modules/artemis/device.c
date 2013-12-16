#include <linux/kernel.h>
#include <linux/module.h>

#include "macro.h"

void LART_MOD_PRIVATE
lart_device(void)
{
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ "[FUNCTION] lart_device (void -> void)"
#endif /* PRETTY_FUNCTION */
	printk(KERN_INFO __PRETTY_FUNCTION__);
}
