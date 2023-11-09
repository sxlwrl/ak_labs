
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Kazak Vadym");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(event_list);

void print_hello(unsigned int counter_1)
{
	int i;

	for (i = 0; i < counter_1; i++) {
		struct hello_event *event = kmalloc(sizeof(struct hello_event), GFP_KERNEL);
		
		if (!event) {
			pr_warn("Помилка!");
			return;
		}
		
		event->timestamp = ktime_get();
		list_add(&event->list, &event_list);
		
		pr_info("Hello, world!\n");
		
		event->durationToConsole = ktime_sub(ktime_get(), event->timestamp);
	}
}

EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("Hello1 has been loaded\n");
	
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *event, *next;

	list_for_each_entry_safe(event, next, &event_list, list) {
		ktime_t duration = ktime_sub(ktime_get(), event->timestamp);

		pr_info("Час: %lld ns\n", ktime_to_ns(duration));
		pr_info("Час друку: %lld ns\n", ktime_to_ns(event->durationToConsole));
		list_del(&event->list);
		kfree(event);
	}
	
	pr_info("Hello1 has been unluaded");
}

module_init(hello1_init);
module_exit(hello1_exit);
