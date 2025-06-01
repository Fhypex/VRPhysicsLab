#include <BleKeyboard.h>

// Create a BleKeyboard instance with device name, manufacturer and initial battery level
BleKeyboard bleKeyboard("ESP32 Controller", "DIY", 100);

// Joystick pins
const int joyX = 33;      // Analog pin for joystick X axis
const int joyY = 32;      // Analog pin for joystick Y axis
const int joyButtonE = 25; // Digital pin for joystick button (if your joystick has one)

const int ledPin = 16;

// Button pins
const int buttonZ = 26; //sol
const int buttonB = 27; //orta
const int buttonV = 14; // aşağı
const int buttonX = 12; // üstt
const int buttonC = 13; // sağ

// Variables to store button states
bool lastJoyButtonEState = HIGH;
bool lastButtonZState = HIGH;
bool lastButtonBState = HIGH;
bool lastButtonVState = HIGH;
bool lastButtonXState = HIGH;
bool lastButtonCState = HIGH;

// Joystick parameters
const int joyThreshold = 500;  // Threshold for joystick movement detection
const int joyCenter = 2048;    // Center value for joystick (adjust based on your ADC resolution)

// Track which arrow keys are currently pressed
bool rightPressed = false;
bool leftPressed = false;
bool upPressed = false;
bool downPressed = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Keyboard");
  
  // Initialize BLE Keyboard
  bleKeyboard.begin();
  
  // Set pin modes for buttons with internal pull-ups
  pinMode(joyButtonE, INPUT_PULLUP);
  pinMode(buttonZ, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonV, INPUT_PULLUP);
  pinMode(buttonX, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("BLE Keyboard ready!");
}

void loop() {
  if(bleKeyboard.isConnected()) {
    digitalWrite(ledPin, HIGH);
    // Read joystick values
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);
    
    // Read button states (buttons are PULLUP, so LOW means pressed)
    bool joyButtonEState = digitalRead(joyButtonE);
    bool buttonZState = digitalRead(buttonZ); 
    bool buttonBState = digitalRead(buttonB);
    bool buttonVState = digitalRead(buttonV);
    bool buttonXState = digitalRead(buttonX);
    bool buttonCState = digitalRead(buttonC);
    
    // Handle joystick as arrow keys with continuous press behavior
    // Handle X-axis (left/right)
    if (xValue > joyCenter + joyThreshold) { 
      // Right movement
      if (!rightPressed) {
        bleKeyboard.press('d');
        rightPressed = true;
        Serial.println("Right arrow pressed");
      }
      // Make sure left is released
      if (leftPressed) {
        bleKeyboard.release('a');
        leftPressed = false;
      }
    } 
    else if (xValue < joyCenter - joyThreshold) {
      // Left movement
      if (!leftPressed) {
        bleKeyboard.press('a');
        leftPressed = true;
        Serial.println("Left arrow pressed");
      }
      // Make sure right is released
      if (rightPressed) {
        bleKeyboard.release('d');
        rightPressed = false;
      }
    } 
    else {
      // Center X position - release both left and right
      if (rightPressed) {
        bleKeyboard.release('d');
        rightPressed = false;
        Serial.println("Right arrow released");
      }
      if (leftPressed) {
        bleKeyboard.release('a');
        leftPressed = false;
        Serial.println("Left arrow released");
      }
    }
    
    // Handle Y-axis (up/down)
    if (yValue < joyCenter - joyThreshold) {
      // Down movement
      if (!downPressed) {
        bleKeyboard.press('s');
        downPressed = true;
        Serial.println("Down arrow pressed");
      }
      // Make sure up is released
      if (upPressed) {
        bleKeyboard.release('w');
        upPressed = false;
      }
    } 
    else if (yValue > joyCenter + joyThreshold) {
      // Up movement
      if (!upPressed) {
        bleKeyboard.press('w');
        upPressed = true;
        Serial.println("Up arrow pressed");
      }
      // Make sure down is released
      if (downPressed) {
        bleKeyboard.release('s');
        downPressed = false;
      }
    } 
    else {
      // Center Y position - release both up and down
      if (upPressed) {
        bleKeyboard.release('w');
        upPressed = false;
        Serial.println("Up arrow released");
      }
      if (downPressed) {
        bleKeyboard.release('s');
        downPressed = false;
        Serial.println("Down arrow released");
      }
    }
    
    // Handle joystick button E
    if (joyButtonEState != lastJoyButtonEState) {
      if (joyButtonEState == LOW) {
        bleKeyboard.press('e');
        Serial.println("E pressed");
      } else {
        bleKeyboard.release('e');
        Serial.println("E released");
      }
      lastJoyButtonEState = joyButtonEState;
      delay(10); // Small delay for debouncing
    }

    // Handle button Z
    if (buttonZState != lastButtonZState) {
      if (buttonZState == LOW) {
        bleKeyboard.press('z');
        Serial.println("Z pressed");
      } else {
        bleKeyboard.release('z');
        Serial.println("Z released");
      }
      lastButtonZState = buttonZState;
      delay(10);
    }
    
    // Handle button X
    if (buttonBState != lastButtonBState) {
      if (buttonBState == LOW) {
        bleKeyboard.press('b');
        Serial.println("B pressed");
      } else {
        bleKeyboard.release('b');
        Serial.println("B released");
      }
      lastButtonBState = buttonBState;
      delay(10);
    }
    
    // Handle button Y
    if (buttonVState != lastButtonVState) {
      if (buttonVState == LOW) {
        bleKeyboard.press('v');
        Serial.println("V pressed");
      } else {
        bleKeyboard.release('v');
        Serial.println("V released");
      }
      lastButtonVState = buttonVState;
      delay(10);
    }
    
    // Handle button X
    if (buttonXState != lastButtonXState) {
      if (buttonXState == LOW) {
        bleKeyboard.press('x');
        Serial.println("X pressed");
      } else {
        bleKeyboard.release('x');
        Serial.println("X released");
      }
      lastButtonXState = buttonXState;
      delay(10);
    }
    
    // Handle button C
    if (buttonCState != lastButtonCState) {
      if (buttonCState == LOW) {
        bleKeyboard.press('c');
        Serial.println("C pressed");
      } else {
        bleKeyboard.release('c');
        Serial.println("C released");
      }
      lastButtonCState = buttonCState;
      delay(10);
    }
  } else {
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
    // If not connected, try to reconnect
    Serial.println("Not connected. Waiting for connection...");
    delay(1000);
  }
}