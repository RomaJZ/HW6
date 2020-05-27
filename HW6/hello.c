#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");

static uint value = 1;
module_param(value, uint, S_IRUGO);
MODULE_PARM_DESC(value, "This parameter defines a number of output strings");

struct time_list {
	struct list_head header;
	ktime_t time;
};

static LIST_HEAD(time_list_head);

void list_free(struct list_head *head)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, head) {
		kfree(pos);
	}

	INIT_LIST_HEAD(head);
}

static int __init hello_init(void)
{
	BUG_ON(value > 10);

	if (!value || (value >= 5 && value <= 10))
		printk(KERN_WARNING "Warning: parameter is set to %u\n", value);

	struct time_list *next;
	uint i;

	for (i = 0; i < value; ++i) {
		next = kmalloc(sizeof(struct time_list), GFP_KERNEL);
		if (next) {

			if (value == 10) {
				next = NULL;
			}

			list_add_tail(&next->header, &time_list_head);

			next->time = ktime_get();

			printk(KERN_NOTICE "Hello, world!!!\n");
		} else {
			list_free(&time_list_head);
			return -ENOMEM;
		}
	}

	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	struct list_head *pos;

	list_for_each(pos, &time_list_head) {
		printk(KERN_NOTICE "%lld", ktime_to_ns(list_entry(pos, struct time_list, header)->time));
	}

	list_free(&time_list_head);

	printk(KERN_NOTICE "Goodbye, world!!!\n");
}
module_exit(hello_exit);
