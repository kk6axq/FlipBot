#include <NintendoExtensionCtrl.h>
#include <SPI.h>
#include <NRFLite.h>

#define RATE 20//Hz
#define PERIOD 1000.0/RATE

#define LED_PIN 17//17
#define RADIO_CE 3//3
#define RADIO_CSN 4//4

#define RADIO_ID 1
#define DESTINATION_RADIO_ID 0

Nunchuk nc;

NRFLite _radio;
struct RobotMsg {
  byte linear;
  byte angular;
  bool buttonC;
  bool buttonZ;
  float rollAngle;
  float pitchAngle;
};

typedef union {
  RobotMsg msg;
  byte buffer[sizeof(RobotMsg)];
} convert;

long startTime = 0;

void setup() {
  //Serial.begin(9600);
  // put your setup code here, to run once:
  //Serial.println("Start");
  pinMode(17, OUTPUT);
  nc.begin();
  while(!nc.connect()){
    //Serial.println("Connecting");
    blink(100);
  }
  radioInit();
  startTime = millis();

  //while(true) digitalWrite(LED_PIN, HIGH);
}

void loop() {
  bool success = nc.update();
  if(!success){
    blink(250);
  }else{
    struct RobotMsg payload;
    payload.buttonC = nc.buttonC();
    payload.buttonZ = nc.buttonZ();
    payload.linear = nc.joyY();
    payload.angular = nc.joyX();
    payload.rollAngle = nc.rollAngle();
    payload.pitchAngle = nc.pitchAngle();
    //Serial.print("Linear: ");
    //Serial.print(payload.linear);
    //Serial.print(" Angular: ");
    //Serial.println(payload.angular);
    convert converter;
    converter.msg = payload;
    //Serial.print("\t\tPayload: ");
    for(int i = 0; i < sizeof(payload); i++){
      //Serial.print( int(converter.buffer[i]) ); Serial.print(", ");
      
    }
    //Serial.println();
    sendMsg(payload, sizeof(payload));
  }
  int loopTime = millis() - startTime;
  if(loopTime < PERIOD){
    //Serial.print("Delaying for: ");
    //Serial.println(PERIOD - loopTime);
    delay(PERIOD - loopTime);
  }
  startTime = millis();
}

void blink(int time){
  digitalWrite(LED_PIN, HIGH);
  delay(time);
  digitalWrite(LED_PIN, LOW);
  delay(time);
}



void sendMsg(RobotMsg data, int length){
  bool result = _radio.send(DESTINATION_RADIO_ID, &data, length, NRFLite::NO_ACK);
  if(!result){
    blink(1000);
//    Serial.println("Error sending");
  }else{
    blink(5);
  }
}



void radioInit(){
  if(!_radio.init(RADIO_ID, RADIO_CE, RADIO_CSN)){
  //  Serial.println("Error communicating with radio");
    while(1) blink(500);
  }
}
