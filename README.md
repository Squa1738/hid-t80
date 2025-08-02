# Linux kernel module for Thrustmaster T80

> **DISCLAIMER:** This is not an official Thrustmaster driver,
> be careful and read through the code of any kernel module you run!
> I am not responsable for any damaged devices as a result of running
> this code. Use at your own discretion.
> 
> The module is mostly completed, the only thing that doesn't work is the PS button.

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

### Steam Settings
To use on steam, make sure to `disable steam input` for correct inputs. 

```
Right-click the game -> Properties -> Controller -> Override for GAME (Disable steam input)
```
