#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize I2C LCD (address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin definitions
const int LDR_PIN = A0;
const int HCSR501_PIN = 2;
const int LED_ALERTA_PIN = 3;

// Sensor status variables
volatile bool presencaDetectada = false;
bool luzAcesa = false;
int ldrValue = 0;

// LDR threshold
const int LDR_THRESHOLD = 300;

// Timing for sensor reading
unsigned long previousMillis_sensor = 0;
const long interval_sensor = 1000; // every 1 second

// Timer logic for alert trigger
unsigned long tempoTimerInicio = 0;
const long INTERVALO_TIMER = 10000; // 10 seconds
volatile bool timerAtivo = false;
bool alertaAtivo = false;

// LCD update control
String ultimaMensagem = "";
unsigned long previousMillis_lcd = 0;
const long interval_lcd = 500; // every 500 ms

bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // Communication with PIC
  Serial.println("Initializing Monitoring System...");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait...");
  delay(2000);

  pinMode(HCSR501_PIN, INPUT);
  pinMode(LED_ALERTA_PIN, OUTPUT);

  // Interrupt on falling edge (no motion)
  attachInterrupt(digitalPinToInterrupt(HCSR501_PIN), iniciaTimer, FALLING);

  Serial.println("System Ready!");
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // --- Sensor reading ---
  if (currentMillis - previousMillis_sensor >= interval_sensor) {
    previousMillis_sensor = currentMillis;

    ldrValue = analogRead(LDR_PIN);
    luzAcesa = (ldrValue > LDR_THRESHOLD);
    presencaDetectada = digitalRead(HCSR501_PIN);

    // --- Timer & alert logic ---
    if (!alertaAtivo) {
      if (timerAtivo) {
        if (!presencaDetectada && luzAcesa) {
          ledState = !ledState;
          digitalWrite(LED_ALERTA_PIN, ledState);

          if (currentMillis - tempoTimerInicio >= INTERVALO_TIMER) {
            Serial.println("*** ALERT: LIGHT ON WITH NO MOVEMENT FOR 10s! ***");
            Serial1.write('A'); // Send alert to PIC
            alertaAtivo = true;
            digitalWrite(LED_ALERTA_PIN, HIGH);
            timerAtivo = false;
          }
        } else {
          // Cancel timer if presence or light off
          if (presencaDetectada) {
            Serial.println("Movement detected - Timer canceled");
          } else if (!luzAcesa) {
            Serial.println("Light off - Timer canceled");
          }
          timerAtivo = false;
          digitalWrite(LED_ALERTA_PIN, LOW);
        }
      } else {
        digitalWrite(LED_ALERTA_PIN, LOW);
      }
    }

    // Define message for LCD
    if (alertaAtivo) {
      ultimaMensagem = "TURN OFF THE LIGHT";
    } else if (presencaDetectada && luzAcesa) {
      ultimaMensagem = "Presence Y Light Y";
    } else if (presencaDetectada && !luzAcesa) {
      ultimaMensagem = "Presence Y Light N";
    } else if (!presencaDetectada && luzAcesa) {
      ultimaMensagem = "Presence N Light Y";
    } else {
      ultimaMensagem = "Presence N Light N";
    }
  }

  // --- LCD update ---
  if (currentMillis - previousMillis_lcd >= interval_lcd) {
    previousMillis_lcd = currentMillis;
    atualizaLCD(ultimaMensagem, ldrValue);
  }

  // --- Read PIC response ---
  if (Serial1.available()) {
    char picResponse = Serial1.read();
    if (picResponse == 'R') {
      Serial.println("PIC responded: Resetting alert.");
      alertaAtivo = false;
      timerAtivo = false;
      digitalWrite(LED_ALERTA_PIN, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Monitoring...");
      ultimaMensagem = "Monitoring...";
    }
  }
}

// LCD display function
void atualizaLCD(String mensagem) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensagem);

  lcd.setCursor(0, 1);
  lcd.print("LDR: ");
  lcd.print(ldrValue);

  if (timerAtivo && !alertaAtivo) {
    unsigned long tempoRestante = (INTERVALO_TIMER - (millis() - tempoTimerInicio)) / 1000;
    Serial.print("T:");
    Serial.print(tempoRestante);
    Serial.println(" s");
  } else if (alertaAtivo) {
    lcd.setCursor(0, 1);
    lcd.print(">>> SAVE POWER <<<");
  }
}

// Interrupt function: triggered when motion stops
void iniciaTimer() {
  if (!alertaAtivo) {
    tempoTimerInicio = millis();
    timerAtivo = true;
    alertaAtivo = false;
  }
}