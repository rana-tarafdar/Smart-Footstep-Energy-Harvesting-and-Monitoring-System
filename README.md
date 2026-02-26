# Smart Footstep Energy Harvesting and Monitoring System

**Circuit Craft 2K26 – Winning Project (3rd)**

---

## 1. Overview

The _Smart Footstep Energy Harvesting and Monitoring System_ is an IoT-based project that generates electrical energy from human footsteps using piezoelectric sensors and monitors the output in real time using an ESP32 microcontroller.

The system converts mechanical pressure into electrical energy and displays voltage data both locally (OLED display) and remotely (web-based dashboard hosted by ESP32).

---

## 2. Problem Statement

In high-footfall public areas such as railway stations and shopping malls, mechanical energy from walking is wasted. Simultaneously, there is an increasing demand for renewable and smart energy solutions.

This project aims to utilize human footsteps as a micro renewable energy source and monitor the output through IoT.

---

## 3. Objectives

- Generate electrical energy from footsteps using piezoelectric sensors
- Convert AC output to regulated DC supply
- Measure voltage using ESP32
- Display output on OLED screen
- Host a local web server to monitor system parameters in real time

---

## 4. System Architecture

Footstep Pressure
→ Piezoelectric Sensors
→ Bridge Rectifier
→ Capacitor Filtering
→ ESP32
→ OLED Display + Web Dashboard

---

## 5. Hardware Components

- Piezoelectric Sensors
- ESP32 Development Board
- Bridge Rectifier
- Capacitors and Resistors
- OLED Display (I2C)
- Breadboard / PCB
- Connecting Wires
- More...

---

## 6. Installation Guide

### Step 1: Install Arduino IDE

Download and install Arduino IDE.

### Step 2: Add ESP32 Board Package

Add the following URLs in **File → Preferences → Additional Board Manager URLs**:

* https://arduino.esp8266.com/stable/package_esp8266com_index.json
* https://dl.espressif.com/dl/package_esp32_index.json
* https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

### Step 3: Required Libraries

- WiFi
- WebServer
- Wire
- Adafruit_GFX
- Adafruit_SH110X

### Step 4: Upload Code

- Select correct ESP32 board
- Select correct COM port
- Upload Final.ino

---

## 7. Features

- Real-time voltage monitoring
- OLED local display
- Auto-refresh web dashboard {In every 2 second (Considering it as a bad idea.) }
- WiFi-based local access (IP address : 192.168.4.1)
- Maximum AC and DC voltage tracking
- Day/Night status detection

---

## 8. Problems Faced and Solutions

- ESP32 driver recognition issues
- OLED initialization errors
- Noise in piezo output

Resolved :)

---

## 9. Results

- Successful voltage generation from footsteps
- Stable DC conversion
- Real-time web monitoring through ESP32 local network
- Functional OLED display output

---

## 10. Team Members

- Rana – Team Leader, Circuit Design & ESP32 Backend
- Abhradeep – Model Design
- Rajdeep – Web Dashboard Development
- Akanshu – Project Support
- Sanchita – Project Support

---

## 11. Academic Context

Developed as an IoT academic demonstration project and presented at Circuit Craft 2K26.
