/*
Description: State Machine for Olin Slot Machine Final PIE Project
Date: Fall 2024
Authors: Kenneth Xiong
*/

#include <Servo.h>
#include <SlotWheel.h>

// Define the states
enum State {
  IDLE, 
  READY,
  SPINNING,
  STOPPED,
  JACKPOT,
  MEGA_JACKPOT,
  LOSE,
};

State currentState = IDLE;

//init buttons
const int coinPin = 8;
const int leverPin = 9;

//predtermined win sequence
const String winSeq = "0101011101100101001000000110000101110100011001010010000001110100011011110110111100100000011011010111010101100011011010000010000001101111011001100010000001110100011010000110010100100000011100000110100101100101";
int winIdx = 0;

int coinState;
int leverState;

//define slot wheels
SlotWheel leftSlotWheel(3,2);
SlotWheel midSlotWheel(5,4);
SlotWheel rightSlotWheel(6,7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  leftSlotWheel.begin();
  midSlotWheel.begin();
  rightSlotWheel.begin();
  pinMode(coinPin, INPUT);
  pinMode(leverPin, INPUT);

  //set slotwheels to their home position
  leftSlotWheel.reset();
  midSlotWheel.reset();
  rightSlotWheel.reset();
}

void loop() {
  coinState = digitalRead(coinPin);
  leverState = digitalRead(leverPin);

  switch (currentState){
    case IDLE:
      currentState = READY;
      break;
    case READY:
       if (coinState == HIGH) {
        currentState = SPINNING;
       }
      break;
    case SPINNING:
      //spin at full speed
      leftSlotWheel.write(170);  
      midSlotWheel.write(170);  
      rightSlotWheel.write(170);   
      if (leverState == HIGH) {
        currentState = STOPPED;
       }
      break;
    case STOPPED:
      //check if it should be a win or loss
      if (winSeq.charAt(winIdx) == '1'){
        int randomFace = (random(0,5) * 256/5) + 25;
        Serial.println(randomFace);
        fullSpin(randomFace, randomFace, randomFace);
      }
      else{
        // if loss, spin to random faces
        int leftFace = (random(0,5) * 256/5) + 25;
        int rightFace = (random(0,5) * 256/5) + 25;
        int midFace = (random(0,5) * 256/5) + 25;
        if ((leftFace == rightFace)&&(leftFace == midFace)){
          leftFace = leftFace-1;
        }
        fullSpin(leftFace,rightFace,midFace);
      }
      //debuggin info
      Serial.print(leftSlotWheel.read());
      Serial.print(",");
      Serial.print(midSlotWheel.read());
      Serial.print(",");
      Serial.println(rightSlotWheel.read());
      currentState = detectWin();
      break;
    case JACKPOT:
      Serial.println("You Won!");
      currentState = IDLE;
      break;
    case MEGA_JACKPOT:
      Serial.println("You MEGA Won!");
      currentState = IDLE;
      break;
    case LOSE:
      Serial.println("You Lost!");

      currentState = IDLE;
      break;
  }
}

void fullSpin(int leftPos, int midPos, int rightPos){
  /* This is functions runs a PID for each 
  slot wheel to spin it to the correct position*/
  int leftRemaining = 255;
  int midRemaining = 255;
  int rightRemaining = 255;
  int speed = 0;

  // Keep spinning until all wheels are within 10 encoder 10
  while ((leftSlotWheel.getSpeed() > 102 && leftRemaining > 10)||( midSlotWheel.getSpeed() > 102 && midRemaining > 10) || (rightSlotWheel.getSpeed() > 102 && rightRemaining > 10)){
  Serial.print(leftRemaining);
  Serial.print(",");
  Serial.print(midRemaining);
    Serial.print(",");
  Serial.println(rightRemaining);

  // Porportional controller
  if (leftSlotWheel.read() > leftPos){
      leftRemaining = 255 - leftSlotWheel.read() + leftPos;
    }
    else{
      leftRemaining = leftPos - leftSlotWheel.read();
    }
    speed = map(leftRemaining, 0, 255, 96, 170);
    leftSlotWheel.write(speed);
  if (midSlotWheel.read() > midPos){
      midRemaining = 255 - midSlotWheel.read() + midPos;
    }
    else{
      midRemaining = midPos - midSlotWheel.read();
    }
    speed = map(midRemaining, 0, 255, 101, 170);
    midSlotWheel.write(speed);
  if (rightSlotWheel.read() > rightPos){
      rightRemaining = 255 - rightSlotWheel.read() + rightPos;
    }
  else{
      rightRemaining = rightPos - rightSlotWheel.read();
    }
    speed = map(rightRemaining, 0, 255, 96, 170);
    rightSlotWheel.write(speed);
  }
    leftSlotWheel.write(90);
    midSlotWheel.write(90);
    rightSlotWheel.write(90);
  
}
 
State detectWin(){
  winIdx = winIdx + 1;
  // detect if there is a jackpot and the type of jackpot
  if ((leftSlotWheel.getFace() == rightSlotWheel.getFace()) && (leftSlotWheel.getFace() == midSlotWheel.getFace())){
    if (leftSlotWheel.getFace()  == 4){
      return MEGA_JACKPOT;
    }
    else{
    return JACKPOT;
  }
  }
  else{
  return LOSE;
  }
  
}

