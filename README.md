# iot-workshop

This repo contains a number of example projects that run on an ESP32.

## Getting started

These instructions have only been tested on macOS although minimal effort will be required to get them working on Linux.

Developing for the ESP32 requires installing the Espressif IoT Development Framework (ESP-IDF) and the ESP32 toolchain.

Building and flashing the app to the chip can be done from the commandline using the build tool `make`.

**The following instructions were sourced from [official docs](https://docs.espressif.com/projects/esp-idf/en/v4.0-beta1/get-started/index.html), if you wish you may use that link to setup your environment.**



### Install Dependencies

To install the dependencies for the ESP32 toolchain run the following commands in your terminal of choice

```bash
sudo easy_install pip
pip install --user pyserial
brew install cmake ninja
```

It is strongly recommended to also install ccache for faster builds

```bash
brew install ccache
```

### Install ESP-IDF

Run the following commands in your terminal of choice

```bash
cd ~/esp
git clone -b v4.0-beta1 --recursive https://github.com/espressif/esp-idf.git
```

ESP-IDF will be downloaded into `~/esp/esp-idf`.

### Set up Tools

Run the following commands in your terminal of choice

```bash
cd ~/esp/esp-idf
./install.sh
```

### Set up environment variables

#### Env vars the lazy way

The `hello-world` directory in this repo contains an `.envrc` file which configures all environment variables via [direnv](https://direnv.net).

**NOTE:** You may need to update the `ESPPORT` env var in the `.envrc` file but at the time of writing this document the variable is correct for macOS. See the [official docs](https://docs.espressif.com/projects/esp-idf/en/v4.0-beta1/get-started/index.html#get-started-connect) for the serial port patterns for other Operating Systems.

#### Env vars without direnv

The previous installation step does not include adding/changing any environment variables. The ESP-IDF folder contains an `export.sh` script to perform these changes, however these are only a one off and the docs recommend adding the following command to your `.bash_profile`, `.profile`, `.zshrc`, etc. Run the `export.sh` file with the following command (_Note the space between the leading dot and the path!_)

```bash
. ~/esp/esp-idf/export.sh
```

**NOTE:** You will also need to set the `ESPPORT` env var to match the board's serial port, as the default used by the tools is the Linux port. At the time of writing this document the port on macOS is `/dev/cu.SLAB_USBtoUART`. See the [official docs](https://docs.espressif.com/projects/esp-idf/en/v4.0-beta1/get-started/index.html#get-started-connect) for the serial port patterns for other Operating Systems.

### Test installation

Run the following commands in the `hello-world` directory

```bash
make all
make flash
make monitor
```

You should see a slab of green and yellow output followed by

```
Hello world!
This is ESP32 chip with 2 CPU cores, WiFi/BT/BLE, silicon revision 1, 4MB embedded flash
Restarting in 10 seconds...
Restarting in 9 seconds...
Restarting in 8 seconds...
Restarting in 7 seconds...
Restarting in 6 seconds...
Restarting in 5 seconds...
Restarting in 4 seconds...
Restarting in 3 seconds...
Restarting in 2 seconds...
Restarting in 1 seconds...
Restarting in 0 seconds...
Restarting now.
```

To exit `make monitor` press `⌃]`