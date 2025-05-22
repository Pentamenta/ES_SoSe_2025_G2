#ifndef ACC_DATA
#define ACC_DATA

void Acc_Setup(){
	
	if (!
  IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
	
	
	Serial.print("Accelerometer sample rate = ");
	Serial.print(IMU.accelerationSampleRate());
	Serial.println(" Hz");
	
  }	
}

void Acc_Read() {
	/*
	if (IMU.accelerationAvailable()) {
    previousMillis = currentMillis;
	
	float x,y,z;
	
    IMU.readAcceleration(x, y, z);

    // Calculate tilt angles in degrees
    angleX = atan2(x, sqrt(y * y + z * z)) * 180 / PI;
    angleY = atan2(y, sqrt(x * x + z * z)) * 180 / PI;

    // Determine the tilting direction based on angleX and angleY
    if (angleX > MINIMUM_TILT) {  // Tilting up
      Serial.print("Tilting up ");
      Serial.print(angleX);
      Serial.println(" degrees");
    } else if (angleX < -MINIMUM_TILT) {  // Tilting down
      Serial.print("Tilting down ");
      Serial.print(-angleX);
      Serial.println(" degrees");
    }

    if (angleY > MINIMUM_TILT) {  // Tilting left
      Serial.print("Tilting left ");
      Serial.print(angleY);
      Serial.println(" degrees");
    } else if (angleY < -MINIMUM_TILT) {  // Tilting right
      Serial.print("Tilting right ");
      Serial.print(-angleY);
      Serial.println(" degrees");
    }
  }
  */
	
}

#endif