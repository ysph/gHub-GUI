#include <sys/ioctl.h>
#include <libusb-1.0/libusb.h>

#include <stdio.h>

#define CTRL_IN			(LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_OUT		(LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define USB_RQ			0x04

static int ID_VENDOR  = 0x046D;
static int ID_PRODUCT = 0xC083;

struct libusb_device_descriptor desc;
int ret;

void handle_device(libusb_device *dev)
{
    libusb_device_handle *handle = NULL;
    unsigned char red, green, blue;

    ret = libusb_get_device_descriptor(dev, &desc);
    if (ret < 0)
    {
        fprintf(stderr, "failed to get device descriptor");
        return;
    }

    unsigned char data[9] = {
        0xff,
        0x0e,
        0x3b,
        0x00,	/* wheel or logo */
        0x01,
        red,
        green,
        blue,
        0x02
    };

    ret = libusb_open(dev, &handle);

    if (LIBUSB_SUCCESS == ret)
    {
        /*r = libusb_control_transfer(dev, CTRL_IN, USB_RQ, 0xf0, 0, data,
                sizeof(data), 0);*/
    }
}

int main(int argc, char *argv[])
{
    libusb_device **devs;

    //devh = libusb_open_device_with_vid_pid(NULL, 0x05ba, 0x000a);

    ssize_t cnt;
    int responseCode, i;

    responseCode = libusb_init(NULL);
    if (responseCode < 0)
        return responseCode;

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int)cnt;

    for (i = 0; devs[i]; i++)
    {
        ret = libusb_get_device_descriptor(devs[i], &desc);
        if (ret < 0)
        {
            fprintf(stderr, "failed to get device descriptor");
            return 1;
        }

        if ((desc.idVendor == ID_VENDOR) && (desc.idProduct == ID_PRODUCT))
        {
            printf("yes\n");

            handle_device(devs[i]);
            break;
        }
    }

    //printf("Please try to connect your Logitech G403 prodigy device.\n");
    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);

    return 0;
}
