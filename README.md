# IoT Smart Control Board
This project is intended to provide a fast and efficient approach for configuring and controlling [Home] a set of heterogeneous Automation devices. The challenge faced in most of the heterogeneous scenarios is the one of assigning a dedicated Graphical User Interface to each device, given the plethora of devices there could be.  
The proposed solution mainly relies on a self-configuration devices idea around a centralized server, where each device knows its type and communicates it to the server upon its connection. The server (which also holds the control board) adds the GUI dedicated to this device according to its type. The different types of devices handled and their GUI are coded in advance.  
All the implemented communication schemes rely on the TCP protocol.  
All the devices' GUIs have a common part including:
- Device type: displays the type of the device
- Device name: holds the name the name of the device. This could be useful to make distinction between devices of the same type.
- Close: it is a button which, when clicked, will close the socket dedicated to communication with the device. It has been added to simulate a disconnection or connection loss, and to check the effectiveness of a reconnection mechanism implemented on the device side.

It the current version, the device type is represented by an unsigned 8-bits coded integer.  
The server and control board code is in the _iot_control_board_ folder. It is written in C++, with the Qt framework.  
A test client program is in the _esp32_iot_display_ folder, written on platformio in C++. It is for interfacing a I2C OLED display with a ESP32 board, and which displays the message received from the server.  

During the test step, we have experimented with 3 differents device types:
- Camera: a remote camera which captures and sends image frames in a video-stream fashion to the server which displays it on the associated GUI.
- Display: a remote device with a screen where it displays the text (or any characters string) sent through the GUI.
- Lamp: a lamp/light device, receiving ON/OFF commands from the associated GUI and turns on/off a connected lamp accordingly.

This repositery currently holds files of Qt server application as well as a test code for a Display ESP32-based client device.  
## The server application
It mainly consists of 5 classes:
- BaseGui class: serving as a  building block for all device type classes, it defines attributes (widgets) common to them. It is inherited by every device type class.
- TmpSocket: a generic class which holds incoming client connections and related device type and name, waiting for the connection to be authenticated (here, only by checking the device type validity).
- DisplayGui: defines an interface for Display devices, by adding a text zone and a button to the interface inherited from the BaseGui class.
- LampGui: offers an interface for Lamp devices, by completing BaseGui with a switch button.
- ControlBoard: handles incoming client connections (which are affected a TmpSocket instance), proceed with their authentication. If an incoming client is authenticated (if device type is supported), an interface is created accordingly and added to the control board. The connection is closed otherwise (device type not supported).  When a client closes its connection, the associated interface is automatically removed from the control board.

## The Display device test code
Located in the _esp32_iot_display/src_ folder, it is made of:
- a TcpClient class: a multi-threaded custom TCP client based on POSIX thread and BSD C Sockets APIs. Given a server IP and port number, it handles the connection/disconnection, data transmission/reception. It alo provides callback mechanism in case of deconnection (this is used for automatic reconnection trial, in this example), and packet reception. The use of POSIX thread and BSD C Sockets APIs makes this code easily portable, and it can easily be adapted for devices running any Linux-based or Unix-like operating system, like a Raspberry Pi board running Raspberry Pi OS. 
- a main file, which describes the interface with a I2C OLED device, defines the disconnection and packet reception handlers, and instanciates and starts a client with Display device type.

## Demo Video
Below is a link to a demo video, realized with two devices, a ESP32-Cam and a ESP32 configured respectively as Camera and Display types:   
https://youtu.be/g_imCpFkrwo

## Extension
As the skeleton is already provided, this project can be easily extended <ith any type of IP-based IoT device. All it takes is extending (inheriting) the BaseGui class for the interface, and implementing the TCP-based application level protocol in a function defined as _Qt's slot_ and connected to the socket __ReadyRead__ _signal_.
