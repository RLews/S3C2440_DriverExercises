#include <linux/module.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>

static struct gendisk *ramblock_disk;
static request_queue_t *ramblock_queue;

static int major;

static DEFINE_SPINLOCK(ramblock_lock);

static struct block_device_operations ramblock_fops = {
	.owner = THIS_MODULE,
};

#define RAMBLOCK_SIZE			(1024*1024)
static unsigned char *ramblock_buf;

static void do_ramblock_request(request_queue_t *q)
{
	static int r_cnt = 0;
	static int w_cnt = 0;
	struct request *req;

	while ( (req = elv_next_request(q)) != NULL )
	{
		unsigned long offset = req->sector * 512;
		unsigned long len = req->current_nr_sectors * 512;

		if (rq_data_dir(req) == READ)
		{
			printk("do_ramblock_request read %d\n", ++r_cnt);
			memcpy(req->buffer, ramblock_buf+offset, len);
		}
		else
		{
			printk("do_ramblock_request write %d\n", ++w_cnt);
			memcpy(ramblock_buf+offset, req->buffer, len);
		}

		end_request(req, 1);
	}
}

static int ramblock_init(void)
{
	/* assignation gendisk struct */
	ramblock_disk = alloc_disk(16);/* 次设备号个数， 分区个数+1 */

	/* setting */
	ramblock_queue = blk_init_queue(do_ramblock_request, &ramblock_lock);
	ramblock_disk->queue = ramblock_queue;

	/* property */
	major = register_blkdev(0, "ramblock");/* cat /proc/device */
	ramblock_disk->major = major;
	ramblock_disk->first_minor = 0;
	sprintf(ramblock_disk->disk_name, "ramblock");
	ramblock_disk->fops = &ramblock_fops;
	set_capacity(ramblock_disk, RAMBLOCK_SIZE/512);

	/* hardware operating */
	ramblock_buf = kzalloc(RAMBLOCK_SIZE, GFP_KERNEL);

	/* register */
	add_disk(ramblock_disk);

	return 0;
}

static void ramblock_exit(void)
{
	unregister_blkdev(major, "ramblock");
	del_gendisk(ramblock_disk);
	put_disk(ramblock_disk);
	blk_cleanup_queue(ramblock_queue);

	kfree(ramblock_buf);
}

module_init(ramblock_init);
module_exit(ramblock_exit);

MODULE_LICENSE("GPL");









