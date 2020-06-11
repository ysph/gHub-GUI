# g403-GUI-control
Control logitech g403 LEDs and DPI on Linux via GUI.
Sync your colours and set default DPI options.

## Dependency

Ubuntu:

```bash
sudo apt-get install libusb-1.0.0-dev
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

### Note
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
