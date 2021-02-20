/* Code based off example code provided by David Madison's GamepadPins example included in the Xinput Library
 * 
 * Before you can use this code, make sure to download needed libraries
 * - https://github.com/dmadison/ArduinoXInput
 * - https://github.com/FastLED/FastLED
 * 
 * You will also need to add this to your arduino IDE's hardware folder
 * - https://github.com/dmadison/ArduinoXInput_AVR
 * 
 * This file was created with Arduino Leonardo board in mind
 * 
 * USAGE:
 * this will cover your basic XInput controller for ONGEKI
 * 
 * ONGEKI's XInput lever is processed as when neither trigger is pressed, your character is in the center of the screen.
 * Press the left side and it moves left, release and it returns to center. Same with the right side
 * 
 * I set the lever range to 519 which is an approximate halfway point between the min and max values of my potentiometer.
 * That half way point has to be calculated because you're splitting one input method between two "triggers"
 * I also set the lever calculations to work around that half way point.
 * 
 * Lights use a ws2812b strip. This currently only has 12 lights programmed. 3 for each side button and 2 sets of button lights
 * the lights are set to always be on as segatools does not yet support HID for ONGEKI
 */

#include <XInput.h>

// Lever Setup
int leverRange = 519;//519
int positionA = 0;
int positionB = 0;

// Lever Pins
const int Pin_Lever = A0;

// Button Pins
const int Pin_ThumbL = 0;
const int Pin_ThumbR = 1;
const int Pin_LeftA = 2;
const int Pin_LeftB = 3;
const int Pin_LeftC = 4;
const int Pin_RightA = 5;
const int Pin_RightB = 6;
const int Pin_RightC = 7;
const int Pin_MidL = 8;
const int Pin_MidR = 9;
const int Pin_LeftMenu = 10;
const int Pin_RightMenu = 11;
const int Pin_PinkyL = A5;
const int Pin_PinkyR = A4;

void setup() {
  // Set Lever Range
  XInput.setTriggerRange(0, leverRange);
  
  // Set buttons as inputs, using internal pull-up resistors
  pinMode(Pin_LeftA, INPUT_PULLUP);
  pinMode(Pin_LeftB, INPUT_PULLUP);
  pinMode(Pin_LeftC, INPUT_PULLUP);
  pinMode(Pin_RightA, INPUT_PULLUP);
  pinMode(Pin_RightB, INPUT_PULLUP);
  pinMode(Pin_RightC, INPUT_PULLUP);
  pinMode(Pin_MidL, INPUT_PULLUP);
  pinMode(Pin_MidR, INPUT_PULLUP);
  pinMode(Pin_LeftMenu, INPUT_PULLUP);
  pinMode(Pin_RightMenu, INPUT_PULLUP);
  pinMode(Pin_ThumbL, INPUT_PULLUP);
  pinMode(Pin_ThumbR, INPUT_PULLUP);
  pinMode(Pin_PinkyL, INPUT_PULLUP);
  pinMode(Pin_PinkyR, INPUT_PULLUP);
  XInput.setAutoSend(false);  // Wait for all controls before sending
  XInput.begin();
}

void loop() {
  // Read pin values and store in variables
  int lever = analogRead(Pin_Lever);
  boolean leftA = !digitalRead(Pin_LeftA);
  boolean leftB = !digitalRead(Pin_LeftB);
  boolean leftC = !digitalRead(Pin_LeftC);
  boolean rightA = !digitalRead(Pin_RightA);
  boolean rightB = !digitalRead(Pin_RightB);
  boolean rightC = !digitalRead(Pin_RightC);
  boolean midL = !digitalRead(Pin_MidL);
  boolean midR = !digitalRead(Pin_MidR);
  boolean leftMenu  = !digitalRead(Pin_LeftMenu);
  boolean rightMenu = !digitalRead(Pin_RightMenu);
  boolean thumbL = !digitalRead(Pin_ThumbL);
  boolean thumbR = !digitalRead(Pin_ThumbR);
  boolean pinkyL = !digitalRead(Pin_PinkyR);
  boolean pinkyR = !digitalRead(Pin_PinkyR);
  
  // Set XInput DPAD values and allow simultaneous opposite direction pressing
  XInput.setDpad(leftB, thumbL, leftA, leftC, false);// leftB listed twice because I needed something to fill that second dpad slot..it's unused in game
  
  // Set XInput buttons
  XInput.setButton(BUTTON_X, rightA);
  XInput.setButton(BUTTON_Y, rightB);
  XInput.setButton(BUTTON_B, rightC);
  XInput.setButton(BUTTON_LB, midL);
  XInput.setButton(BUTTON_RB, midR);
  XInput.setButton(BUTTON_BACK, leftMenu);
  XInput.setButton(BUTTON_START, rightMenu);
  XInput.setButton(BUTTON_A, thumbR);
  XInput.setButton(BUTTON_L3, pinkyL);
  XInput.setButton(BUTTON_R3, pinkyR);

  // Set XInput trigger values
  XInput.setTrigger(TRIGGER_LEFT, positionA);
  XInput.setTrigger(TRIGGER_RIGHT, positionB);

  // Calculate lever values
  if(lever < leverRange){
    positionB = -(lever-leverRange);
  }else if( lever > leverRange){
    positionA = lever-leverRange;
  }else{
    positionA = 0;
    positionB = 0;
  }
  
  // Send control data to the computer
  XInput.send();
}
