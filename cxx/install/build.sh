# only run as root (source: https://electrictoolbox.com/check-user-root-sudo-before-running/)
if [ `whoami` == root ]; then
    echo Please run this script as root or using sudo
    exit
fi

# todo exit if not on raspi

# declare variables
START_DIR=$(pwd)
# get path of script (if called from other location)
# source: https://stackoverflow.com/a/1638397/11186407
SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname "$SCRIPT")
BUILD_DIR="$SCRIPT_DIR/../build"
BINARIES_DIR="$SCRIPT_DIR/bin"


# install dependencies
sudo apt install build-essential cmake libgpiod-dev -y

# build
echo -n "Building Project ... "
{
    cd "$SCRIPT_DIR"/.. || exit
    cmake -B "$BUILD_DIR/"
    cmake --build "$BUILD_DIR/"
} &> /dev/null
echo "Done"

# move binaries
echo -n "Moving binaries ... "
{
    mkdir -p "$BINARIES_DIR"
    cp "$BUILD_DIR/led_cube" "$BINARIES_DIR/"
} &> /dev/null
echo "Done"

# download dependency library
echo -n "Downloading dependency ... "
{
    cd "$BINARIES_DIR"/ || exit
    apt download libgpiod2
    # wget http://ftp.de.debian.org/debian/pool/main/libg/libgpiod/libgpiod2_1.6.2-1_armel.deb
} &> /dev/null
echo "Done"


# ask user if also want to install to usb
# source: https://stackoverflow.com/a/27875395/11186407
echo "Do you want to create a install stick (please make sure you have an USB inserted as /dev/sda1)? [Y|n]"
old_stty_cfg=$(stty -g)
stty raw -echo ; answer=$(head -c 1) ; stty $old_stty_cfg # Careful playing with stty
if echo "$answer" | grep -iq "^y" ; then
    echo "Yes"
else
    echo "No"
    cd "$START_DIR"/ || exit
    exit
fi



# declare variables
MOUNT_DIR=/mnt
USB_DIR="$MOUNT_DIR/cube"


echo -n "Moving files to usb ... "

# mount and give access rights
sudo mount /dev/sda1 "$MOUNT_DIR/"
sudo mkdir -p "$MOUNT_DIR"

# move configurations
sudo cp -r "$SCRIPT_DIR/configurations" "$USB_DIR/"

# move assets
sudo cp -r "$SCRIPT_DIR/assets" "$USB_DIR/"
sudo mv "$USB_DIR/assets/install.sh" "$USB_DIR/install.sh"

# move binaries
sudo cp -r "$SCRIPT_DIR/bin" "$USB_DIR/"


sudo umount "$MOUNT_DIR/"
echo "Done"

cd "$START_DIR"/ || exit

echo "Finished creating install USB. Stick the USB into the Raspi and then run the install script."
