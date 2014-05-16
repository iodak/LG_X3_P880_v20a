/*
* Author: Ivan Odak o93.ivan@gmail.com
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
#include <linux/sysfs.h>
#include <linux/otg.h>

int otg_enable = 0;

/* sysfs interface */
static ssize_t otg_enable_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", otg_enable);
}

static ssize_t otg_enable_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;

	ret = sscanf(buf, "%du", &otg_enable);

	if(ret != 1)
		return -EINVAL;

	if (otg_enable == 1)
		otg_switch(1);
	else
		otg_switch(0);

	return count;

}

static struct kobj_attribute otg_enable_attribute =
	__ATTR(otg_enable, 0666, otg_enable_show, otg_enable_store);

static struct attribute *attrs[] = {
	&otg_enable_attribute.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *otg_mode_kobj;

int otg_mode_init(void)
{
	int retval;

        otg_mode_kobj = kobject_create_and_add("otg_mode", kernel_kobj);
        if (!otg_mode_kobj) {
                return -ENOMEM;
        }
        retval = sysfs_create_group(otg_mode_kobj, &attr_group);
        if (retval)
                kobject_put(otg_mode_kobj);
        return retval;
}
/* end sysfs interface */

void otg_mode_exit(void)
{
	kobject_put(otg_mode_kobj);
}

module_init(otg_mode_init);
module_exit(otg_mode_exit);
