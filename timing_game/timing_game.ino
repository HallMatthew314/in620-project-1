const int LED_START = 6;
const int LED_MIDDLE = 9;
const int LED_END = 12;

int pauseLength;
int currentLED;
bool movingUp;

void setup() {
    // Configure LED pins
    for(int i = LED_START; i <= LED_END; ++i){
        pinMode(i, OUTPUT);
    }

    // Start on the first LED
    // This is actually the second LED, but the
    // nextLED() function will move it to the first.
    currentLED = LED_START + 1;
    movingUp = false;

    pauseLength = 500;
}

void loop() {
    nextLED();
    delay(pauseLength);
}

void nextLED(){

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
}
