/*
Einrichten und Auswerten von Tastern
geschrieben von Adrian Behrens
*/
#ifndef BUTTONS
#define BUTTONS

#define T_1         5   // Tempomat
#define T_2         6   // Licht
#define T_3         7   // Fernlicht
#define T_4         10    
#define DEBOUNCE    50

bool t1, t2, t3, t4 = LOW;

void i_t1() {
    t1 = HIGH;
}
void i_t2() {
    t2 = HIGH;
}
void i_t3() {
    t3 = HIGH;
}
void i_t4() {
    t4 = HIGH;
}

void Button_Setup() {

    pinMode(T_1, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(T_1), i_t1, FALLING);
    pinMode(T_2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(T_2), i_t2, FALLING);
    pinMode(T_3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(T_3), i_t3, FALLING);
    pinMode(T_4, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(T_4), i_t4, FALLING);
    

}

#endif