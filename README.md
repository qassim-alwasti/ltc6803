<h1 align="center">Arduino UNO Library for LTC6803 BMS IC</h1>

# Overview
LTC6803 library is an Arduino UNO library for Linear technology's LTC6803 Battery Monitoring and Management System "BMS" IC. It was very difficult for me to find an easy to use library with exmaples for arduino UNO. So I had to write the code from scratch based on LTC6803-1's datasheet few years ago to be able to create a BMS project. Now I'm converting this code into a library which anyone can implement into their project very easily and with minimum modifications. 


# Table of content


# About LTC6803-1

LTC6803-1 is one of the best solutions in the market for Battery Monitoring System "BMS". Below are some of the main features, please refer to LTC6803 Datasheet for full features\capabilities of this IC or check out this youtube video from Linear Technology.
(https://youtu.be/eDXXNF7h-vQ)
1. This single IC could manage up to 12 battery cells and it is possile to connect multiple ICs together to manage higher number of battery cells. 
2. It monitors the voltage of each cell - where battery cells are connected in series - and discharges the overcharged cells to balance the whole batteries stack. 
3. Using LTC6803-1 requires a micro-controller -which is in this case an Arduino UNO atmega328- which will drive the LTC and process its data.


# Getting started
This section will demonestrate how to get an LTC6803-1 IC working with an Arduino UNO (or Atmega328 microcontroller) using the provided example. 
## Required parts
## Circuit diagram
## Code example

---

# More in-depth details:
## Understanding the configuration registers
## Calculating the PEC value

---

# Future Improvements checklist
1. Power consumption improvement
2. Add example with SD card reader
3. Add example which works with ESP8266 and ESP32 as the host uController
4. Replace any blocking delays.
