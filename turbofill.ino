
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(8, 3, 4, 5, 6, 7); // arduino to lcd pins
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 10, 11}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
const int maxVolume = 9999;
const int maxPulsesPerLitre = 99999;

boolean running = false;
boolean state = true;

const uint8_t pump_pin = 12;
const uint8_t sensor_pin = 2;
const uint8_t start_pin = 13;

uint16_t sensorLeastCount = 50;     /** match this to your sensor if its different from the regular one **/

uint32_t value = 0;
uint32_t target_value = 0;
uint32_t filled = 0;
bool done = false;
bool override = false;

uint16_t calibrationTarget = 0;
uint32_t calibrationPulses = 0;
uint16_t pulsesPerLitre = 0;

uint8_t pulsesPerLiterLowByteAddress = 0x00;
uint8_t pulsesPerLiterHighByteAddress = 0x01;


const int screen_splash = -1;
const int screen_main = 0;
const int screen_filling = 1;
const int screen_calibrate = 2;
const int screen_calibration_specify_target = 3;
const int screen_calibration_empty = 4;
const int screen_calibration_fill_target = 5;
const int screen_calibration_confirm = 6;
const int screen_calibration_saved = 7;

int screen = screen_splash;



void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  pulsesPerLitre = (EEPROM.read(pulsesPerLiterHighByteAddress) << 0x08) | EEPROM.read(pulsesPerLiterLowByteAddress);
  
  pinMode (sensor_pin, INPUT_PULLUP);
  pinMode (start_pin, INPUT_PULLUP);
  pinMode (pump_pin, OUTPUT);

}

void onPulse() {
  switch (screen) {
    case screen_filling:
      if (filled++ >= target_value) {
        digitalWrite(pump_pin, LOW);
        detachInterrupt(digitalPinToInterrupt(sensor_pin));
        done = true;
      }
      break;
    case screen_calibration_fill_target:
      calibrationPulses++;
      break;
    default:
      break;
  }
}

void loop() {
  char key;
  switch (screen) {
    case screen_splash:
      lcd.setCursor(0, 0);
      lcd.print("---TURBO FILL---");
      lcd.setCursor(0, 1);
      lcd.print("  www.gtgs.in  ");
      delay(2000);
      lcd.clear();
      done = false;
      screen = screen_main;
      break;

    case screen_main:
      /** read from EEPROM **/
      if (pulsesPerLitre == 0) {
        screen = screen_calibrate;
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT");
      lcd.setCursor(7, 0);
      lcd.print(value);
      lcd.print("ml");

      lcd.setCursor(0, 1);
      lcd.print("o-start  *-cal");

      key = keypad.getKey();
      switch (key) {
        case '*':
          lcd.clear();
          screen = screen_calibrate;
          break;
        case '#':
          value = 0;
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          value = (value * 10) + (key - 48);
          value = constrain(value, 0, maxVolume);
          break;
      }

      if (digitalRead(start_pin) == HIGH) {
        lcd.clear();
        if (value != 0) {
          done = false;
          filled = 0;
          target_value = (value * pulsesPerLitre) / 1000;
          attachInterrupt(digitalPinToInterrupt(sensor_pin), onPulse, RISING);
          digitalWrite(pump_pin, HIGH);
          screen = screen_filling;
        }
        delay(250);
      }
      break;

    case screen_filling:
      lcd.setCursor(0, 0);
      lcd.print(filled * 1000 / pulsesPerLitre);
      lcd.print(" ml/");
      lcd.print(value);
      lcd.print(" ml");
      lcd.setCursor(0, 1);
      if (digitalRead(pump_pin) == HIGH) {
        lcd.print("o-pause *-cancel");
      } else {
        lcd.print("o-cont. *-cancel");
      }
      key = keypad.getKey();
      switch (key) {
        case '*':
          lcd.clear();

          digitalWrite(pump_pin, LOW);
          screen = screen_main;
          break;
      }
      if (digitalRead(start_pin) == HIGH) {
        digitalWrite(pump_pin, !digitalRead(pump_pin));
        lcd.clear();
        delay(250);
      }
      if (done) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DONE!");
        delay(1000);
        value = 0;
        screen = screen_main;
      }
      break;
    case screen_calibrate:
      lcd.setCursor(0, 0);
      lcd.print("1000ml="); lcd.print((uint16_t)pulsesPerLitre);
      lcd.setCursor(0, 1);
      lcd.print("*-cal.  #-back");
      key = keypad.getKey();
      switch (key) {
        case '*':
          lcd.clear();
          calibrationTarget = 0;
          screen = screen_calibration_specify_target;
          break;
        case '#':
          lcd.clear();
          screen = screen_main;
          break;
        default:
          /** add the number **/
          break;
      }
      break;

    case screen_calibration_specify_target:
      lcd.setCursor(0, 0);
      lcd.print("TARGET");
      lcd.setCursor(7, 0);
      lcd.print(calibrationTarget);
      lcd.print("ml");

      lcd.setCursor(0, 1);
      lcd.print("*-set  #-back");

      key = keypad.getKey();
      switch (key) {
        case '*':
          lcd.clear();
          if (calibrationTarget != 0) {
            screen = screen_calibration_empty;
          }
          break;
        case '#':
          calibrationTarget = 0;
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          calibrationTarget = (calibrationTarget * 10) + (key - 48);
          calibrationTarget = constrain(calibrationTarget, 0, maxVolume);
          break;
      }
      break;

    case screen_calibration_empty:
      lcd.setCursor(0, 0);
      lcd.print("EMPTY THE CAN...");
      lcd.setCursor(0, 1);
      lcd.print("*-next  #-back");
      key = keypad.getKey();
      switch (key) {
        case '*':
          lcd.clear();
          calibrationPulses = 0;
          attachInterrupt(digitalPinToInterrupt(sensor_pin), onPulse, RISING);
          digitalWrite(pump_pin, HIGH);
          screen = screen_calibration_fill_target;
          break;
        case '#':
          lcd.clear();
          screen = screen_main;
          break;
        default:
          /** add the number **/
          break;
      }
      break;

    case screen_calibration_fill_target:
      lcd.setCursor(0, 0);
      lcd.print(calibrationTarget); lcd.print("ml="); lcd.print(calibrationPulses);
      lcd.setCursor(0, 1);
      if (digitalRead(pump_pin) == HIGH) {
        lcd.print("o-pause");
      } else {
        lcd.print("o-cont. *-save");
        key = keypad.getKey();
        switch (key) {
          case '*':
            lcd.clear();
            screen = screen_calibration_saved;
            break;
          case '#':
            break;
          default:
            /** add the number **/
            break;
        }
      }

      if (digitalRead(start_pin) == HIGH) {
        digitalWrite(pump_pin, !digitalRead(pump_pin));
        lcd.clear();
        delay(250);
      }
      break;

    case screen_calibration_saved:
      pulsesPerLitre = (uint16_t)((1000 * calibrationPulses) / calibrationTarget);
      lcd.setCursor(0, 0);
      lcd.print("1000ml="); lcd.print((uint16_t)pulsesPerLitre);
      lcd.setCursor(0, 1);
      EEPROM.write(pulsesPerLiterHighByteAddress, (pulsesPerLitre >> 8));
      EEPROM.write(pulsesPerLiterLowByteAddress, (pulsesPerLitre & 0xFF));
      lcd.print("SAVED!");
      delay(2000);
      lcd.clear();
      screen = screen_main;
      break;
  }

}
