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

    digitalWrite(currentLED, LOW);
  
    if(movingUp){
        ++currentLED;
        if(currentLED == LED_END){
            movingUp = false;
        }
    }
    else{
        --currentLED;
        if(currentLED == LED_START){
            movingUp = true;
        }
    }

    digitalWrite(currentLED, HIGH);
}
