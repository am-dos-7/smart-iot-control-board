# IoT Smart Control Board
This project is intended to provide a fast and efficient approach for configuring and controlling [Home] a set of heterogeneous Automation devices. The challenge faced in most of the heterogeneous scenarios is the one of assigning a dedicated Graphical User Interface to each device, given the plethora of devices there could be.  
The proposed solution mainly relies on a self-configuration devices idea around a centralized server, where each device knows its type and communicates it to the server upon its connection. The server (which also holds the control board) adds the GUI dedicated to this device according to its type. The different types of devices handled and their GUI are coded in advance.  
All the implemented communication schemes rely on the TCP protocol.  
All the devices' GUIs have a common part including:
- Device type: displays the type of the device
- Device name: holds the name the name of the device. This could be useful to make distinction between devices of the same type.
- Close: it is a button which, when clicked, will close the socket dedicated to communication with the device. It has been added to simulate a disconnection or connection loss, and to check the effectiveness of a reconnection mechanism implemented on the device side.

It the current version, the device type is represented by an unsigned 8-bits coded integer.  
The server and control board code is in the iot_control_board folder. It is written in C++, with the Qt framework.  
A test client program is in the esp32_iot_display folder, written on platformio in C++. It is for interfacing a I2C OLED display with a ESP32 board, and which displays the message received from the server.
