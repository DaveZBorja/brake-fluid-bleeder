/*
 * Dear programmer:
 * 
 * When I wrote this code, only god and
 * I knew how it worked.
 * Now, only god knows it!
 * 
 * Therefore, if are trying to optimize
 * this routine and it fails (most surely),
 * please increase this counter as a
 * Warning for the next person:
 * 
 * total_hours_wasted_here = 658
 * 
 */

/*
 *
 * Don't add library here or any instance of class.
 * 
 */
 
#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 20, 4); // Default address of most PCF8574 modules, change according

char ON[] = "ON";
char OFF[] = "OFF";
char BANNER[] = "Brake Fluid Bleeder";
char PROC1[] = "Automatic";
char PROC2[] = "Manual";
char INITIALIZE[] = "Auto Bleeding.....";
char TIMER[] = "Timer:";
char MIN[] = "Min ";
char PSI[] = "Psi: ";

// button pin

const int AutoPin = 8; // constant pin for 
const int ManPin = 9; // constant pin for 

const int AutonOn = 1; // automatic button
const int ManOn = 2;  // manual button 

const int relay1 = 11; // auto relay
const int relay2 = 6; // //man relay

const int relay1_Stat = 5;  // automatic led violet
const int relay2_Stat = 2; // manual led green

const int beep = 4; // buzzer

int status; // status variable 

// Potentiometer pin

const int potentiometer = A0; //constant pin for Potentiometer Speed control
const int raw_psi = A1; // raw_psi variable that hold analog data  

void setup() {

    Serial.begin(9600); // serial communication with the speed of 9600
    pinMode(beep, OUTPUT);

    pinMode(relay1_Stat, OUTPUT);
    pinMode(relay2_Stat, OUTPUT);

    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

    // button pin as input pullup (button will be low when pressed

    pinMode(AutoPin, INPUT_PULLUP);
    pinMode(ManPin, INPUT_PULLUP);
   
    lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
    lcd.backlight(); // this stop the library(LCD_I2C) from calling Wire.begin()

    banner(); 
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
}

void loop() {
   

    /*
     * 
     * TODO: - Optimize this routine
     * 
     */
    
    // int pot_value = analogRead(A0);
    // int min = map(pot_value, 0, 1024, 0, 15000 );
    
    banner(); // function call 

    int Automatic = digitalRead(AutoPin);
    int Manual = digitalRead(ManPin);  

     
    if (Automatic == 0)
       {
           lcd.clear();
          status = AutonOn;
       }if(Manual == 0)
        {
          lcd.clear();
          status = ManOn;
        }

      /*
       * TODO: - refactor this routine 
       *       - add tests 
       * 
       */

    
    if(status == 1){
        banner();
          lcd.setCursor(6, 1); // Or setting the cursor in the desired position.
          lcd.print(PROC1);
          lcd.setCursor(0, 2); // Or setting the cursor in the desired position.
          lcd.print(INITIALIZE);
        
  
          lcd.setCursor(10, 3); // Or setting the cursor in the desired position.
          lcd.print("State: "); 
          lcd.print(ON);     

          digitalWrite(relay1, LOW);
          digitalWrite(relay1_Stat, HIGH);
          
    }else if(status == 2){
        banner();
        lcd.setCursor(6, 1); // Or setting the cursor in the desired position.
        lcd.print(PROC2);
        
        if (Manual == 0){
          
           lcd.setCursor(10, 3); // Or setting the cursor in the desired position.
           lcd.print("State: "); 
           lcd.print(ON);
           digitalWrite(relay1, HIGH);
           digitalWrite(relay1_Stat, LOW);

           digitalWrite(relay2, LOW);
           digitalWrite(relay2_Stat, HIGH);
           
           //digitalWrite(beep, HIGH);

        }else{
          banner();
          digitalWrite(relay2, HIGH);

          //digitalWrite(beep, LOW);

          digitalWrite(relay2_Stat, LOW);
          lcd.setCursor(10, 3); // Or setting the cursor in the desired position.
          lcd.print("State: "); 
          lcd.print(OFF);
        
            }
        delay(50);

    }else{
       digitalWrite(relay1, HIGH);
       digitalWrite(relay2, HIGH);

       digitalWrite(relay1_Stat, LOW);
       digitalWrite(relay2_Stat, LOW);
       //digitalWrite(beep, LOW);    
    }

}  

void banner(){
      //int pot_value = analogRead(A0);
      //int min = map(pot_value, 0, 1024, 0, 15 );

      //int ps = analogRead(A1);
      //float map_psi = map(ps, 0, 1024, 0, 2 );

      lcd.setCursor(0, 0); // Or setting the cursor in the desired position.
      lcd.print(BANNER);

      float OilPressure = (float(OilPressure1(analogRead(A1))));
      float map_press = map(OilPressure, 0, 1024, 0, 2);
      lcd.setCursor(0, 3);
      //lcd.print(float(OilPressure));
      lcd.print(map_press);
      lcd.print(" PSI");
}


float OilPressure1(float RawADC)
{
  float Pressure;
  
  Serial.println(RawADC);
  Pressure = (RawADC/1024.00)*5; // calculate actual voltage across the sensor
  Pressure = (-.177 + sqrt(.03133-(4*-.0065*(.052-Pressure))))/-.013; // quadratic formula to get a more acurate reading based on the curve
  Pressure = Pressure * 14.7; // convert from bar to PSI
  return Pressure; // return value
}


