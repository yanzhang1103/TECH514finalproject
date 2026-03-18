# TECH514finalproject
CatGuard
Proximity-Based Alert & Visualization System

<img width="355" height="355" alt="Screenshot 2026-03-18 at 13 08 12" src="https://github.com/user-attachments/assets/0ea1c011-64cf-4ba7-b54e-7d1838e60303" />


A two-device proximity sensing system that detects how close an object or person is and provides intuitive physical feedback through a gauge needle and LED indicators. The system focuses on translating continuous proximity data into stable, non-screen-based feedback that can support spatial awareness and proximity alerts.

1. Sensor Device: Handheld Proximity Sensor

The sensor device is a compact, handheld proximity-sensing unit designed to detect nearby objects and transmit processed proximity data wirelessly to a separate display device. It emphasizes directional sensing and portable, USBC-powered operation.

Hardware Components

Microcontroller: ESP32-C3 

Distance Sensor: Sharp GP2Y0A21YK0F infrared distance sensor

Status Indicator: LED

Battery: 3.7V LiPo battery


How It Works

The infrared distance sensor continuously measures the distance between the device and nearby objects. The ESP32-C3 samples the analog sensor output and applies basic digital signal processing, including smoothing and thresholding, to reduce noise and generate a stable proximity level. This processed proximity data is transmitted to the display device using ESP-NOW.


2. Display Device: Proximity Gauge Display

The display device is a standalone physical interface that visualizes proximity information received from the sensor device. It provides continuous, intuitive feedback through a moving gauge needle and LED indicators and a buzzer.

Hardware Components

Microcontroller: ESP32-C3

Actuator: Stepper motor driving a gauge needle

Visual Indicator: LED

Sound Indicatior: Passive buzzer

User Input: Physical press button switch

Battery: 3.7V LiPo battery

Sketch
![sketch](https://github.com/user-attachments/assets/3f74d47c-5d92-4212-a854-cd29027f152b)

Safe range: >150 cm. Display sensor has no reaction within this range.
Warning range: 100-150 cm. The stepper motor needle rotates 90 degree to the middle/vertical position to indicate detection.
Alart range: <100 cm. The stepper motor needle rotates 180 degree all the way to the left to indicate detection, with LED light up and the buzzer buzzing.


How It Works

The display device receives proximity data from the sensor device via ESP-NOW. The ESP32-C3 maps the received proximity value to a corresponding needle angle and controls the stepper motor to move the gauge needle accordingly. LED and buzzer provide additional status feedback, such as indicating safe or alert proximity ranges.

User Interaction

A physical button allows basic user interaction, such as powering the device on or switching operating modes. The display continuously reflects proximity changes without requiring screen-based interaction.

3. System Architecture
3.1 Communication Diagram

The system consists of two independent, battery-powered devices connected via ESP-NOW. The sensor device acts as the server, while the display device functions as the client, receiving proximity updates in real time.

3.2 Workflow
![workflow](https://github.com/user-attachments/assets/2d046adb-15eb-4f32-b05e-80aa41b27e37)

diagram
![diagram](https://github.com/user-attachments/assets/faebd0c1-51a2-4e58-a2c8-28e8386782d8)

<img width="474" height="476" alt="Screenshot 2026-03-18 at 13 06 34" src="https://github.com/user-attachments/assets/25c1770e-8532-42f8-a488-be3b717e1850" />
<img width="478" height="480" alt="Screenshot 2026-03-18 at 13 06 46" src="https://github.com/user-attachments/assets/d957c259-3404-4ac6-8c33-a7e53f7cf6a4" />


The sensor device measures distance using the IR sensor.

Raw sensor data is filtered and processed on the ESP32-C3.

Processed proximity data is transmitted via ESP-NOW.

The display device receives the data and updates the gauge needle and LEDs accordingly.

Critical Components and PCB Design
<img width="1203" height="458" alt="Screenshot 2026-03-18 at 13 07 39" src="https://github.com/user-attachments/assets/8e02c4bd-c370-4437-8783-15206b0b84d0" />

