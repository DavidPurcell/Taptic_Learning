const int onIntensity = HIGH;
const int offIntensity = LOW;

const int frontDigitalOutPin = 9;
const int backDigitalOutPin = 10
;

// -1 = Back, 0 = Nothing, 1 = Front.
const int steps[] = {0, -1, 0, 0, -1, 0, -1, -1, -1, 0};
const int stepDelay = 250;
const int stepCount = sizeof(steps) / sizeof(int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(frontDigitalOutPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long time = millis();
  for (int i=0; i < stepCount; i++){
    long endTime = time + stepDelay * i;
    int nextStep = steps[i];
    Serial.println(nextStep);
    while(millis() < endTime){
      runStep(nextStep, i);
    }
    clearMotors();
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

void clearMotors(){
  digitalWrite(frontDigitalOutPin, offIntensity);
  digitalWrite(backDigitalOutPin, offIntensity);
}

