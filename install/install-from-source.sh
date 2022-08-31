# declare variables
CONFIG_DIR=/usr/share/led-cube
DEFAULT_DIR=/var/lib/led-cube
INSTALL_DIR=/usr/local/led-cube
ASSET_DIR=../assets/files
BUILD_DIR=cxx/build
SERVICE_FILE=/etc/systemd/system/led-cube.service


# only run as root (source: https://electrictoolbox.com/check-user-root-sudo-before-running/)
if [ `whoami` != root ]; then
    echo Please run this script as root or using sudo
    exit
fi


# install dependencies
sudo apt install build-essential cmake libgpiod-dev -y

# clone repo
git clone --recurse-submodules git@github.com:mobergmann/led-cube.git
cd led-cube || exit


# move cube configuration-files
mkdir -p $CONFIG_DIR
mkdir -p $DEFAULT_DIR
cp $ASSET_DIR/* $DEFAULT_DIR
chmod --recursive 444 $DEFAULT_DIR


# build
cmake -B ./build
cmake --build ./build


# move binaries
mkdir -p $INSTALL_DIR
cp $BUILD_DIR $INSTALL_DIR

# create systemd service
cat led-cube.service > $SERVICE_FILE
systemctl --now enable led-cube.service
