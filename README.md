# g403-GUI-control
Control logitech g403 LEDs and DPI on Linux via GUI.  
Sync your colours and set default DPI options.

## Dependency

Ubuntu:

```bash
sudo apt-get install libusb-1.0-0-dev
```

## Compile

```bash
gcc main.c -o g403hub -lusb-1.0
```

## Usage

Run as root:
```bash
sudo ./g403hub
```

## TroubleShooting

### Mouse stopped working after execution
The problem is that the OS has failed to attach driver (see "can't add hid device" in dmesg)  
To solve that, either try to execute program again or switch the port.

### Can I run the program without root privilege?

create a rule in /etc/udev/rules.d/:

```bash
printf 'KERNEL=="hidraw*", SUBSYSTEM=="hidraw", MODE="0664", GROUP="plugdev"\nSUBSYSTEM=="usb", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c083", GROUP="ubuntu", MODE="066"\nSUBSYSTEM=="usb_DEVICE", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c083", GROUP="ubuntu", MODE="066"' | sudo tee /etc/udev/rules.d/99-hidraw-permission.rules
```

If rules fail to reload automatically:

```bash
sudo udevadm control --reload
```

To manually force udev to trigger your rules:

```bash
sudo udevadm trigger
```
Reboot also may be needed.  

Links regarding udev rules:  
[udev homepage](http://www.kernel.org/pub/linux/utils/kernel/hotplug/udev/udev.html)  
[Debian's udev overview](http://wiki.debian.org/udev)  
[Arch's udev overview](https://wiki.archlinux.org/index.php/udev)  
[Writing udev rules](http://www.reactivated.net/udevrules.php)  
[Ask about udev on the forum](http://vger.kernel.org/vger-lists.html#linux-hotplug)  

## Miscellaneous
The program is not complete yet.
It also requires admin privileges

### Completed features
- Set the static LED colour for the logo
 
### Missing features

- Set custom LED colours
- Set the cycle, breathing, or static mode
- Set the LED for the logo or scroll wheel
- Set DPI
- Comprehensive GUI
