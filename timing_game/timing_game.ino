const int LED_START = 6;
const int LED_MIDDLE = 9;
const int LED_END = 12;
const int BUTTON = 2;

int pauseLength;
int currentLED;
bool movingUp;
bool buttonPressed;

void setup() {

    // Initialise RNG
    randomSeed(analogRead(A6));
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
    attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressActions, FALLING);
}

void loop() {
    if(buttonPressed){
        digitalWrite(LED_BUILTIN, HIGH); // Testing

        delay(3000);
        digitalWrite(currentLED, LOW);
        delay(800);

        // Choose a new starting LED.
        randomLED();

        digitalWrite(LED_BUILTIN, LOW); // Testing
        buttonPressed = false;
    }
    else{
        nextLED();
        delay(pauseLength);
    }
}

void randomLED(){
    currentLED = random(LED_START, LED_END + 1);

    // We have to move up from the first LED.
    if(currentLED == LED_START){
        movingUp = true;
    }
    // We have to move down from the last LED.
    else if(currentLED == LED_END){
        movingUp = false;
    }
    // We can go either way for the other LEDs, so flip a coin.
    else{
        // random(2) returns either 1 or 0,
        // so true or false ~50% of the time.
        movingUp = random(2) == 0;
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
