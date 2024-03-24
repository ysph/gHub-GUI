# gHub-GUI
Manage your logitech device on Linux using GUI.
The program is lightweight and user-friendly.

## Dependencies

Ubuntu:

```bash
sudo apt-get install libusb-1.0-0-dev libx11-dev libgl-dev
```

Fedora (38):

```bash
sudo dnf install libusb1-devel libX11-devel mesa-libGL-devel
```

## Compile

```bash
make all
```

## Usage

Run as root:
```bash
sudo ./ghub-gui
```

## Cleaning up

```bash
make clean
```

## TroubleShooting

### The mouse stopped working after execution
The problem is that the OS has failed to attach driver (see "can't add hid device" in dmesg)  
To solve that, either try to execute program again or switch the port.  
It may also help in future if you add udev rules listed in the case below.

### Can I run the program without root privileges?

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
Reboot may also be needed.  

Links regarding udev rules:  
[udev homepage](http://www.kernel.org/pub/linux/utils/kernel/hotplug/udev/udev.html)  
[Debian's udev overview](http://wiki.debian.org/udev)  
[Arch's udev overview](https://wiki.archlinux.org/index.php/udev)  
[Writing udev rules](http://www.reactivated.net/udevrules.php)  
[Ask about udev on the forum](http://vger.kernel.org/vger-lists.html#linux-hotplug)  

### My device is UNIDENTIFIED, what could that be?
Some logitech devices use the same id for bluetooth receiver.  
However, the Linux kernel since version of 5.2 may recognize IDs properly.  
Possible solution for debian-based distributions would be:  

```bash
sudo apt-get upgrade
```

## Miscellaneous
The program is not complete yet.
It also requires root privileges.

### Completed features
- Set the static LED colour for the primary or logo

### Missing features
- Set custom LED colours
- Set the cycle or breathing
- Set the colour for logo or primary
- Set DPI
- Set report rate
- Battery level report
- Comprehensive GUI
