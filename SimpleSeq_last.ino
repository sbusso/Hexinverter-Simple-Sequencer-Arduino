//  ============================================================
//
//  Program: Arduino 8 step Sequencer using Hexinverter SimpleSeq Hardware.
//  Created:  19 September 2018  by Thopa
//  ============================================================
//
//  License:
//
//  This software is licensed under the Creative Commons
//  "Attribution-NonCommercial license. This license allows you
//  to tweak and build upon the code for non-commercial purposes,
//  without the requirement to license derivative works on the
//  same terms. If you wish to use this (or derived) work for
//  commercial work, please contact Thopa at our website
//  (www.djthopa.net
//
//  For more information on the Creative Commons CC BY-NC license,
//  visit http://creativecommons.org/licenses/

//  ================= start of global section ==================

const int clkIn = 2;   // the digital (clock) input
const int resetclk = 3; //reset the sequencer

const int ledPin1 = 4;  //First 4 leds
const int ledPin2 = 5;
const int ledPin3 = 6;
const int ledPin4 = 7;

const int ledPin5 = 8; // Second 4 leds
const int ledPin6 = 9;
const int ledPin7 = 10;
const int ledPin8 = 11;


byte direction_Array[] {
  0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, // forward
  7, 6, 5, 4, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0, // backwards
  1, 3, 5, 7, 1, 3, 5, 7, 1, 3, 5, 7, 1, 3, 5, 7, // prime
  1, 3, 5, 7, 5, 3, 1, 3, 5, 7, 5, 3, 1, 3, 5, 7, // prime pendulum
  0, 2, 4, 6, 0, 2, 4, 6, 0, 2, 4, 6, 0, 2, 4, 6, // even
  0, 2, 4, 6, 4, 2, 0, 2, 4, 6, 4, 2, 0, 2, 4, 6, // even pendulum
  0, 7, 1, 6, 2, 5, 3, 4, 4, 3, 5, 2, 6, 1, 7, 0, // mamma mia
  7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7 // pendulum
};


byte mode; // this is the direction

byte clock_current;
byte clock_previous;

byte reset_current;
byte reset_previous;

byte clock_count;

////////////////////////////////////////////BUTTON PUSH COUNTER FOR CHANGING PLAYMODES //////////

// this constant won't change:
const int  buttonPin = 13;    // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

///////////////////////////////////RGB LED //////////////////////////


const byte RGB_LED_PIN1 = 14;     // connect green pin of the RGB LED to ~9
const byte RGB_LED_PIN2 = 15;     // connect blue pin of the RGB LED to ~10
const byte RGB_LED_PIN3 = 16;      // connect red pin of the RGB LED to ~11


void setup() {

  //Start Serial for tracking
  Serial.begin(9600);

  pinMode (ledPin1, OUTPUT);
  pinMode (ledPin2, OUTPUT);
  pinMode (ledPin3, OUTPUT);
  pinMode (ledPin4, OUTPUT);
  pinMode (ledPin5, OUTPUT);
  pinMode (ledPin6, OUTPUT);
  pinMode (ledPin7, OUTPUT);
  pinMode (ledPin8, OUTPUT);

  pinMode(clkIn, INPUT);
  pinMode(resetclk, INPUT);

  ////ADDED CODE FOR PUSHBUTTON THAT CHANGES PLAYMODE//

  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);

  /////////////////////////////////////

  pinMode(RGB_LED_PIN1, OUTPUT);
  pinMode(RGB_LED_PIN2, OUTPUT);
  pinMode(RGB_LED_PIN3, OUTPUT);

  /////////////////////////////////

  attachInterrupt(0, isr, RISING);
}

void loop() {


  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      mode++;
      //Serial.println("on");
      //Serial.print("number of button pushes: ");
      //Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    //delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  if (buttonPushCounter == 0) {
    digitalWrite(RGB_LED_PIN1, 0);     // NO LIGHT
    digitalWrite(RGB_LED_PIN2, 0);
    digitalWrite(RGB_LED_PIN3, 0);

  } else if (buttonPushCounter == 1) {
    digitalWrite(RGB_LED_PIN1, 1);     // RGB LED PIN 1 ON
    digitalWrite(RGB_LED_PIN2, 0);
    digitalWrite(RGB_LED_PIN3, 0);
  }

  else if (buttonPushCounter == 2) {
    digitalWrite(RGB_LED_PIN1, 0);
    digitalWrite(RGB_LED_PIN2, 1);  // RGB LED PIN 2 ON
    digitalWrite(RGB_LED_PIN3, 0);
  }

  else if (buttonPushCounter == 3) {
    digitalWrite(RGB_LED_PIN1, 0);
    digitalWrite(RGB_LED_PIN2, 0 );
    digitalWrite(RGB_LED_PIN3, 1); // RGB LED PIN 3 ON
  }

  else if (buttonPushCounter == 4) {  // NO LIGHT
    digitalWrite(RGB_LED_PIN1, 1);
    digitalWrite(RGB_LED_PIN2, 1);
    digitalWrite(RGB_LED_PIN3, 0);
  }

  else if (buttonPushCounter == 5) {
    digitalWrite(RGB_LED_PIN1, 1);      // RGB LED PIN 1 ON
    digitalWrite(RGB_LED_PIN2, 0);
    digitalWrite(RGB_LED_PIN3, 1);
  }

  else if (buttonPushCounter == 6) {
    digitalWrite(RGB_LED_PIN1, 1);
    digitalWrite(RGB_LED_PIN2, 1 );   // RGB LED PIN 2 ON
    digitalWrite(RGB_LED_PIN3, 1);
  }

  else if (buttonPushCounter == 7) {
    digitalWrite(RGB_LED_PIN1, 0);
    digitalWrite(RGB_LED_PIN2, 1 );
    digitalWrite(RGB_LED_PIN3, 1); // RGB LED PIN 3 ON
  }

  if (buttonPushCounter == 8) {
    buttonPushCounter = 0;
    mode = 0;
  }



  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  clock_current = digitalRead(clkIn);

  if (clock_current != clock_previous) {
    clock_previous = clock_current;

    if (clock_current == 1) {

      for (int i = 0; i < 8; i ++) {
        digitalWrite(i + 4, LOW); // I had to add 4 because its starts at pin 4
      }
      digitalWrite(direction_Array[clock_count + (mode << 4)] + 4, HIGH); // I had to add 4 because its starts at pin 4
      clock_count ++;

      if (clock_count > 15) {
        clock_count = 0;
      }
    }
  }

  reset_current = digitalRead(resetclk);

  if (reset_current != reset_previous) {
    reset_previous = reset_current;

    if (reset_current == 1) {
      clock_count = 0;
    }
  }
}

//  isr() - quickly handle interrupts from the clock input
//  ------------------------------------------------------
void isr()
{
  clock_current = HIGH;
}
