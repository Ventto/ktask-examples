// SPDX-License-Identifier: GPL-2.0

/*
 * my_deferred_work.c: Kernel deferred work example
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
#include <linux/init.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Venriès <me@ventto.io>");
MODULE_DESCRIPTION("Deffered work example");

#define DEFERRED_TASK_DELAY 1000

static struct delayed_work g_work;

static void my_deferred_work_handler_fn(struct work_struct *work)
{
	pr_info("%s(): rescheduling work...\n", __func__);
	if (!schedule_delayed_work(&g_work,
				   msecs_to_jiffies(DEFERRED_TASK_DELAY))) {
		pr_warn("%s(): work is already on a queue\n", __func__);
	}
}

static int __init my_deferred_work_init(void)
{
	pr_info("%s(): scheduling work...\n", __func__);

	INIT_DELAYED_WORK(&g_work, my_deferred_work_handler_fn);

	if (!schedule_delayed_work(&g_work,
				   msecs_to_jiffies(DEFERRED_TASK_DELAY))) {
		pr_warn("%s(): work is already on a queue\n", __func__);
	}
	pr_info("%s(): work scheduled\n", __func__);
	return 0;
}

static void __exit my_deferred_work_exit(void)
{
	pr_info("%s(): flushing work from the queue\n", __func__);

	if (flush_delayed_work(&g_work)) {
		pr_info("%s(): the work queue has been flushed\n", __func__);
	}
}

module_init(my_deferred_work_init);
module_exit(my_deferred_work_exit);
