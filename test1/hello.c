
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

MODULE_AUTHOR("Kazak Vadym");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int counter = 1;
module_param(counter, uint, 0644);
MODULE_PARM_DESC(counter, "Counter");

struct hello_event {
	struct list_head list;
	ktime_t timestamp;
};

static LIST_HEAD(event_list);

static int __init hello_init(void)
{
	if (counter == 0 || (counter >= 5 && counter <= 10)) {
		pr_warn("Помилка.\n");
	}

	if (counter > 10) {
		pr_err("Помилка. Лічильник не може бути більше 10\n");
		return -EINVAL;
	}

	int i;

	for (i = 0; i < counter; i++) {
		struct hello_event *event = kmalloc(sizeof(struct hello_event), GFP_KERNEL);

		if (!event) {
			return -ENOMEM;
		}
		event->timestamp = ktime_get();
		list_add(&event->list, &event_list);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_event *event, *next;

	list_for_each_entry_safe(event, next, &event_list, list) {
		ktime_t duration = ktime_sub(ktime_get(), event->timestamp);

		pr_info("Час: %lld ns\n", ktime_to_ns(duration));
		list_del(&event->list);
		kfree(event);
	}
}

module_init(hello_init);
module_exit(hello_exit);

