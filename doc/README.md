# Documentation
This is the documentation for the 3D LED Cube.
Here is documented how to assemble the cube, the assets needed for it, how to compile the program and install it on the cube, how to use the editor, and how to use the cube.

## Assets
For the assembly, a few assets are needed, which are summarized here.

### PCB
In the `assets/PCB` folder is the KiCad files for the PCB. 
You will need to have this made/ordered.

### 3D Print
In the `assets/3D-Print` folder are the STL files for the cube exterior.
You will need to have these printed.


## Assemble
See the [`assemble.md` file](./assemble.md) for more information on how to assemble the cube.

## Main Program
The "Main Program" is the program that runs continuously on the Pi to respond to I/O input from the buttons and to display the current configuration frame by frame.

### Buttons Function
There are several buttons.
- **Sync Button**: The Sync button persistently loads files from an inserted USB stick into the internal memory.
  If files have the same name as files that are already on the cube, they will be overwritten.
  The LED next to the Sync button will blink to indicate how long the transfer will take (this should not take too long).
- **Previous-/ Next Button**: The Previous and Next button toggles between the configurations it has stored.
  Previous toggles back one configuration and Next toggles forward one configuration.
- **On/ Off Button**: Turns off the LEDs, but continues to simulate the frames.
- **Reset Button**: The Reset Button is a special button, whose functionality can only be obtained by pressing the *Sync Button* for a long time - at least 3 seconds.
Then the current configuration is deleted from the persistent storage and the next program is loaded. Only configurations uploaded by the user can be deleted, and pre-installed programs cannot be deleted.

### Compile & Install
To make the installation as easy as possible we have added a few scripts in the `cxx/install/` folder.
1. build script
2. install script

To do the installation you need a Pi with internet access because you have to compile it for the operating system and architecture.
You can also do without the Pi with the right tools, but this is relatively complex and will not be explained here.
If you still want to try to compile *without Pi* or *without internet access*, you should know that you need the `libgpiod2` dependency for the *main program* and the `libgpiod-dev` dependency *to compile*.

#### Build
But if you use the Pi, you can simply clone the repo (with the submodules) and run the `cxx/build/build.sh` script.
Make sure you have a USB stick plugged into the compile pi and confirm during the script if you want the files to be written to the stick with yes.
If your USB stick cannot be found, then it can be because it is not accessible under the `/dev/sda1` path.
In this case, you can change the path from `/dev/sda1` to your path in the script.

#### Install
If you have installed the programs from the build to the USB stick, then you can put the stick into your LED-Cube-Pi.
On the USB you should find a `cube/` folder, where you can also find an `install.sh` file.
You have to execute this file to install the program on the Pi.

The installation also includes the entry and activation as a system service, so that the program starts automatically when the Pi is started.

#### Commands
On the Compile-Pi run:
``bash
git clone --recurse-submodules git@github.com:mobergmann/led-cube.git
cd led-cube/cxx/install
# plug in a USB stick
./build.sh # watch out for I/O
            # enter your (sudo) password
            # press y to install to usb
```

Switch to the Cube-Pi, plug in the prepared USB, and then run:
``bash
mount /dev/sda1 /mnt
cd /mnt/cube
sudo sh install.sh
reboot
```

## Editor
See the [`/editor/README.md` file](../editor/README.md) for more information on how to compile and use the editor.
