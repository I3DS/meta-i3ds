#! /bin/bash

BB="ebus-sdk_5.0.bb"

# The installer must run in the recipe directory
if [ ! -f "$BB" ]
then
    echo "Must run in same directory as $BB"
    exit 1
fi

# Check that we get a valid file argument
if [ -f "$1" ]
then
    FILE=$1
else
    echo "Usage: ./extract-sdk.sh /path/to/eBUS_SDK_linux-aarch64-arm-5.0.*.deb"
    exit -1
fi

echo -n "Extracting Pleora eBUS SDK from $FILE..."

# Create and jump into files directory
mkdir -p files

# Delete any old files
rm -rf files/ebus-sdk_5.0.tar.gz

# Extract eBUS SDK 5.0
rm -rf tmp/
dpkg -x $FILE tmp/
mv tmp/opt/pleora/ebus_sdk/linux-aarch64-arm/ files/ebus-sdk
rm -rf tmp/

# This unused file has a reference to bin/bash which causes warnings during build
rm files/ebus-sdk/bin/install_daemon.sh

# Copy the licenses into the directory
mkdir -p files/ebus-sdk/lib/licenses
cp -r licenses/* files/ebus-sdk/lib/licenses

# Pack and gzip the files
cd files/
tar czf ebus-sdk-5.0.tar.gz ebus-sdk
rm -rf ebus-sdk

echo " [Done]"
