const int onIntensity = 100;
const int offIntensity = 0;

const int frontAnalogOutPin = A0;
const int backAnalogOutPin = A1;

// -1 = Back, 0 = Nothing, 1 = Front.
const int steps[] = {-1, 0, 0, 0, 1}
const int stepDelay = 1000;

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  long time = millis();
  for (int i=0; i <= steps.length; i++){
    long endTime = time + stepDelay * i;
    int nextStep = steps[i];
    while(millis() < endTime){
      runStep(nextStep, i);
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

voic clearMotors(){
  analogWrite(frontAnalogOutPin, offIntensity);
  analogWrite(backAnalogOutPin, offIntensity);
}

