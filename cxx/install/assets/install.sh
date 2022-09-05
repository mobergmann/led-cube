# ensure running as root (source: https://electrictoolbox.com/check-user-root-sudo-before-running/)
if [ `whoami` != root ]; then
    echo "Please run this script as root or using sudo"
    exit
fi



# declare variables

# get path of script (if called from other location)
# source: https://stackoverflow.com/a/1638397/11186407
SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname "$SCRIPT")

BINARIES_DIR="$SCRIPT_DIR/bin"
ASSET_DIR="$SCRIPT_DIR/assets"
CONFIG_DIR="$SCRIPT_DIR/configurations"

SYS_INSTALL_DIR=/usr/local/led-cube
SYS_DEFAULT_DIR=/usr/share/led-cube
SYS_CONFIG_DIR=/var/lib/led-cube
SYS_SERVICE_FILE=/etc/systemd/system/led-cube.service



# create user config folder
mkdir -p "$SYS_CONFIG_DIR"
# set permissions for everyone
chmod --recursive 666 $SYS_CONFIG_DIR

# create program default config folder
mkdir -p "$SYS_DEFAULT_DIR"
# copy default files over
cp -r "$CONFIG_DIR/" "$SYS_DEFAULT_DIR/"
# set permissions only for program
chmod --recursive 100 "$SYS_DEFAULT_DIR"



# install dpkg binaries
sudo dpkg -i "$BINARIES_DIR/libgpiod2*.deb"

# move program to install dir
mkdir -p "$SYS_INSTALL_DIR"
cp "$BINARIES_DIR/led_cube" "$SYS_INSTALL_DIR/"



# create systemd service
cat "$ASSET_DIR/led-cube.service" > "$SYS_SERVICE_FILE"
# start systemd service
systemctl --now enable led-cube.service
