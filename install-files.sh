CONFIG_DIR=/usr/share/led-cube
DEFAULT_DIR=/var/lib/led-cube
SRC_DIR=assets/files



# move files
mkdir $CONFIG_DIR
mkdir $DEFAULT_DIR

cp $SRC_DIR/* $DEFAULT_DIR

chmod --recursive 444 $DEFAULT_DIR



# clone
git clone --recurse-submodules git@github.com:mobergmann/led-cube.git

# install dependencies
sudo apt install build-essential cmake libgpiod-dev -y

# build
cmake -B ./build
cmake --build ./build -j8



# create systemd service
# todo create
# todo enable systemd service
# todo start service
