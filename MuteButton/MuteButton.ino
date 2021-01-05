/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

/* Defines */
#define LED_PIN (0)
#define BUTTON_PIN (2)
#define BUTTON_DEBOUNCE_DELAY (100)

/* Types */
enum LedState {
  LED_ON,
  LED_OFF,
  LED_TOGGLE_SLOW = 500,
  LED_TOGGLE_FAST = 200
};

enum ButtonState {
  BUTTON_PRESSED,
  BUTTON_LONG_PRESSED = 2000,
  BUTTON_RELEASED
};

struct locals {
  LedState ledState;
  ButtonState buttonState;
  
  unsigned long lastToggle;
  
  int lastButtonState;
  unsigned long lastDebounceTime;
};

/* Variables */
static struct locals l;

/* Prototypes */
static void handleLed();
static void handleButton();

/* Implementations */
void setup() {
  l.ledState = LED_OFF;
  l.buttonState = BUTTON_RELEASED;
  l.lastButtonState = LOW;
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  handleLed();
  handleButton();
  
  if(l.buttonState == BUTTON_PRESSED)
  {
    l.ledState = LED_TOGGLE_SLOW;
  }
  else if (l.buttonState == BUTTON_RELEASED)
  {
    l.ledState = LED_TOGGLE_FAST;
  }
}

static void handleLed() {
  switch (l.ledState)
  {
    case LED_OFF:
      digitalWrite(LED_PIN, HIGH);
      break;
    case LED_ON:
      digitalWrite(LED_PIN, LOW);
      break;
    case LED_TOGGLE_SLOW:
    case LED_TOGGLE_FAST:
      if ((millis() - l.lastToggle) > l.ledState) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        l.lastToggle = millis();
      }
      break;
  }
}

static void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != l.lastButtonState)
  {
    // reset the debouncing timer
    l.lastDebounceTime = millis();
  }

  if ((millis() - l.lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
    if(reading == LOW) {
      l.buttonState = BUTTON_PRESSED;
    } else  if(reading == HIGH) {
      l.buttonState = BUTTON_RELEASED;
    }
  }

    // save the reading. Next time through the loop, it'll be the lastButtonState:
  l.lastButtonState = reading;
}
