



#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

#define KEY_BOARD_DEBUG




static struct input_dev *uk_dev;
static char *usb_buf;
static dma_addr_t usb_buf_phys;
static int len;
static struct urb *uk_urb;

static struct usb_device_id usb_keyboard_id_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }						/* Terminating entry */
};

static void usb_keyboard_irq(struct urb *urb)
{
#ifdef KEY_BOARD_DEBUG
	static int cnt = 0;
	int i;

	printk("data cnt %d: ", ++cnt);
	for (i = 0; i < len; i++)
	{
		printk("%02x ", usb_buf[i]);
	}
	printk("\n");
#endif

	/* Resubmit	urb */
	usb_submit_urb(uk_urb, GFP_KERNEL);
}


static int usb_keyboard_probe(struct usb_interface * intf, const struct usb_device_id * id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor * endpoint;
	int pipe;

	interface = intf->cur_altsetting;
	endpoint = &interface->endpoint[0].desc;

	/* assignation input_dev */
	uk_dev = input_allocate_device();

	/* set */
	set_bit(EV_KEY, uk_dev->evbit);
	set_bit(EV_REP, uk_dev->evbit);

	set_bit(KEY_L, uk_dev->keybit);
	set_bit(KEY_S, uk_dev->keybit);
	set_bit(KEY_ENTER, uk_dev->keybit);

	/* register */
	input_register_device(uk_dev);

	/* hardware operating */
	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	len = endpoint->wMaxPacketSize;
	usb_buf = usb_buffer_alloc(dev, len, GFP_ATOMIC, &usb_buf_phys);
	uk_urb = usb_alloc_urb(0, GFP_KERNEL);
	usb_fill_int_urb(uk_urb, dev, pipe, usb_buf, len, usb_keyboard_irq, NULL, endpoint->bInterval);
	uk_urb->transfer_dma = usb_buf_phys;
	uk_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	usb_submit_urb(uk_urb, GFP_KERNEL);

	return 0;
	
}


static void usb_keyboard_disconnect(struct usb_interface * intf)
{
	struct usb_device *dev = interface_to_usbdev(intf);

	usb_kill_urb(uk_urb);
	usb_free_urb(uk_urb);

	usb_buffer_free(dev, len, usb_buf, usb_buf_phys);
	input_unregister_device(uk_dev);
	input_free_device(uk_dev);
}

static struct usb_driver usb_keyboard_driver = {
	.name =		"usb_keyboard_",
	.probe =	usb_keyboard_probe,
	.disconnect =	usb_keyboard_disconnect,
	.id_table =	usb_keyboard_id_table,
};

static int usb_keyboard_init(void)
{
	usb_register(&usb_keyboard_driver);
	return 0;
}

static void usb_keyboard_exit(void)
{
	usb_deregister(&usb_keyboard_driver);
}

module_init(usb_keyboard_init);
module_exit(usb_keyboard_exit);


MODULE_LICENSE("GPL");


