
## Overview
Internet of things is an ecosystem of connected physical objects that are accessible through the internet. It is made up of a number of physical objects that are embedded with sensors, softwares and other technologies for the purpose of connecting and exchanging data with other devices and systems over the internet. These devices range from normal household objects to sophisticated industrial tools. IoT allows appliances such as microwaves, electronic doors, cameras, etc. to connect to the internet.

## Components
An IoT device is essentially made up of five components - thing/device , gateway, cloud, analytics and user interface.

<img src='https://media.geeksforgeeks.org/wp-content/uploads/20200206131008/4414.png'/>
Source : Geeks for Geeks

* `Things or Device`: These components are fitted with sensors and actuators. Sensors collect data from the environment and and give to the gateway. Actuators perform physical actions based on data that originates from one or more sensors.

* `Gateway`: The sensors provide data to the gateway which performs data pre-processing. Data preprocessing or filtering in reducing the amount of data that should be sent to the cloud. This reduces the power consumption and extends the battery life of the device. A gateway also enables device-to-device or device-to-cloud communications and adds an extra layer of security.

* `Cloud`: The collected data is uploaded to the cloud from where it can be accessed anytime.

* `Analytics`: Different algorithms are tools are used to process the data received on the cloud. Edge IT systems use machine learning and visualization algorithms to generate insights from the collected data.

* `User Interface`: The application where the user can monitor the data.


## Different Layers if IoT Architecture
The standard model comprises of 4 layers.
* `Perception/Sensing Layer`:  Things/ devices

* `Network Layer`: It provides an overview of how data is moved throughout the application. This layer contains Data Acquiring Systems (DAS) and internet/network gateways. A DAS performs data aggregation and conversion functions (conllecting data from sensors, then converting analog data into digital data). Thus, the network layer handles all data transmission for IoT devices, allows devices to connect and communicate with servers and other devices.

* `Processing Layer`: This layer is the brain of the IoT ecosystem. Data is analysed, pre-processed and stored here before being sent to the cloud.

* `Application Layer`: User interaction takes place at this layer.

## Embedded Systems
An embedded system is a combination of processor, computer memory and I/O devices that has a dedicated function within a mechanical or electronic system.

## Microcontrollers
A microcontroller is a complex integrated circuit designed to govern a specific operation in an embedded system. It is made up of CPUs along with memory and programmable I/O peripherals. 

## Serial Communication 
The process of sending data one bit at a time over a communication channel or computer bus.

## Asynchronous Serial Communication
A form of serial communication in which the communicating endpoints' interfaces are not continuously synchronized by a common signal. The data stream stores synchronization information in form of start and stop signals before and after each unit of transmission.

## Bus
A bus is a communication system that transfers data between components inside a computer, or betweeen computers. An internal bus enables communication between internal components such as video card and memory. An external bus is capable of communicating with external i/o devices (also known as peripherals), such as USB devices.

## Parallel bus vs Serial Bus
A parallel bus can transmit several bits at a time whereas a serial bus can only transmit one bit at a time.

## Address bus, data bus and control bus
Address bus stores the memory address of data. Data bus is used for transferring data and the control bus communicates with devices, sends and receives signals.

## Hardware Serial
It denotes that a specific hardware component enables serial communication. 

## Pinout
Pinout is a reference to the pins or contacts that connect an electrical device or connector.

## Master Slave Architecture
It is a model of asymmetric communication or control where one device or process (master) controls one or more other devices or processes (slaves) and serves as their communication hub.
## Attack Surface
There are four possible attack surfaces in an IoT ecosystem: device, cloud, communication and user interface.



References :
[https://payatu.com/iot-security-part-1-101-iot-introduction-architecture](https://payatu.com/iot-security-part-1-101-iot-introduction-architecture)
[https://www.oracle.com/in/internet-of-things/] (https://www.oracle.com/in/internet-of-things/)
[https://www.geeksforgeeks.org/components-of-iot-and-relation-with-cloud-computing/](https://www.geeksforgeeks.org/components-of-iot-and-relation-with-cloud-computing/)
[https://www.interviewbit.com/blog/iot-architecture/](https://www.interviewbit.com/blog/iot-architecture/)
[https://en.wikipedia.org/wiki/Embedded_system](https://en.wikipedia.org/wiki/Embedded_system)
[https://en.wikipedia.org/wiki](https://en.wikipedia.org/wiki)
[https://www.computerhope.com/jargon/b/bus.htm](https://www.computerhope.com/jargon/b/bus.htm)