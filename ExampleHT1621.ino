#include <HT1621.h> // include our library

HT1621 lcd; // create an "lcd" object

int i;


void setup() {

  lcd.begin(12, 11, 10, 9); // (cs, wr, Data, backlight)

  lcd.backlight(); // turn on the backlight led

  lcd.clear(); // clear the screen

}

void loop() {

    lcd.print(107.2, 1, 1);
    lcd.print(1234, 2, 0);

    lcd.Symbol(1);
    lcd.Symbol(2);

    i++;
    lcd.setLevel(i); //0 to 13
    delay(500);
    if (i > 17) i = 0;

//===========for the bars===========
  

}
