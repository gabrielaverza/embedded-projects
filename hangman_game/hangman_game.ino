#include <ctype.h> // For tolower()

/* ----------------------------- Pin Configuration ----------------------------- */
#define BUTTON1_PIN 3
#define BUTTON2_PIN 4
#define BUTTON3_PIN 5
#define LED1_PIN 6
#define LED2_PIN 7
#define LED3_PIN 8
#define BUZZER_PIN 9

const int BUTTON1 = BUTTON1_PIN;
const int BUTTON2 = BUTTON2_PIN;
const int BUTTON3 = BUTTON3_PIN;
const int LED1 = LED1_PIN;
const int LED2 = LED2_PIN;
const int LED3 = LED3_PIN;
const int BUZZER = BUZZER_PIN;

/* ----------------------------- Debounce Variables ----------------------------- */
unsigned long lastDebounceTimeB1 = 0;
unsigned long lastDebounceTimeB2 = 0;
unsigned long lastDebounceTimeB3 = 0;
unsigned long debounceDelay = 50;

/* ----------------------------- Button States ----------------------------- */
int prevButtonStateB1 = HIGH; // HIGH when not pressed (INPUT_PULLUP)
int prevButtonStateB2 = HIGH;
int prevButtonStateB3 = HIGH;
int currentButtonStateB1 = HIGH;
int currentButtonStateB2 = HIGH;
int currentButtonStateB3 = HIGH;

bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;

/* ----------------------------- Game State Variables ----------------------------- */
int difficulty;
int currentWord = 1;
int attempts = 5;

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long waitInterval = 1000;

unsigned long timeoutStart = 0;
unsigned long timeoutDuration = 0;
bool timeoutActive = false;

/* ----------------------------- Game States ----------------------------- */
enum GameState {
  DIFFICULTY_MENU,
  WAIT_DIFFICULTY,
  OPTIONS_MENU,
  WAIT_OPTION,
  GAME_INTERFACE,
  RESULT_INTERFACE,
  WAIT_GAME_OPTION,
  GUESS_WORD,
  GUESS_LETTER,
  GAME_OVER
};

GameState currentState = DIFFICULTY_MENU;

/* ----------------------------- Game Content ----------------------------- */
String words[] = {"place", "brave", "lover"};
String displayWord = "_._._._._";
String guessedWord;
char guessedLetter;

bool playerWon = false;
bool messageDisplayed = false;

/* ----------------------------- Setup ----------------------------- */
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

/* ----------------------------- Utility Functions ----------------------------- */

// Debounce button read function
bool readButtonWithDebounce(int pin, unsigned long &lastDebounceTime, int &prevState, int &currState) {
  int reading = digitalRead(pin);
  bool pressed = false;

  if (reading != prevState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currState) {
      currState = reading;
      if (currState == LOW) { // LOW because of PULLUP
        pressed = true;
      }
    }
  }

  prevState = reading;
  return pressed;
}

// Start timeout timer
void startTimeout(unsigned long duration) {
  timeoutStart = millis();
  timeoutDuration = duration;
  timeoutActive = true;
}

// Check if timeout has expired
bool isTimeoutExpired() {
  if (!timeoutActive) return false;

  if (millis() - timeoutStart >= timeoutDuration) {
    timeoutActive = false;
    return true;
  }
  return false;
}

// Cancel timeout timer
void cancelTimeout() {
  timeoutActive = false;
}

/* ----------------------------- Main Loop ----------------------------- */
void loop() {
  currentMillis = millis();

  // ----------- Button Debounce Handling ------------
  if (readButtonWithDebounce(BUTTON1, lastDebounceTimeB1, prevButtonStateB1, currentButtonStateB1)) {
    button1Pressed = true;
  } else if (readButtonWithDebounce(BUTTON2, lastDebounceTimeB2, prevButtonStateB2, currentButtonStateB2)) {
    button2Pressed = true;
  } else if (readButtonWithDebounce(BUTTON3, lastDebounceTimeB3, prevButtonStateB3, currentButtonStateB3)) {
    button3Pressed = true;
  }

  // ----------- Game Logic Update Interval ------------
  if (currentMillis - previousMillis >= waitInterval) {
    previousMillis = currentMillis;

    // ----------- Timeout Check ------------
    if (isTimeoutExpired()) {
      Serial.println("Time's up!");
      attempts--;
      messageDisplayed = false;
      if (attempts == 0) {
        playerWon = false;
        currentState = GAME_OVER;
      } else {
        currentState = GAME_INTERFACE;
      }
    }

    // ----------- State Machine ------------
    switch (currentState) {
      case DIFFICULTY_MENU:
        waitInterval = 1000;
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(BUZZER, LOW);
        Serial.println("Press a button to choose difficulty:");
        Serial.println("(1) Easy");
        Serial.println("(2) Medium");
        Serial.println("(3) Hard");
        currentState = WAIT_DIFFICULTY;
        break;

      case WAIT_DIFFICULTY:
        if (button1Pressed && !button2Pressed && !button3Pressed) {
          difficulty = 1;
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = OPTIONS_MENU;
          digitalWrite(LED1, HIGH);
        } else if (!button1Pressed && button2Pressed && !button3Pressed) {
          difficulty = 2;
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = OPTIONS_MENU;
          digitalWrite(LED2, HIGH);
        } else if (!button1Pressed && !button2Pressed && button3Pressed) {
          difficulty = 3;
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = OPTIONS_MENU;
          digitalWrite(LED3, HIGH);
        }
        break;

      case OPTIONS_MENU:
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        Serial.println("Press a button to choose an option:");
        Serial.println("(1) Start Game");
        Serial.println("(2) Go Back");
        currentState = WAIT_OPTION;
        break;

      case WAIT_OPTION:
        if (button1Pressed && !button2Pressed) {
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = GAME_INTERFACE;
          digitalWrite(LED1, HIGH);
        } else if (!button1Pressed && button2Pressed) {
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = DIFFICULTY_MENU;
          digitalWrite(LED2, HIGH);
        }
        break;

      case GAME_INTERFACE:
        digitalWrite(LED1, LOW);
        digitalWrite(LED3, LOW);
        Serial.print("Word ");
        Serial.print(currentWord);
        Serial.print(": ");
        Serial.println(displayWord);
        Serial.print("Attempts: ");
        Serial.println(attempts);
        Serial.println("Press a button to choose:");
        Serial.println("(1) Guess full word");
        Serial.println("(2) Guess a letter");
        currentState = WAIT_GAME_OPTION;
        break;

      case RESULT_INTERFACE:
        Serial.print("Word ");
        Serial.print(currentWord);
        Serial.print(": ");
        Serial.println(displayWord);
        switch (difficulty) {
          case 1:
            playerWon = true;
            currentState = GAME_OVER;
            break;
          case 2:
            if (currentWord < 2) {
              Serial.println("Press button (3) to continue.");
              currentState = WAIT_GAME_OPTION;
            } else {
              playerWon = true;
              currentState = GAME_OVER;
            }
            break;
          case 3:
            if (currentWord < 3) {
              Serial.println("Press button (3) to continue.");
              currentState = WAIT_GAME_OPTION;
            } else {
              playerWon = true;
              currentState = GAME_OVER;
            }
            break;
        }
        break;

      case WAIT_GAME_OPTION:
        if (button1Pressed && !button2Pressed && !button3Pressed) {
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = GUESS_WORD;
          digitalWrite(LED1, HIGH);
        } else if (!button1Pressed && button2Pressed && !button3Pressed) {
          button1Pressed = button2Pressed = button3Pressed = false;
          currentState = GUESS_LETTER;
          digitalWrite(LED2, HIGH);
        } else if (!button1Pressed && !button2Pressed && button3Pressed) {
          button1Pressed = button2Pressed = button3Pressed = false;
          currentWord++;
          displayWord = "_._._._._";
          currentState = GAME_INTERFACE;
          digitalWrite(LED3, HIGH);
        }
        break;

      case GUESS_WORD:
        digitalWrite(LED1, LOW);
        if (!messageDisplayed) {
          Serial.println("Enter the full word:");
          messageDisplayed = true;
          switch (difficulty) {
            case 1: startTimeout(120000); break;
            case 2: startTimeout(60000); break;
            case 3: startTimeout(30000); break;
          }
        }

        if (Serial.available() > 0) {
          guessedWord = Serial.readStringUntil('\n');
          guessedWord.trim();
          guessedWord.toLowerCase();
          cancelTimeout();
          messageDisplayed = false;

          if (guessedWord.compareTo(words[currentWord - 1]) == 0) {
            Serial.println("Correct word!");
            displayWord = words[currentWord - 1];
            currentState = RESULT_INTERFACE;
          } else if(guessedWord.length() != 5) {
            Serial.println("The word must have 5 letters.");
          } else {
            Serial.println("Incorrect word!");
            attempts--;
            if (attempts == 0) {
              playerWon = false;
              currentState = GAME_OVER;
            } else {
              currentState = GAME_INTERFACE;
            }
          }
        }
        break;

      case GUESS_LETTER:
        digitalWrite(LED2, LOW);
        if (!messageDisplayed) {
          Serial.println("Enter a letter:");
          messageDisplayed = true;
          switch (difficulty) {
            case 1: startTimeout(120000); break;
            case 2: startTimeout(60000); break;
            case 3: startTimeout(30000); break;
          }
        }

        if (Serial.available() > 0) {
          guessedLetter = Serial.read();
          guessedLetter = tolower(guessedLetter);
          cancelTimeout();
          messageDisplayed = false;

          if (words[currentWord - 1].indexOf(guessedLetter) == -1) {
            Serial.println("Incorrect letter!");
            attempts--;
            currentState = (attempts == 0) ? GAME_OVER : GAME_INTERFACE;
          } else {
            Serial.println("Correct letter!");
            // The position in the interface is calculated considering the format "_._._._._"
            // Word position:         0 1 2 3 4
            // Interface position:    0 2 4 6 8
            for (int i = 0; i < words[currentWord - 1].length(); i++) {
              if (words[currentWord - 1].charAt(i) == guessedLetter) {
                int uiPos = i * 2;
                displayWord.setCharAt(uiPos, guessedLetter);
              }
            }
            if (displayWord.indexOf("_") == -1) {
              currentState = RESULT_INTERFACE;
              displayWord.replace(".","");
            } else {
              currentState = GAME_INTERFACE;
            }
          }
        }
        break;

      case GAME_OVER:
        if (playerWon) {
          Serial.println("Congratulations!");
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
        } else {
          Serial.println("Game Over!");
          digitalWrite(BUZZER, HIGH);
        }
        attempts = 5;
        currentWord = 1;
        displayWord = "_._._._._";
        waitInterval = 2000;
        currentState = DIFFICULTY_MENU;
        break;

      default:
        break;
    }
  }
}