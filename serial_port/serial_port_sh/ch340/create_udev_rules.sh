#!/bin/bash

echo "remap the device serial port(ttyUSBX) to  ch340"
echo "rplidar usb connection as /dev/ch340 , check it using the command : ls -l /dev|grep ttyUSB"
echo "start copy ch340.rules to  /etc/udev/rules.d/"
echo "`rospack find serial_port`/serial_port_sh/ch340/ch340.rules"
sudo cp `rospack find serial_port`/serial_port_sh/ch340/ch340.rules  /etc/udev/rules.d
echo " "
echo "Restarting udev"
echo ""
sudo service udev reload
sudo service udev restart
echo "finish "
