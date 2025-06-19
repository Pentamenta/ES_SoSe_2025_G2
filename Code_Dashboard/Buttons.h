/*
Einrichten und Auswerten von Tastern
geschrieben von Adrian Behrens
*/

#define T_1         5   // Tempomat
#define T_2         6   // Licht
#define T_3         7   // Fernlicht
#define T_4         10    
#define DEBOUNCE    50

unsigned long t_t1, t_t2, t_t3, t_t4;
bool t1, t2, t3, t4 = LOW;

void Button_Setup() {

    t_t1, t_t2, t_t3, t_t4 = millis();

    pinMode(T_1, INPUT_PULLUP);
    pinMode(T_2, INPUT_PULLUP);
    pinMode(T_3, INPUT_PULLUP);
    pinMode(T_4, INPUT_PULLUP);
    
}

void button_check() {

    if ((millis() >= t_t1 + DEBOUNCE) && digitalRead(T_1)) {
        t1 = HIGH;
        t_t1 = millis();
    }
    if ((millis() >= t_t2 + DEBOUNCE) && digitalRead(T_2)) {
        t2 = HIGH;
        t_t2 = millis();
    }
    if ((millis() >= t_t3 + DEBOUNCE) && digitalRead(T_3)) {
        t3 = HIGH;
        t_t3 = millis();
    }
    if ((millis() >= t_t4 + DEBOUNCE) && digitalRead(T_4)) {
        t4 = HIGH;
        t_t4 = millis();
    }

}