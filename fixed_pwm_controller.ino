#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// Variables for buttons
int upButton = 8;
int downButton = 9;
int selectButton = 10;

// Variables for duty cycle
int dutyCycle = 50;
int maxDutyCycle = 100;
int minDutyCycle = 0;

// Variables for LCD menu
String menuOptions[] = {"Set Duty Cycle", "Run Motor"};
int menuLength = 2;
int currentMenuIndex = 0;
bool inSubMenu = false;

void setup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Set up button pins as inputs with internal pull-up resistors
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  
  // Initialize PWM pin (assuming pin 3 for this example)
  pinMode(3, OUTPUT);
  analogWrite(3, map(dutyCycle, 0, 100, 0, 255)); // Initial PWM output
  
  // Print initial screen
  printScreen();
}

void loop() {
  // Read button states
  int upState = digitalRead(upButton);
  int downState = digitalRead(downButton);
  int selectState = digitalRead(selectButton);
  
  // Debounce delay - simple implementation
  delay(50);
  
  // Check if select button is pressed
  if (selectState == LOW) {
    if (!inSubMenu) {
      // Enter submenu
      inSubMenu = true;
      currentMenuIndex = 0;
    } else {
      // Handle submenu selection
      if (currentMenuIndex == 0) { // Set Duty Cycle
        // We're already in the duty cycle adjustment mode
        // Just stay in submenu
      }
    }
    printScreen();
    delay(200); // Prevent multiple selections
  }
  
  // Check if we're in submenu for duty cycle adjustment
  if (inSubMenu && currentMenuIndex == 0) {
    if (upState == LOW) {
      dutyCycle += 5;
      if (dutyCycle > maxDutyCycle) {
        dutyCycle = maxDutyCycle;
      }
      updatePWMDisplay();
      analogWrite(3, map(dutyCycle, 0, 100, 0, 255)); // Update PWM output
      Serial.print("Duty Cycle increased to: ");
      Serial.println(dutyCycle);
      delay(200); // Prevent rapid changes
    }
    
    if (downState == LOW) {
      dutyCycle -= 5;
      if (dutyCycle < minDutyCycle) {
        dutyCycle = minDutyCycle;
      }
      updatePWMDisplay();
      analogWrite(3, map(dutyCycle, 0, 100, 0, 255)); // Update PWM output
      Serial.print("Duty Cycle decreased to: ");
      Serial.println(dutyCycle);
      delay(200); // Prevent rapid changes
    }
  } else if (inSubMenu) {
    // Menu navigation when in submenu but not in duty cycle adjustment
    if (upState == LOW) {
      currentMenuIndex--;
      if (currentMenuIndex < 0) {
        currentMenuIndex = menuLength - 1;
      }
      printScreen();
      delay(200);
    }
    
    if (downState == LOW) {
      currentMenuIndex++;
      if (currentMenuIndex >= menuLength) {
        currentMenuIndex = 0;
      }
      printScreen();
      delay(200);
    }
  }
  
  // Check for exiting submenu (long press on select or separate back button logic)
  // For simplicity, we'll just handle the main menu navigation here
  if (selectState == LOW && inSubMenu && currentMenuIndex == 1) { // Run Motor selected
    // Run motor operation
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motor Running");
    lcd.setCursor(0, 1);
    lcd.print("Duty: ");
    lcd.print(dutyCycle);
    lcd.print("%");
    analogWrite(3, map(dutyCycle, 0, 100, 0, 255)); // Ensure PWM is set
    delay(200);
  }
}

void printScreen() {
  lcd.clear();
  if (!inSubMenu) {
    // Main menu
    lcd.setCursor(0, 0);
    lcd.print("Select Option:");
    lcd.setCursor(0, 1);
    lcd.print("> ");
    lcd.print(menuOptions[currentMenuIndex]);
  } else {
    if (currentMenuIndex == 0) { // Set Duty Cycle
      updatePWMDisplay();
    } else { // Run Motor
      lcd.setCursor(0, 0);
      lcd.print("Run Motor");
      lcd.setCursor(0, 1);
      lcd.print("Press SELECT");
    }
  }
}

void updatePWMDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Duty Cycle:");
  lcd.setCursor(0, 1);
  lcd.print(dutyCycle);
  lcd.print("%");
  lcd.print(" ");
  // Show visual indicator of duty cycle with bars
  int numBars = dutyCycle / 10; // Each bar represents 10%
  for (int i = 0; i < numBars; i++) {
    lcd.print((char)219); // Solid block character
  }
  for (int i = numBars; i < 10; i++) {
    lcd.print(' '); // Empty space for remaining
  }
}