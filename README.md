# g403-GUI-control
Control the logitech's g403 LED and DPI on linux via GUI
Sync your colours and set default DPI options.

## Dependency

Ubuntu:

```bash
sudo apt-get install libusb-1.0-dev
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
The program isn't completed yet. 
It requires admin privileges

### Completed features
- Set static LED colour for the logo
 
### Missing features

- Set custom LED colours
- Set mode either cycle, breathe or static
- Set LED for either logo or scrollwheel
- Set DPI
- Comprehensive GUI
