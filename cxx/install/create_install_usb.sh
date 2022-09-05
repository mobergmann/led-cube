MOUNT_DIR=/mnt
USB_DIR=${MOUNT_DIR}/cube

sudo mount /dev/sda1 ${MOUNT_DIR}

# move configurations
mkdir -p ${USB_DIR}/configurations/
cp configurations/* ${USB_DIR}/configurations/

# move assets
mkdir -p ${USB_DIR}/assets/
cp assets/* ${USB_DIR}/assets/

# move binaries
mkdir -p ${USB_DIR}/binaries
cp ../binaries/* ${USB_DIR}/binaries

sudo umount ${MOUNT_DIR}
