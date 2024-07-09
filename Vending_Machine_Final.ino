/*      Vending Machine code 
This project done by :
  Hamza Waseem
  Mohammed Zaloom
  Mohammed Haswah
*/

// Include librarys
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <Keypad.h>
#include <Adafruit_PWMServoDriver.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
Servo myservo; 
String password = "4321";
String pad;
char keypressed;


// Declare PWM  object and signals for servo motors
 Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_FREQ 50

// Set the LCD_I2C adress
LiquidCrystal_I2C lcd(0x27,16,2);

// Constants for row and column sizes
const byte ROWS = 4;                // Keypad rows
const byte COLS = 4;                // Keypad Columns
const int infrared0 = A0;           // First IR to check the coin
const int infrared1 = 11;           // Second IR to check the coin 
const int HX711_dout = 13;           // mcu > HX711 dout pin
const int HX711_sck = 12;            // mcu > HX711 sck pin
unsigned long startTime = 0;        // Start detecting the coin 
unsigned long Coin_Time;            // Coin timming 
float Coin_Time_Type;               // The value of the coin detected by IR
float Coin_Weight_Type;             // The calue of the coin detected by the LoadCell
String Coin_Value;                  // The real value of the coin
char customKey;                     // Virable for Keybad selection
const int trigPin=10;
const int echoPin=11;
long duration;
int distance;
int Motor_Num;
 
 // HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// HX711 Starting Virables:
const int calVal_eepromAdress = 0;
unsigned long t = 0;

// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
 
// Connections to Arduino
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
 
// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
 
void setup(){
    myservo.attach(53);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);                     // For Using the Serial monitor
    pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);
    lcd.init();                              // Start the LCD
    lcd.backlight();   
                      // Turn on the back light 
    pwm.begin(); 
                               // Start the PCA9685 (Motors I2C)
    pwm.setOscillatorFrequency(27000000);
        //Chose the Frequency for the PCA9685
    pwm.setPWMFreq(SERVO_FREQ);              // Analog servos run at ~50 Hz updates
    pinMode(infrared0, INPUT);
    pinMode(infrared1, INPUT);
    
    LoadCell.begin();    
                      //Start the LoadCell
    float calibrationValue;                 // calibration value (see example file "Calibration.ino")
    calibrationValue = 696.0;               // uncomment this if you want to set the calibration value in the sketch
    #if defined(ESP8266)|| defined(ESP32)
    EEPROM.begin(512);                      // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
    #endif
    
    EEPROM.get(calVal_eepromAdress, calibrationValue);     // uncomment this if you want to fetch the calibration value from eeprom
    unsigned long stabilizingtime = 2000;                   // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare = true;  
                                     //set this to false if you don't want tare to be performed in the next step
    LoadCell.start(stabilizingtime, _tare);
    
    if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    //while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue);               // set calibration value (float)
    Serial.println("Startup is complete");
  }
  
  
  yield();                                                // Don't go to the loop until everything is done processing 
}
 
void loop(){
  myservo.write(0);  
    //delay(200) ;         // tell servo to go to position in variable 'pos'
   lcd_HI();
   IR_Check();
   Load_Cell();
   Servo_Coin();
   Coin_Type(Coin_Weight_Type);
   lcd_Coin_Value(); 
   
   Chose_Num();

if (customKey=='1' ) {
Motor_Num = customKey - '0';
Motor_Num--;
      lcd_Motor("1");
      pwm.setPWM(0, 0, 290);
      Object_Recieve(Motor_Num);
      //delay(6100);
     // pwm.setPWM(0, 0, 0);

}

     

    else if (customKey=='2' ) {
      Motor_Num = customKey - '0';
      Motor_Num--;
      lcd_Motor("2");
      pwm.setPWM(1, 0, 265);
      Object_Recieve(Motor_Num);

      //delay(6100);
      //pwm.setPWM(1, 0, 0);


    }
     
      
      
    else if(customKey=='3'){
      Motor_Num = customKey - '0';
      Motor_Num--;
       lcd_Motor("3");
       pwm.setPWM(2, 0, 290);
       Object_Recieve(Motor_Num);

       //delay(6100);
       //pwm.setPWM(2, 0, 0);
    } 
      

      
      
    else if(customKey=='4'){
      Motor_Num = customKey - '0';
      Motor_Num--;
      lcd_Motor("4");
      pwm.setPWM(3, 0, 265);
      Object_Recieve(Motor_Num);

      //delay(6100);
      //pwm.setPWM(3, 0, 0);
    }
     

     

else{
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("Wrong num");
      lcd.setCursor(0,1);
      lcd.print("Try Again");
      lcd.blink();
      Chose_Num();
}
      

     
   

   lcd_End();

  
}// The end of the loop


                                                              //Functions\\   


// Ask the user to enter a coin
void lcd_HI(){
  lcd.print("Please enter");
  lcd.setCursor(0,1);
  lcd.print("your coin O_O");
  lcd.blink();
}

// Check the type of the coin using the IR sensor
void IR_Check(){
int x = true;
while(x){
int A =analogRead(infrared0);
if(A<=900)
x=false;
}


}

void startCounter() {
  startTime = millis();
  Serial.println("Counter started!");
}

void stopCounter() {
  unsigned long elapsedTime = millis() - startTime;
  Serial.print("Counter stopped. Elapsed time: ");
  Serial.print(elapsedTime );  // Convert milliseconds to seconds
  Serial.println(" milisecond");
  Coin_Time = elapsedTime;
}


//   Check the Value of the coin using the weight sensor 
void Load_Cell() {
  for ( unsigned long count = 0; count <= 500000; count++) {
    static boolean newDataReady = 0;
    const int serialPrintInterval = 0; // increase value to slow down serial print activity

    // check for new data/start the next conversion:
    if (LoadCell.update()) newDataReady = true;

    // get smoothed value from the dataset:
    if (newDataReady) {
      if (millis() > t + serialPrintInterval) {
        float i = LoadCell.getData();
        Serial.print("Load_cell output val: ");
        Serial.println(i);
        newDataReady = 0;
        t = millis();
      }
    }

    // receive command from the serial terminal, send 't' to initiate the tare operation:
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 't') LoadCell.tareNoDelay();
    }

    // check if the last tare operation is complete:
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
    }
  }
    
  float i = LoadCell.getData(); // Assuming you want to get the data after the loop

  if (i >= 9.3 && i <= 9.9)
    Coin_Weight_Type = 0.5;
  else if (i >= 7.1 && i <= 7.7)
    Coin_Weight_Type = 0.25;
  else if (i >= 7.8 && i <= 8.2)
    Coin_Weight_Type = 0.1;
  else if (i >= 4.7 && i <= 5.3)
    Coin_Weight_Type = 0.05;
  else
    Coin_Weight_Type = 0;
  }


// The final Check for the value of the coin by comparing both IR and LoadCell results  
void Coin_Type(float W){
  Coin_Value="0";
  if( W==0.5 ){Coin_Value = "0.5"; Serial.print("The value is 0.5");}
  
  else if( W==0.25){Coin_Value = "0.25"; Serial.print("The value is 0.25");}
  
  else if( W==0.1){Coin_Value = "0.1"; Serial.print("The value is 0.10");}
  
  else if( W==0.05){Coin_Value = "0.05"; Serial.print("The value is 0.05");}
  /*
  T==0.5 && 
T==0.25 && 
  T==0.1 && 
  T==0.05 && 
  */
  else{
   lcd.clear();
   lcd.print(" Fake coin");
   lcd.setCursor(0, 1);
   lcd.print(" X_X");
   Serial.print("value is 0");
/*
Here we will rite the code for returning the fake coin to the user
*/
}

}

// Print the value of the coin that he inserted
void lcd_Coin_Value(){
 lcd.clear();
 lcd.print("You have entered");
 lcd.setCursor(0,1);
 lcd.print("   " + Coin_Value); //supposed to be lcd.print(Coin_by_the_user);
 }

 
// Let the User chose number using the Keypad
void Chose_Num(){
  bool x =true;

  while(x){
    customKey = customKeypad.getKey();

    if(customKey){   
      Serial.println(customKey);
      x=false;
        }
  }

 }
 
//If chose the number of the item that you want
void lcd_Motor(String a){
 lcd.clear();
 lcd.noBlink();
 lcd.print("You have chosen");
 lcd.setCursor(0,1);
 lcd.print("number "+ a );
 delay(2000);
 lcd.clear();
 lcd.print("Ready at...");
 lcd.setCursor(0,1);
 lcd.print(3);
 delay(1000);
 lcd.setCursor(0,1);
 lcd.print(2);
 delay(1000);
 lcd.setCursor(0,1);
 lcd.print(1);
 delay(1000);
 lcd.clear();
 lcd.print("Loading...");
 lcd.blink();
}

// Thank the user and finish the process
void lcd_End(){
 lcd.clear();
 lcd.print("    Thank you" );
 lcd.setCursor(0,1);
 lcd.print("    Goodbye ^_^");
 delay(3000);
 lcd.clear();
}


   void Object_Recieve(int Motor_Num){
bool x = true;
while(x){
digitalWrite(trigPin,LOW);
delayMicroseconds(2);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);

duration=pulseIn(echoPin,HIGH);
distance=duration*0.034/2;
Serial.print("Distance ");
Serial.println(distance);
delay(10);
if(distance <=20){

      pwm.setPWM(Motor_Num, 0, 0);
      x=false;
}



}

   }
      void Servo_Coin(){


    myservo.write(0);  
    delay(200)  ;         // tell servo to go to position in variable 'pos'
  
    myservo.write(180);   
        delay(200)  ;       
        
    myservo.write(0);   
        delay(200)  ; 
        // tell servo to go to position in variable 'pos'

      }

/*
switch (customKey) {
    case '1':
      lcd_Motor("1");
      pwm.setPWM(0, 0, 265);
      delay(6100);
      pwm.setPWM(0, 0, 0);

      break;

    case '2':
      lcd_Motor("2");
      pwm.setPWM(1, 0, 265);
      delay(6100);
      pwm.setPWM(1, 0, 0);

      break;
      
    case '3':
       lcd_Motor("3");
       pwm.setPWM(2, 0, 265);
       delay(6100);
       pwm.setPWM(2, 0, 0);

      break;
      
    case '4':
      lcd_Motor("4");
      pwm.setPWM(3, 0, 265);
      delay(6100);
      pwm.setPWM(3, 0, 0);

      break;

default:
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("Wrong num");
      lcd.setCursor(0,1);
      lcd.print("Try Again");
      delay(2000);
      Chose_Num();

      break;
   }

*/





/*
    bool Check = false;
    while(!Check){
     if (digitalRead(infrared0) == LOW ) {
     startCounter();
    
     while(true){
      if (digitalRead(infrared1) == LOW) {
      stopCounter();
      Check = true;
      if(  Coin_Time >=100 &&  Coin_Time <=3000)
       Coin_Time_Type = 0.5;
      else if (Coin_Time >=0 &&  Coin_Time <=0)
       Coin_Time_Type = 0.25;
      else if (Coin_Time >=0 &&  Coin_Time <=1)
       Coin_Time_Type = 0.1;
      else if (Coin_Time >=2 &&  Coin_Time <=0)
       Coin_Time_Type = 0.05;
      else 
       Coin_Time_Type = 0;

    break;

    }}}}*/



