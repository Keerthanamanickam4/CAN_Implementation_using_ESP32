# ESP32 CAN Communication with Ultrasonic Distance Sensor

This project demonstrates **CAN (Controller Area Network) communication** between two ESP32 nodes using the **TWAI (CAN) driver** in **ESP-IDF**.  
An **ultrasonic sensor (HC-SR04)** measures distance on the transmitter side, and the measured data is sent over the CAN bus.  
The receiver ESP32 reads the CAN message and displays the decoded distance.

---

## Project Overview

* Transmitter ESP32:
  - Measures distance using an ultrasonic sensor
  - Encodes distance data
  - Sends distance over CAN bus

* Receiver ESP32:
  - Receives CAN messages
  - Decodes distance data
  - Displays object detection status

This project is suitable for **automotive, industrial applications** where sensor data must be shared reliably over CAN.

---

## Features

* CAN communication using **ESP32 TWAI driver**
* Ultrasonic distance measurement (2 cm – 400 cm)
* CAN speed: **500 kbps**
* Timeout handling for sensor errors
* Object detection logic
* Modular transmit and receive code
* Uses **FreeRTOS tasks and delays**

---

## Hardware Used

* ESP32 Dev Board (2 units)
* Ultrasonic Sensor (HC-SR04)
* CAN Transceiver (MCP2551)
* Connecting wires
* Common GND between all devices
* 120Ω CAN termination resistors


## System Working

### Transmitter Side

* The ultrasonic sensor measures distance continuously.
* If no object is detected or timeout occurs:
  - Distance is set to `0 cm`
* Distance data is packed into a CAN frame.
* CAN message is transmitted every **2 seconds**.

### Receiver Side

* ESP32 waits for incoming CAN messages.
* On receiving a message with ID `0x100`:
  -Data is decoded into distance value.
* If distance is `0 cm`:
  - Displays **No Object Detected**
* Otherwise:
  - Displays the measured distance in centimeters.

---

## Software Flow

### Transmitter

1. Initialize GPIO and CAN driver
2. Trigger ultrasonic sensor
3. Measure echo pulse duration
4. Calculate distance
5. Create CAN message
6. Transmit CAN frame
7. Delay for 2 seconds

### Receiver

1. Initialize CAN driver
2. Wait for CAN message
3. Decode CAN data
4. Display distance
5. Repeat continuously

---

## Important Notes

* CAN speed must match on both nodes (**500 kbps**)
* CAN transceiver is mandatory (ESP32 cannot connect directly to CAN bus)
* Proper CAN termination (120Ω) is required
* Ultrasonic timeout handling prevents blocking
* TWAI driver runs under FreeRTOS

---





## Author

**Keerthana M**  
Embedded Software Engineer  

