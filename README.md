# IMU_360D_Pepperl-Fuchs
IMU Sensor IMU360D-F99-B20-V15 from Pepperl+Fuchs, C++ API, CAN J1939 

Usage:











[    3.094821] input: HDA Intel PCH HDMI/DP,pcm=3 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input15
[    5.396237] e1000e: enp0s31f6 NIC Link is Up 1000 Mbps Full Duplex, Flow Control: Rx/Tx
[    5.396286] IPv6: ADDRCONF(NETDEV_CHANGE): enp0s31f6: link becomes ready
Cannot find device "can0"
Cannot find device "can0"






Note: 

After Linux system updating, if IXXAT USBtoCAN dapater cannot work and you get an error of [cannot find device "can0"]

[    3.094821] input: HDA Intel PCH HDMI/DP,pcm=3 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input15
[    5.396237] e1000e: enp0s31f6 NIC Link is Up 1000 Mbps Full Duplex, Flow Control: Rx/Tx
[    5.396286] IPv6: ADDRCONF(NETDEV_CHANGE): enp0s31f6: link becomes ready
Cannot find device "can0"


Try demsg you may see the device is recognised but not attached to can0

[  349.325457] usb 1-7: New USB device found, idVendor=08d8, idProduct=0008
[  349.325464] usb 1-7: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[  349.325469] usb 1-7: Product: USB-to-CAN V2
[  349.325473] usb 1-7: Manufacturer: IXXAT
[  349.325476] usb 1-7: SerialNumber: HW428447
[  349.325457] usb 1-7: New USB device found, idVendor=08d8, idProduct=0008
[  349.325464] usb 1-7: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[  349.325469] usb 1-7: Product: USB-to-CAN V2
[  349.325473] usb 1-7: Manufacturer: IXXAT
[  349.325476] usb 1-7: SerialNumber: HW428447

Then: 
go to the dirctory of socketcan_1-1-92-0_20150508 (the socketcan linux driver version maybe different, so the file name maybe different) which is downloaded from IXAAT official website.

Find the readme file under the directory of socketcan_1-1-92-0_20150508, follow the steps to compile the driver
- make
if failed, run
- sudo make install
- dmesg

At last, you may see:

[  349.325457] usb 1-7: New USB device found, idVendor=08d8, idProduct=0008
[  349.325464] usb 1-7: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[  349.325469] usb 1-7: Product: USB-to-CAN V2
[  349.325473] usb 1-7: Manufacturer: IXXAT
[  349.325476] usb 1-7: SerialNumber: HW428447
[  635.812184] ixx_usb: module verification failed: signature and/or required key missing - tainting kernel
[  635.835492] ixx_usb 1-7:1.0 can0: attached to USB-to-CAN_V2 channel 0 (device HW428447)
[  635.835517] usbcore: registered new interface driver ixx_usb

Now, the device is attached to can0.  

Set up the baudrate as 250 kbit/s
- sudo ip link set can0 type can bitrate 250000

start the can communication 
- sudo ip link set can0 up
