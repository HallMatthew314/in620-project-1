const int LED_START = 6;
const int LED_MIDDLE = 9;
const int LED_END = 12;
const int BUTTON = 2;
const int BUZZER = 5;
const int START_SPEED = 210;
const int SPEED_CHANGE = 30;
const int MINIMUM_SPEED = 30;
const int MAXIMUM_SPEED = 300;

int pauseLength;
int currentLED;
bool movingUp;
bool buttonPressed;

void setup(void){

	// Initialise RNG
	randomSeed(analogRead(A6));

	// Configure pins
	for(int i = LED_START; i <= LED_END; ++i){
		pinMode(i, OUTPUT);
	}

	pinMode(BUZZER, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP);

	// Start on the first LED.
	// This is actually the second LED, but the
	// nextLED() function will move it to the first.
	currentLED = LED_START + 1;
	movingUp = false;

	// Variable initialization.
	pauseLength = START_SPEED;
	buttonPressed = false;

	// Interrupt when the button is pressed.
	attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressActions, FALLING);
}

void loop(){
	// Secondary Loop:
	// If the button has been pressed, check if the player won.
	if(buttonPressed){
		// Change the speed, based on whether or not the player won.
		changeSpeed();

		// Play the win jingle if the player stopped on the middle LED.
		if(currentLED == LED_MIDDLE){
			winJingle();
		}
		// Otherwise, play the lose jingle.
		else{
			loseJingle();
		}

		// Wait for a moment before starting the next round.
		delay(1000);

		// Turn off the LED before the next round.
		digitalWrite(currentLED, LOW);
		delay(800);

		// Choose a new starting LED.
		randomLED();

		// Go back to the main loop.
		buttonPressed = false;
	}
	// Main Loop:
	// Move the LED up and down.
	else{
		// Move to the next LED.
		nextLED();
		// Give the player time to press the button.
		delay(pauseLength);
	}
}

// Chooses a random LED and direction to move.
void randomLED(){
	// Pick new LED.
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

// Moves the current LED up or down by one,
// changing the direction to move if we're at the end of the row.
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

// Plays a short melody of an arpeggiated C major chord.
void winJingle(){
	// C (ish)
	tone(BUZZER, 261);
	delay(150);

	// E (ish)
	tone(BUZZER, 329);
	delay(150);

	// G
	tone(BUZZER, 392);
	delay(150);

	// C (ish)
	tone(BUZZER, 523);
	delay(450);

	// Turn off the buzzer.
	noTone(BUZZER);
}

// Plays a short melody of notes chromatically descending from C to A.
void loseJingle(){
	// C (ish)
	tone(BUZZER, 261);
	delay(150);

	// B (ish)
	tone(BUZZER, 246);
	delay(150);

	// Bb(ish)
	tone(BUZZER, 233);
	delay(150);

	// A
	tone(BUZZER, 220);
	delay(450);

	// Turn off the buzzer.
	noTone(BUZZER);
}

// Interrupt handler, moves to the secondary loop.
void buttonPressActions(){
	buttonPressed = true;
}
