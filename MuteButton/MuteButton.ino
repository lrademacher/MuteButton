/* Includes */
#include "DigiKeyboard.h"

/* Defines */
#define LED_PIN (0)
#define BUTTON_PIN (2)
#define BUTTON_DEBOUNCE_DELAY (50)

#define KEY_OE 0xBB

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
  bool muted;
  
  LedState ledState;
  ButtonState buttonState;
  ButtonState lastButtonState;

  unsigned long lastLedToggle;

  int lastButtonReading;
  unsigned long lastDebounceTime;
};

/* Variables */
static struct locals l;

/* Prototypes */
static void handleLed();
static void debounceButton();
static void handleButton();

/* Implementations */
void setup() {
  l.ledState = LED_OFF;
  l.buttonState = l.lastButtonState = BUTTON_RELEASED;
  l.lastButtonReading = LOW;

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.sendKeyStroke(0);
}

void loop() {
  handleLed();
  debounceButton();
  handleButton();
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
      if ((millis() - l.lastLedToggle) > l.ledState) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        l.lastLedToggle = millis();
      }
      break;
  }
}

static void debounceButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != l.lastButtonReading)
  {
    // reset the debouncing timer
    l.lastDebounceTime = millis();
  }

  if ((millis() - l.lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
    if (reading == LOW) {
      l.buttonState = BUTTON_PRESSED;
      if ((millis() - l.lastDebounceTime) > BUTTON_LONG_PRESSED) {
        l.buttonState = BUTTON_LONG_PRESSED;
      }
    } else  if (reading == HIGH) {
      l.buttonState = BUTTON_RELEASED;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonReading:
  l.lastButtonReading = reading;
}

static void handleButton() {
  if(l.buttonState != l.lastButtonState)
  {
    if (l.buttonState == BUTTON_PRESSED)
    {
      // toggle mute state
      mute(!l.muted);
      l.ledState = l.muted ? LED_ON : LED_OFF;
    }
    else if (l.buttonState == BUTTON_LONG_PRESSED)
    {
      if(!l.muted)
      {
        // push-to-talk detected  
        l.ledState = LED_TOGGLE_FAST;
      }
    }
    else if (l.buttonState == BUTTON_RELEASED)
    {
      if(l.lastButtonState == BUTTON_LONG_PRESSED && !l.muted)
      {
        // finished push-to-talk
        mute(true);
        l.ledState = LED_ON;
      }
    }

    l.lastButtonState = l.buttonState;
  }
}

static void mute(bool activate) {
  l.muted = activate;

  // Send CTRL+MOD+M
  DigiKeyboard.sendKeyStroke(KEY_M, MOD_GUI_LEFT | MOD_CONTROL_LEFT);
}
