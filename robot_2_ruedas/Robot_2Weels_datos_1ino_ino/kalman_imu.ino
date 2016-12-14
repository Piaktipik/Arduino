

//calculate roll/pitch for acc/gyro, remove level bias. kalman filtering for pitch/roll, calc yaw
void Calculations(){
  /*
  Gyro in deg/s
   pitchGyro = (GYROx - bGYROx) / GyroGain;
   rollGyro = (GYROy - bGYROy) / GyroGain;
   */
  pitchGyro = pitchGyro + ((GYROx - bGYROx) / GyroGain) * timeStep; // gyro pitch in deg
  pitchAccel = atan2((ACCy - bACCy) / AccGain,(ACCz - bACCz) / AccGain) * 180.0 / PI;
  pitchPrediction = pitchPrediction +(GYROx - bGYROx) / GyroGain * timeStep;

  rollGyro = rollGyro + ((GYROy - bGYROy) / GyroGain) * timeStep; //gyro roll in deg
  rollAccel = atan2((ACCx - bACCx) / AccGain,(ACCz - bACCz) / AccGain) * 180.0 / PI;
  
  rollPrediction = rollPrediction - (GYROy - bGYROy) / GyroGain* timeStep;

  YawCalc();  //calc yaw with mag!

  Kalman(); //predict pitch, roll
}

// calculate yaw from mag
void YawCalc(){
  //this part is required to normalize the magnetic vector
  //get Min and Max Reading for MAGic Axis
  if (MAGx > xMAGMax) {xMAGMax = MAGx;}
  if (MAGy > yMAGMax) {yMAGMax = MAGy;}
  if (MAGz > zMAGMax) {zMAGMax = MAGz;}
  if (MAGx < xMAGMin) {xMAGMin = MAGx;}
  if (MAGy < yMAGMin) {yMAGMin = MAGy;}
  if (MAGz < zMAGMin) {zMAGMin = MAGz;}

  //Map the incoming Data from -1 to 1
  xMAGMap = float(map(MAGx, xMAGMin, xMAGMax, -30000, 30000))/30000.0;
  yMAGMap = float(map(MAGy, yMAGMin, yMAGMax, -30000, 30000))/30000.0;
  zMAGMap = float(map(MAGz, zMAGMin, zMAGMax, -30000, 30000))/30000.0;

  //normalize the magnetic vector
  float norm = sqrt(sq(xMAGMap) + sq(yMAGMap) + sq(zMAGMap));
  xMAGMap /= norm;
  yMAGMap /= norm;
  zMAGMap /= norm;

  //new calcs:
  float xh = float(xMAGMap) * cos(pitchPrediction) + float(zMAGMap) * sin(pitchPrediction);
  float yh = float(xMAGMap) * sin(rollPrediction) * sin(pitchPrediction) + float(yMAGMap) * cos(rollPrediction) - float(zMAGMap) * sin(rollPrediction) * cos(pitchPrediction);
  float zh = float(-xMAGMap) * cos(rollPrediction) * sin(pitchPrediction) + float(yMAGMap) * sin(rollPrediction) + float(zMAGMap) * cos(rollPrediction) * cos(pitchPrediction);

  yawRaw = 180 * atan2(yh, xh)/PI;
  if (yh >= 0){} //do nothing, yaw value is ok
  else{
    yawRaw += 360;
  }
}

//kalman filter for pitch / roll
void Kalman(){
  
  Pxx += timeStep * (20.0 * Pxv + timeStep * Pvv);
  Pxv += timeStep * Pvv;
  Pxx += timeStep * giroVar;
  Pvv += timeStep * deltaGiroVar;
  kx = Pxx * (1.0 / (Pxx + accelVar));
  kv = Pxv * (1.0 / (Pxx + accelVar));

  pitchPrediction += (pitchAccel - pitchPrediction) * kx;
  rollPrediction += (rollAccel - rollPrediction) * kx;

  Pxx *= (1 - kx);
  Pxv *= (1 - kx);
  Pvv -= kv * Pxv;
}
