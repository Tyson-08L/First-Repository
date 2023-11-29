// Code for wallet tester presented by Tyson Laevens on November 29th, 2023 at the Georgian College Orillia Campus during Cybercrime with Eleanor Gittens 
// Anything written in this grey colour text after "//" is just a note from me (Tyson) explaining in really simple terms what that line of code is doing
//Arduino (the microproccesor brand) projects are written in C++ (a coding language)
//Download the Arduino IDE on your computer to program the Arduino, or Read the code on your personal computer

#include <SPI.h> //Simplifies communication with RFID
#include <MFRC522.h> //Simplifies RFID reader programming
#include <Wire.h> //Simplifies communication via I2C
#include <LiquidCrystal_I2C.h> //Simplifies LCD programing

#define PIN_LED_RED 7 //Tells arduino where RED LED + is connected
#define PIN_LED_GREEN 6 //Tells arduino where GREEN LED + is connected
#define PIN_RST 9 //Tells arduino where RFID RST PIN is connected
#define PIN_SDA 10//Tells arduino where RFID SDA PIN is connected
#define PIEZO_BUZZER_PIN 8 //Tells arduino where SPEAKER + is connected

LiquidCrystal_I2C lcd(0x27, 16, 2);//Tells above mentioned LCD library the LCD size
MFRC522 rfid(PIN_SDA, PIN_RST);//Tells above mentioned MFRC522 library connection setup of RFID reader

bool testNowSoundPlayed = false;//Tells arduino not to play the test now sound constantly (while the display says "Test Now")

void setup() //Done once at the begining of the code
{
  SPI.begin(); //Turns on communications
  rfid.PCD_Init(); //Starts rfid reader
  pinMode(PIN_LED_RED, OUTPUT);//Defines the RED led as an output device
  pinMode(PIN_LED_GREEN, OUTPUT);//Defines the GREEN led as an output device
  pinMode(PIEZO_BUZZER_PIN, OUTPUT);//Defines the SPEAKER as an output device

  lcd.init(); //starts the lcd
  lcd.backlight();//turns on the lcd's backlight
  lcd.setCursor(1, 0); //tells lcd where to start typing out the text
  lcd.print("Tyson Laevens"); //The owners name to be written on the display
  lcd.setCursor(2, 1);//tells lcd where to start typing out the text
  lcd.print("Cyber Crime");//The class to be written on the display
  digitalWrite(PIN_LED_GREEN, HIGH);//Turns on green led
  playTestNowSound();//plays happy sound defined later in code
  delay(1000);//sets the duration
  lcd.clear();//clears the display
  lcd.setCursor(1, 0);//tells lcd where to start typing out the text
  lcd.print("Wallet Tester");//project name to be displayed
  lcd.setCursor(3, 1);//tells lcd where to start typing out the text
  lcd.print("TEST NOW");//tells user to test (on the display)
}

void playTone(int frequency, int duration)//defines how inputted variables will be intereoreted 
{
  tone(PIEZO_BUZZER_PIN, frequency, duration); //tells arduino what "tone" means (speaker on at specific pwm frequency for specifc amount of time)
  delay(duration + 50);//sets time duration
  noTone(PIEZO_BUZZER_PIN);//tells arduino how to stop the sound
}

void playTestNowSound() // the setup for the first sound the arduino makes on powerup
{
  if (!testNowSoundPlayed)
  {
    playTone(1050, 200); //high pitched and short
    playTone(1575, 500);// evener higher pitched and longer
    testNowSoundPlayed = true; //affirms that the sound was played
  }
}

void loop()//what occurs over and over and over and over again 
{
  if (rfid.PICC_IsNewCardPresent()) //checks for card presence
  {
    if (rfid.PICC_ReadCardSerial())//reads presented cards UID
    {
      digitalWrite(PIN_LED_GREEN, LOW);//tells the green led to turn off
      digitalWrite(PIN_LED_RED, HIGH);//tells the red led to turn on
      lcd.clear();//erases the display
      lcd.setCursor(2, 0);//defines where on the display to write
      lcd.print("Test Failed");//writes test failed when a UID is pulled
      playTone(375, 200); //short and low tone
      playTone(200, 500);//longer but lower tone
      delay(1000);//length of time for the tone and display to remain in the above state

      lcd.clear();// clears display
      lcd.setCursor(0, 0);// defines where to start displaying text
      lcd.print("UID:");//writes out the name of the info its pulling from the card (Google what a "UID" is on a bank card)
      for (int i = 0; i < 4; ++i) //tells the display how to interpret all the info pulled from the card 
      {
        lcd.print(rfid.uid.uidByte[i], HEX);//converts what it interpretted into a common format (hexidecimal)
        lcd.print(" ");//Prints the UID in hexidecimals to the LCD
      }
      delay(4000);//keeps it there for 4 seconds
      lcd.clear();//erases the display
       digitalWrite(PIN_LED_RED, LOW); //turns off the red led
  lcd.setCursor(1, 0); //tells the display where to start the text it displays
  lcd.print("Wallet Tester"); //displays info about the project
  lcd.setCursor(3, 1);//tells the display where to start the text it displays
  lcd.print("TEST NOW");//displays instruction for user
      lcd.print("                ");//different way of clearing the display for restarting the code from the top
      digitalWrite(PIN_LED_GREEN, HIGH); //turns the green led on
      playTestNowSound(); //plays the happy high pitched sound when it starts up
      delay(1000); //how long the sound is played for
    }
  }
}//code restarts from the top
