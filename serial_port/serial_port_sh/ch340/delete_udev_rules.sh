#!/bin/bash

echo "delete remap the device serial port(ttyUSBX) to  ch340"
echo "sudo rm   /etc/udev/rules.d/ch340.rules"
sudo rm   /etc/udev/rules.d/ch340.rules
echo " "
echo "Restarting udev"
echo ""
sudo service udev reload
sudo service udev restart
echo "finish  delete"
