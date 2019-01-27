#!/bin/sh

mkdir tmp
cd tmp

#############################################
## download curl-7.63.0

wget https://curl.haxx.se/download/curl-7.63.0.tar.gz

#############################################
## clean dir

rm -rf curl-7.63.0

#############################################
## unpack

tar -xvzf curl-7.63.0.tar.gz
cd curl-7.63.0

#############################################
## build and install libressl

cmake .
make
make install