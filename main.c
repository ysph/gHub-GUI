#include <libusb-1.0/libusb.h>
#include <stdio.h>

#define LIBUSB_OPTION_LOG_LEVEL	0
#define LIBUSB_LOG_LEVEL_ERROR	1

#define ID_VENDOR				0x046d
#define ID_PRODUCT				0xc083
#define ID_PRODUCT_BLUETOOTH	0xc539
static libusb_device_handle *devh = NULL;
libusb_context *global_context;

int returnCode;

void CloseDevice(void) {
	if (devh)
		libusb_close(devh);
	libusb_exit(NULL);
}

void DetachKernel(void) {
	int control = 1; // 0 - data
	
	if (libusb_kernel_driver_active(devh, control)) {
		libusb_detach_kernel_driver(devh, control);
	}

	returnCode = libusb_claim_interface(devh, control);
	
	if (returnCode < 0) {
		fprintf(stderr, "Error claiming interface: %s\n",
		libusb_error_name(returnCode));
		
		CloseDevice();
		return;
	}
}

void AttachKernel(void) {
	int control = 1; 
	libusb_release_interface(devh, control);

	if (!libusb_kernel_driver_active(devh, control)) {
		libusb_attach_kernel_driver(devh, control);
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
			if ((desc.idVendor == ID_VENDOR) && (desc.idProduct == ID_PRODUCT) || (desc.idProduct == ID_PRODUCT_BLUETOOTH)) {
			
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
	R = 0x00;
	G = 0x77;
	B = 0xff;

	// only static change is implemented
	unsigned char data[20] = {0x11, 0xff, 0x0e, 0x3b, source, type, R, G, B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	// init
	returnCode = libusb_init(NULL);	


	#if defined(LIBUSB_API_VERSION) && (LIBUSB_API_VERSION >= 0x01000106) // >=1.0.22
		libusb_set_option(global_context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_ERROR);
	#else
		libusb_set_debug(global_context, LIBUSB_LOG_LEVEL_ERROR);
	#endif

	if (returnCode < 0) {
		fprintf(stderr, "Error during libusb initializing: %s\n", libusb_error_name(returnCode));

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

	returnCode = libusb_control_transfer(devh, bmRequestType, bRequest, wValue, 
										 wIndex, data, sizeof(data), 2000);

	if (returnCode < 0) {
		fprintf(stderr, "Error during control transfer: %s\n",
		libusb_error_name(returnCode));
	}

    /* release the interface previously claimed
     	&
       attach kernel */
	AttachKernel();
	CloseDevice();
	 
	printf("Now, the color of your logo is #%02x%02x%02x\n",R,G,B);

	return 0;
}
