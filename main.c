#include <libusb-1.0/libusb.h>

#include <stdio.h>

static int ID_VENDOR  = 0x046d;
static int ID_PRODUCT = 0xc083;

static libusb_device_handle *devh = NULL;
libusb_context *global_context;

int returnCode;

void CloseDevice(void) {
    if (devh)
		libusb_close(devh);
    libusb_exit(NULL);
}

void DetachKernel(void) {
    for (int interface = 0; interface < 2; interface++) {
        if (libusb_kernel_driver_active(devh, interface)) {
            libusb_detach_kernel_driver(devh, interface);
        }
        
        returnCode = libusb_claim_interface(devh, interface);
        
        if (returnCode < 0) {
            fprintf(stderr, "Error claiming interface: %s\n",
                    libusb_error_name(returnCode));
            CloseDevice();
            return;
        }
    }
}

void AttachKernel(void) {
    for (int interface = 0; interface < 2; interface++) {
    	libusb_release_interface(devh, interface);
    	
        if (!libusb_kernel_driver_active(devh, interface)) {
            libusb_attach_kernel_driver(devh, interface);
        }
	}
}

int getDevice(void) {
	libusb_device **list;
	struct libusb_device_descriptor desc;
	
	int i;
	int found = 0;
	
	ssize_t count = libusb_get_device_list(global_context, &list);
	
	for (i = 0; i < count; i++) {
		libusb_device *device = list[i];
		
		if (!libusb_get_device_descriptor(device, &desc)) {
			if ((desc.idVendor == ID_VENDOR) && (desc.idProduct == ID_PRODUCT)) {
			
				returnCode = libusb_open(device, &devh);
				if (returnCode < 0) {
					fprintf(stderr, "Error opening device: %s\n", libusb_error_name(returnCode));
				}
				
				found = 1;
				break;
			}
		}
	}
	
	if (!found) {
	 	return found;
	}
	
	libusb_free_device_list(list, 1);
	
	return 1;
}

int main(int argc, char *argv[]) {
	int bmRequestType = 0x21; // type and recipient: 0b00100001
	int bRequest = 0x09; // type of request: set_report
    int wValue = 0x0211; // report type and id: vendor, 0x11 
    int wIndex = 0x01; // report type: input
    
    int source, type, R, G, B;

	source = 0x01; // 0 - scrollwheel, 1 - logo
	type = 0x01; // static
    R = 0xff;
    G = 0xff;
    B = 0x00;
    
    // only static change is implemented
	unsigned char data[20] = { 0x11, 0xff, 0x0e, 0x3b, source, type, R, G, B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	                                                          
    // init
    returnCode = libusb_init(NULL);
    
    libusb_set_option(global_context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_ERROR);
    
    if (returnCode < 0) {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(returnCode));
        
        return returnCode;
    }
    
    // find device 
    devh = libusb_open_device_with_vid_pid(NULL, ID_VENDOR, ID_PRODUCT);
    
    if (!devh) {
    	returnCode = getDevice();
    	
    	if (!returnCode) {
        	fprintf(stderr, "Error: Cannot find the g403 prodigy mouse.\n");
        	CloseDevice();
        	
        	return returnCode;
    	}
    }
    
    /* detach kernel
        &
       claim an interface on a given device handle */
    DetachKernel();
                                         
    returnCode = libusb_control_transfer(devh, bmRequestType, bRequest, wValue, wIndex, 
                                         data, sizeof(data), 2000);
    if (returnCode < 0) {
        fprintf(stderr, "Error during control transfer: %s\n",
		libusb_error_name(returnCode));
    }
    
    /* release the interface previously claimed
     	&
       attach kernel */
	AttachKernel();
	
    return 0;
}
