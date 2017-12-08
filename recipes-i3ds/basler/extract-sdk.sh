#! /bin/bash

# Create and jump into files directory
mkdir -p files
cd files/

# Delete any old files
rm -f pylon*.tar.gz2

# Extract Pylon SDK 5.0
tar -xf $1 --wildcards --no-anchored 'pylonSDK-5.0.*.tar.gz' --strip 1
mv $(ls pylonSDK-5.0*.tar.gz) pylon-sdk-5.0.tar.gz
