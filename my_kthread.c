// SPDX-License-Identifier: GPL-2.0

/*
 * my_kthread.c: Kernel thread example
 *
 * (C) Copyright 2019 Thomas Venriès
 * Author: Thomas Venriès <me@ventto.io>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/module.h>
#include <linux/kernel.h>       // do_exit()
#include <linux/sched/signal.h> // signal_pending()
#include <linux/signal.h>       // allow_signal()
#include <linux/kthread.h>      // kthread_create()
#include <linux/delay.h>        // ssleep()

#define WORKER_THREAD_DELAY 4

static struct task_struct *worker_task;
static int get_current_cpu;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Venriès <me@ventto.io>");
MODULE_DESCRIPTION("Kernel thread example");

static int my_kthread_handler_fn(void *arguments)
{
	pr_info("%s(): begin\n", __func__);
	allow_signal(SIGKILL);

	while (!kthread_should_stop()) {
		pr_info("%s(): executed on system CPU:%d\n", __func__,
			get_cpu());
		ssleep(WORKER_THREAD_DELAY);

		if (signal_pending(worker_task))
			break;
	}
	do_exit(0);

	pr_info("%s(): end\n", __func__);
	return 0;
}

static int __init my_kthread_init(void)
{
	get_current_cpu = get_cpu();
	pr_info("%s(): Getting current CPU %d to binding worker thread\n",
		__func__,
		get_current_cpu);

	pr_info("%s(): creating thread...\n", __func__);
	worker_task = kthread_create(my_kthread_handler_fn,
				     (void *)"arguments as char pointer",
				     "thread_name");

	if (!worker_task)
		pr_err("%s(): failed to create kthread\n", __func__);

	kthread_bind(worker_task, get_current_cpu);

	wake_up_process(worker_task);

	if (!worker_task)
		pr_err("%s(): worker task can't start\n", __func__);


	return 0;
}

static void __exit my_kthread_exit(void)
{
	pr_info("%s(): stopping kthread...\n", __func__);
	if (worker_task)
		kthread_stop(worker_task);

	pr_info("%s(): kthread stopped\n", __func__);
}

module_init(my_kthread_init);
module_exit(my_kthread_exit);
