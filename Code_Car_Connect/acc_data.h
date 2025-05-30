#ifndef ACC_DATA
#define ACC_DATA

void Acc_Setup(){
	
	if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
	
	
	Serial.print("Accelerometer sample rate = ");
	Serial.print(IMU.accelerationSampleRate());
	Serial.println(" Hz");
	
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
	
	data_to_car.angleX = (uint16_t)angleX_int;
	data_to_car.angleY = (uint16_t)angleY_int;
	
  }
	
}

#endif