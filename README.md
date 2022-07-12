#Arduino Library for ltc6803 BMS IC

#Intorduction
Arduino UNO library for Linear technology's LTC6803-1 Battery management system IC. It was very difficult for me to find a library with exmaples for arduino which is easy to use. So I had to write the code from scratch based on LTC6803-1's datasheet few years ago. Now I'm converting this code into a library which can easily be used by anybody. 

#About LTC6803-1:
LTC6803-1 is one of the best solutions in the market for Battery Monitoring System "BMS". 
1- This single IC could manage up to 12 battery cells and it is possile to connect multiple ICs together to manage higher number of battery cells. 
2- It monitors the voltage of each cell - where battery cells are connected in series - and discharges the overcharged cells to balance the whole batteries stack. 
3- Using LTC6803-1 requires a micro-controller -which is in this case an Arduino UNO atmega328- which will drive the LTC and process its data.

#How to use it with arduino UNO
