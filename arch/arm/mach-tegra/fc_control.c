/*
* Author: iodak <o93.ivan@gmail.com>
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
*/

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fc_control.h>


int usr_freq;
int usr_core;

static ssize_t usr_freq_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", usr_freq);
}

static ssize_t usr_freq_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%du", &usr_freq);
	return count;
}

static struct kobj_attribute usr_freq_attribute =
	__ATTR(usr_freq, 0666, usr_freq_show, usr_freq_store);

static ssize_t usr_core_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", usr_core);
}

static ssize_t usr_core_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%du", &usr_core);
	return count;
}

static struct kobj_attribute usr_core_attribute =
	__ATTR(usr_core, 0666, usr_core_show, usr_core_store);

static struct attribute *attrs[] = {
	&usr_freq_attribute.attr,
	&usr_core_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *fc_control_kobj;

static int __init fc_control_init(void)
{
	int retval;

usr_freq = 0;
usr_core = 0;

	fc_control_kobj = kobject_create_and_add("fc_ctrl", kernel_kobj);
	if (!fc_control_kobj)
		return -ENOMEM;

	retval = sysfs_create_group(fc_control_kobj, &attr_group);
	if (retval)
		kobject_put(fc_control_kobj);

	return retval;
}

static void __exit fc_control_exit(void)
{
	kobject_put(fc_control_kobj);
}

module_init(fc_control_init);
module_exit(fc_control_exit);
