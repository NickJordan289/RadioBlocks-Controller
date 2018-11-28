/* Code adapted from RadioBlocks example and Joystick example
   to send joystick packets for use in controlling a motor.
   https://github.com/NickJordan289/RadioBlocks-Controller */

#include <RadioBlock.h>

//Pins connected to RadioBlock pins 1/2/3/4
RadioBlockSerialInterface interface = RadioBlockSerialInterface(5,4,3,2); // Plugged directly into board

#define CODE_TEMP   1
#define CODE_ALARM  2
#define CODE_LED    3
#define CODE_VALVE  4

#define OUR_ADDRESS   0x1000 // 0x1000 = Pin Powered
#define THEIR_ADDRESS 0x1001 // 0x1001 = Battery Powered

const int xAxis = A0;         // joystick X axis
const int yAxis = A1;         // joystick Y axis

// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value

void setup()  
{
  interface.begin();  

  //Give RadioBlock time to init
  delay(500);

  //Tell the world we are alive  
  interface.setLED(true);
  delay(1000);
  interface.setLED(false);
  
  interface.setChannel(15);
  interface.setPanID(0xABCD);
  interface.setAddress(OUR_ADDRESS);
    
  Serial.begin(115200); 
  Serial.println("Starting Send...");
}

void loop() // run over and over
{
  //This is the OTHER guys address
  interface.setupMessage(THEIR_ADDRESS);
  
  // Values here are arbitrary and were only used to distinguish
  // where values were in the packet data (Feel free to change)
  
  int x = readAxis(xAxis);
  if(x<0){
    interface.addData(CODE_TEMP, 1); // Send 1 if Negative
  } else if(x>0) {
    interface.addData(CODE_TEMP, 2); // Send 2 if Positive
  } else {
    interface.addData(CODE_TEMP, 3); // Send 3 if Zero
  }

  interface.addData(CODE_TEMP, abs(readAxis(xAxis)));
  
  int y = readAxis(yAxis);
  if(y<0){
    interface.addData(CODE_TEMP, 4); // Send 4 if Negative
  } else if(y>0) {
    interface.addData(CODE_TEMP, 5); // Send 5 if Positive
  } else {
    interface.addData(CODE_TEMP, 6); // Send 6 if Zero
  }
  
  interface.addData(CODE_TEMP, abs(readAxis(yAxis)));
 
  //Send data OTA
  interface.sendMessage();
  
  delay(40); // Packet send frequency
}


/*
  -From Joystick Example-
  reads an axis (0 or 1 for x or y) and scales the analog input range to a range
  from 0 to <range>
*/
int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}