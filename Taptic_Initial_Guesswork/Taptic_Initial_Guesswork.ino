const int onIntensity = 100;
const int offIntensity = 0;

const int frontAnalogOutPin = A0;
const int backAnalogOutPin = A1;

const int frontDigitalInPin = A2;
const int backDigitalInPin = A3;

// -1 = Back, 0 = Nothing, 1 = Front.
const int steps[] = {-1, 0, 0, 0, 1}
const int stepCount = 5;
const int stepDelay = 1000;

const long stepInitialTapTime = [sizeof(steps)];
const float acceptedAccuracy = .5;

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  long time = millis();
  for (int i=0; i <= stepCount; i++){
    long endTime = time + stepDelay * i;
    int nextStep = steps[i];
    bool hasTapped = false;
    while(millis() < endTime){
      runStep(nextStep, i);
      if(!hasTapped){
        hasTapped = checkForTap(nextStep);
        stepInitialTapTime[i] = millis();
      }
    }
  }

  float accuracy = calculateAccuracy();
  if(accuracy > acceptedAccuracy){
    //Indicate victory
  } else {
    // Indicate Failure
  }
}

// Output the analog signal 
void runStep(int step, int stepNumber){
  // Setup correct input and output pins based on what the step is.
  int targetMotor = 0;
  if(step == -1){
    targetMotor = frontAnalogOutPin;
  } else if (step == 1){
    targetMotor = backAnalogOutPin;
  } else {
    return;
  }

  analogWrite(targetMotor, onIntensity);
}

// Check if there has been a tap on the target foot
bool checkForTap(){
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
  for (int i=0; i <= steps.length; i++){
    long delta = (i * stepDelay) - stepInitialTapTime[i]; 
    deltaSum += delta;
  }
  
  return ((float)deltaSum)/steps.length; 
}

voic clearMotors(){
  analogWrite(frontAnalogOutPin, offIntensity);
  analogWrite(backAnalogOutPin, offIntensity);
}

