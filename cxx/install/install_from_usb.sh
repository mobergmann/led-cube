# ensure running as root (source: https://electrictoolbox.com/check-user-root-sudo-before-running/)
if [ `whoami` != root ]; then
    echo Please run this script as root or using sudo
    exit
fi

# declare variables
BINARY_DIR=binaries
CONFIG_DIR=/usr/share/led-cube
DEFAULT_DIR=/var/lib/led-cube
INSTALL_DIR=/usr/local/led-cube
ASSET_DIR=assets/files
SERVICE_FILE=/etc/systemd/system/led-cube.service

# install dpkg binaries


# move cube configuration-files
mkdir -p $CONFIG_DIR
mkdir -p $DEFAULT_DIR
cp $ASSET_DIR/* $DEFAULT_DIR
chmod --recursive 444 $DEFAULT_DIR
#chmod --recursive 444 $CONFIG_DIR

# move binaries
mkdir -p $INSTALL_DIR
cp $BUILD_DIR $INSTALL_DIR

# create systemd service
cat led-cube.service > $SERVICE_FILE
systemctl --now enable led-cube.service
