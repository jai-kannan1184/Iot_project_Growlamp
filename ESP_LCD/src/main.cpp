#include <Arduino.h>
#include "LiquidCrystal.h"

// This declares the pins used we can use the esp pins since they allow for serial data 
LiquidCrystal lcd(22, 23, 5, 18, 19, 21);


/*Blake you can also create custom byte chars this creates a skull the liquid crystal lib enables u to do this stuff*/
byte Skull[8] = {
0b00000,
0b01110,
0b10101,
0b11011,
0b01110,
0b01110,
0b00000,
0b00000
};

void setup() {
  /*
    Initialises the LCD 1602A with rows and columns guessing we are using the same LCD so
  */
lcd.begin(16, 2);

lcd.createChar(1, Skull);

//clear the LCD screen
lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

  //this will place the cursor on row 0 col 0 and print the text
  lcd.print("I Am JAI");

  // move the cursor to col 0  and row 1
  lcd.setCursor(0, 1);
  lcd.print("Hello Blake!");
  // print the skull 
  lcd.setCursor(14, 1);
	lcd.write(byte(1));


  sleep(2);

  lcd.clear();

  /*If you keep adding stuff to print on the screen make sure u set you cursor appropriately and you may need to call the clear function to clear text on the screen*/
}