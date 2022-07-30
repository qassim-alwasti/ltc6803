
//\\|-------------------------------------------------|//\\
//\\| |---------------------------------------------| |//\\
//\\| |           Done by: Qassim Alwasti           | |//\\
//\\| |                  July 2022                  | |//\\
//\\| |                 MIT License                 | |//\\
//\\| |  Battery Monitoring System Using LTC6801-1  | |//\\
//\\| |---------------------------------------------| |//\\
//\\|-------------------------------------------------|//\\

#include <LTC6803.h> // Including LTC library
ltc6803 ltc; //Create a new object of ltc6803 class called "ltc"

//Below constants are defined for testing - change them to match your setup
int chipSelect = 10;    //SPI chip select pin number - usually pin 10
float uv_limit = 1.0;   //Under voltage limit for these battery cells - lowest is -0.3 volts
float ov_limit = 1.9;   //Over voltage limit for these battery cells highest is 5 volts
int cellNumber = 12;    //Number of battery cells - maximum 12 cells


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
      
void setup()
{
  Serial.begin(115200); //For debugging
  ltc.begin(chipSelect, cellNumber, uv_limit, ov_limit, tempMuxPinA, tempMuxPinB);
  delay(500);
}

//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

// The main code here, it will run repeatedly.
void loop()
{
  //Periodically reads the battery cells voltages and temperature values
  ltc.getData();
  float totalVoltage = 0.0;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Printing the internal teperature value of the LCT IC
  Serial.print("-    IC Temp = ");
  Serial.print(ltc.IC_tmp);
  Serial.println(" degrees celsius   -");

  for(int z = 0; z < cellNumber; z++)
  {

    Serial.print("Cell ");
    Serial.print(z+1);

    Serial.print(" voltage: ");
    Serial.print(ltc.cell_voltages[z]);

    totalVoltage = totalVoltage + ltc.cell_voltages[z];
  
    Serial.print("v -- ");
    Serial.print("Flag: ");
    Serial.println(ltc.flag_cell[z]);
  }
  
  Serial.print("Total voltage = ");
  Serial.println(totalVoltage);

  Serial.print("Temperature A: ");
  Serial.println(ltc.tmp_cell[0]);
  Serial.print("Temperature B: ");
  Serial.println(ltc.tmp_cell[1]);
  Serial.println();

delay(1000);

}
