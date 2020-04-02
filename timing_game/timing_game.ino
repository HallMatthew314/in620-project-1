const int LED_START = 6;
const int LED_MIDDLE = 9;
const int LED_END = 12;
const int BUTTON = 2;
const int BUZZER = 5;
const int START_SPEED = 210;
const int SPEED_CHANGE = 30;
const int MINIMUM_SPEED = 30;
const int MAXIMUM_SPEED = 300;
const int WIN_JINGLE[] = {261, 329, 392, 523};
const int LOSE_JINGLE[] = {261, 246, 233, 220};

int pauseLength;
int currentLED;
bool movingUp;
bool buttonPressed;
bool playerHasWon;

void setup(void){
	// Initialise RNG
	randomSeed(analogRead(A6));

	// Configure pins
	pinMode(BUZZER, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP);

	for(int i = LED_START; i <= LED_END; ++i){
		pinMode(i, OUTPUT);
	}

	// Start on the first LED.
	// This is actually the second LED, but the
	// nextLED() function will move it to the first.
	currentLED = LED_START + 1;
	movingUp = false;

	// Variable initialization.
	pauseLength = START_SPEED;
	buttonPressed = false;
	playerHasWon = false;

	// Interrupt when the button is pressed.
	attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressActions, FALLING);
}

void loop(void){
	// Secondary Loop:
	// If the button has been pressed, check if the player won.
	if(buttonPressed){
		// Check if the player won or not.
		playerHasWon = currentLED == LED_MIDDLE;

		// Change the speed, based on whether or not the player won.
		changeSpeed();

		// Play the appropriate jingle.
		playJingle();

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
void randomLED(void){
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
void nextLED(void){
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

void changeSpeed(void){
	// Speed up if the player won, up to the fastest speed.
	if(playerHasWon && pauseLength > MINIMUM_SPEED){
		pauseLength -= SPEED_CHANGE;
	}
	// Slow down if the player lost,  up to the slowest speed.
	else if(!playerHasWon && pauseLength < MAXIMUM_SPEED){
		pauseLength += SPEED_CHANGE;
	}
}

// Plays one of two jingles based on
// whether or not the player has won.
void playJingle(void){
	// Decide which jingle to use.
	int *jingle = playerHasWon ? WIN_JINGLE : LOSE_JINGLE;

	tone(BUZZER, jingle[0]);
	delay(150);

	tone(BUZZER, jingle[1]);
	delay(150);

	tone(BUZZER, jingle[2]);
	delay(150);

	tone(BUZZER, jingle[3]);
	delay(450);

	noTone(BUZZER);
}

// Interrupt handler, moves to the secondary loop.
void buttonPressActions(void){
	buttonPressed = true;
}
