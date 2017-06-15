#!/bin/sh

# This script creates the envirement to run the project inside the virtual
# machine offered on the discipline or on the computers prepared for it

wget http://www.ic.unicamp.br/~lucas/teaching/mc723/2017-1/mips-tlm-per.tar.gz
tar xzf mips-tlm-per.tar.gz
rm mips-tlm-per.tar.gz
cd mips-tlm/
rm -rf sw
ln -s ../sw ./sw
cd peripheral/
rm -rf peripheral.cpp
ln -s ../../peripheral/peripheral.cpp ./
