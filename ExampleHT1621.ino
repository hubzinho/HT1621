#include <HT1621.h> // include our library

HT1621 lcd; // create an "lcd" object


void setup() {

  lcd.begin(12, 11, 10, 9); // (cs, wr, Data, backlight)

  lcd.backlight(); // turn on the backlight led

  lcd.clear(); // clear the screen

}

void loop() {

    lcd.print(1234, 1, 2);

    lcd.Symbol(1);

//===========for the bars===========
    int i;
 for ( i = 0; i <= 17; i++){  
  lcd.setLevel(i);
  
  delay(500);
 }

 for (i = 16; i > 0; i--){
    lcd.setLevel(i);   
  delay(500);
 
  
 }

}
