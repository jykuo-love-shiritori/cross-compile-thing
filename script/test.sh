#!/bin/bash

make -C . install
make -C ./client test
sudo ./client/test
make -C . uninstall
sudo dmesg | tail -20

