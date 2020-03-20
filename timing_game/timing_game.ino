const int LED_START = 6;
const int LED_MIDDLE = 9;
const int LED_END = 12;
const int BUTTON = 2;
const int START_SPEED = 210;
const int SPEED_CHANGE = 30;
const int MINIMUM_SPEED = 30;
const int MAXIMUM_SPEED = 300;

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

	// Start on the first LED.
	// This is actually the second LED, but the
	// nextLED() function will move it to the first.
	currentLED = LED_START + 1;
	movingUp = false;

	pauseLength = START_SPEED;
	buttonPressed = false;

	// Interrupt when the button is pressed.
	attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressActions, FALLING);
}

void loop() {
	if(buttonPressed){
		// Change the speed, based on whether or not the player won.
		changeSpeed();

		// Show the player which LED they stopped at.
		delay(3000);

		// Turn of the LED before the next round.
		digitalWrite(currentLED, LOW);
		delay(800);

		// Choose a new starting LED.
		randomLED();

		// Go back to main loop.
		buttonPressed = false;
	}
	else{
		// Move to the next LED.
		nextLED();
		delay(pauseLength);
	}
}

void randomLED(){
	currentLED = random(LED_START, LED_END + 1);

	// We have to move up from the first LED.
	// Test comment
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

void changeSpeed(){
	// Speed up if the player won, up to the fastest speed.
	if(currentLED == LED_MIDDLE && pauseLength > MINIMUM_SPEED){
		pauseLength -= SPEED_CHANGE;
	}
	// Slow down if the player lost,  up to the slowest speed.
	else if(currentLED != LED_MIDDLE && pauseLength < MAXIMUM_SPEED){
		pauseLength += SPEED_CHANGE;
	}
}

void buttonPressActions(){
	buttonPressed = true;
}
