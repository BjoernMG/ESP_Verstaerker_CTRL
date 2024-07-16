#include <ArduinoJson.h>

#define OUTPUT_RELAY_POWER 16

#define OUTPUT_TONE_CONTROL 14
#define OUTPUT_VOL_UP 12
#define OUTPUT_VOL_DOWN 13

#define OUTPUT_RELAY_INPUT_1 5
#define OUTPUT_RELAY_INPUT_2 4
#define OUTPUT_RELAY_INPUT_3 0
#define OUTPUT_RELAY_INPUT_4 2
#define OUTPUT_RELAY_INPUT_5 15
	
#define RELAY_OFF  0x1
#define RELAY_ON  0x0

boolean power = true;
boolean mute = false;
boolean toneControl = false;

int activeInput = 88;

void setup() {

  // set output pins:
  pinMode(OUTPUT_RELAY_POWER, OUTPUT);

  pinMode(OUTPUT_TONE_CONTROL, OUTPUT);
  pinMode(OUTPUT_VOL_UP, OUTPUT);
  pinMode(OUTPUT_VOL_DOWN, OUTPUT);

  pinMode(OUTPUT_RELAY_INPUT_1, OUTPUT);
  pinMode(OUTPUT_RELAY_INPUT_2, OUTPUT);
  pinMode(OUTPUT_RELAY_INPUT_3, OUTPUT);
  pinMode(OUTPUT_RELAY_INPUT_4, OUTPUT);
  pinMode(OUTPUT_RELAY_INPUT_5, OUTPUT);

  // on start: set all inputs to OFF
  muteInput();
  digitalWrite(OUTPUT_RELAY_INPUT_1, RELAY_ON);
  activeInput = OUTPUT_RELAY_INPUT_1;

  digitalWrite(OUTPUT_VOL_UP, RELAY_OFF);
  digitalWrite(OUTPUT_VOL_DOWN, RELAY_OFF);
  digitalWrite(OUTPUT_TONE_CONTROL, RELAY_OFF);

  Serial.begin(57600);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use a low data rate to reduce the error ratio
  //Serial1.begin(9600);
}

void loop() {
  // Check if the other Arduino is transmitting
  if (Serial.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than the sender's because it must store the strings
    StaticJsonDocument<300> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      //String tmp = JSON.stringify(doc);

      String remoteCmd = doc["REMOTE_CMD"].as<String>();
      Serial.print("RemoteCmd: ");
      Serial.println(remoteCmd);
      if(remoteCmd.equals("POWER_ON")){
        power=true;
        digitalWrite(OUTPUT_RELAY_POWER, RELAY_ON);
      } else if(remoteCmd.equals("POWER_OFF")){
        power=false;
        digitalWrite(OUTPUT_RELAY_POWER, RELAY_OFF);
      }else if((remoteCmd.equals("INPUT_1"))&&(power)){
        if(activeInput != OUTPUT_RELAY_INPUT_1){
          activeInput = OUTPUT_RELAY_INPUT_1;
          if(!mute){
            switchInput();
          }
        }
      }else if((remoteCmd.equals("INPUT_2"))&&(power)){
        if(activeInput != OUTPUT_RELAY_INPUT_2){
          activeInput = OUTPUT_RELAY_INPUT_2;
          if(!mute){
            switchInput();
          }
        }

      }else if((remoteCmd.equals("INPUT_3"))&&(power)){
        if(activeInput != OUTPUT_RELAY_INPUT_3){
          activeInput = OUTPUT_RELAY_INPUT_3;
          if(!mute){
            switchInput();
          }
        }

      }else if((remoteCmd.equals("INPUT_4"))&&(power)){
        if(activeInput != OUTPUT_RELAY_INPUT_4){
            activeInput = OUTPUT_RELAY_INPUT_4;
            if(!mute){
              switchInput();
            }
        }
   
      }else if((remoteCmd.equals("INPUT_5"))&&(power)){
        if(activeInput != OUTPUT_RELAY_INPUT_5){
            activeInput = OUTPUT_RELAY_INPUT_5;
            if(!mute){
              switchInput();
            }
        }
   
      }else if((remoteCmd.equals("MUTE_ON"))&&(power)){
        mute=true;
        muteInput();
        
      }else if((remoteCmd.equals("MUTE_OFF"))&&(power)){
        mute=false;
        switchInput();

      }else if((remoteCmd.equals("TONE_CONTROL_ON"))&&(power)){
        muteInput();
        digitalWrite(OUTPUT_TONE_CONTROL, RELAY_ON);
        delay(100);
        switchInput();
        
      }else if((remoteCmd.equals("TONE_CONTROL_OFF"))&&(power)){
        muteInput();
        digitalWrite(OUTPUT_TONE_CONTROL, RELAY_OFF);
        delay(100);
        switchInput();
      }else if((remoteCmd.equals("VOLUME_UP"))&&(power)){
        digitalWrite(OUTPUT_VOL_UP, RELAY_ON);
        delay(100);
        digitalWrite(OUTPUT_VOL_UP, RELAY_OFF);
        
      }else if((remoteCmd.equals("VOLUME_DOWN"))&&(power)){
        digitalWrite(OUTPUT_VOL_DOWN, RELAY_ON);
        delay(100);
        digitalWrite(OUTPUT_VOL_DOWN, RELAY_OFF);
      }
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0)
        Serial.read();
    }
  }

}


void switchInput(){
    muteInput();
    delay(100);
    digitalWrite(activeInput, RELAY_ON);
}

void muteInput(){
    digitalWrite(OUTPUT_RELAY_INPUT_1, RELAY_OFF);
    digitalWrite(OUTPUT_RELAY_INPUT_2, RELAY_OFF);
    digitalWrite(OUTPUT_RELAY_INPUT_3, RELAY_OFF);
    digitalWrite(OUTPUT_RELAY_INPUT_4, RELAY_OFF);
    digitalWrite(OUTPUT_RELAY_INPUT_5, RELAY_OFF);
}
