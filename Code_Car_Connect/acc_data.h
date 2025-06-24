#ifndef ACC_DATA
#define ACC_DATA



float old_temp;

void Acc_Setup(){
	
	if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
	Serial.print("Accelerometer sample rate = ");
	Serial.print(IMU.accelerationSampleRate());
	Serial.println(" Hz");
	
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }	
}

void Acc_Read() {
	
	if (IMU.accelerationAvailable()) {
	
	float x,y,z;
	
    IMU.readAcceleration(x, y, z);

    // Calculate tilt angles in degrees
    int angleX_int = atan2(x, sqrt(y * y + z * z)) * 180 / PI;
    int angleY_int = atan2(y, sqrt(x * x + z * z)) * 180 / PI;

	/*
    // Determine the tilting direction based on angleX and angleY
    if (angleX_int > MINIMUM_TILT) {  // Tilting up
      Serial.print("Tilting up ");
      Serial.print(angleX_int);
      Serial.println(" degrees");
    } else if (angleX_int < -MINIMUM_TILT) {  // Tilting down
      Serial.print("Tilting down ");
      Serial.print(-angleX_int);
      Serial.println(" degrees");
    }

    if (angleY_int > MINIMUM_TILT) {  // Tilting left
      Serial.print("Tilting left ");
      Serial.print(angleY_int);
      Serial.println(" degrees");
    } else if (angleY_int < -MINIMUM_TILT) {  // Tilting right
      Serial.print("Tilting right ");
      Serial.print(-angleY_int);
      Serial.println(" degrees");
    }
	*/
	
	data_to_car.angleX = angleX_int;
	data_to_car.angleY = angleY_int;
	data_to_car.acc_X_val = x;
	data_to_car.acc_Y_val = y;
	data_to_car.acc_Z_val = z;
	
  }
  
  if (IMU.gyroscopeAvailable()) {
	  
	float gx, gy, gz;
	  
	IMU.readGyroscope(gx, gy, gz);
	
	data_to_car.dps_X_val = gx;
	data_to_car.dps_Y_val = gy;
	data_to_car.dps_Z_val = gz;
	  
  }
  float temperature = HTS.readTemperature();

  if (abs(old_temp - temperature) >= 0.5 ){
    old_temp = temperature;
    data_to_car.temperature_val = temperature;
  }
	
}



#endif