// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-10-07 - initial release

/* ============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2011 Jeff Rowberg

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
*/

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t x,y,z;
int16_t times=0;
float x1,y1,z1;
float  bx,by,bz,b1,b2,b3;
int16_t x3,y3,z3;
#define LED_PIN 13
bool blinkState = false;
float gravity[]={0,0,0.98};
float linear_acceleration[]={0,0,0};
float alpha=0.8;
float previousTime,currentTime,elapsedTime;
float gyroAngleX,gyroAngleY,gyroAngleZ;
float yaw,roll,pitch,roll2,yaw2,pitch2;
float accx_no_gra,accy_no_gra,accz_no_gra;
int16_t accx,accy,accz;
void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  
  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(38400);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  // configure Arduino LED for
  pinMode(LED_PIN, OUTPUT);
  accx,accy,accz=set_argle_speed_zero();
  Serial.println(accx);
}

void loop() {
  // read raw accel/gyro measurements from device
  //accelgyro.setFullScaleAccelRange(11);  //we choose the range is 2g
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);  //MPU6050_ACCEL_FS_16 means that we choose the range is 16g
  accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_250);  //Select the smallest range for angular velocity
  accelgyro.getAcceleration(&ax, &ay, &az);
  accelgyro.getRotation(&x, &y, &z);




  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();  
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  // display tab-separated accel x/y/z values
  bx = float(ax);
  by = float(ay);
  bz = float(az);
  bx = float(bx / 2048);
  by = float(by / 2048);
  bz = float(bz / 2048);
  
  if(times==0){
     b1=bx;
     b2=by;
     b3=bz;}
  times=times+1;

 
  // display tab-separated angles speed x/y/z values
  x1 = float(x);
  y1 = float(y);
  z1 = float(z);
  x1 = x1-accx;
  y1 = y1-accy;
  z1 = z1-accz;
 
  x1 = float(x1 / 131);
  y1 = float(y1 / 131);
  z1 = float(z1 / 131);
 
  // Correct the outputs with the calculated error values


  gyroAngleX = gyroAngleX + x1 * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + y1 * elapsedTime;
  roll =  yaw + z1 * elapsedTime;
  // Complementary filter - combine acceleromter and gyro angle values
  pitch = 0.96 * gyroAngleX + 0.04 * bx;
  yaw = 0.96 * gyroAngleY + 0.04 * by;

 //convert to angle
// roll2=roll/180*3.14;
// pitch2=pitch/180*3.14;
// yaw2=yaw/180*3.14;
 roll2=roll/57.3;
 pitch2=pitch/57.3;
 yaw2=yaw/57.3;
//z rollα y yawβ x pitchγ
  accx_no_gra=bx-b3*(cos(roll2)*sin(yaw2)*cos(pitch2)+sin(yaw2)*sin(roll2))
                  -b1*(cos(roll2)*cos(yaw2))-b2*(cos(roll2)*sin(yaw2)*sin(pitch2)-sin(yaw2)*cos(roll2));
                  
  accy_no_gra=by-b3*(sin(roll2)*sin(yaw2)*cos(pitch2)-cos(roll2)*sin(yaw2))
                    -b1*(sin(roll2)*cos(yaw2))-b2*(sin(roll2)*sin(yaw2)*sin(pitch2)-cos(yaw2)*cos(roll2));
               
  accz_no_gra=bz-b3*cos(yaw2)*cos(pitch2)+b1*sin(yaw2)-b2*cos(yaw2)*sin(pitch2);
//  gravity[0] = alpha * gravity[0] + (1 - alpha) * bx;
//  gravity[1] = alpha * gravity[1] + (1 - alpha) * by;
//  gravity[2] = alpha * gravity[2] + (1 - alpha) * bz;
//
//  linear_acceleration[0] = bx - gravity[0];
//  linear_acceleration[1] = by - gravity[1];
//  linear_acceleration[2] = bz - gravity[2];
  //  ax=float(ax/16384)
  //  ay=float(ay/16384)
  //  az=float(az/16384)
  //angle speed
//  Serial.print(x1);
//  Serial.print(",");
//  Serial.print(y1);
//  Serial.print(",");
//  Serial.print(z1);
//  Serial.print(",");

  //roll angle
//  Serial.print("/");
//  Serial.print(pitch);
//  Serial.print("/");
//  Serial.print(yaw);
//  Serial.print("/");
//  Serial.println(roll);
//  Serial.print("/");

  //acc
//  Serial.print(bx);
//  Serial.print(",");
//  Serial.print(by);
//  Serial.print(",");
//  Serial.print(bz);
//  Serial.print(",");

  //acc no grativate
  Serial.print(accx_no_gra);
  Serial.print(",");
  Serial.print(accy_no_gra);
  Serial.print(",");
  Serial.print(accz_no_gra);
  Serial.print(",");
  Serial.println("");
  
  delay(100);
//  Serial.print(bx);
//  Serial.print(",");
//  Serial.print(by);
//  Serial.print(",");
//  Serial.print(bz);
//  Serial.print(",");
//  Serial.println("");

  //The accuracy of the acceleration measurement is 16 bits, so it is also int16, so the range is: -32768~32767
  //Range selection if 16g
  //So the sensitivity is:32767/16 =  2048 g
  //The final calculation formula isACC_X  / 2048 g
  //The final calculation formula is 2g  ACC_X  / 16384 g
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

// Calibrated angular velocity
int16_t set_argle_speed_zero()
{
  for(int i=0;i<10;i++)
  {
  accelgyro.getRotation(&x3, &y3, &z3);
  accx=accx+x3;
  accy=accy+y3;
  accz=accz+z3;
  }
  accx=accx/10;
  accy=accy/10;
  accz=accz/10;
  return accx,accy,accz;
}
