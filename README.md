<h1 align="center">Arduino UNO Library for LTC6803 BMS IC</h1>

# Overview
LTC6803 library is an Arduino UNO library for Linear technology's LTC6803 Battery Monitoring and Management System "BMS" IC. It was very difficult for me to find an easy to use library with exmaples for arduino UNO. So I had to write the code from scratch based on LTC6803-1's datasheet few years ago to be able to create a BMS project. Now I'm converting this code into a library which anyone can implement into their project very easily and with minimum modifications. 


# Table of content


# About LTC6803-1

LTC6803-1 is one of the best solutions in the market for Battery Monitoring System "BMS". Below are some of the main features, please refer to [LTC6803 Datasheet](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwiaoaKGip75AhX6JMUKHfi9CWEQFnoECAkQAQ&url=https%3A%2F%2Fwww.analog.com%2Fmedia%2Fen%2Ftechnical-documentation%2Fdata-sheets%2F680313fa.pdf&usg=AOvVaw0DliQdDvnjmocmrJoVoMZz "LTC6803 Datasheet") for full features\capabilities of this IC or check out this [Youtube Video](https://www.youtube.com/watch?v=eDXXNF7h-vQ "video") from Linear Technology.

1. This single IC could manage up to 12 battery cells and it is possile to connect multiple ICs together -daisy chain- to manage higher number of battery cells. 
2. It monitors the voltage of each cell - where the battery cells are connected in series.
3. If one or multiple cells reached the over voltage limit while the other cells are still chraging, it automatically enables a discharing circuit for each cell to balance the whole batteries stack. 
4. Using LTC6803-1 requires a micro-controller -which is in this case an Arduino UNO or atmega328 uController- which drives the LTC IC and process its data.


# Getting started
This section will demonestrate how to get an LTC6803-1 IC working with an Arduino UNO (or Atmega328 microcontroller) using the provided example. 
## Required parts
1. LTC6803-1 usually soldered on a break out board for prototyping. 
2. Arduino UNO or stand alone Atmega328 uController. 
3. Some ceramic capacitors and resistors as shown in the circuit diagram. 

## Code example
After adding this library to the Arduino IDE, go to "File" -> "Examples" -> "LTC6803" -> "Simple_LTC6803_Demo".
There are few variables which you need to modify on this demo Arduino sketch.

![variables](https://user-images.githubusercontent.com/109140923/181738522-28dea316-4de4-4145-9c5b-70ed07bb308b.PNG)

1. chipSelect -> this is the SPI chip select pin number - usually pin 10.
2. uv_limit -> Under voltage limit for the battery cells - lowest is -0.3 volts.
3. ov_limit -> Over voltage limit for the battery cells highest is 5 volts.
4. cellNumber -> Number of battery cells in your project - maximum 12 cells.

### Once these variables are set, you're good to go. Just upload the sketch and open the serial terminal to verify that it is working fine.

"ltc.getData();" is called periodically to read the current voltage and temperature values from LTC IC. 
The below global variables from the created LTC object will contain the readings from LTC IC:
1. ltc.IC_tmp -> retrns the internal temperature of the IC. Thsi temperature is a good indicator that the IC is working properly.
2. ltc.cell_voltages[index 0 to 11] -> array which returns the voltage reading for each cell from cell 1 to cell 12.
3. ltc.flag_cell[index 0 to 11] -> array which returns a string (OV, UV, OK) indicating the cell status compared to the over and under voltage limits specified previously. 
4. ltc.tmp_cell[index 0 to 1] -> array which returns the temperature reading from "Temp1" and "Temp2" pins on LTC IC. 

---

# Future Improvements checklist
1. Power consumption improvement
2. Add example with SD card reader
3. Add example which works with ESP8266 and ESP32 as the host uController
4. Replace any blocking delays.
