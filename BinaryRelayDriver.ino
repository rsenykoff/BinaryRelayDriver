/*
 * This is some code hacked together from various sources.
 * It treats a bank of relays as places in a binary number
 * and increments them accordingly.
 * 
 * We're hoping to use this to drive switching of radio coax segments
 * in order to rotate phase 360 via 6, 7 or 8 digit binary
 */

#include <RotaryEncoder.h>  //https://github.com/mathertel/RotaryEncoder

RotaryEncoder encoder(14, 12);


const int controlPin[6] = {4, 5, 2, 16, 0, 13}; // define pins
const int triggerType = LOW; // relay type, supports HIGH and LOW

void setup() {

  for (int i = 0; i < 6; i++)
  {
    pinMode(controlPin[i], OUTPUT);// set pin as output
    if (triggerType == LOW) {
      digitalWrite(controlPin[i], HIGH); // set initial state OFF for low trigger relay
    } else {
      digitalWrite(controlPin[i], LOW); // set initial state OFF for high trigger relay
    }
  }
  Serial.begin(9600);// initialize serial monitor with 9600 baud

}

unsigned long interval = 50;
unsigned long previousMillis;

int pos = 0;
void loop() {

  encoder.tick();
  int newPos = encoder.getPosition();

  if (newPos == -1) {
    newPos = 63;
    encoder.setPosition(63);
  }
  else if (newPos == 64)
  {
    newPos = 0;
    encoder.setPosition(0);
  }

  boolean changed = newPos != pos;

  if (changed)
  {

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      Serial.print(newPos);
      Serial.println();
      pos = newPos;
      decToBinary(pos);
    }
  }  
}


void decToBinary(int n)
{
  // array to store binary number
  // would be good to make the size configurable
  // vs hard coded here at 6
  int binaryNum[6];
  for (int w = 0; w < 6; w++)
    binaryNum[w] = 0;

  // counter for binary array
  int i = 0;
  while (n > 0) {

    // storing remainder in binary array
    binaryNum[i] = n % 2;
    n = n / 2;
    i++;
  }

  Serial.print("full binary: ");
  for (int w = 0; w < 6; w++)
    Serial.print(binaryNum[w]);

  Serial.println();

  // Write the binary to the relays
  for (int j = 0; j < 6; j++)
    relayControl(j, binaryNum[j]);  
}


/*
 * Thanks for this instruction and base code which I modified: https://youtu.be/Q9aBI4ELKC4
   funciton: relayControl
   turns ON or OFF specific relay
   @param relayNumber is integer value channel from 0 to 15
   @param action is 1 for ON or 0 for OFF
*/
void relayControl(int relayNumber, int action)
{
  int state = LOW;
  String dasStatus;
  if (triggerType == LOW)
  {
    if (action == 0) // off
    {
      state = HIGH;
      dasStatus = " OFF";
    }
    digitalWrite(controlPin[relayNumber], state);

  } else {
    if (action == 1) // on
    {
      state = HIGH;
    } else {
      dasStatus = " OFF"; 
      Serial.println("Shouldn't be here, check action value.");
    }
    digitalWrite(controlPin[relayNumber], state);
  }
//  Serial.print("Relay: ");
//  Serial.print(relayNumber);
//  Serial.println(" " + dasStatus);
}
