#include <libusb-1.0/libusb.h>

#include <stdio.h>

static int ID_VENDOR  = 0x046d;
static int ID_PRODUCT = 0xc083;

static struct libusb_device_handle *devh = NULL;

void CloseDevice() 
{
    if (devh)
            libusb_close(devh);
    libusb_exit(NULL);
}

int main(int argc, char *argv[])
{
    int returnCode;
    int wIndex = 0x01;
    
    // changes only logo, has implicit values
	unsigned char data[20] = { 0x11, 0xff, 0x0e, 0x3b, 0x01, 0x01, 0xf7, 
	                               0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	                               
    // init
    returnCode = libusb_init(NULL);
    if (returnCode < 0) {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(returnCode));
        return returnCode;
    }
    
    // find device
    devh = libusb_open_device_with_vid_pid(NULL, 0x046D, 0xC083);
    if (!devh) {
        fprintf(stderr, "Error finding the g403 prodigy mouse.\n");
        CloseDevice();
        return returnCode;
    }
    
    /* detach if kernel driver is active
        &
       claim an interface on a given device handle */
    for (int interfaces = 0; interfaces < 2; interfaces++) 
    {
        if (libusb_kernel_driver_active(devh, interfaces)) 
        {
            libusb_detach_kernel_driver(devh, interfaces);
        }
        
        returnCode = libusb_claim_interface(devh, interfaces);
        if (returnCode < 0) 
        {
            fprintf(stderr, "Error claiming interface: %s\n",
                    libusb_error_name(returnCode));
            CloseDevice();
            return 1;
        }
    }
    
	
    returnCode = libusb_control_transfer(devh, 0x21, 0x09, 0x0211, wIndex, 
                                         data, sizeof(data), 0);
    
    if (returnCode < 0) {
        fprintf(stderr, "Error during control transfer: %s\n",
                libusb_error_name(returnCode));
    }
    
    // release an interface previously claimed with libusb_claim_interface()
	libusb_release_interface(devh, 0);
	
	for (int interfaces = 0; interfaces < 2; interfaces++) 
    {
        if (!libusb_kernel_driver_active(devh, interfaces)) 
        {
            libusb_attach_kernel_driver(devh, interfaces);
        }
	}
	
    return 0;
}
