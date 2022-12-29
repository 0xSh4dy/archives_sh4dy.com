---
layout: page
title: IoT
tags: [iot]
---

## Device Attack Surface in IoT
Various parts of the device such as the storage, hardware communication interface, WiFi can be prone to attacks.

The device contains sensors that interact with the environment and collect data which is further sent to the gateway for pre-processing. [Payatu's blog](https://payatu.com/iot-security-part-2-101-iot-attack-surface/)  explains such attack surfaces in detail. Let's see what each component does.

- `SD Card / USB Drive` : Stores configuration, product data and firmware updates.

- `Non Volatile Memory`: Used for various stuff such as the bootloader, encryption keys, sensor data, firmware, etc. The `FLASH` memory chip is widely used because of its speed and efficiency.

- `Volatile Memory`: This memory holds the instructions and data at runtime. The data is lost when the device is powered off. 

- `Microcontroller Internal Memory`: The memory of the microcontroller can be accessed while debugging. 

----------------------------------------------------------------------------
## Hardware Communication Interface
All the communication between the hardware and the external world is done using standard hardware communication protocols. 



### Attack surfaces
- UART (Universal Asynchronous Receiver / Transmitter)
It is a hardware device for `asynchronous serial communication` in which the data format and transmission speeds are configurable. It sends data bits one by one, from the least significant to the most significant. In many devices, the UART ports are left open. Attackers can connect to the ports and get access over the serial to get a console


### Microcontroller Debug Port
Microcontrollers can be debugged using specified pins which are connected to pinouts(ports) on the board. These pinouts can be used for debugging, reading-writing firmware or microcontroller internal memory, etc. This makes debug ports one of the most critical attack surface. There are standard intrfaces used for this purpose such as JTAG, cJTAG and SWD.

* JTAG (Joint Test Action Group)
JTAG is an industry standard for testing printed circuit boards after manufacture. It defines standard protocols and commands that can be used to test and debug the microcontroller. It defines a four pin interface: TMS (Test Mode Select),TCK (Test Clock),TDI(Test data in),TDO(Test data out) along with one additional option pin TRST(Test reset). These pins are used by debuggers to communicate with TAP (Test Access Port) which is implemented on the microcontroller. An attacker can exploit pins and the port for extracting firmware, injecting malicious firmware and reverse engineering.

* cJTAG (Compact JTAG)
It defines a two pin interface (TCK and TMS) and a new TAP known as TAP 7.

* SWD (Serial Wire Debug)
A protocol/interface used for debugging microcontrollers. It defines a two pin interface: SWDIO(bidirectional) and SWCLK(clock). It used ARM CPU standard bi-directional wire protocol, defined in the [ARM Debug Interface v5](https://developer.arm.com/documentation/ihi0031/a/The-Serial-Wire-Debug-Port--SW-DP-/Introduction-to-the-ARM-Serial-Wire-Debug--SWD--protocol).


### Inter Integrated Circuit (I2C)
It is a short distance communication protocol used for communication between chips on the same board. It has a master-slave architecture and uses two-wire bus: SDA(serial data) and SCL(serial clock). One of the use case of I2C is in EEPROM chips that are connected to the microcontroller I2C pins and typically store data or code. Typical attacks would include tampering with the data, extracting sensitive information, corrupting the data etc. We should analyze the data at rest on the EEPROM chip as well as perform run-time analysis by sniffing the I2C communication to understand the behavior and security implications.

### Serial Peripheral Interface (SPI)
It is a short distance communication protocol used for communication between chips on the same board. It is full duplex and uses master-slave architecture. It has a four-wire serial bus: SCLK (serial clock), MOSI (master out slave in), MISO (master in slave out) and SS(slave select). FLASH and EEPROM chips use SPI.

### USB (Universal Serial Bus)
Security researchers can perform packet sniffing or even fuzz the USB interface to detect bugs.

### Sensor
The data collected by sensors is responsible for the proper functioning of the IoT ecosystem. Tampering with it might bring significant changes.

### Human Machine Interface
It can be damaged by stealers, ransomwares, etc.

--------------------------------------------------------------------------

## Network Communication Interface
It allows the device to talk to the cloud, user interface and the sensor network. Some possible attack surfaces are the WiFi interface, ethernet and radio.

--------------------------------------------------------------------------

References
[https://payatu.com/iot-security-part-2-101-iot-attack-surface/](https://payatu.com/iot-security-part-2-101-iot-attack-surface/)
[https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter)