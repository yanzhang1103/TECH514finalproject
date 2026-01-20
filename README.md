# TECH514finalproject
Proximity-Based Alert & Visualization System

A two-device proximity sensing system that detects how close an object or person is and provides intuitive physical feedback through a gauge needle and LED indicators. The system focuses on translating continuous proximity data into stable, non-screen-based feedback that can support spatial awareness and proximity alerts.

1. Sensor Device: Handheld Proximity Sensor

The sensor device is a compact, handheld proximity-sensing unit designed to detect nearby objects and transmit processed proximity data wirelessly to a separate display device. It emphasizes directional sensing and portable, battery-powered operation.

Hardware Components

Microcontroller: ESP32-C3 (Bluetooth Low Energy)

Distance Sensor: Sharp GP2Y0A21YK0F infrared distance sensor

Status Indicator: LED

Battery: 3.7V LiPo battery

Power Control: Slide switch

How It Works

The infrared distance sensor continuously measures the distance between the device and nearby objects. The ESP32-C3 samples the analog sensor output and applies basic digital signal processing, including smoothing and thresholding, to reduce noise and generate a stable proximity level. This processed proximity data is transmitted to the display device using Bluetooth Low Energy (BLE).

Power Design

The sensor device is battery-powered using a 3.7V LiPo battery. Low-power operation is supported through intermittent sampling and BLE communication, making the device suitable for portable use.

2. Display Device: Proximity Gauge Display

The display device is a standalone physical interface that visualizes proximity information received from the sensor device. It provides continuous, intuitive feedback through a moving gauge needle and LED indicators.

Hardware Components

Microcontroller: ESP32-C3

Actuator: Stepper motor driving a gauge needle

Motor Driver: ULN2003

Visual Indicator: LED

User Input: Physical button / switch

Battery: 3.7V LiPo battery

Sketch
![sketch](https://github.com/user-attachments/assets/3f74d47c-5d92-4212-a854-cd29027f152b)


How It Works

The display device receives proximity data from the sensor device via BLE. The ESP32-C3 maps the received proximity value to a corresponding needle angle and controls the stepper motor to move the gauge needle accordingly. LEDs provide additional status feedback, such as indicating safe or alert proximity ranges.

User Interaction

A physical button or switch allows basic user interaction, such as powering the device on or switching operating modes. The display continuously reflects proximity changes without requiring screen-based interaction.

3. System Architecture
3.1 Communication Diagram

The system consists of two independent, battery-powered devices connected via Bluetooth Low Energy. The sensor device acts as the BLE server, while the display device functions as the BLE client, receiving proximity updates in real time.

3.2 Workflow
![workflow](https://github.com/user-attachments/assets/2d046adb-15eb-4f32-b05e-80aa41b27e37)

diagram
![diagram](https://github.com/user-attachments/assets/faebd0c1-51a2-4e58-a2c8-28e8386782d8)


The sensor device measures distance using the IR sensor.

Raw sensor data is filtered and processed on the ESP32-C3.

Processed proximity data is transmitted via BLE.

The display device receives the data and updates the gauge needle and LEDs accordingly.
