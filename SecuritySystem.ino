#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <MFRC522.h>
#include <Servo.h>

#define ULTRASONIC_TRIGGER_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define MOTION_SENSOR_PIN 4
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9
#define SERVO_PIN 5


#define DISTANCE_THRESHOLD 50
#define MOTION_DELAY 10000    

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Servo doorLockServo;

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

void setup() {
  Serial.begin(9600);
  bno.begin();
  doorLockServo.attach(SERVO_PIN);

  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(RFID_SS_PIN, OUTPUT);
  pinMode(RFID_RST_PIN, OUTPUT);

  mfrc522.PCD_Init();
}

void loop() {
  if (detectMotion()) {
    Serial.println("Motion detected!");


    captureSecurityEvent();

    delay(MOTION_DELAY);
  }


  if (detectUnauthorizedAccess()) {
    Serial.println("Unauthorized access detected!");


    captureSecurityEvent();


    lockDoor();
    

    delay(10000); 
  }

  delay(1000); 
}

bool detectMotion() {
  return digitalRead(MOTION_SENSOR_PIN) == HIGH;
}

bool detectUnauthorizedAccess() {

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardID = getCardID();
    mfrc522.PICC_HaltA();
    return !grantAccess(cardID);
  }
  return false;
}

void captureSecurityEvent() {
  
  Serial.println("Capturing security event...");

  if (!cam.begin()) {
  Serial.println(F("Failed to start camera!"));
  return;
  }
  cam.setImageSize(VC0706_640x480);

  uint8_t imgNum = cam.takePicture();
  (imgNum == 0) {
  Serial.println(F("Failed to capture image!"));
  return;
  }


  cam.resumeVideo();
}
void lockDoor() {
  doorLockServo.write(90); 
  delay(2000); 
  doorLockServo.write(0);

String getCardID() {
  String cardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    cardID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  return cardID;
}

bool grantAccess(String cardID) {
  return !cardID.isEmpty();
}
