#! /bin/bash

BB="pylon-sdk_5.0.bb"

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
    echo "Usage: ./extract-sdk.sh /path/to/pylon-5.0.*.tar.gz"
    exit -1
fi

echo -n "Extracting Basler Pylon SDK from $FILE..."

# Create and jump into files directory
mkdir -p files

# Delete any old files
rm -f files/pylon*.tar.gz2

# Extract Pylon SDK 5.0
tar -xf $FILE --wildcards --no-anchored 'pylonSDK-5.0.*.tar.gz' --strip 1
mv $(ls pylonSDK-5.0*.tar.gz) files/pylon-sdk-5.0.tar.gz

echo " [Done]"
