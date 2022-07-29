//Third version - 22 July 2022
//Removing the temperature multiplexing - only reading temp1 and temp2 pins
/**
 * LTC6803.cpp
 * 
 * LTC6803, a library for Arduino UNO
 * to control and utilize many of the powerful LTC6803 IC features
 * 
 * @author Creator Qassim Alwasti
 * @version 1.0.0 - July 2022
 * @license MIT
 */

#include "LTC6803.h"

//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

//Setting up SPI speed, data order and data mode.
SPISettings settings(1000000, MSBFIRST, SPI_MODE3); // 1MHz , Most significant bit first, Mode3
// In mode 3, the clock polarity "CPOL" is high >> Data is shifted on the rising edge.
// In mode 3, the clock phase "CPHA" is high >> clock is idle when high.
// This is described in "LTC6803" datasheet, so MODE3 is chosen.


void ltc6803::begin(int CS, int cNum, float uv_limit, float ov_limit, int muxPinA, int muxPinB)
{
  _CS = CS; 			//SPI Chip select pin used to select LTC IC
  _cellNumber = cNum;	//Number of battery cells connected to the system
  
  if (_cellNumber < 12) // equal to 1 if less than 12 cells
  {
	  CELL10 = 1;
  }
  else
  {
	  CELL10 = 0;
  }
  
  _UVlimit = uv_limit;     // Under voltage limit in volts
  _OVlimit = ov_limit;     // Over voltage limit in volts
  pinMode(CS,   OUTPUT);   // Set Chip select "usually pin 10" as an output. CS
  SPI.begin();             // Initialize SPI comm.
  
}	

void ltc6803::getData()
{
	
  findRegisters();            // Calculate each byte of config registers automatically.
  pec1 = getPEC();            // Calculate the PEC for config registers automatically in every loop.
  writeRegisters();           // Calling writing to configuration registers function.
  readVolts();                // Reading each battery cell voltage.
  cellBalance();              // Reading the OV/UV flags for each cell.
  readTemp();  				  // Reading the temperature from Vtemp1 and Vtemp2 pins.
  
}

//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//***************************************************//
//     Calculating the Config. Registers values      //
//***************************************************//
void ltc6803::findRegisters()
{
  // Assigning each bit to its place in the configuration registers bytes
  WRCFGval[0] = (WDT) << 7 | (GPI02) << 6 | (GPI01) << 5 | (LVLPL) << 4 | (CELL10) << 3 | (CDC & 0x07);
  WRCFGval[1] = DCC[7]<<7|DCC[6]<<6|DCC[5]<<5|DCC[4]<<4|DCC[3]<<3|DCC[2]<<2|DCC[1]<<1|DCC[0];
  WRCFGval[2] = MC4<<7|MC3<<6|MC2<<5|MC1<<4|DCC[11]<<3|DCC[10]<<2|DCC[9]<<1|DCC[8];
  WRCFGval[3] = MC12 << 7 | MC11 << 6 | MC10 << 5 | MC9 << 4 | MC8 << 3 | MC7 << 2 | MC6 << 1 | MC5;
  WRCFGval[4] = 0; //VUV
  WRCFGval[5] = 0; //VOV
}


//***************************************************//
// To calculate PEC of the transmitted data sequence //
//***************************************************//
char ltc6803::getPEC() //Declaring the funcion "getPEC", it has "WRCFGval" input, char and "n" inputs. "n" represents number of bytes - 6 for config registers
{
  int n =6; // this should be an input to getPEC function not a static value - set to static value for testing only
  char pec, in0, in1, in2;    //Local variables
  int j, i;                   //Local variables for both "for loops"
  pec = 0x41;                 //initial value of PIC "01000001"- Given in datasheet Page 17

  for (j = 0; j < n; j++) // First loop to shift all data and find the final 8bit PEC
  {
    for (i = 0; i < 8; i++) //Second - main - loop
    {
      in0 = ((WRCFGval[j] >> (7 - i)) & 0x01) ^ ((pec >> 7) & 0x01);
      in1 = in0 ^ ((pec >> 0) & 0x01);
      in2 = in0 ^ ((pec >> 1) & 0x01);
      pec = in0 | (in1 << 1) | (in2 << 2) | ((pec << 1) & ~0x07);
    }
  }

  return pec; // The output is the variable pec
}


//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//***************************************************//
//           Writing configuration registers         //
//***************************************************//
void ltc6803::writeRegisters() {

  digitalWrite (_CS, LOW);        // Selecting the chip
  delay(0.01);                    //wait 10uSec
  SPI.beginTransaction(settings); // Initialize the SPI comm. Based on the settings provided by the user

  SPI.transfer(WRCFG);            //Send write config. registers command 0x01
  SPI.transfer(0xC7);             //Send write config. registers command PEC 0xC7

  SPI.transfer(WRCFGval[0]);      // Send first Config. Register.  WRCFG[1]
  SPI.transfer(WRCFGval[1]);      // Send second Config. Register. WRCFG[2]
  SPI.transfer(WRCFGval[2]);      // Send third Config. Register.  WRCFG[3]
  SPI.transfer(WRCFGval[3]);      // Send fourth Config. Register. WRCFG[4]
  SPI.transfer(WRCFGval[4]);      // Send fifth Config. Register.  WRCFG[5]
  SPI.transfer(WRCFGval[5]);      // Send sixth Config. Register.  WRCFG[6]

  SPI.transfer(pec1);             // Send te PEC for all above 6 Registers combined. Found by getPEC function.

  delay(0.15);                    //wait 150uSec
  digitalWrite (_CS, HIGH);       //De-selecting the chip
}

////-----------------------------------------------------------------------------------------------------//
////-----------------------------------------------------------------------------------------------------//
//***************************************************//
// To start ADC and reads cell reg. & calc voltages  //
//***************************************************//
void ltc6803::readVolts()
{
  int j;
  int i;
  
  digitalWrite (_CS, LOW); // Selecting the chip

  SPI.transfer(STCVDC); //   Send start ADC conversion command
  SPI.transfer(0xE7);   //   Send start ADC conversion command PEC

  digitalWrite (_CS, HIGH); // De-selecting the chip

  delay(15);  //wait 13 mSec -Recommended
  
  ////////// Reading values after starting the ADC //////////

  digitalWrite (_CS, LOW);  // Selecting the chip

  SPI.transfer(RDCV);     // Reading all Cell Voltage Group command
  SPI.transfer(0xDC);     // RDCV PEC
  
   

  for (j = 0; j < 18; j++)
  {
    cvr[j] = SPI.transfer(RDCV); //Store each byte in cvr[x]
  }

  digitalWrite (_CS, HIGH);  // De-selecting the chip
  
  
  ////////// Calculating the voltages from the read array "cvr" //////////
  // Shifting bits; because each reading is described by 12bits, while recieved bits are 8but at each time
  // some shifting is done to re-assemble each 12bits that describes each cell voltage

  _cell_voltages[0]  = (cvr[0]  & 0xFF) | (cvr[1] & 0x0F) << 8;
  _cell_voltages[1]  = (cvr[1]  & 0xF0) >> 4 | (cvr[2] & 0xFF) << 4;
  _cell_voltages[2]  = (cvr[3]  & 0xFF) | (cvr[4] & 0x0F) << 8;
  _cell_voltages[3]  = (cvr[4]  & 0xF0) >> 4 | (cvr[5] & 0xFF) << 4;
  _cell_voltages[4]  = (cvr[6]  & 0xFF) | (cvr[7] & 0x0F) << 8;
  _cell_voltages[5]  = (cvr[7]  & 0xF0) >> 4 | (cvr[8] & 0xFF) << 4;
  _cell_voltages[6]  = (cvr[9]  & 0xFF) | (cvr[10] & 0x0F) << 8;
  _cell_voltages[7]  = (cvr[10] & 0xF0) >> 4 | (cvr[11] & 0xFF) << 4;
  _cell_voltages[8]  = (cvr[12] & 0xFF) | (cvr[13] & 0x0F) << 8;
  _cell_voltages[9]  = (cvr[13] & 0xF0) >> 4 | (cvr[14] & 0xFF) << 4;
  _cell_voltages[10] = (cvr[15] & 0xFF) | (cvr[16] & 0x0F) << 8;
  _cell_voltages[11] = (cvr[16] & 0xF0) >> 4 | (cvr[17] & 0xFF) << 4;

  for (i = 0; i < _cellNumber; i++)
  {
    _cell_voltages[i] = (_cell_voltages[i] - 512) * offset; // Converting the 12bit value to the real measured voltage
  }
  
  //make these values available publicly
    for (i = 0; i < _cellNumber; i++)
  {
    cell_voltages[i] = _cell_voltages[i];
  }
}


//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//***************************************************//
// To start ADC and reads Temp reg. & calc Temp in C //
//***************************************************//

void ltc6803::readTemp()
{
	
  int j;

  digitalWrite (_CS, LOW); // Selecting the chip

  SPI.transfer(STTMPAD); //Send start all Temp. ADC conversion command
  SPI.transfer(0x50);    //Send start ADC conversion command PEC

  delay(10);  //wait

  digitalWrite (_CS, HIGH); // De-selecting the chip

  ////////// Reading values after starting the ADC //////////

  digitalWrite (_CS, LOW);  // Selecting the chip

  SPI.transfer(RDTMP);     // Read all Temp reg. Group command
  SPI.transfer(0xEA);      // RDTMP PEC

  for (j = 0; j < 5; j++)
  {
    tmp[j] = SPI.transfer(RDTMP); // Storing each received byte in a different variable
  }

  digitalWrite (_CS, HIGH);  // De-selecting the chip
  delay(10);  //wait
  ////////// Calculating the voltages from the read array "cvr" //////////

  // Shifting bits to re-assemble every 12bits
  _tmp_cell[0] = (tmp[0] & 0xFF) | (tmp[1] & 0x0F) << 8;      // First Temp. voltage
  _tmp_cell[1] = (tmp[1] & 0xF0) >> 4 | (tmp[2] & 0xFF) << 4; // Second internal Temp. voltage
  
  _tmp_val  = (tmp[3] & 0xFF) | (tmp[4] & 0x0F) << 8;       // For internal Temp. voltage

  // Converting to degrees Celsuis
  resist[0] = 10000 * ((3.02 / ((_tmp_cell[0] - 512) * offset)) - 1); // 10K ohm NTC
  resist[1] = 10000 * ((3.02 / ((_tmp_cell[1] - 512) * offset)) - 1);

  rinf = 12000 * exp(-1 * (B / 298.15)); // 12K ohm resistor

  // Finding the temp. value in degrees
  _tmp_cell[0] = (B / log(resist[0] / rinf)) - 273.15;
  _tmp_cell[1] = (B / log(resist[1] / rinf)) - 273.15;
  
  _tmp_val = (((_tmp_val - 512) * offset) / 0.00839) - 273.15; // For IC internal Temp. voltage
  
  //Making the calculated values publicly acccessble
  IC_tmp = _tmp_val;
  tmp_cell[0] = _tmp_cell[0];
  tmp_cell[1] = _tmp_cell[1];
  }
  
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

//***************************************************//
//  To balance cells that reached over voltage limit //
//***************************************************//
void ltc6803::cellBalance()
{
  int i; //local variable

  // if statement to print if each bettery is over/under voltage or within limits
  for (i = 0; i < _cellNumber; i++) // for all 8 cells
  {
    if (_cell_voltages[i] <= _UVlimit)      // if under voltage
    {
      _flag_cell[i] = ("UV");
      UV[i] = 1;
    }
    else if (_cell_voltages[i] >= _OVlimit) // if over voltage
    {
      _flag_cell[i] = ("OV");
      OV[i] = 1;
    }
    else
    {
      _flag_cell[i] = ("OK");
      UV[i] = 0;
      OV[i] = 0;
    }
	
	//Make this available to the Public variable array flag_cell
	flag_cell[i] = _flag_cell[i];
  }
  
  // To turn on discharging cell when OV limit flag is high
  for(i = 0; i < _cellNumber; i++)
 {
  if(OV[i]==1) // check if the OV flag is high
  {
    DCC[i]=1;  // Allow discharging the OV cell
  }
  else
  {
    DCC[i]=0;  // Don't allow discharging the OV cell
  }
 }
}
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

