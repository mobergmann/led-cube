# only run as root (source: https://electrictoolbox.com/check-user-root-sudo-before-running/)
if [ `whoami` == root ]; then
    echo Please don\'t run this script as root or using sudo
    exit
fi

# region declare variables
START_DIR=$(pwd)

# get path of script (if called from other location)
# source: https://stackoverflow.com/a/1638397/11186407
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

BUILD_DIR="$SCRIPTPATH/../build"

BINARIES_DIR="$SCRIPTPATH/binaries"
# endregion

# install dependencies
sudo apt install build-essential cmake libgpiod-dev -y
# todo if on raspi/ apt available continue

# build
echo "DEBUG: $BUILD_DIR"
cd "$SCRIPTPATH"/.. || exit
cmake -B "$BUILD_DIR/"
cmake --build "$BUILD_DIR/"

# move binaries
mkdir -p "$BINARIES_DIR"
cp "$BUILD_DIR"/led_cube "$BINARIES_DIR"/

# download dependency library
cd "$BINARIES_DIR"/ || exit
apt download libgpiod2
cd "$START_DIR"/ || exit


# todo ask user if want to install to usb
