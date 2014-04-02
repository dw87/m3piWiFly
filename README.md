m3pi WiFly
==========

A configuration for the mbed Microcontroller for use as part of an m3pi Robot allowing it to be controlled by my m3pi Controller Android app.

## Requirements

- A [Pololu m3pi Robot](http://www.pololu.com/product/2151) (a [Pololu 3pi robot](http://www.pololu.com/product/975) with the [m3pi Expansion board](http://www.pololu.com/product/2152) and an [mbed LPC1768 microcontroller](http://mbed.org/platforms/mbed-LPC1768/))
- An [RN-XV WiFly] (https://www.sparkfun.com/products/10822) Xbee footprint WiFi module
- An [XBee USB Explorer Dongle] (https://www.sparkfun.com/products/9819) (optional, but very useful!)
- An Android device with WiFi connectivity

## Hardware Configuration

If you're looking at this project, and you have an m3pi Robot, you hopefully have some degree of experience with electronics and embedded systems.  This project doesn't involve any electronics, or modifications to the hardware - as long as you've been able to plug the mbed and WiFly into to the top of the m3pi you're good to go.

The tricky part can be configuring the RN-XV WiFly.  There are 2 ways to do this:
1. Access Point Mode - connect your Android device directly to the WiFly
2. Using an existing WiFi network - both the WiFly and Android device are connected to the same SSID network

I will cover how to configure the WiFly in Access Point Mode.  I have tested both, but prefer Access Point mode as it allows you to control the m3pi Robot anywhere, without relying on an external WiFi network.  

### WiFly Configuration

#### WiFly Access Point Mode Configuration

You will need a Serial Port (RS232) Terminal Program - I used [Termite] (http://www.compuphase.com/software_termite.htm), but there are many others.  

If you don't have a USB Explorer Dongle, you will need to put the WiFly into Adhoc Mode first, connect to the WiFi network it creates, and then connect to it via Telnet.  The commands will then be the same as below.  See this guide to help you: http://www.tinkerfailure.com/2012/02/setting-up-the-wifly-rn-xv/

Using the XBee Explorer USB Dongle to connect via RS232, select the correct Serial Port (e.g. COM4) and use the settings:
Baud rate: 9600
Data bits: 8
Parity: None
Stop bits: 1
Flow Control: None

This is a common setting for an RS232 connection, and typically written in shorthand as 9600 8N1.  

Once you have a connection, using RS232 or Telnet, send '$$$' to enter Command Mode.  You should see 'CMD' sent back.
Do not send CR or LF with '$$$'.  In Termite, open the Settings and select 'Append nothing'.  

Once in Command Mode (CMD), change your setting back to 'Append CR-LF' to each further line.

To set the WiFly as an Access Point, use the commands below.  After each command the WiFly should send back 'AOK'.  (These commands are based on the latest firmware version 4.41.  See [Curious Marc's Guide to Setting up the WiFly Radio](http://www.curiousmarc.com/r2-touch-r2-d2-remote-control-iphone-app/setting-up-the-wifly-radio) for other firmware settings.)

Change the settings (e.g. IP address, SSID, password etc) to what you want to use.  

```
set ip address 192.168.14.14
set ip gateway 192.168.14.14
set ip netmask 255.255.255.0
set ip dhcp 4
set ip protocol 3
set ip remote 31416
set apmode ssid m3piWiFly
set apmode pass yourpassword
set wlan ssid m3piWiFly
set wlan join 7
set wlan rate 0
set wlan channel 7
save
reboot
```

Remember to 'save' when you are finished or you will lose the configuration.  
When you 'reboot', the WiFly will restart using your new settings.  You will now be able to connect to it directly.  

#### WiFly Firmware Update

The are many different versions of firmware available for the WiFly, including the original ones from Roving Networks, and newer versions since they were purchased by Microchip.  

[Curious Marc's Guide to Setting up the WiFly Radio](http://www.curiousmarc.com/r2-touch-r2-d2-remote-control-iphone-app/setting-up-the-wifly-radio) includes as section on how to update the Firmware if it is something you want to do.  

### m3pi Configuration

With the m3pi build, the WiFly and the mbed installed, all that is needed is for a configuration to be uploaded into the mbed.  The mbed connects to a computer using a mini USB cable, and appears as a flash drive.  

You can download the m3piWiFly.bin configuration from here, save it on the mbed's flash memory, and when you unplug the mbed and turn on the m3pi, it will run that program.  

On the mbed will be a HTML file which will let you create an account with [mbed.org](http://mbed.org) to use their online compiler.  You can then write your own programs to the mbed in C++, and compile them into the .bin files to put on the mbed.

I have included my m3piWiFly.cpp file here, so you can grab the code and have a look.  It's currently basic, and only offers proportional control forwards and backwards in a straight line, as well as spinning on the spot.  As I develop this to provide better control, I will update the files here.  

Remember, there should only be one .bin file on the mbed at once.  When the mbed is given power it will run the .bin file in memory.  If there is more than one, it could cause problems.  

## Questions/Problems/Improvements

This project is still in development, but if you have any questions you want to ask or improvements you think I should make, let me know.  I'm always learning and looking for better ways to do things.  
