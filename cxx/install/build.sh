# only run as root (source: https://electrictoolbox.com/check-user-root-sudo-before-running/)
if [ `whoami` == root ]; then
    echo Please run this script as root or using sudo
    exit
fi

# declare variables
BUILD_DIR=build
BINARIES_DIR=binaries
CURRENT_DIR=$(pwd)

# install dependencies
sudo apt install build-essential cmake libgpiod-dev -y

# build
cd ..
cmake -B $BUILD_DIR
cmake --build $BUILD_DIR
cd $CURRENT_DIR/

# move binaries
mkdir -p $BINARIES_DIR
cp ../build/led_cube $BINARIES_DIR/

# download dependency library
cd $BINARIES_DIR/ || exit
apt download libgpiod2
cd $CURRENT_DIR/ || exit

# todo ask user if want to install to usb
