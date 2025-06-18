#ifndef JOYSTICK
#define JOYSTICK

// Defines linker Joystick
#define JL_X    0
#define JL_Y    1
#define JL_SW   2   // Pin für Switch
#define JL_X_p  A1  // Pin für X-Achse
#define JL_Y_p  A0  // Pin für Y-Achse
// Defines rechter Joystick
#define JR_X    2
#define JR_Y    3
#define JR_SW   3   // Pin für Switch
#define JR_X_p  A3  // Pin für X-Achse
#define JR_Y_p  A2  // Pin für Y-Achse

// Joystick boundries
#define DEADZONE_UPPER  532
#define DEADZONE_LOWER  492

void joystick_setup() { // Initialisiert Joysticks (Adrian)

    pinMode(JL_SW, INPUT_PULLUP);
    pinMode(JR_SW, INPUT_PULLUP);

    pinMode(JL_X_p, INPUT);
    pinMode(JL_Y_p, INPUT);
    pinMode(JR_X_p, INPUT);
    pinMode(JR_Y_p, INPUT);

}

int read_joystick_pos(int axis) { // Gibt die gewünschte Joystick Positon zurück
int buffer = 0;

if (axis == 0) {
        buffer = analogRead(JL_X_p);
        if(DEADZONE_LOWER < buffer && buffer < DEADZONE_UPPER) {
            return 0;
        }
        else if(buffer >= DEADZONE_UPPER) {
            buffer = buffer - DEADZONE_UPPER;
            return buffer;
        }
        else {
            buffer = buffer - DEADZONE_LOWER;
            return buffer;
        }
    }
else if(axis == 1) {
        buffer = analogRead(JL_Y_p);
        if(DEADZONE_LOWER < buffer && buffer < DEADZONE_UPPER) {
            return 0;
        }
        else if(buffer >= DEADZONE_UPPER) {
            buffer = buffer - DEADZONE_UPPER;
            return -buffer;
        }
        else {
            buffer = buffer - DEADZONE_LOWER;
            return -buffer;
        }
    }
else if(axis == 2) {
        buffer = analogRead(JR_X_p);
        if(DEADZONE_LOWER < buffer && buffer < DEADZONE_UPPER) {
            return 0;
        }
        else if(buffer >= DEADZONE_UPPER) {
            buffer = buffer - DEADZONE_UPPER;
            return buffer;
        }
        else {
            buffer = buffer - DEADZONE_LOWER;
            return buffer;
        }
    }
else if(axis == 3) {
        buffer = analogRead(JR_Y_p);
        if(DEADZONE_LOWER < buffer && buffer < DEADZONE_UPPER) {
            return 0;
        }
        else if(buffer >= DEADZONE_UPPER) {
            buffer = buffer - DEADZONE_UPPER;
            return -buffer;
        }
        else {
            buffer = buffer - DEADZONE_LOWER;
            return -buffer;
        }
    }
return 0;
}

void joystick_debug() {
    Serial.println("Joystick L:");
    Serial.print("X: ");
    Serial.print(read_joystick_pos(0));
    Serial.print(" ; ");
    Serial.println(analogRead(JL_X_p));
    Serial.print("Y: ");
    Serial.print(read_joystick_pos(1));
    Serial.print(" ; ");
    Serial.println(analogRead(JL_Y_p));

    Serial.println("Joystick R:");
    Serial.print("X: ");
    Serial.print(read_joystick_pos(2));
    Serial.print(" ; ");
    Serial.println(analogRead(JR_X_p));
    Serial.print("Y: ");
    Serial.print(read_joystick_pos(3));
    Serial.print(" ; ");
    Serial.println(analogRead(JR_Y_p));
}

#endif
