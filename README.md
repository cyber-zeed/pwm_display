# pwm_display
Based on the example of dfRobot Display Shield
Simple Variable PWM for arduino
 This is for use with dfRobot Display Shield with 5 bottons SKU:DFR0009
 This display have 2 different versions, if you have the 1.0 version, its ok but if you have the 1.1 version you need tu uncomment this lines:
 
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 250)  return btnUP; 
     if (adc_key_in < 450)  return btnDOWN; 
     if (adc_key_in < 650)  return btnLEFT; 
     if (adc_key_in < 850)  return btnSELECT;   
    
And comment this lines:

    //if (adc_key_in < 50)   return btnRIGHT;  
    //if (adc_key_in < 195)  return btnUP; 
    //if (adc_key_in < 380)  return btnDOWN; 
    //if (adc_key_in < 555)  return btnLEFT; 
    //if (adc_key_in < 790)  return btnSELECT;   
   
Visit my page! http://www.eletroforum.com.br
