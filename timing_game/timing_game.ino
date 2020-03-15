const int LED_START = 6;
const int LED_MIDDLE = 9;
const int LED_END = 12;
const int BUTTON = 2;

int pauseLength;
int currentLED;
bool movingUp;
bool buttonPressed;

void setup() {

    // For testing interrupts
    pinMode(LED_BUILTIN, OUTPUT);

    // Configure pins
    for(int i = LED_START; i <= LED_END; ++i){
        pinMode(i, OUTPUT);
    }

    pinMode(BUTTON, INPUT_PULLUP);

    // Start on the first LED
    // This is actually the second LED, but the
    // nextLED() function will move it to the first.
    currentLED = LED_START + 1;
    movingUp = false;

    pauseLength = 500;
    buttonPressed = false;

    // Interrupt when the button is pressed.
    attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressActions, LOW);
}

void loop() {
    if(buttonPressed){
        noInterrupts();

        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
        digitalWrite(LED_BUILTIN, LOW);
        buttonPressed = false;

        interrupts();
    }
    else{
        nextLED();
        delay(pauseLength);
    }
}

void nextLED(){
    noInterrupts();

    // Turn off the the current LED
    digitalWrite(currentLED, LOW);

    // Move the currentLED variable to the next LED.
    movingUp ? ++currentLED : --currentLED;

    // Change direction if we have reached the ned of the row.
    if(currentLED == LED_START || currentLED == LED_END){
        movingUp = !movingUp;
    }

    // Turn on the next LED
    digitalWrite(currentLED, HIGH);

    interrupts();
}

void buttonPressActions(){
    buttonPressed = true;
}
