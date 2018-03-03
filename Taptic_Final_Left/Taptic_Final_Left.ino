#include <Adafruit_L3GD20.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

Adafruit_L3GD20 gyro(16, 17, 18, 19);

static int xTapLowerThreshold = -255;
static int yTapLowerThreshold = -255;
static int zTapLowerThreshold = -255;

static int xTapUpperThreshold = -10;
static int yTapUpperThreshold = 255;
static int zTapUpperThreshold = 255;

const int onIntensity = HIGH;
const int offIntensity = LOW;

const int frontDigitalOutPin = 9;
const int backDigitalOutPin = 12;

const int goodLedDigitalOutPin = 11;
const int badLedDigitalOutPin = 10;

const int onOffDigitalInPin = 13;

// -1 = Back, 0 = Nothing, 1 = Front.
const int steps[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, 
                     0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0};
const int stepDelay = 200;
const int pauseDelay = 200;
const int stepCount = sizeof(steps) / sizeof(int);
long time = 0;

long stepInitialTapTime[stepCount];
const float acceptedAccuracy = .50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(frontDigitalOutPin, OUTPUT);
  pinMode(backDigitalOutPin, OUTPUT);
  pinMode(badLedDigitalOutPin, OUTPUT);
  pinMode(goodLedDigitalOutPin, OUTPUT);
  pinMode(onOffDigitalInPin, INPUT);

  if(!gyro.begin(GYRO_RANGE_500DPS, L3GD20_ADDRESS)){
    
  }
  clearTapTimes();
}

void loop() {
  digitalWrite(goodLedDigitalOutPin, LOW);
  digitalWrite(badLedDigitalOutPin, LOW);
  // put your main code here, to run repeatedly:
  time = millis();
  int onOff = digitalRead(onOffDigitalInPin);
  if(onOff == HIGH){
    for (int i=0; i < stepCount; i++){
      int nextStep = steps[i];
      long endTime = time;
      if(nextStep == 0){
        endTime += pauseDelay * i;
      } else {
        endTime += stepDelay * i;
      }
      
      bool hasTapped = false;
      long startTime = millis();
      //Serial.println(nextStep);
      while(millis() < endTime){
        runStep(nextStep, i);
        if(!hasTapped){
          int tapValue = checkForTap(nextStep);
          hasTapped = tapValue != 0;
          long timeDifference = millis() - startTime;
          stepInitialTapTime[i] = abs(timeDifference);
        }
      }
      clearMotors();
    }
  }
  float accuracy = calculateAccuracy();
  if(accuracy > acceptedAccuracy){
    Serial.println("Accurracy Success");
    Serial.println(accuracy);
    digitalWrite(goodLedDigitalOutPin, HIGH);
  } else {
    // Indicate Failure
    Serial.println("Accurracy Failure");
    Serial.println(accuracy);
    digitalWrite(badLedDigitalOutPin, HIGH);
  }
  clearTapTimes();
  delay(5000);
}

// Output the analog signal 
void runStep(int step, int stepNumber){
  // Setup correct input and output pins based on what the step is.
  int targetMotor = 0;
  if(step == -1){
    //Serial.println("FRONT STEP");
    targetMotor = frontDigitalOutPin;
  } else if (step == 1){
    //Serial.println("BACK STEP");
    targetMotor = backDigitalOutPin;
  } else {
    return;
  }
  digitalWrite(targetMotor, onIntensity);
}

// Check if there has been a tap on the target foot
int checkForTap(int step){
  gyro.read();
  Serial.print("X: "); Serial.print(gyro.data.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gyro.data.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gyro.data.z); Serial.print("  ");
  Serial.println("rad/s ");
  if(gyro.data.x > xTapLowerThreshold && gyro.data.y > yTapLowerThreshold && gyro.data.z > zTapLowerThreshold &&
     gyro.data.x < xTapUpperThreshold && gyro.data.y < yTapUpperThreshold && gyro.data.z < zTapUpperThreshold){
    Serial.println("TAP Detected!");
    return 1;
  }
}

float calculateAccuracy(){
  long deltaSum = 0;
  int nonZeroStepCount = 0;
  for (int i=0; i <= stepCount; i++){
    if(steps[i] != 0){
      long delta = stepDelay - stepInitialTapTime[i];
      if(delta > stepDelay){
        delta = stepDelay;
      } else if(delta < 0){
        delta = 0;
      }
      //Serial.print("Step Time "); Serial.println(stepInitialTapTime[i]); 
      //Serial.print("Delta "); Serial.println(delta);
      deltaSum += delta;
      nonZeroStepCount++;
    }
  }
  //Serial.print("DeltaSum ");Serial.println(deltaSum);
  
  return ((float)deltaSum)/(nonZeroStepCount * stepDelay);
}

void clearMotors(){
  digitalWrite(frontDigitalOutPin, offIntensity);
  digitalWrite(backDigitalOutPin, offIntensity);
}

void clearTapTimes(){
  for (int i=0; i <= stepCount; i++){
    stepInitialTapTime[i] = stepDelay;
  }
}

