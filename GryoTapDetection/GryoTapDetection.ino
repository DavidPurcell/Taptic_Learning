#include <Adafruit_L3GD20.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

Adafruit_L3GD20 gyro(16, 17, 18, 19);

static int xBackLowerThreshold = -255;
static int yBackLowerThreshold = 20;
static int zBackLowerThreshold = -255;

static int xBackUpperThreshold = 255;
static int yBackUpperThreshold = 70;
static int zBackUpperThreshold = 255;

static int xFrontLowerThreshold = -255;
static int yFrontLowerThreshold = -70;
static int zFrontLowerThreshold = -255;

static int xFrontUpperThreshold = 255;
static int yFrontUpperThreshold = -20;
static int zFrontUpperThreshold = 255;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Gyroscope Test"); Serial.println("");

  if(!gyro.begin(GYRO_RANGE_500DPS, L3GD20_ADDRESS)){
    
  }
}

void loop(void) 
{
  /* Get a new sensor event */ 
  gyro.read();
 
  /* Display the results (speed is measured in rad/s) */
  Serial.print("X: "); Serial.print(gyro.data.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gyro.data.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gyro.data.z); Serial.print("  ");
  Serial.println("rad/s ");
  int tapResult = detectTap();
  if(tapResult == 1){
    Serial.println("BACK TAP");
  } else if(tapResult == -1){
    Serial.println("FRONT TAP");
  }
  delay(200);
  
}

int detectTap(){
  if(gyro.data.x > xBackLowerThreshold && gyro.data.y > yBackLowerThreshold && gyro.data.z > zBackLowerThreshold &&
     gyro.data.x < xBackUpperThreshold && gyro.data.y < yBackUpperThreshold && gyro.data.z < zBackUpperThreshold){
    return 1;
  } else if(gyro.data.x > xFrontLowerThreshold && gyro.data.y > yFrontLowerThreshold && gyro.data.z > zFrontLowerThreshold &&
     gyro.data.x < xFrontUpperThreshold && gyro.data.y < yFrontUpperThreshold && gyro.data.z < zFrontUpperThreshold){
    return -1;
  } else {
    return 0;
  }
}

