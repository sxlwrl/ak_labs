#ifndef HELLO1_H
#define HELLO1_H

#include <linux/list.h>
#include <linux/ktime.h>

struct hello_event {
	struct list_head list;
	ktime_t timestamp;
	ktime_t durationToConsole;
};

void print_hello(unsigned int counter);

#endif
