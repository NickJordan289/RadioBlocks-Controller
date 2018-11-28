/* Code adapted from RadioBlocks example
   to recieve joystick packets and control motors.
   https://github.com/NickJordan289/RadioBlocks-Controller */

#include <RadioBlock.h>

//Pins connected to RadioBlock pins 1/2/3/4
RadioBlockSerialInterface interface = RadioBlockSerialInterface(-1,-1,3,2); // powered using 5v, GND

#define CODE_TEMP   1
#define CODE_ALARM  2
#define CODE_LED    3
#define CODE_VALVE  4

#define OUR_ADDRESS   0x1001 // 0x1001 = Battery Powered
#define THEIR_ADDRESS 0x1000 // 0x1000 = Pin Powered

void setup()  
{
  pinMode(5, OUTPUT); // Motor pin 1
  pinMode(6, OUTPUT); // Motor pin 2

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
  Serial.println("Starting Receive...");
}

void loop() // run over and over
{
  interface.readPacket(); // Wait for next packet (~40ms according to Send.ino)
  
  //New Message
  if (interface.getResponse().isAvailable()) {
    //Serial.println("New Response...");
    RadioBlockResponse& rbr = interface.getResponse();
    auto data = rbr.getFrameData();

    // Load joystick readings from packet
    int x = data[10];
    auto xP = data[7];
    if(xP == 2) { // if code==2, reading is negative
      x=-x;
    }
    int y = data[16];
    auto yP = data[13];
    if(yP == 4) { // if code==4, reading is negative
      y=-y;
    }
    
    //Print all Frame Data
    //for(int i=0; i < 20; i++) {
      //Serial.print("\n");
      //Serial.print(i);
      //Serial.print(": ");
      //Serial.println(data[i]);
    //}
      
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    Serial.print("\n");
    
    if(y > 0) { // Joystick Up
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
    } else if (y < 0) { // Joystick Down
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
    } else { // Joystick Middle
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
    }
  }
  
  //Oops?
  if (interface.getResponse().isError()) {
    Serial.println("You had ONE job Arduino. ONE job.");
  }
}
