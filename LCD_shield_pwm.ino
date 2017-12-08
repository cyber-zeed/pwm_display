// Programa : Simple Variable PWM for Arduino
// Autor : Flavio Lira

#include <Wire.h>
#include <LiquidCrystal.h>

// Inicializa o display no endereco 0x27
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int pwmpin = 11; //The N-Channel MOSFET is on digital pin 3
int pwm_duty = 0; //Power level fro 0 to 100%
int power = map(power, 0, 100, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET 
int lcd_key     = 0;
int adc_key_in  = 0;
 
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 
 
    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result
 
    if (adc_key_in > 1000) return btnNONE; 
 
    // For V1.1 us this threshold
    //if (adc_key_in < 50)   return btnRIGHT;  
    //if (adc_key_in < 250)  return btnUP; 
    //if (adc_key_in < 450)  return btnDOWN; 
    //if (adc_key_in < 650)  return btnLEFT; 
    //if (adc_key_in < 850)  return btnSELECT;  
 
   // For V1.0 comment the other threshold and use the one below:
   
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;   
   
 
    return btnNONE;                // when all others fail, return this.
}

void setup()
{
//TCCR0B = TCCR0B & 0b11111000 | 0x02;
TCCR1B = TCCR1B & 0b11111000 | 0x02;
TCCR2B = TCCR2B & 0b11111000 | 0x02; 
//TCCR3B = TCCR3B & 0b11111000 | 0x02;
 lcd.begin (16,2);
 Serial.begin(9600);
}
 
void loop()
{
   
  lcd_key = read_LCD_buttons();   // read the buttons
 
   switch (lcd_key){               // depending on which button was pushed, we perform an action
 
       case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
          lcd.clear();
          //lcd.setCursor(8,0);
          pwm_duty = 100;
          power = map(pwm_duty, 0, 100, 0, 255);
          lcd.print("Duty Cycle 100%");
          delay (100);
          lcd.clear();
          break;
         }
       case btnLEFT:{
          lcd.clear();
          //lcd.setCursor(8,0);
          pwm_duty = 0;
          power = map(pwm_duty, 0, 100, 0, 255);
          lcd.print("Duty Cycle 0%");
          delay (100);
          lcd.clear();
          break;
       }    
       case btnUP:{
             lcd.setCursor(8,0);
             pwm_duty += 5;
             if(pwm_duty > 100) pwm_duty = 100;
             power = map(pwm_duty, 0, 100, 0, 255);
             lcd.print("+5% Duty");  //  push button "UP" and show the word on the screen
             delay (100);
             lcd.clear();
             break;
       }
       case btnDOWN:{
             lcd.setCursor(8,0);
             pwm_duty -= 5;
             if(pwm_duty < 0)   pwm_duty = 0;
             power = map(pwm_duty, 0, 100, 0, 255);
             lcd.print("-5% Duty");  //  push button "DOWN" and show the word on the screen
             delay (100);
             lcd.clear();
             break;
       }
       case btnSELECT:{
          lcd.clear();
          //lcd.setCursor(8,0);
          pwm_duty = 50;
          power = map(pwm_duty, 0, 100, 0, 255);
          lcd.print("Duty Cycle 50%");
          delay (100);
          lcd.clear();
          break;
       }
       //case btnNONE:{
       //      lcd.print("NONE  ");  //  No action  will show "None" on the screen
       //      break;
       //}
   }

  
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Power=");
  lcd.print(power);
  lcd.setCursor(0,1);
  lcd.print("Duty Cycle=");
  lcd.print(pwm_duty);
  lcd.print("%");
  delay (100);
  char option;

  if(Serial.available() > 0)
  {
    option = Serial.read();
    if(option == 'a') 
    pwm_duty += 5;
    else if(option == 'z')
    pwm_duty -= 5;
    if(pwm_duty > 100) pwm_duty = 100;
    if(pwm_duty < 0)   pwm_duty = 0;

  power = map(pwm_duty, 0, 100, 0, 255);
  }

Serial.print("Power=");
Serial.print(power);
Serial.print(" pwm_duty=");
Serial.println(pwm_duty);


analogWrite(pwmpin, power); //Write this new value out to the port



}
  //delay(10);
  //lcd.setBacklight(LOW);
  //delay(10);

