//Third version - 22 July 2022
//Removing the temperature multiplexing - only reading temp1 and temp2 pins
/**
 * LTC6803.h
 * 
 * LTC6803, a library for Arduino UNO
 * to control and utilize many of the powerful LTC6803 IC features
 * 
 * @author Creator Qassim Alwasti
 * @version 1.0. - July 2022
 * @license MIT
 */

#ifndef LTC6803_H
#define LTC6803_H

#include <SPI.h>               // To comm. with LTC6803-1 through SPI
#include <Arduino.h>

class ltc6803
{
	public:

			void begin(int CS, int cNum, float uv_limit, float ov_limit, int muxPinA, int muxPinB);
			void getData();
	
			float IC_tmp;
			float cell_voltages[12];
			String flag_cell[12];
			int tmp_cell[12];
			

	private:
	
			void readTemp();
			void cellBalance();
			void readVolts();
			char getPEC();
			void writeRegisters();
			void findRegisters();
				
			int _CS; 			// chip select pin
			int _cellNumber; 	// Number of battery cells - maximum is 12 cells
			float _UVlimit ;    // Under voltage limit in volts
			float _OVlimit ;    // Over voltage limit in volts 

			//LTC Commands - names as per datasheet
			#define WRCFG 0x01             // Write Config. Registers, the PEC =0xC7
			#define STCVDC 0x60            // A/D converter and poll status command, te PEC =0xE7
			#define RDCV 0x04              // Read cell voltages command, the PEC =0xDC
			#define STTMPAD 0x30           // Start All Temp. ADC and Poll Status, the PEC =0x50
			#define RDTMP 0x0E             // Read Temp registers group, PEC = 0xEA	
			
			// valriable to hold all 6 bytes of config registers values
			int WRCFGval[6];
			// cvr to store the 8bit value read from LTC
			char cvr[18];
			// cell_voltage[x] contains the actual 12bit measured voltage value for Cell number "x"
			float _cell_voltages[12];
			// tmp to store the 8bit value read from LTC
			char tmp[6];
			// _tmp_cell[x] contains the actual 12bit measured temperature value in Degrees Celsuis
			float _tmp_val;
			int _tmp_cell[12];

			
			// Variables and constant for temperature conversion
			int B = -4300; // From NTC datasheet, the beta value.
			float resist[2];
			float rinf;
			
			unsigned char OV[12];  // The over voltage flag for each cell stored seperatly
			unsigned char UV[12];  // The under voltage flag for each cell stored seperatly
			String _flag_cell[12]; // To output - OK, UV or OV depending on battery condotion


			/* Setting the variables for configuration registers,
			this will make it easier and faster to cange parameters in the configuration registers*/
			int   VOV, VUV;            // variables to store over/under voltage config bytes
			float offset  = 0.0015;    // V-LSB, Quantization of the ADC - V/Bit - 1.69mV
			bool  CELL10  = 0;         // equal to 1 if less than 12 cells
			bool  WDT     = 1;         // watch dog timer bit - 1 by default
			bool  LVLPL   = 0;         // Polling or toggle mode bit - 0 by default
			char  CDC     = 1;         // Comparator duty cycle - last 3 bits only - value from 0 to 7
			bool  GPI01   = 1;         // General purpose pin 1 bit - 1 by default
			bool  GPI02   = 1;         // General purpose pin 2 bit - 1 by default
			
			bool DCC[12];  // Cell discharging bits in Config registers
			
			bool MC1  = 0; // Mask cell 1 in Config registers
			bool MC2  = 0; // Mask cell 2 in Config registers
			bool MC3  = 0; // Mask cell 3 in Config registers
			bool MC4  = 0; // Mask cell 4 in Config registers
			bool MC5  = 0; // Mask cell 5 in Config registers
			bool MC6  = 0; // Mask cell 6 in Config registers
			bool MC7  = 0; // Mask cell 7 in Config registers
			bool MC8  = 0; // Mask cell 8 in Config registers
			bool MC9  = 0; // Mask cell 9 in Config registers
			bool MC10 = 0; // Mask cell 10 in Config registers
			bool MC11 = 0; // Mask cell 11 in Config registers
			bool MC12 = 0; // Mask cell 12 in Config registers
			
			char pec1; // The calculated PEC value using the function
			
							
};



#endif

