# Linux kernel module for Thrustmaster T80

> **DISCLAIMER:** This is not an official Thrustmaster driver,
> be careful and read through the code of any kernel module you run!
> I am not responsible for any damaged devices as a result of running
> this code. Use at your own discretion.
> 
> The module is mostly completed, the only thing that doesn't work is the PS button.
> This code was partly vibecoded using Claude, as I do not know C, but I reverse engineered the correct inputs.
> This driver is based on https://github.com/aregnak/hid-t80.

## Description

A linux kernel module for the Thrustmaster T80 racing wheel. 


# Installation

### Building
The Makefile simplifies this task, and using it is very simple.
```shell
# Step 1:
make
```
#### To install:
```shell
# Step 2:
# Either temporarily load:
make load

# Or permanantly install:
make install
```
#### To uninstall:
```shell
# If loaded:
make unload

# If installed:
make uninstall
```
