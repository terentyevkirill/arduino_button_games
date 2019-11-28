#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

const int yellow_but_pin = 2;
const int blue_but_pin = 3;
const int restart_but_pin = 4;

const int red_pin = 5;
const int green_pin = 6;
const int blue_pin = 7;


bool isPlaying = false;

byte customChar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};


void setup() {
  lcd_init();
  init_buttons();
  initRGB();
  servo.attach(9);
  servo.write(80);
  
}



void loop() {
  menu();
  delay(100);
}


void menu() {
    lcd.clear();
    turn_off_rgb();
    servo.write(80);
    lcd.setCursor(0,0);
    lcd.print("L - play brain");
    lcd.setCursor(0, 1);
    lcd.print("R - play clicker");
    for(;;)
    {
      if (digitalRead(blue_but_pin)) {
        start_brain();
        delay(3000);
        break;
      }
      if(digitalRead(yellow_but_pin))
      {
        start_clicker();
        delay(3000);
        break;
      }
      if (digitalRead(restart_but_pin)) {
        show_team();
        delay(3000);
        break;
      }
    }
  
}

void init_buttons() {
  pinMode(blue_but_pin, INPUT);
  pinMode(yellow_but_pin, INPUT);
  pinMode(restart_but_pin, INPUT);
}
void lcd_init() {
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

}

void start_brain() {
    if (!isPlaying) {
      isPlaying = true;
      turn_off_rgb();
      lcd.clear();
      lcd.print("Brain ring");
      lcd.setCursor(0, 1);
      lcd.print("Start in: ");
      for (int i=3; i>=0; i--) {
        lcd.setCursor(10, 1);
        lcd.print(i);
        delay(1000);
      }
      lcd.clear();
      digitalWrite(red_pin, HIGH);
      int randomTimeInSec = random(3, 11);
      delay(randomTimeInSec * 1000);
      // lcd and rgb
      digitalWrite(red_pin, LOW);
      digitalWrite(green_pin, HIGH);
      lcd.setCursor(5, 0);
      lcd.print("PRESS!");
      while(true) {
        if (digitalRead(blue_but_pin)){
          digitalWrite(green_pin, LOW);
          digitalWrite(blue_pin, HIGH);
          lcd.clear();
          lcd.print("Left player WON!");
          servo.write(0);
          isPlaying = false;
          break;
        }
        if (digitalRead(yellow_but_pin)){
          digitalWrite(green_pin, LOW);
          digitalWrite(red_pin, HIGH);
          digitalWrite(green_pin, HIGH);
          lcd.clear();
          lcd.print("Right player WON!");
          servo.write(160);
          isPlaying = false;
          break;
        }
      }
    }
    
    
}

void start_clicker() {
  float counter = random(7, 9);
  if (!isPlaying) {
      isPlaying = true;
      turn_off_rgb();
      lcd.clear();
      update_servo(counter);
      lcd.print("Clicker");
      lcd.setCursor(0, 1);
      lcd.print("Start in: ");
      for (int i=3; i>=0; i--) {
        lcd.setCursor(10, 1);
        lcd.print(i);
        delay(1000);
      }
      lcd.clear();
      update_progressbar(counter);
      while(true) {
         if (counter <= 0) {
          digitalWrite(blue_pin, HIGH);
          lcd.clear();
          lcd.print("Left player WON!");
          isPlaying = false;
          break;
        } else if (counter >= 16) {
          digitalWrite(red_pin, HIGH);
          digitalWrite(green_pin, HIGH);
          lcd.clear();
          lcd.print("Right player WON!");
          isPlaying = false;
          break;
        }

        if (digitalRead(blue_but_pin)){
          counter -= 0.5;
          update_progressbar(counter);
          update_servo(counter);
        }
        if (digitalRead(yellow_but_pin)){
          counter += 0.5;
          update_progressbar(counter);
          update_servo(counter);
        }
        
        delay(50);
      }
      
  }
}

void turn_off_rgb() {
  digitalWrite(red_pin, LOW);
  digitalWrite(blue_pin, LOW);
  digitalWrite(green_pin, LOW);
}

void update_progressbar(float counter) {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("PRESS!");
  int point = ((int)counter);
  lcd.createChar(0, customChar);
  lcd.setCursor(counter, 1);
  lcd.write(0);
}

void update_servo(float counter) {
  servo.write(map(counter, 0.0, 16.0, 0, 160));
}

void initRGB() {
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
}

void show_team() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("$$$Numerical$$$");
  lcd.setCursor(0,1);
  lcd.print("$$$Advantage$$$");
}
