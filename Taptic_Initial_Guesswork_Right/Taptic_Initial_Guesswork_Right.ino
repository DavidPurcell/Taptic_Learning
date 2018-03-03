const int onIntensity = HIGH;
const int offIntensity = LOW;

const int frontDigitalOutPin = 9;
const int backDigitalOutPin = 12;

const int frontDigitalInPin = 8;
const int backDigitalInPin = 7;

const int onOffDigitalInPin = 13;

// -1 = Back, 0 = Nothing, 1 = Front.
const int steps[] = {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                     0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1,
                     0, 0, 0, -1};
const int stepDelay = 200;
const int pauseDelay = 200;
const int stepCount = sizeof(steps) / sizeof(int);

long stepInitialTapTime[stepCount];
const float acceptedAccuracy = .5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(frontDigitalOutPin, OUTPUT);
  pinMode(backDigitalOutPin, OUTPUT);
  pinMode(onOffDigitalInPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long time = millis();
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
      Serial.println(nextStep);
      while(millis() < endTime){
        runStep(nextStep, i);
        if(!hasTapped){
          hasTapped = checkForTap(nextStep);
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
  } else {
    // Indicate Failure
    Serial.println("Accurracy Failure");
    Serial.println(accuracy);
  }
}

// Output the analog signal 
void runStep(int step, int stepNumber){
  // Setup correct input and output pins based on what the step is.
  int targetMotor = 0;
  if(step == -1){
    targetMotor = frontDigitalOutPin;
  } else if (step == 1){
    targetMotor = backDigitalOutPin;
  } else {
    return;
  }
  digitalWrite(targetMotor, onIntensity);
}

// Check if there has been a tap on the target foot
bool checkForTap(int step){
  int targetReceiver = 0;
  if(step == -1){
    targetReceiver = frontDigitalInPin;
  } else if (step == 1){
    targetReceiver = backDigitalInPin;
  } else {
    return false;
  }

  return digitalRead(targetReceiver) == HIGH;
}

float calculateAccuracy(){
  long deltaSum = 0;
  for (int i=0; i <= stepCount; i++){
    long delta = (i * stepDelay) - stepInitialTapTime[i]; 
    deltaSum += delta;
  }
  
  return (((float)deltaSum)/stepCount) / stepDelay;
}

void clearMotors(){
  digitalWrite(frontDigitalOutPin, offIntensity);
  digitalWrite(backDigitalOutPin, offIntensity);
}

