 
/*
 * Watch video instruction for this code: https://youtu.be/Q9aBI4ELKC4
 */

#include <RotaryEncoder.h>

RotaryEncoder encoder(14, 12);


const int controlPin[6] = {4,5,2,16,0,13}; // define pins

const int triggerType = LOW;// your relay type
int tmpStat =1;


void setup() {

  for(int i=0; i<6; i++)
  {
    pinMode(controlPin[i], OUTPUT);// set pin as output
    if(triggerType ==LOW){
      digitalWrite(controlPin[i], HIGH); // set initial state OFF for low trigger relay
    }else{
       digitalWrite(controlPin[i], LOW); // set initial state OFF for high trigger relay     
    }
  }
  Serial.begin(9600);// initialize serial monitor with 9600 baud
  
}

unsigned long interval = 80;
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
        // channelControl(pos,0,0);
        pos = newPos;
        //channelControl(pos,1,0);
        decToBinary(pos);
        
        } 
    }  //
}


void decToBinary(int n) 
{ 
    // array to store binary number 
    int binaryNum[32]; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 

    for (int i = 0; i < 6; i++)
      channelControl(i, 0, 0);
       
    // printing binary array in reverse order 
    for (int j = i - 1; j >= 0; j--) {
        Serial.print( binaryNum[j] );
        channelControl(j, binaryNum[j], 0);
    }
    Serial.println("");
         
} 


/*
 * funciton: channelControl
 * turns ON or OFF specific relay channel
 * @param relayChannel is integer value channel from 0 to 15
 * @param action is 1 for ON or 0 for OFF
 * @param t is time in melisecond
 */
void channelControl(int relayChannel, int action, int t)
{
  int state =LOW;
  String statTXT =" ON";
  if(triggerType == LOW)
  {    
    if (action ==0)// if OFF requested
    {
      state = HIGH;
      statTXT = " OFF";
    }
    digitalWrite(controlPin[relayChannel], state);
    if(t >0 )
    {
      delay(t);
    }
      // Serial.print ("Channel: ");
     ///  Serial.print(relayChannel); 
     //  Serial.print(statTXT);
     //  Serial.print(" - "); 
     //  Serial.println(t);        
  }else{
    if (action ==1)// if ON requested
    {
      state = HIGH;     
    }else{
      statTXT = " OFF";    
    }
    digitalWrite(controlPin[relayChannel], state);
    if(t >0 )
    {
      delay(t);
    }
       //Serial.print ("Channel: ");
       //Serial.print(relayChannel); 
       //Serial.print(statTXT);
       //Serial.print(" - "); 
       //Serial.println(t);    
  }

}
