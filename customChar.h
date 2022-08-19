/*    This header file defines 8 custom characters that will be 
 * used on the LCD1602.
 *    LCD1602 only allows 8 custom characters, each character is 
 * made up of 40 pixels(5 * 8). You can use the following link
 * to help generating your own characters:
 *  
 * https://maxpromer.github.io/LCD-Character-Creator/
 *  
 *  
 *    Each character needs to be assigned to a number, check out 
 * the setup() in the above link on how to set up for a custom character.
 */

// set to byte 0
// display the top left of Muffet's face
byte spiderFace0[] = {
  B00000,
  B00000,
  B11000,
  B11100,
  B00111,
  B01110,
  B01111,
  B11001
};

// set to byte 1
// display the top right of Muffet's face
byte spiderFace1[] = {
  B00000,
  B00000,
  B00011,
  B00111,
  B11100,
  B01110,
  B11110,
  B10011
};

// set to byte 2
// display the bottom left of Muffet's face
byte spiderFace2[] = {
  B11111,
  B11011,
  B01101,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000
};

// set to byte 3
// display the bottom right of Muffet's face
byte spiderFace3[] = {
  B11111,
  B11011,
  B10110,
  B11000,
  B00000,
  B00000,
  B00000,
  B00000
};

// set to byte 4
// display "~" character
byte wave[] = {
  B00000,
  B00000,
  B00000,
  B01000,
  B10101,
  B00010,
  B00000,
  B00000
};

// set to byte 5
// display the left half of a heart
byte heart1[] = {
  B00000,
  B00000,
  B00110,
  B01111,
  B01111,
  B00111,
  B00011,
  B00001
};

// set to byte 6
// display the right half of a heart
byte heart2[] = {
  B00000,
  B00000,
  B01100,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};

// set to byte 7
// display a single spider
byte spider[] = {
  B00000,
  B10001,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B10001
};
