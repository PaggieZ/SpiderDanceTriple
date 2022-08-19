/*    This is the main file of the entire program. */

/************************ HEADER FILES AND LIBRARIES ************************/ 
#include "notePeriod.h"
#include "noteScore.h"
#include "customChar.h"
#include "pitches.h"
#include "displayNoteFunc.h"
#include <LiquidCrystal.h>



/******************************* PIN NUMBERS ********************************/
#define BuzzB 2 // buzzer to display the bass part
#define BuzzV1 3 // buzzer to display the violin1 part (main melody)
#define BuzzV2 11 // buzzer to display the violin2 part (counter-melody)
#define BuzzSE 12 // buzzer to display the sound effects



/*************************** SET UP LCD1602 ******************************/
int rs = 4;
int en = 5;
int d4 = 6;
int d5 = 7;
int d6 = 8;
int d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// When indicating a coordinate on lcd in the comment, the first element 
// represents columns, the second element represents rows on LCD1602.
// (col, row)
// There are 2 rows and 16 columns on a lcd screen.


/**************************** GLOBAL VARIABLES ***************************/

/*    Create a long const variable to set the tempo for the entire song,
 * it represents the length of a beat(a quarter note) in microsecond.
 
 *    When the tempo is 115bpm(beats per minute), each beat would be 
 * 521739 microseconds long.
 * (60,000,000 microseconds / 115 beats) = 521739 microseconds per beat.
 
 *    Since each beat will be sub divided into 16 parts to generate different 
 * note length, and all variables are represented by integers. Therefore,
 * (BEATLENGTH % 16 = 0) must be true. 521744 is the closest number to 521739
 * which satisfies this condition.
 */
  const unsigned long BEATLENGTH = 521744; // tempo 115

/*    Since the music score is made up of arrays, we need an index for 
 * each array to run through the entire music score.
 */
  unsigned int V1index = 0; // index for main melody(V1)
  unsigned int V2index = 0; // index for counter melody(V2)
  unsigned int Bindex = 0; // index for bass(B)

/*    In order for a buzzer to play in the right tone, we need to turn the
 * buzzer on and off for a constant period of time. Check "notePeriod.h" 
 * for details on how to define and use the note period values.
 */
  // create an int variable to store the delay time for the next switch
  // motion on any buzzer,
  // delayTime = 10 represents the next switch mostion will take place
  // after 10 microseconds.
  unsigned int delayTime = 0; 
  // create a long variable to keep track of the current length of the beat,
  // curLength = 0 represents the very beginning of a beat.
  unsigned long curLength = 0;
  // create three long variables to store the next time for each buzzer to 
  // be switched on or off.
  // for convinence, NST = next switch time
  // V1nextSwitchTime = 2000 means when curLength = 2000, switch V1 buzzer.
  unsigned long V1nextSwitchTime = 0; // NST for V1
  unsigned long V2nextSwitchTime = 0; // NST for V2
  unsigned long BnextSwitchTime = 0; // NST for B
  // create three byte variables to store the current state of each buzzer,
  // V1state = LOW means the V1 buzzer is off, HIGH means on.
  byte V1state = LOW; // buzzer state for V1
  byte V2state = LOW; // buzzer state for V2
  byte Bstate = LOW; // buzzer state for B
  // create three long variables to store the note length for the current note
  // on each buzzer,
  // for convinence, NL = note length,
  // V1noteLength = 5000 means the current note will play for 5000 microseconds
  // on V1 buzzer.
  unsigned long V1noteLength = 0; // NL for the current V1 note
  unsigned long V2noteLength = 0; // NL for the current V2 note
  unsigned long BnoteLength = 0; // NL for the current B note
  // create and initialize max and min variables for NST and NL, they are used to
  // determine which buzzer will be switched first(minNST) or last(maxNST).
  unsigned long minNextSwitchTime = 0; // min for NST
  unsigned long maxNextSwitchTime = 0; // max for NST
  unsigned long minNoteLength = 0; // min for NL
  unsigned long maxNoteLength = 0; // max for NL



/******************************* PROTOTYPES ******************************/
/* Functions for displaying the music and animation */
  // function for playing the music on the buzzers
  void play(int[], int[], int, int[], int[], int, int[], int[], int);
  // switch the state of a buzzer
  byte switchState(byte, int);
  // check whether an int is zero (used for passing rest notes)
  int checkZero(int);
  // find the min of three long variables 
  long minVal(long, long, long);
  // find the max of three long variables
  long maxVal(long, long, long);

/* Functions for displaying text and images on the LCD1602 screen */
  // display the current note that is playing on the buzzer
  String displayNote(int); // defined in a seperated header file named "displayNoteFunc.h"
  // display text on the screen
  void displayL1(String); // display a string at (0, 0)
  void displayL2(String); // display a string at (0, 1)
  void displayL2Empty(); // empty line 2
  void displayEmpty(int, int); // display a single empty character
  // display battle entrance images
  void displayFour();
  void displayThree();
  void displayTwo();
  void displayFullHeart();
  // display heart related images
  void displayHeartL1(int, int); // display a heart in line 1
  void displayHeartL1Left(); // display a heart at (0, 0)
  void displayHeartL1Right(); // display a heart at (1, 0)
  void displayHeartL2(int, int); // display a heart in line 2
  void displayHeartL2Fulla(); // display eight complete hearts in L2 
  void displayHeartL2Fullb(); // display seven complete hearts in L2 with two edges
  void displayHeartL2Space(); // display 5 hearts with 1 space in between in L2 (transparent)
  void displayHeartL2a(); // display 3 hearts with 3 spaces in between in L2 (transparent)
  // display spider related images
  void displaySpiderFace(); // display Muffet's face at the left side of the screen
  void displaySpider(int, int); // display a spider
  void displaySpiderL1a(); // display spider L1 right
  void displaySpiderL1b(); // display spider L1 left
  void displaySpiderL1c(); // display all spider L1
  void displaySpiderL1d(); // empty all spider L1
  void displaySpiderL2a(); // display all spider L2
  void displaySpiderL2b(); // display odd index spider L2
  void displaySpiderL2c(); // display even index spider L2
  void displaySpiderL2d(); // display odd index spider L2 (transparent)
  void displaySpiderL2e(); // display even index spider L2 (transparent)
  void displaySpiderL2f(); // display three spiders with 4 spaces (transparent)

/* Functions for displaying sound effects on BuzzSE */
  void playPhoneRing(); // display the phone ring
  void playMuffetLaugh(); // display Muffet's laugh
  void playClick(); // display the click sound
  void playEnterBattle(); // display the battle entrance

/* Functions for displaying different parts of the music and animation */
  void displayBeginning(); // display the beginning
  void displayM1_M16(); // display measure 1 to measure 16
  void displayM1R_M18(); // repeat measure 1 to measure 14 with different animations
  void displayM19_M34(); // display measure 19 to measure 34
  void displayM35_M50(); // display measure 35 to measure 50
  void displayM51_M66(); // display measure 51 to measure 66
  void displayM67_M78(); // display measure 67 to measure 78
  void displayM78_M86(); // display measure 78 to measure 86
  void displayEnding(); // display the ending


/**************************** setup FUNCTION ****************************/
/*    The setup function calls all the functions that are needed to run the
 * entire program.
 */
void setup() {
  lcd.begin(16, 2);// initialize the LCD1602
  lcd.createChar(0, spiderFace0); // set byte 0 of LCD
  lcd.createChar(1, spiderFace1); // set byte 1 of LCD
  lcd.createChar(2, spiderFace2); // set byte 2 of LCD
  lcd.createChar(3, spiderFace3); // set byte 3 of LCD
  lcd.createChar(4, wave); // set byte 4 of LCD
  lcd.createChar(5, heart1); // set byte 5 of LCD
  lcd.createChar(6, heart2); // set byte 6 of LCD
  lcd.createChar(7, spider); // set byte 7 of LCD
  // set all the pins connected with a buzzer to OUTPUT mode
  pinMode(BuzzB, OUTPUT); // set BuzzB to OUTPUT mode
  pinMode(BuzzV1, OUTPUT); // set BuzzV1 to OUTPUT mode
  pinMode(BuzzV2, OUTPUT); // set BuzzV2 to OUTPUT mode
  pinMode(BuzzSE, OUTPUT); // set BuzzSe to OUTPUT mode
  // run the entire music and animation
  lcd.clear(); // clear the LCD screen
  displayBeginning(); // display Muffet's phone call and battle entrance
  lcd.clear(); // clear the LCD screen
  displayHeartL1(0, 1); // display a heat at (0, 0)
  displayM1_M16(); // display music and animation from measure 1 to measure 16
  displayM1R_M18(); // display music and animation from measure 1 to measure 14, measure 17 and 18
  displayM19_M34(); // display music and animation from measure 19 to measure 34
  displayM35_M50(); // display music and animation from measure 35 to measure 50
  displayM51_M66(); // display music and animation from measure 51 to measure 66
  displayM67_M78(); // display music and animation from measure 67 to measure 78
  displayM78_M86(); // display music and animation from measure 78 to measure 86
  displayEnding(); // display the battle exit, dot spider, thank you note.
} // end setup()


/**************************** loop FUNCTION ****************************/
void loop() {
  // empty
} // end loop()


/********** FUNCTION DEFINITION FOR DISPLAYING MUSIC AND ANIMATION ************/
/******************************* play FUNCTION ********************************/
/*    The play() function can play three different notes on three buzzer
 * simultaneousely with different note lengths for one beat.
 *    It takes in three sets of arrays, each set contains two arrays and a int.
 * The note[] arrays stores the notes in this beat, the beat[] arrays stores the 
 * noteLength of each note in the note[] array. The corresponding note[] and beat[]
 * should have the same size which is one of the parameters in the set.
 */
void play(int V1note[], int V1beat[], int V1size, 
          int V2note[], int V2beat[], int V2size, 
          int Bnote[], int Bbeat[], int Bsize) {
  // initialize all the indexes
  V1index = 0;
  V2index = 0;
  Bindex = 0;
  // initialize delayTime and curLength
  delayTime = 0;
  curLength = 0;
  // initialize all NST, the first NST should just be the note period
  // use checkZero() to assign NST for rest notes
  V1nextSwitchTime = checkZero(V1note[V1index]);
  V2nextSwitchTime = checkZero(V2note[V2index]);
  BnextSwitchTime = checkZero(Bnote[Bindex]);
  // initialize the buzzers and their states
  V1state = LOW;
  V2state = LOW;
  Bstate = LOW;
  digitalWrite(BuzzV1, V1state);
  digitalWrite(BuzzV2, V2state);
  digitalWrite(BuzzB, Bstate);
  // initialize the note length for the first note
  V1noteLength = BEATLENGTH / 16 * V1beat[V1index];
  V2noteLength = BEATLENGTH / 16 * V2beat[V2index];
  BnoteLength = BEATLENGTH / 16 * Bbeat[Bindex];
  // initialize max and min variables for NST and NL
  minNextSwitchTime = minVal(V1nextSwitchTime, V2nextSwitchTime, BnextSwitchTime);
  maxNextSwitchTime = maxVal(V1nextSwitchTime, V2nextSwitchTime, BnextSwitchTime);
  minNoteLength = minVal(V1noteLength, V2noteLength, BnoteLength);
  maxNoteLength = maxVal(V1noteLength, V2noteLength, BnoteLength);

  // display the currently playing note on the LCD screen
  lcd.setCursor(8, 0);
  lcd.print(displayNote(V1note[V1index])); // display V1 note at (8, 0)
  lcd.setCursor(13, 0);
  lcd.print(displayNote(V2note[V2index])); // display V2 note at (13, 0)
  lcd.setCursor(3, 0);
  lcd.print(displayNote(Bnote[Bindex])); // display B note at (3, 0)

  // use a while loop to play notes in this beat
  // when the beat is not over
  while(maxNoteLength <= BEATLENGTH) {
    // when the shortest note is not over
    while(maxNextSwitchTime <= minNoteLength) {
      // delay to the minNST
      delayTime = minNextSwitchTime - curLength; // calculate delayTime in microseconds
      delayMicroseconds(delayTime); // delay for "delayTime" microseconds
      curLength = curLength + delayTime; // update current length

      // use an if statement to update NST
      if((V1nextSwitchTime == minNextSwitchTime) && 
            (V2nextSwitchTime == minNextSwitchTime) &&
              (BnextSwitchTime == minNextSwitchTime)) { // update all V1NST, V2NST, BNST
        // update V1, V2, B states
        V1state = switchState(V1state, V1note[V1index]);
        V2state = switchState(V2state, V2note[V2index]);
        Bstate = switchState(Bstate, Bnote[Bindex]);
        // update V1, V2, B buzzers
        digitalWrite(BuzzV1, V1state);
        digitalWrite(BuzzV2, V2state);
        digitalWrite(BuzzB, Bstate);
        // update V1, V2, B NST
        V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]);
        V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]);
        BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]);          
      } // end if
      else if((V1nextSwitchTime == minNextSwitchTime) && 
                 (V2nextSwitchTime == minNextSwitchTime)) { // update V1NST, V2NST
        // update V1, V2 states
        V1state = switchState(V1state, V1note[V1index]);
        V2state = switchState(V2state, V2note[V2index]);
        // update V1, V2 buzzers
        digitalWrite(BuzzV1, V1state);
        digitalWrite(BuzzV2, V2state);
        // update V1, V2 NST
        V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]);
        V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]);
      } // end else if
      else if((V1nextSwitchTime == minNextSwitchTime) && 
                  (BnextSwitchTime == minNextSwitchTime)) { // update V1NST, BNST 
        // update V1, B states
        V1state = switchState(V1state, V1note[V1index]);
        Bstate = switchState(Bstate, Bnote[Bindex]);
        // update V1, B buzzers
        digitalWrite(BuzzV1, V1state);
        digitalWrite(BuzzB, Bstate);
        // update V1, B NST
        V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]);
        BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); 
      } // end else if
      else if((V2nextSwitchTime == minNextSwitchTime) && 
                (BnextSwitchTime == minNextSwitchTime)) { // update V2NST, BNST
        // update V2, B states
        V2state = switchState(V2state, V2note[V2index]);
        Bstate = switchState(Bstate, Bnote[Bindex]);
        // update V2, B buzzers
        digitalWrite(BuzzV2, V2state);
        digitalWrite(BuzzB, Bstate);
        // update V2, b NST
        V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]);
        BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]);
      } // end else if
      else if(minNextSwitchTime == V1nextSwitchTime) { // update V1NST only
        V1state = switchState(V1state, V1note[V1index]); // update V1 state
        digitalWrite(BuzzV1, V1state); // update V1 buzzer
        V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]); // update V1 NST
      } // end else if
      else if(minNextSwitchTime == V2nextSwitchTime) { // update V2NST only
        V2state = switchState(V2state, V2note[V2index]); // update V2 state
        digitalWrite(BuzzV2, V2state); // update V2 buzzer
        V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]); // update V2 NST
      } // end else if
      else if(minNextSwitchTime == BnextSwitchTime) { // update BNST only
        Bstate = switchState(Bstate, Bnote[Bindex]); // update B state
        digitalWrite(BuzzB, Bstate); // update B buzzer
        BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); // update B NST
      } // end else if

      // update max and min NST
      minNextSwitchTime = minVal(V1nextSwitchTime, V2nextSwitchTime, BnextSwitchTime); 
      maxNextSwitchTime = maxVal(V1nextSwitchTime, V2nextSwitchTime, BnextSwitchTime);
    } // end while


    // use an if statement to move V1NST, V2NST and BNST behind minNL
    if((BnextSwitchTime >= minNoteLength) && 
          (V1nextSwitchTime >= minNoteLength) &&
              (V2nextSwitchTime < minNoteLength)) { // when only BNST and V1NST are behind minBL
      // use while statement to move V2NST behind minBL
      while(V2nextSwitchTime < minNoteLength) {
        delayTime = V2nextSwitchTime - curLength; // calculate delay time in microseconds
        delayMicroseconds(delayTime); // delay for "delayTime" microseconds
        curLength = curLength + delayTime; // update curLength by adding delayTime
        V2state = switchState(V2state, V2note[V2index]); // update V2 state
        digitalWrite(BuzzV2, V2state); // update V2 buzzer
        V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]); // update V2 NST
      } // end while
    } // end if
    else if((BnextSwitchTime >= minNoteLength) && 
              (V2nextSwitchTime >= minNoteLength) &&
                (V1nextSwitchTime < minNoteLength)) { // when only BNST and V2NST are behind minBL
      // use while statement to move V1NST behind minBL
      while(V1nextSwitchTime < minNoteLength) {
        delayTime = V1nextSwitchTime - curLength; // calculate delay time in microseconds
        delayMicroseconds(delayTime); // delay for "delayTime" microseconds
        curLength = curLength + delayTime; // update curLength by adding delayTime
        V1state = switchState(V1state, V1note[V1index]); // update V1 state
        digitalWrite(BuzzV1, V1state); // update V1 buzzer
        V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]); // update V1 NST
      } // end while
    } // end else if
    else if((V1nextSwitchTime >= minNoteLength) && 
              (V2nextSwitchTime >= minNoteLength) &&
                (BnextSwitchTime < minNoteLength)) { // when only V1NST and V2NST are behind minBL
      // use while statement to move BNST behind minBL
      while(BnextSwitchTime < minNoteLength) {
        delayTime = BnextSwitchTime - curLength; // calculate delay time in microseconds
        delayMicroseconds(delayTime); // delay for "delayTime" microseconds
        curLength = curLength + delayTime; // update curLength by adding delayTime
        Bstate = switchState(Bstate, Bnote[Bindex]); // update B state
        digitalWrite(BuzzB, Bstate); // update B buzzer
        BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); // update B NST
      } // end while
    } // end else if
    else if((V1nextSwitchTime >= minNoteLength) && 
              (V2nextSwitchTime < minNoteLength) &&
                (BnextSwitchTime < minNoteLength)) { // when only V1NST is behind minBL
      // evaluate max and min NST between V2 and B
      maxNextSwitchTime = max(V2nextSwitchTime, BnextSwitchTime);
      minNextSwitchTime = min(V2nextSwitchTime, BnextSwitchTime);
      // use while loop to move (one or both) of (V2NST and BNST) behind minBL
      while(maxNextSwitchTime < minNoteLength) {
        // delay to minNST
        delayTime = minNextSwitchTime - curLength; // calculate delayTime in microseconds
        delayMicroseconds(delayTime); // delay "delayTime" microseconds
        curLength = curLength + delayTime; // update curLength by adding delayTime
        // use an if statement to update V2 and B NST
        if(V2nextSwitchTime == BnextSwitchTime) { // update both V2NST and BNST 
          // update V2 and B states
          V2state = switchState(V2state, V2note[V2index]); 
          Bstate = switchState(Bstate, Bnote[Bindex]);
          // update V2 and B buzzers
          digitalWrite(BuzzV2, V2state);
          digitalWrite(BuzzB, Bstate);
          // update V2 and B NST
          V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]);
          BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]);
        } // end if
        else if(minNextSwitchTime == V2nextSwitchTime) { // update V2NST only
          V2state = switchState(V2state, V2note[V2index]); // update V2 state
          digitalWrite(BuzzV2, V2state); // update V2 buzzers
          V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]); // update V2 NST
        } // end else if
        else if(minNextSwitchTime == BnextSwitchTime) { // update BNST only
          Bstate = switchState(Bstate, Bnote[Bindex]); // update B state
          digitalWrite(BuzzB, Bstate); // update B buzzer
          BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); // update B NST
        } // end else if
        // update max and min NST between V2 and B NST
        minNextSwitchTime = min(V2nextSwitchTime, BnextSwitchTime); 
        maxNextSwitchTime = max(V2nextSwitchTime, BnextSwitchTime);
        // (I repeated these two lines of code before the while loop because I needed
        //  the new max and min NST to enter the while loop)
      } // end while
      // use an if statement to move the remaining (V2NST or BNST) behind minBL
      if((BnextSwitchTime >= minNoteLength) && 
            (V2nextSwitchTime < minNoteLength)) { // when BNST is behind minBL
        // use while statement to move V2NST behind minBL
        while(V2nextSwitchTime < minNoteLength) {
          delayTime = V2nextSwitchTime - curLength; // calculate delayTime in microseconds
          delayMicroseconds(delayTime); // delay for "delayTime" microseconds
          curLength = curLength + delayTime; // update curLength by adding delayTime
          V2state = switchState(V2state, V2note[V2index]); // update V2 state
          digitalWrite(BuzzV2, V2state); // update V2 buzzer
          V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]); // update V2 NST
        } // end while
      } // end else if
      else if((V2nextSwitchTime >= minNoteLength) && 
                  (BnextSwitchTime < minNoteLength)) { // when V2NST is behind minBL
        // use while statement to move BNST behind minBL
        while(BnextSwitchTime < minNoteLength) {
          delayTime = BnextSwitchTime - curLength; // calculate delayTime in microseconds
          delayMicroseconds(delayTime); // delay for "delayTime" microseconds
          curLength = curLength + delayTime; // update curLength by adding delayTime
          Bstate = switchState(Bstate, Bnote[Bindex]); // update B state
          digitalWrite(BuzzB, Bstate); // update B buzzer
          BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); // update B NST
        } // end while
      } // end else if
    } // end else if
    else if((V2nextSwitchTime >= minNoteLength) && 
              (V1nextSwitchTime < minNoteLength) &&
                (BnextSwitchTime < minNoteLength)) { // when only V2NST is behind minBL
      // evaluate max and min NST between V1 and B
      maxNextSwitchTime = max(V1nextSwitchTime, BnextSwitchTime);
      minNextSwitchTime = min(V1nextSwitchTime, BnextSwitchTime);
      // use while loop to (move one or both) of (V1NST and BNST) behind minBL
      while(maxNextSwitchTime < minNoteLength) {
        // delay to minNST
        delayTime = minNextSwitchTime - curLength; // calculate delayTime in microseconds
        delayMicroseconds(delayTime); // delay "delayTime" microseconds
        curLength = curLength + delayTime; // update curLength by adding delayTime
        // use an if statement to update V1 and B NST
        if(V1nextSwitchTime == BnextSwitchTime) { // update both V1NST and BNST 
          // update V1 and B state
          V1state = switchState(V1state, V1note[V1index]);
          Bstate = switchState(Bstate, Bnote[Bindex]);
          // update V1 and B buzzers
          digitalWrite(BuzzV1, V1state);
          digitalWrite(BuzzB, Bstate);
          // update V1 and B NST
          V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]);
          BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]);
        } // end if
        else if(minNextSwitchTime == V1nextSwitchTime) { // update V1NST only
          V1state = switchState(V1state, V1note[V1index]); // update V1 state
          digitalWrite(BuzzV1, V1state); // update V1 buzzers
          V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]); // update V1 NST
        } // end else if
        else if(minNextSwitchTime == BnextSwitchTime) { // update BNST only
          Bstate = switchState(Bstate, Bnote[Bindex]); // update B state
          digitalWrite(BuzzB, Bstate); // update B buzzer
          BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); // update B NST
        } // end else if
        // update max and min NST between V1 and B
        minNextSwitchTime = min(V1nextSwitchTime, BnextSwitchTime); 
        maxNextSwitchTime = max(V1nextSwitchTime, BnextSwitchTime);
      } // end while
      // use an if statement to move the remaining (V1NST or BNST) behind minBL
      if((BnextSwitchTime >= minNoteLength) && 
            (V1nextSwitchTime < minNoteLength)) { // when BNST is behind minBL
        // use while statement to move V1NST behind minBL
        while(V1nextSwitchTime < minNoteLength) {
          delayTime = V1nextSwitchTime - curLength; // calculate delayTime in microseconds
          delayMicroseconds(delayTime); // delay for "delayTime" microseconds
          curLength = curLength + delayTime; // update curLength by adding delayTime
          V1state = switchState(V1state, V1note[V1index]); // update V1 state
          digitalWrite(BuzzV1, V1state); // update V1 buzzer
          V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]); // update V1 NST
        } // end while
      } // end else if
      else if((V1nextSwitchTime >= minNoteLength) && 
                  (BnextSwitchTime < minNoteLength)) { // when V1NST is behind minBL
        // use while statement to move BNST behind minBL
        while(BnextSwitchTime < minNoteLength) {
          delayTime = BnextSwitchTime - curLength; // calculate delayTime in microseconds
          delayMicroseconds(delayTime); // delay for "delayTime" microseconds
          curLength = curLength + delayTime; // update curLength by adding delayTime
          Bstate = switchState(Bstate, Bnote[Bindex]); // update B state
          digitalWrite(BuzzB, Bstate); // update B buzzer
          BnextSwitchTime = BnextSwitchTime + checkZero(Bnote[Bindex]); // update B NST
        } // end while
      } // end else if
    } // end else if
    else if((BnextSwitchTime >= minNoteLength) && 
              (V1nextSwitchTime < minNoteLength) &&
                (V2nextSwitchTime < minNoteLength)) { // when only BNST is behind minBL
      // evaluate max and min NST between V1 and V2 NST
      maxNextSwitchTime = max(V1nextSwitchTime, V2nextSwitchTime);
      minNextSwitchTime = min(V1nextSwitchTime, V2nextSwitchTime);
      // use while loop to move (one or both) of (V2NST and BNST) behind minBL
      while(maxNextSwitchTime < minNoteLength) {
        // delay to minNST
        delayTime = minNextSwitchTime - curLength; // calculate delayTime in microseconds
        delayMicroseconds(delayTime); // delay "delayTime" microseconds
        curLength = curLength + delayTime; // update curLength by adding delayTime
        // use an if statement to update V1 and V2 NST
        if(V1nextSwitchTime == V2nextSwitchTime) { // update both V1NST and V2NST
          // update V1 and V2 state
          V1state = switchState(V1state, V1note[V1index]);
          V2state = switchState(V2state, V2note[V2index]);
          // update V1 and V2 buzzers
          digitalWrite(BuzzV1, V1state);
          digitalWrite(BuzzV2, V2state);
          // update V1 and V2 NST
          V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]);
          V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]);
        } // end if
        else if(minNextSwitchTime == V1nextSwitchTime) { // update V1NST only
          V1state = switchState(V1state, V1note[V1index]); // update V1 state
          digitalWrite(BuzzV1, V1state); // update V1 buzzer
          V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]); // update V1 NST
        } // end else if
        else if(minNextSwitchTime == V2nextSwitchTime) { // update V2NST only
          V2state = switchState(V2state, V2note[V2index]); // update V2 state
          digitalWrite(BuzzV2, V2state); // update V2 buzzer
          V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]); // update V2 NST
        } // end else if
        // update max and min NST between V1 and V2
        minNextSwitchTime = min(V1nextSwitchTime, V2nextSwitchTime); 
        maxNextSwitchTime = max(V1nextSwitchTime, V2nextSwitchTime);
      } // end while
      // use an if statement to move the remaining (V2NST or BNST) behind minBL
      if((V2nextSwitchTime >= minNoteLength) && 
            (V1nextSwitchTime < minNoteLength)) { // when V2NST is behind minBL
        // use while statement to move V1NST behind minBL
        while(V1nextSwitchTime < minNoteLength) {
          delayTime = V1nextSwitchTime - curLength; // calculate delayTime in microseconds
          delayMicroseconds(delayTime); // delay for "delayTime" microseconds
          curLength = curLength + delayTime; // update curLength by adding delayTime
          V1state = switchState(V1state, V1note[V1index]); // update V1 state
          digitalWrite(BuzzV1, V1state); // update V1 buzzer
          V1nextSwitchTime = V1nextSwitchTime + checkZero(V1note[V1index]); // update V1 NST
        } // end while
      } // end else if
      else if((V1nextSwitchTime >= minNoteLength) && 
                  (V2nextSwitchTime < minNoteLength)) { // when V1NST is behind minBL
        // use while statement to move V2NST behind minBL
        while(V2nextSwitchTime < minNoteLength) {
          delayTime = V2nextSwitchTime - curLength; // calculate delayTime in microseconds
          delayMicroseconds(delayTime); // delay for "delayTime" microseconds
          curLength = curLength + delayTime; // update curLength by adding delayTime
          V2state = switchState(V2state, V2note[V2index]); // update V2 state
          digitalWrite(BuzzV2, V2state); // update V2 buzzer
          V2nextSwitchTime = V2nextSwitchTime + checkZero(V2note[V2index]); // update V2 NST
        } // end while
      } // end else if
    } // end else if    
    // update curLength to minBL
    delayTime = minNoteLength - curLength; // calculate delayTime in microseconds
    delayMicroseconds(delayTime); // delay for "delayTime" microseconds
    curLength = minNoteLength; // update curLength

    // use an if to update all variables when the beat is not over (curLength < beatLength)
    if(curLength != BEATLENGTH) {
      // use an if to prepare all variables before changing any note(s)
      if((V1noteLength == minNoteLength) &&
            (V2noteLength == minNoteLength) &&
              (BnoteLength == minNoteLength)) { // if all V1, V2, B need to be updated
        // update all index
        V1index = V1index + 1;
        V2index = V2index + 1;
        Bindex = Bindex + 1;
        // update all noteLength
        V1noteLength = V1noteLength + (BEATLENGTH / 16 * V1beat[V1index]);
        V2noteLength = V2noteLength + (BEATLENGTH / 16 * V2beat[V2index]);
        BnoteLength = BnoteLength + (BEATLENGTH / 16 * Bbeat[Bindex]);
        // update all NST
        V1nextSwitchTime = minNoteLength + checkZero(V1note[V1index]);
        V2nextSwitchTime = minNoteLength + checkZero(V2note[V2index]);
        BnextSwitchTime = minNoteLength + checkZero(Bnote[Bindex]);
        // update all state
        V1state = LOW;
        V2state = LOW;
        Bstate = LOW;
        // update all buzzers
        digitalWrite(BuzzV1, V1state);
        digitalWrite(BuzzV2, V2state);
        digitalWrite(BuzzB, Bstate);
      } // end if
      else if((V1noteLength == minNoteLength) &&
                 (V2noteLength == minNoteLength)){ // when V1 and V2 need to be updated
        // update V1 and V2 index
        V1index = V1index + 1;
        V2index = V2index + 1;
        // update V1 and V2 noteLength
        V1noteLength = V1noteLength + (BEATLENGTH / 16 * V1beat[V1index]);
        V2noteLength = V2noteLength + (BEATLENGTH / 16 * V2beat[V2index]);
        // update V1 and V2 NST
        V1nextSwitchTime = minNoteLength + checkZero(V1note[V1index]);
        V2nextSwitchTime = minNoteLength + checkZero(V2note[V2index]);
        // update V1 and V2 states
        V1state = LOW;
        V2state = LOW;
        // update V1 and V2 buzzers
        digitalWrite(BuzzV1, V1state);
        digitalWrite(BuzzV2, V2state);
      } // end else if
      else if((V1noteLength == minNoteLength) &&
                 (BnoteLength == minNoteLength)){ // when V1 and B need to be updated
        // update V1 and B index
        V1index = V1index + 1;
        Bindex = Bindex + 1;
        // update V1 and B noteLength
        V1noteLength = V1noteLength + (BEATLENGTH / 16 * V1beat[V1index]);
        BnoteLength = BnoteLength + (BEATLENGTH / 16 * Bbeat[Bindex]);
        // update V1 and B NST
        V1nextSwitchTime = minNoteLength + checkZero(V1note[V1index]);
        BnextSwitchTime = minNoteLength + checkZero(Bnote[Bindex]);
        // update V1 and B states
        V1state = LOW;
        Bstate = LOW;
        // update V1 and B buzzers
        digitalWrite(BuzzV1, V1state);
        digitalWrite(BuzzB, Bstate);
      } // end else if
      else if((BnoteLength == minNoteLength) &&
                 (V2noteLength == minNoteLength)){ // when V2 and B need to be updated
        // update V2 and B index
        V2index = V2index + 1;
        Bindex = Bindex + 1;
        // update V2 and B noteLength
        V2noteLength = V2noteLength + (BEATLENGTH / 16 * V2beat[V2index]);
        BnoteLength = BnoteLength + (BEATLENGTH / 16 * Bbeat[Bindex]);
        // update V2 and B NST
        V2nextSwitchTime = minNoteLength + checkZero(V2note[V2index]);
        BnextSwitchTime = minNoteLength + checkZero(Bnote[Bindex]);
        // update V2 and B states
        V2state = LOW;
        Bstate = LOW;
        // update V2 and B buzzers
        digitalWrite(BuzzV2, V2state);
        digitalWrite(BuzzB, Bstate);
      } // end else if
      else if(minNoteLength == V1noteLength) { // update V1
        V1index = V1index + 1; // update V1 index
        V1noteLength = V1noteLength + (BEATLENGTH / 16 * V1beat[V1index]); // update V1 noteLength
        V1nextSwitchTime = minNoteLength + checkZero(V1note[V1index]); // update V1 NST
        V1state = LOW; // update V1 state
        digitalWrite(BuzzV1, V1state); // update V1 buzzer
      } // end else if
      else if(minNoteLength == V2noteLength) { // update V2
        V2index = V2index + 1; // update V2 index
        V2noteLength = V2noteLength + (BEATLENGTH / 16 * V2beat[V2index]); // update V2 noteLength
        V2nextSwitchTime = minNoteLength + checkZero(V2note[V2index]); // update V2 NST
        V2state = LOW; // update V2 state
        digitalWrite(BuzzV2, V2state); // update V2 buzzer
      } // end else if
      else if(minNoteLength == BnoteLength) { // update B
        Bindex = Bindex + 1; // update B index
        BnoteLength = BnoteLength + (BEATLENGTH / 16 * Bbeat[Bindex]); // update B noteLength
        BnextSwitchTime = minNoteLength + checkZero(Bnote[Bindex]); // update B state
        Bstate = LOW; // update B state
        digitalWrite(BuzzB, Bstate); // update B buzzer
      } // end else if
      
      // update all max and min NST and noteLength
      minNextSwitchTime = minVal(V1nextSwitchTime, V2nextSwitchTime, BnextSwitchTime);
      maxNextSwitchTime = maxVal(V1nextSwitchTime, V2nextSwitchTime, BnextSwitchTime);
      minNoteLength = minVal(V1noteLength, V2noteLength, BnoteLength);
      maxNoteLength = maxVal(V1noteLength, V2noteLength, BnoteLength);

      // display V1, V2, B note names on the LCD screen
      lcd.setCursor(8, 0);
      lcd.print(displayNote(V1note[V1index])); // display V1 note name
      lcd.setCursor(13, 0);
      lcd.print(displayNote(V2note[V2index])); // display V2 note name
      lcd.setCursor(3, 0);
      lcd.print(displayNote(Bnote[Bindex])); // display B note name
    } // end if
    else { // update maxNL to exit the loop
      maxNoteLength = maxNoteLength + 1;
    } // end else
  } // end while (outer)
  return;
} // end play()


/******************************* switchState FUNCTION ********************************/
/*    The switchState() function can return the next state of a buzzer.
 *    It takes in a byte and an int. The byte variable is the current state of the 
 * buzzer, the int variable is the note period of the currently playing note. When
 * the currently playing note is a rest note (= 0), the switchState() function will
 * return LOW for the next state.
 */
byte switchState(byte curState, int note) {
  byte nextState = LOW; // create and initialize a byte variable for return
  if(note == 0) // if it is a rest note
    nextState = LOW; // always keeps the buzzer off 
  else { // if it is not a rest note
    if(curState == LOW) // when the buzzer is off
      nextState = HIGH; // turn it on 
  }
  return nextState; // return the next state of the buzzer
} // end switchState()


/******************************* checkZero FUNCTION ********************************/
/*    The checkZero() function can return the note period(int) for both rest notes and 
 * regular notes. Without this function, the program will skip the rest note instead
 * of delaying for the rest note length.
 *    The checkZwro() function takes in a note period, then evaluate the note type. 
 * If it is a regular note, the function will return its original period.
 * If it is a rest note, the function will set the note period to 10000.
 * (I tried setting this value to (BEATLENGTH / 16) but the outcome is rushing). 
 */
int checkZero(int note) {
  int period = 0; // create and initialize a return value
  if(note == 0) // if it is a rest note
    period = 10000; // set its period to 10000
  else // if it is a regular note
    period = note; // return its period
  return period; // return period
} // end checkZero()


/******************************* minVal FUNCTION ********************************/
/*    The minVal() function can return the minimum value out of three values.
 *    It takes in three long variables and returns a long variable.
 */
long minVal(long val1, long val2, long val3) {
  long minimum = 0; // create and initialize a return value
  minimum = min((min(val1, val2)), val3); // find the minimum
  return minimum; // return the minimum
} // end minVal()


/******************************* maxVal FUNCTION ********************************/
/*    The maxVal() function can return the maximum value out of three values.
 *    It takes in three long variables and returns a long variable.
 */
long maxVal(long val1, long val2, long val3) {
  long maximum = 0; // create and initialize a return value
  maximum = max((max(val1, val2)), val3); // find the maximum
  return maximum; // return the maximum
} // end maxVal()



/********** FUNCTION DEFINITION FOR DISPLAYING TEXT AND IMAGES ON LCD1602 ************/
/******************************* displayNote FUNCTION ********************************/
// defined in "displayNoteFunc.h"


/******************************* displayL1 FUNCTION ********************************/
/*    The displayL1() function can set the cursor at (0, 0) and display a string in 
 * row 1 of the LCD screen
 *    It takes in a string with no return value.
 */
void displayL1(String text) {
  lcd.setCursor(0, 0); // set the cursor at (0, 0)
  lcd.print(text); // print the text at the cursor
  return; // no return value
} // end displayL1()


/******************************* displayL2 FUNCTION ********************************/
/*    The displayL2() function can set the cursor at (0, 1) and display a string in 
 * row 2 of the LCD screen
 *    It takes in a string with no return value.
 */
void displayL2(String text) {
  lcd.setCursor(0, 1); // set the cursor at (0, 1)
  lcd.print(text); // print the text at the cursor
  return; // no return value
} // end displayL2()


/******************************* displayL2Empty FUNCTION ********************************/
/*    The displayL2Empty() function can set the cursor at (0, 1) and display 16 spaces
 * in row 2 of the LCD screen.
 *    There is no parameter and no return value.
 */
void displayL2Empty() {
  lcd.setCursor(0, 1); // set the cursor at (0, 1)
  lcd.write("                "); // print 16 spaces at the cursor
  return; // no return value
} // end displayL2Empty()


/******************************* displayEmpty FUNCTION ********************************/
/*    The displayEmpty() function can display a space at the indicated coordinate
 *    The displayEmpty() function takes in two parameter, the row and the column, 
 * set the cursor, and print a space at the cursor. Ther is no return value. 
 */
void displayEmpty(int col, int row) {
  lcd.setCursor(col, row); // set the cursor at the coordinate
  lcd.write(" "); // print a space at the cursor
  return; // no return value
} // end displayEmpty()


/******************************* displayFour FUNCTION ********************************/
/*    The displayFour() function can display "4"s at the battle entrance.
 *    The displayFour() function takes in 0 parameter with no return value. 
 */
void displayFour() {
  displayL1("444444"); // display "4"s at the beginning of line 1
  displayL2("4444444"); // display "4"s at the beginning of line 2
  lcd.setCursor(10,0); // set cursor at (10, 0)
  lcd.print("444444"); // display "4"s at the end of line 1
  lcd.setCursor(9, 1); // set cursor at (9, 1)
  lcd.print("4444444"); // display "4"s at the end of line 2
} // end of displayFour()


/******************************* displayThree FUNCTION ********************************/
/*    The displayThree() function can display "3"s at the battle entrance.
 *    The displayThree() function takes in 0 parameter with no return value. 
 */
void displayThree() {
  displayL1("333333"); // display "3"s at the beginning of line 1
  displayL2("3333333"); // display "3"s at the beginning of line 2
  lcd.setCursor(10,0); // set cursor at (10, 0)
  lcd.print("333333"); // display "3"s at the end of line 1
  lcd.setCursor(9, 1); // set cursor at (9, 1)
  lcd.print("3333333"); // display "3"s at the end of line 2
} // end of displayThree()


/******************************* displayTwo FUNCTION ********************************/
/*    The displayTwo() function can display "2"s at the battle entrance.
 *    The displayTwo() function takes in 0 parameter with no return value. 
 */
void displayTwo() {
  displayL1("222222"); // display "2"s at the beginning of line 1
  displayL2("2222222"); // display "2"s at the beginning of line 2
  lcd.setCursor(10,0); // set cursor at (10, 0)
  lcd.print("222222"); // display "2"s at the end of line 1
  lcd.setCursor(9, 1); // set cursor at (9, 1)
  lcd.print("2222222"); // display "2"s at the end of line 2
} // end of displayTwo()


/******************************* displayFullHeart FUNCTION ********************************/
/*    The displayFullHeart() function can display full screen of hearts at the battle entrance.
 *    The displayFullHeart() function takes in 0 parameter with no return value. 
 */
void displayFullHeart() {
  displayHeartL1(0, 1); // display a heart at line 1, col 0 and 1
  displayHeartL1(2, 3); // display a heart at line 1, col 2 and 3
  displayHeartL1(4, 5); // display a heart at line 1, col 4 and 5
  displayHeartL1(10, 11); // display a heart at line 1, col 10 and 11
  displayHeartL1(12, 13); // display a heart at line 1, col 12 and 13
  displayHeartL1(14, 15); // display a heart at line 1, col 14 and 15
  
  lcd.setCursor(0, 1); // set cursor at (0, 1)
  lcd.write(byte(6)); // display the right half of a heart

  displayHeartL2(1, 2); // display a heart at line 2, col 1 and 2
  displayHeartL2(3, 4); // display a heart at line 2, col 3 and 4
  displayHeartL2(5, 6); // display a heart at line 2, col 5 and 6
  displayHeartL2(9, 10); // display a heart at line 2, col 9 and 10
  displayHeartL2(11, 12); // display a heart at line 2, col 11 and 12
  displayHeartL2(13, 14); // display a heart at line 2, col 13 and 14
  
  lcd.setCursor(15, 1); // set cursor at (15, 1)
  lcd.write(byte(5)); // display the left half of a heart
} // end of displayFullHeart()


/******************************* displayHeartL1 FUNCTION ********************************/
/*    The displayHeartL1() function can display a single heart at line 1 of the lcd screen.
 *    The displayHeartL1() function takes in 2 parameters, two ints representing the  
 * coordinate of the heart with no return value. 
 */
void displayHeartL1(int position1, int position2) {
  lcd.setCursor(position1, 0); // set cursor at the position for the left half of the heart
  lcd.write(byte(5)); // display the left half of the heart
  lcd.setCursor(position2, 0); // set cursor at the position for the right half of the heart
  lcd.write(byte(6)); // display the right half of the heart
} // end of displayHeartL1()


/******************************* displayHeartL1Left FUNCTION ********************************/
/*    The displayHeartL1Left() function can display a single heart on the lcd screen (L1, P0).
 *    The displayHeartL1Left() function takes in 0 parameter with no return value. 
 */
void displayHeartL1Left() {
  lcd.setCursor(2, 0); // set cursor at (2, 0)
  lcd.write(" "); // display a space; rewrite heart displayed by displayHeartL1Right()
  displayHeartL1(0, 1); // display a heart at line 1, col 0 and 1
} // end of displayHearL1Left()


/******************************* displayHeartL1Right FUNCTION ********************************/
/*    The displayHeartL1Right() function can display a single heart on the lcd screen (L1, P1).
 *    The displayHeartL1Right() function takes in 0 parameter with no return value. 
 */
void displayHeartL1Right() {
  lcd.home(); // set cursor at (0, 0)
  lcd.write(" "); // display a space; rewrite heart displayed by sidplayHeartL1Left()
  displayHeartL1(1, 2); // display a heart at line 1, col 1 and 2
} // end of displayHeartL1Right()


/******************************* displayHeartL2 FUNCTION ********************************/
/*    The displayHeartL2() function can display a single heart at line 2 of the lcd screen.
 *    The displayHeartL2() function takes in 2 parameters, two ints representing the  
 * coordinate of the heart with no return value. 
 */
void displayHeartL2(int position1, int position2) {
  lcd.setCursor(position1, 1); // set cursor at the position for the left half of the heart
  lcd.write(byte(5)); // display the left half of the heart
  lcd.setCursor(position2, 1); // set cursor at the position for the right half of the heart
  lcd.write(byte(6)); // display the right half of the heart
} // end of displayHeartL2()


/******************************* displayHeartL2Fulla FUNCTION ********************************/
/*    The displayHeartL2Fulla() function can display eight complete hearts at line 2
 *    The displayHeartL2Fulla() function takes in 0 parameter with no return value. 
 */
void displayHeartL2Fulla() {
  displayHeartL2(0, 1); // display a heart at line 2, col 0 and 1
  displayHeartL2(2, 3); // display a heart at line 2, col 2 and 3
  displayHeartL2(4, 5); // display a heart at line 2, col 4 and 5
  displayHeartL2(6, 7); // display a heart at line 2, col 6 and 7
  displayHeartL2(8, 9); // display a heart at line 2, col 8 and 9
  displayHeartL2(10, 11); // display a heart at line 2, col 10 and 11
  displayHeartL2(12, 13); // display a heart at line 2, col 12 and 13
  displayHeartL2(14, 15); // display a heart at line 2, col 14 and 15
} // end of displayHeartL2Fulla()


/******************************* displayHeartL2Fullb FUNCTION ********************************/
/*    The displayHeartL2Fullb() function can display seven complete hearts with 2 edges at line 2
 *    The displayHeartL2Fullb() function takes in 0 parameter with no return value. 
 */
void displayHeartL2Fullb() {
  lcd.setCursor(0, 1); // set the cursor at line 2, col 1
  lcd.write(byte(6)); // display the right half of a heart
  displayHeartL2(1, 2); // display a heart at line 2, col 1 and 2
  displayHeartL2(3, 4); // display a heart at line 2, col 3 and 4
  displayHeartL2(5, 6); // display a heart at line 2, col 5 and 6
  displayHeartL2(7, 8); // display a heart at line 2, col 7 and 8
  displayHeartL2(9, 10); // display a heart at line 2, col 9 and 10
  displayHeartL2(11, 12); // display a heart at line 2, col 11 and 12
  displayHeartL2(13, 14); // display a heart at line 2, col 13 and 14
  lcd.setCursor(15, 1); // set the cursor at line 2, col 15
  lcd.write(byte(5)); // display the left half of a heart
} // end of displayHeartL2Fullb()


/******************************* displayHeartL2Space FUNCTION ********************************/
/*    The displayHeartL2Space() function can display five complete hearts with one space 
 * in between at line 2
 *    This is function is transparent
 *    The displayHeartL2Space() function takes in 0 parameter with no return value. 
 */
void displayHeartL2Space() {
  // use a for loop to move along the line
  for(int i = 1; i < 14; (i = i + 3)) {
    displayHeartL2(i, (i + 1)); // display a heart
  } // end of for
} // end of displayHeartL2Space()


/******************************* displayHeartL2a FUNCTION ********************************/
/*    The displayHeartL2a() function can display three complete hearts with three spaces 
 * in between at line 2
 *    This is function is transparent
 *    The displayHeartL2a() function takes in 0 parameter with no return value. 
 */
void displayHeartL2a() {
  // use a for loop to move along the line
  for(int i = 3; i < 14; (i = i + 5)) {
    displayHeartL2(i, (i + 1)); // display a heart
  } // end of for
} // end of displayHeartL2a()


/******************************* displaySpiderFace FUNCTION ********************************/
/*    The displaySpiderFace() function can display Muffet's face on LCD screen.
 *    The displaySpiderFace() function takes in 0 parameter with no return value. 
 */
void displaySpiderFace() {
  lcd.setCursor(0, 0); // set the cursor at (0, 0）
  lcd.write(byte(0)); // display the top left quarter of Muffet's face
  lcd.setCursor(1, 0); // set cursor at (1, 0)
  lcd.write((byte)1); // display the top right quarter of Muffet's face
  lcd.setCursor(0, 1); // set cursor at (0, 1)
  lcd.write((byte)2); // display the bottom left quarter of Muffet's face
  lcd.setCursor(1, 1); // set cursor at (1, 1)
  lcd.write((byte)3); // display the bottom right quarter of Muffet's face
} // end of displaySpiderFace()


/******************************* displaySpider FUNCTION ********************************/
/*    The displaySpider() function can display a spider on LCD screen.
 *    The displaySpider() function takes in 2 parameters, two ints representing the  
 * coordinate of the spider with no return value. 
 */
void displaySpider(int col, int row) {
  lcd.setCursor(col, row); // set cursor at the indicated coordinate
  lcd.write(byte(7)); // display a spider
} // end of displaySpider()


/******************************* displaySpiderL1a FUNCTION ********************************/
/*    The displaySpiderL1a() function can display two spiders at line 1 at their right position.
 *    The displaySpiderL1a() function takes in 0 parameter with no return value. 
 */
void displaySpiderL1a() {
  lcd.setCursor(6, 0); // set cursor at (6, 0)
  lcd.write(" "); // display a space; overwrite spider displayed by displaySpiderL1b()
  displaySpider(7, 0); // display a spider at (7, 0)
  lcd.setCursor(11, 0); // set cursor at (11, 0)
  lcd.write(" "); // display a space; overwrite spider displayed by displaySpiderL1b()
  displaySpider(12, 0); // display a spider at (12, 0)
} // end of displaySpiderL1a()


/******************************* displaySpiderL1b FUNCTION ********************************/
/*    The displaySpiderL1b() function can display two spiders at line 1 at their left position.
 *    The displaySpiderL1b() function takes in 0 parameter with no return value. 
 */
void displaySpiderL1b() {
  displaySpider(6, 0); // display a spider at (6, 0)
  lcd.setCursor(7, 0); // set cursor at (7, 0)
  lcd.write(" "); // display a space; overwrite spider displayed by displaySpiderL1a()
  displaySpider(11, 0); // display a spider at (11, 0)
  lcd.setCursor(12, 0); // set cursor at (12, 0)
  lcd.write(" "); // display a space; overwrite spider displayed by displaySpiderL1a()
} // end of displaySpiderL1b()


/******************************* displaySpiderL1c FUNCTION ********************************/
/*    The displaySpiderL1c() function can display four spiders at line 1 in between the notes.
 *    The displaySpiderL1c() function takes in 0 parameter with no return value. 
 */
void displaySpiderL1c() {
  displaySpider(6, 0); // display a spider at (6, 0)
  displaySpider(7, 0); // display a spider at (7, 0)
  displaySpider(11, 0); // display a spider at (11, 0)
  displaySpider(12, 0); // display a spider at (12, 0)
} // end of displaySpiderL1c()


/******************************* displaySpiderL1d FUNCTION ********************************/
/*    The displaySpiderL1d() function can empty four spiders at line 1 in between the notes.
 *    The displaySpiderL1d() function takes in 0 parameter with no return value. 
 */
void displaySpiderL1d() {
  displayEmpty(6, 0); // display a space at (6, 0)
  displayEmpty(7, 0); // display a space at (7, 0)
  displayEmpty(11, 0); // display a space at (11, 0)
  displayEmpty(12, 0); // display a space at (12, 0)
} // end of displaySpiderL1d


/******************************* displaySpiderL2a FUNCTION ********************************/
/*    The displaySpiderL2a() function can fill up line 2 with spiders.
 *    The displaySpiderL2a() function takes in 0 parameter with no return value. 
 */
void displaySpiderL2a() {
  // use a for loop to move the coordinate
  for(int i = 0; i < 16; i++) {
    displaySpider(i, 1); // display a spider at line 2
  } // end of for
} // end of displaySpiderL2a()


/******************************* displaySpiderL2b FUNCTION ********************************/
/*    The displaySpiderL2b() function can display spiders at odd indexes of line 2.
 *    The displaySpiderL2b() function takes in 0 parameter with no return value. 
 */
void displaySpiderL2b() {
  // use a for loop to move the coordinate
  for(int i = 1; i < 16; (i = i + 2)) {
    displayEmpty((i - 1), 1); // empty any display at the even index
    displaySpider(i, 1); // display a spider at the odd index
  } // end of for
} // end of displaySpiderL2b()


/******************************* displaySpiderL2c FUNCTION ********************************/
/*    The displaySpiderL2c() function can display spiders at even indexes of line 2.
 *    The displaySpiderL2c() function takes in 0 parameter with no return value. 
 */
void displaySpiderL2c() {
  // use a for loop to move the coordinate
  for(int i = 0; i < 16; (i = i + 2)) {
    displaySpider(i, 1); // display a spider at the even index
    displayEmpty((i + 1), 1); // empty any display at the odd index
  } // end of for
} // end of displaySpiderL2c()


/******************************* displaySpiderL2d FUNCTION ********************************/
/*    The displaySpiderL2d() function can display spiders at odd indexes of line 2.
 *    The displaySpiderL2d() function takes in 0 parameter with no return value. 
 *    This function is transparent.
 */
void displaySpiderL2d() {
  // use a for loop to move the coordinate
  for(int i = 1; i < 16; (i = i + 2)) {
    displaySpider(i, 1); // display a spider at the odd index
  } // end of for
} // end of displaySpiderL2d()


/******************************* displaySpiderL2e FUNCTION ********************************/
/*    The displaySpiderL2e() function can display spiders at even indexes of line 2.
 *    The displaySpiderL2e() function takes in 0 parameter with no return value. 
 *    This function is transparent.
 */
void displaySpiderL2e() {
  // use a for loop to move the coordinate
  for(int i = 0; i < 16; (i = i + 2)) {
    displaySpider(i, 1); // display a spider at the even index
  } // end of for
} // end of displaySpiderL2e()


/******************************* displaySpiderL2f FUNCTION ********************************/
/*    The displaySpiderL2f() function can display three spiders with four spaces in between 
 * at line 2.
 *    The displaySpiderL2f() function takes in 0 parameter with no return value. 
 *    This function is transparent.
 */
void displaySpiderL2f() {
  // use a for loop to move the coordinate
  for(int i = 1; i < 12; (i = i + 5)) {
    displaySpider(i, 1); // display a spider
  } // end of for
} // end of displaySpiderL2f()


/******************************* playPhoneRing FUNCTION ********************************/
/*    The playPhoneRing() function can display the phone ring sound effects on BuzzSE.
 *    The playPhoneRing() function takes in 0 parameter with no return value. 
 */
void playPhoneRing() {
  // use a for loop to repeat a note for 4 times
  for(int i = 0; i < 4; i++) {
    tone(BuzzSE, NOTE_D4, 70); // play note D4 for 70ms
    delay(90); // 90ms of silence
  } // end of for
  tone(BuzzSE, NOTE_D4, 100); // play note D4 for 100ms
} // end of playPhoneRing()


/******************************* playMuffetLaugh FUNCTION ********************************/
/*    The playMuffetLaugh() function can display Muffet's laugh sound effects on BuzzSE.
 *    The playMuffetLaugh() function takes in 0 parameter with no return value. 
 */
void playMuffetLaugh() {
  // create an array to hold the melody for Muffet's laugh
  int melodySE1[] = {NOTE_C4, NOTE_C5, NOTE_F4, NOTE_C4, NOTE_C4};
  // use a for loop to play through the melody
  for(int i = 0; i < 5; i++) {
    tone(BuzzSE, melodySE1[i], 70); // display a note for 70ms
    delay(90); // silence for 90ms
  } // end of for
} // end of playMuffetLaugh()


/******************************* playClick FUNCTION ********************************/
/*    The playClick() function can display click sound effects on BuzzSE.
 *    The playClick() function takes in 0 parameter with no return value. 
 */
void playClick() {
    tone(BuzzSE, NOTE_B6, 70); // display note B6 for 70ms
} // end of playClick()


/******************************* playEnterBattle FUNCTION ********************************/
/*    The playEnterBattle() function can display battle entrance sound effects on BuzzSE.
 *    The playEnterBattle() function takes in 0 parameter with no return value. 
 */
void playEnterBattle() {
  tone(BuzzSE, NOTE_A3, 100); // display note A3 for 100ms
  delay(120); // silence for 120ms
  tone(BuzzSE, NOTE_A3, 100); // display note A3 for 100ms
  delay(120); // silence for 120ms
  // create an array to hold the melody for battle entrance
  int melodySE2[] = {NOTE_AS6, NOTE_GS6, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_C5, NOTE_AS4};
  // use a for loop to play through the melody array
  for(int i = 0; i < 8; i++) {
    tone(BuzzSE, melodySE2[i], 50); // display a note for 50ms
    delay(60); // silence for 60ms
  } // end of for
} // end of playEnterBattle()


void displayBeginning() {
  displayL1("* Ring...ring...");  
  playPhoneRing();
  
  delay(2000);
  lcd.clear();

  displaySpiderFace();
  lcd.setCursor(3, 0);
  lcd.print("Ahuhuhu...");
  lcd.setCursor(3, 1);
  lcd.print("Human.");
  playMuffetLaugh();

  delay(2000);
  lcd.clear();
  
  displayL1("* Muffet traps ");
  displayL2("  you!");
  playClick();

  delay(2000);
  lcd.clear();
  
  displayL1("* You struggle");
  displayL2("  to escape the~");
  playClick();
  delay(1500);
  lcd.clear();
  displayL1("* You struggle");
  displayL2("  web.");

  delay(1000);
  lcd.clear();

  displaySpiderFace();
  lcd.setCursor(3, 0);
  lcd.print("Don't look so");
  lcd.setCursor(3, 1);
  lcd.print("blue, deary");
  lcd.setCursor(15,1);
  lcd.write(byte(4));
  playMuffetLaugh();

  delay(2000);
  lcd.clear();
  displaySpiderFace();
  lcd.setCursor(3, 0);
  lcd.print("Let the DANCE");
  lcd.setCursor(3, 1);
  lcd.print("party begin!");
  playMuffetLaugh();

  delay(2000);
  lcd.clear();

  displayHeartL1(7, 8);
  playEnterBattle();
  delay(500);

  displayFour();
  playClick();
  delay(300);
  displayThree();
  playClick();
  delay(300);
  displayTwo();
  playClick();
  delay(300);
  displayFullHeart();
  playClick();
  delay(300);
}

void displayM1_M16() {
  // M1
  displaySpiderL1a();
  displayL2("* All the spide~");
    play(melody_V1_1a, noteLength1, 2, melody0, noteLength0, 1, melody_B_1a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_1b, noteLength1, 2, melody0, noteLength0, 1, melody_B_1b, noteLength2, 4);
  // M2
  displaySpiderL1a();
    play(melody_V1_2a, noteLength3, 3, melody0, noteLength0, 1, melody_B_2a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_2b, noteLength2, 4, melody0, noteLength0, 1, melody_B_2b, noteLength2, 4);

    
  // M3
  displaySpiderL1a();
  displayL2("* rs clap along~");  
    play(melody_V1_3a, noteLength2, 4, melody0, noteLength0, 1, melody_B_3a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_3b, noteLength2, 4, melody0, noteLength0, 1, melody_B_3b, noteLength2, 4);
  // M4
  displaySpiderL1a();
    play(melody_V1_4a, noteLength2, 4, melody0, noteLength0, 1, melody_B_4a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_4b, noteLength4, 5, melody0, noteLength0, 1, melody_B_4b, noteLength2, 4);

 
  // M5
  displaySpiderL1a();
  displayL2("* to the music. ");
    play(melody_V1_5a, noteLength1, 2, melody0, noteLength0, 1, melody_B_5a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_5b, noteLength1, 2, melody0, noteLength0, 1, melody_B_5b, noteLength2, 4);
  // M6
  displaySpiderL1a();
    play(melody_V1_6a, noteLength1, 2, melody0, noteLength0, 1, melody_B_6a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_6b, noteLength1, 2, melody0, noteLength0, 1, melody_B_6b, noteLength2, 4);
 

  // M7
  displaySpiderL1c(); // display all spider L1
  displaySpiderL2a(); // display all spider L2
    play(melody_V1_7a, noteLength1, 2, melody0, noteLength0, 1, melody_B_7a, noteLength1, 2);
  displaySpiderL1d();
  displayL2Empty();
    play(melody_V1_7b, noteLength1, 2, melody0, noteLength0, 1, melody_B_7b, noteLength1, 2);
  // M8
  displaySpiderL1c();
  displaySpiderL2a();
    play(melody_V1_8a, noteLength1, 2, melody0, noteLength0, 1, melody_B_8a, noteLength5, 3);
  displaySpiderL1d();
  displayL2Empty();
    play(melody_V1_8b, noteLength1, 2, melody0, noteLength0, 1, melody_B_8b, noteLength2, 4);


  // M9
  displaySpiderL1a();
  displayL2("* Muffet does a~");
    play(melody_V1_9a, noteLength2, 4, melody_V2_9a, noteLength1, 2, melody_B_9a, noteLength1, 2);
  displaySpiderL1b();
    play(melody_V1_9b, noteLength2, 4, melody_V2_9b, noteLength1, 2, melody_B_9b, noteLength1, 2);
  // M10
  displaySpiderL1a();
  displayL2("* synchronized ~");
    play(melody_V1_10a, noteLength2, 4, melody_V2_10a, noteLength3, 3, melody_B_10a, noteLength3, 3);
  displaySpiderL1b();
    play(melody_V1_10b, noteLength2, 4, melody_V2_10b, noteLength2, 4, melody_B_10b, noteLength2, 4);


  // M11
  displaySpiderL1a();
  displayL2("* dance with th~");
    play(melody_V1_11a, noteLength2, 4, melody_V2_11a, noteLength2, 4, melody_B_11a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_11b, noteLength2, 4, melody_V2_11b, noteLength2, 4, melody_B_11b, noteLength2, 4);
  // M12
  displaySpiderL1a();
  displayL2("* e other spider");
    play(melody_V1_12a, noteLength2, 4, melody_V2_12a, noteLength2, 4, melody_B_12a, noteLength2, 4);
  displaySpiderL1b();
    play(melody_V1_12b, noteLength2, 4, melody_V2_12b, noteLength4, 5, melody_B_12b, noteLength4, 5);
    

  // M13
  displaySpiderL1a();
  displaySpiderL2c();
    play(melody_V1_13a, noteLength2, 4, melody_V2_13a, noteLength1, 2, melody_B_13a, noteLength1, 2);
  displaySpiderL1b();
  displaySpiderL2b();
    play(melody_V1_13b, noteLength2, 4, melody_V2_13b, noteLength1, 2, melody_B_13b, noteLength1, 2);
  // M14
  displaySpiderL1a();
  displaySpiderL2c();
    play(melody_V1_14a, noteLength2, 4, melody_V2_14a, noteLength1, 2, melody_B_14a, noteLength1, 2);
  displaySpiderL1b();
  displaySpiderL2b();
    play(melody_V1_14b, noteLength2, 4, melody_V2_14b, noteLength1, 2, melody_B_14b, noteLength1, 2);
 

  // M15
  displaySpiderL1c();
  displayL2("*    YOU CAN    ");
    play(melody_V1_15a, noteLength1, 2, melody0, noteLength0, 1, melody_B_15a, noteLength1, 2);
  displaySpiderL1d();
  displayL2("*    NOT RE-    ");
    play(melody_V1_15b, noteLength1, 2, melody0, noteLength0, 1, melody_B_15b, noteLength1, 2);
  // M16
  displaySpiderL1c();
  displayL2("*   SIST DAN-     ");
    play(melody_V1_16a, noteLength1, 2, melody0, noteLength0, 1, melody_B_16a, noteLength0, 1);
  displaySpiderL1d();
  lcd.setCursor(0, 1);
  lcd.write("*");
  displayHeartL2(1, 2);
  displayHeartL2(3, 4);
  lcd.write("DANCING ");
  displayHeartL2(12, 13);
  displayHeartL2(14, 15);
    play(melody_V1_16b, noteLength1, 2, melody0, noteLength0, 1, melody_B_16b, noteLength2, 4);

}

void displayM1R_M18() {
   // M1R
  displayHeartL1Right();
  displayHeartL2Fulla();
  displaySpiderL1a(); 
   play(melody_V1_1a, noteLength1, 2, melody0, noteLength0, 1, melody_B_1a, noteLength2, 4);
  displayHeartL1Left();
  displayHeartL2Fullb();
  displaySpiderL1b();
    play(melody_V1_1b, noteLength1, 2, melody0, noteLength0, 1, melody_B_1b, noteLength2, 4);
    
  // M2R
  displayHeartL1Right();
  displayHeartL2Fulla();
  displaySpiderL1a(); 
    play(melody_V1_2a, noteLength3, 3, melody0, noteLength0, 1, melody_B_2a, noteLength2, 4);
  displayHeartL1Left();
  displayHeartL2Fullb();
  displaySpiderL1b();
    play(melody_V1_2b, noteLength2, 4, melody0, noteLength0, 1, melody_B_2b, noteLength2, 4);

  // M3R
  displayHeartL1Right();
  displayHeartL2Fulla();
  displaySpiderL1a(); 
    play(melody_V1_3a, noteLength2, 4, melody0, noteLength0, 1, melody_B_3a, noteLength2, 4);
  displayHeartL1Left();
  displayHeartL2Fullb();
  displaySpiderL1b();
    play(melody_V1_3b, noteLength2, 4, melody0, noteLength0, 1, melody_B_3b, noteLength2, 4);
  // M4R
  displayHeartL1Right();
  displayHeartL2Fulla();
  displaySpiderL1a(); 
    play(melody_V1_4a, noteLength2, 4, melody0, noteLength0, 1, melody_B_4a, noteLength2, 4);
  displayHeartL1Left();
  displayHeartL2Fullb();
  displaySpiderL1b();
    play(melody_V1_4b, noteLength4, 5, melody0, noteLength0, 1, melody_B_4b, noteLength2, 4);

  // M5R
  displaySpiderL1a();
  displayHeartL1Right();
  displayHeartL2Fulla();
  displaySpiderL2e();
    play(melody_V1_5a, noteLength1, 2, melody0, noteLength0, 1, melody_B_5a, noteLength2, 4);
  displayHeartL1Left();
  displaySpiderL1b();
  displayHeartL2Fullb();
  displaySpiderL2d();
    play(melody_V1_5b, noteLength1, 2, melody0, noteLength0, 1, melody_B_5b, noteLength2, 4);
  // M6R
  displaySpiderL1a();
  displayHeartL1Right();
  displayHeartL2Fulla();
  displaySpiderL2e();
    play(melody_V1_6a, noteLength1, 2, melody0, noteLength0, 1, melody_B_6a, noteLength2, 4);
  displayHeartL1Left();
  displaySpiderL1b();
  displayHeartL2Fullb();
  displaySpiderL2d();
    play(melody_V1_6b, noteLength1, 2, melody0, noteLength0, 1, melody_B_6b, noteLength2, 4);


  // M7R
  displaySpiderL1c(); 
  displayHeartL1Right();
  displaySpiderL2a();
  displayHeartL2Space(); 
    play(melody_V1_7a, noteLength1, 2, melody0, noteLength0, 1, melody_B_7a, noteLength1, 2);
  displayHeartL1Left();
  displaySpiderL1d();
  displayHeartL2Fulla();
    play(melody_V1_7b, noteLength1, 2, melody0, noteLength0, 1, melody_B_7b, noteLength1, 2);
  // M8R
  displaySpiderL1c();
  displayHeartL1Right(); 
  displaySpiderL2a();
  displayHeartL2Space(); 
    play(melody_V1_8a, noteLength1, 2, melody0, noteLength0, 1, melody_B_8a, noteLength5, 3);
  displayHeartL1Left();
  displaySpiderL1d();
  displayHeartL2Fulla();
    play(melody_V1_8b, noteLength1, 2, melody0, noteLength0, 1, melody_B_8b, noteLength2, 4);

  // M9R
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* Your DANCE is~");
    play(melody_V1_9a, noteLength2, 4, melody_V2_9a, noteLength1, 2, melody_B_9a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
    play(melody_V1_9b, noteLength2, 4, melody_V2_9b, noteLength1, 2, melody_B_9b, noteLength1, 2);
  // M10R
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* making every ~");
    play(melody_V1_10a, noteLength2, 4, melody_V2_10a, noteLength3, 3, melody_B_10a, noteLength3, 3);
  displaySpiderL1b();
  displayHeartL1Left();
    play(melody_V1_10b, noteLength2, 4, melody_V2_10b, noteLength2, 4, melody_B_10b, noteLength2, 4);


  // M11R
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* spider very  ~");
    play(melody_V1_11a, noteLength2, 4, melody_V2_11a, noteLength2, 4, melody_B_11a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
    play(melody_V1_11b, noteLength2, 4, melody_V2_11b, noteLength2, 4, melody_B_11b, noteLength2, 4);
  // M12R
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* happy ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("      ");
    play(melody_V1_12a, noteLength2, 4, melody_V2_12a, noteLength2, 4, melody_B_12a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
    play(melody_V1_12b, noteLength2, 4, melody_V2_12b, noteLength4, 5, melody_B_12b, noteLength4, 5);


  // M13R
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpiderL2c();
    play(melody_V1_13a, noteLength2, 4, melody_V2_13a, noteLength1, 2, melody_B_13a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpiderL2b();
    play(melody_V1_13b, noteLength2, 4, melody_V2_13b, noteLength1, 2, melody_B_13b, noteLength1, 2);
  // M14R
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpiderL2c();
    play(melody_V1_14a, noteLength2, 4, melody_V2_14a, noteLength1, 2, melody_B_14a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpiderL2b();
    play(melody_V1_14b, noteLength2, 4, melody_V2_14b, noteLength1, 2, melody_B_14b, noteLength1, 2);

  // M17
  
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write("     Oh! I     ");
    play(melody_V1_17a, noteLength1, 2, melody0, noteLength0, 1, melody_B_17a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write("   THINK IT'S   ");
    play(melody_V1_17b, noteLength1, 2, melody0, noteLength0, 1, melody_B_17b, noteLength1, 2);

  // M18
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write("   BREAKFAST     ");
    play(melody_V1_18a, noteLength1, 2, melody0, noteLength0, 1, melody_B_18a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" !!!! TIME !!!!");
    play(melody_V1_18b, noteLength2, 4, melody0, noteLength0, 1, melody_B_18b, noteLength2, 4);
}

void displayM19_M34() {
   //M19
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* You are      ~");
    play(melody_V1_19a, noteLength1, 2, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* dancing on a ~");
    play(melody_V1_19b, noteLength1, 2, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);

  //M20
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* strange purpl~");
    play(melody_V1_20a, noteLength5, 3, melody_V2_20a, noteLength2, 4, melody_B_20a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* e web.          ");
    play(melody_V1_20b, noteLength1, 2, melody_V2_20b, noteLength2, 4, melody_B_20b, noteLength2, 4);

  //M21
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("================");
  displayHeartL2Space();
    play(melody_V1_21a, noteLength5, 3, melody_V2_21a, noteLength2, 4, melody_B_21a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
    play(melody_V1_21b, noteLength1, 2, melody_V2_21b, noteLength2, 4, melody_B_21b, noteLength2, 4);

  //M22
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("================");
  displayHeartL2Space();
    play(melody_V1_22a, noteLength5, 3, melody_V2_22a, noteLength2, 4, melody_B_22a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
    play(melody_V1_22b, noteLength2, 4, melody_V2_22b, noteLength2, 4, melody_B_22b, noteLength2, 4);


  // M23 (19)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* Muffet pours ~");
    play(melody_V1_19a, noteLength1, 2, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* you a        ~");
    play(melody_V1_19b, noteLength1, 2, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);

  // M24 (20)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* cup of       ~");
    play(melody_V1_24a, noteLength5, 3, melody_V2_20a, noteLength2, 4, melody_B_20a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* spiders ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("    ");
    play(melody_V1_24b, noteLength1, 2, melody_V2_20b, noteLength2, 4, melody_B_20b, noteLength2, 4);


  // M25 (24, 21)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("================");
  displayHeartL2a();
    play(melody_V1_24a, noteLength5, 3, melody_V2_21a, noteLength2, 4, melody_B_21a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
  displaySpiderL2f();
    play(melody_V1_24b, noteLength1, 2, melody_V2_21b, noteLength2, 4, melody_B_21b, noteLength2, 4);


  // M26 (22)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("================");
  displayHeartL2a();
    play(melody_V1_26a, noteLength5, 3, melody_V2_22a, noteLength2, 4, melody_B_22a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
  displaySpiderL2f();
    play(melody_V1_26b, noteLength2, 4, melody_V2_22b, noteLength2, 4, melody_B_22b, noteLength2, 4);

  // M27 (19)
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpider(0, 1); 
  lcd.write(" Oh! How rude!~");
    play(melody_V1_19a, noteLength1, 2, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1); 
  lcd.write(" I forgot to  ~");
    play(melody_V1_19b, noteLength1, 2, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);


  // M28 (20)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1); 
  lcd.write(" introduce you~");
    play(melody_V1_20a, noteLength5, 3, melody_V2_20a, noteLength2, 4, melody_B_20a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" to my pet ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("  ");
    play(melody_V1_20b, noteLength1, 2, melody_V2_20b, noteLength2, 4, melody_B_20b, noteLength2, 4);

  // M29 (21)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("========<<@");
  displaySpider(11, 1);
  lcd.write("@>>=");
    play(melody_V1_21a, noteLength5, 3, melody_V2_21a, noteLength2, 4, melody_B_21a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("====<<@");
  displaySpider(7, 1);
  lcd.write("@>>=====");
    play(melody_V1_21b, noteLength1, 2, melody_V2_21b, noteLength2, 4, melody_B_21b, noteLength2, 4);

  //M30 (22)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("=<<@");
  displaySpider(4, 1);
  lcd.write("@>>========");
    play(melody_V1_22a, noteLength5, 3, melody_V2_22a, noteLength2, 4, melody_B_22a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
    play(melody_V1_22b, noteLength2, 4, melody_V2_22b, noteLength2, 4, melody_B_22b, noteLength2, 4);

  //M31 (19)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("================");
  displaySpiderL2f();
  displayHeartL2a();
    play(melody_V1_31a, noteLength1, 2, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("====<<@");
  displaySpider(7, 1);
  lcd.write("@>>=====");
    play(melody_V1_31b, noteLength1, 2, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);

  // M32
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("================");
  displaySpiderL2f();
  displayHeartL2a(); 
    play(melody_V1_32a, noteLength6, 3, melody_V2_32a, noteLength2, 4, melody_B_32a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("====<<@");
  displaySpider(7, 1);
  lcd.write("@>>=====");
    play(melody_V1_32b, noteLength5, 3, melody_V2_32a, noteLength2, 4, melody_B_32a, noteLength2, 4);

  // M33
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write("     Oh! I     ");
    play(melody_V1_33a, noteLength3, 3, melody_V2_33a, noteLength2, 4, melody_B_33a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write("   THINK IT'S   ");
    play(melody_V1_33b, noteLength1, 2, melody_V2_33b, noteLength2, 4, melody_B_33b, noteLength2, 4);

  // M34
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write("    LUNCH    ");
    play(melody_V1_34a, noteLength2, 4, melody_V2_34a, noteLength2, 4, melody_B_34a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" !!!! TIME !!!!");
    play(melody_V1_34b, noteLength0, 1, melody_V2_34b, noteLength0, 1, melody_B_34b, noteLength0, 1);
}

void displayM35_M50() {
  // M35 (19)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" Welcome to   ~");
    play(melody_V1_35a, noteLength1, 2, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" our bake sale~");
    play(melody_V1_35b, noteLength5, 3, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);

  // M36 (20)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" where all    ~");
      play(melody_V1_36a, noteLength1, 2, melody_V2_20a, noteLength2, 4, melody_B_20a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" ciders are   ~");
      play(melody_V1_36b, noteLength1, 2, melody_V2_20b, noteLength2, 4, melody_B_20b, noteLength2, 4);


  //M37 (21)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" made with    ~");
    play(melody_V1_37a, noteLength2, 4, melody_V2_21a, noteLength2, 4, melody_B_21a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" whole spiders~");
    play(melody_V1_37b, noteLength3, 3, melody_V2_21b, noteLength2, 4, melody_B_21b, noteLength2, 4);


  //M38 (22)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" not just     ~");
    play(melody_V1_38a, noteLength2, 4, melody_V2_22a, noteLength2, 4, melody_B_22a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" the juice!    ");
    play(melody_V1_38b, noteLength2, 4, melody_V2_22b, noteLength2, 4, melody_B_22b, noteLength2, 4);


  // M39 (19)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("==<<@");
  displaySpider(5, 1);
  lcd.write("@>>=");
  displayHeartL2(10, 11);
  lcd.write("====");
    play(melody_V1_39a, noteLength2, 4, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("====");
  displayHeartL2(4, 5);
  lcd.write("=<<@");
  displaySpider(10, 1);
  lcd.write("@>>==");
    play(melody_V1_39b, noteLength2, 4, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);


  // M40 (20)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("==<<@");
  displaySpider(5, 1);
  lcd.write("@>>=");
  displayHeartL2(10, 11);
  lcd.write("====");
      play(melody_V1_40a, noteLength2, 4, melody_V2_20a, noteLength2, 4, melody_B_20a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("====");
  displayHeartL2(4, 5);
  lcd.write("=<<@");
  displaySpider(10, 1);
  lcd.write("@>>==");
      play(melody_V1_40b, noteLength2, 4, melody_V2_20b, noteLength2, 4, melody_B_20b, noteLength2, 4);


  //M41 (21)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" Oh! And I    ~");
    play(melody_V1_41a, noteLength2, 4, melody_V2_21a, noteLength2, 4, melody_B_21a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" forgot to    ~");
    play(melody_V1_41b, noteLength2, 4, melody_V2_21b, noteLength2, 4, melody_B_21b, noteLength2, 4);


  //M42 (22)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" feed my      ~");
    play(melody_V1_42a, noteLength2, 4, melody_V2_22a, noteLength2, 4, melody_B_22a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" pet ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("        ");
    play(melody_V1_42b, noteLength0, 1, melody_V2_22b, noteLength2, 4, melody_B_22b, noteLength2, 4);

   //M43 (19)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* You decided  ~");
    play(melody_V1_19a, noteLength1, 2, melody_V2_19a, noteLength2, 4, melody_B_19a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* to spend     ~");
    play(melody_V1_19b, noteLength1, 2, melody_V2_19b, noteLength2, 4, melody_B_19b, noteLength2, 4);

  //M44 (20)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* 99           ~");
    play(melody_V1_20a, noteLength5, 3, melody_V2_20a, noteLength2, 4, melody_B_20a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* 9999         ~");
    play(melody_V1_20b, noteLength1, 2, melody_V2_20b, noteLength2, 4, melody_B_20b, noteLength2, 4);

  //M45 (21)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* 9999G        ~");
    play(melody_V1_21a, noteLength5, 3, melody_V2_21a, noteLength2, 4, melody_B_21a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* 9999G on a   ~");
    play(melody_V1_21b, noteLength1, 2, melody_V2_21b, noteLength2, 4, melody_B_21b, noteLength2, 4);

  //M46 (22)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* spider       ~");
    play(melody_V1_22a, noteLength5, 3, melody_V2_22a, noteLength2, 4, melody_B_22a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();  
  displayL2("* spider cider. ");
    play(melody_V1_22b, noteLength2, 4, melody_V2_22b, noteLength2, 4, melody_B_22b, noteLength2, 4);


  //M47 
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("================");
  displayHeartL2Space();
    play(melody_V1_47a, noteLength1, 2, melody_V2_47a, noteLength2, 4, melody_B_47a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("                ");
  displayHeartL2Space();

    play(melody_V1_47b, noteLength1, 2, melody_V2_47b, noteLength2, 4, melody_B_47b, noteLength2, 4);

  // M48 (32)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* Muffet       ~");
    play(melody_V1_32a, noteLength6, 3, melody_V2_32a, noteLength2, 4, melody_B_32a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* decides      ~");
    play(melody_V1_32b, noteLength5, 3, melody_V2_32a, noteLength2, 4, melody_B_32a, noteLength2, 4);

  // M49 (33)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* not to       ~");
    play(melody_V1_33a, noteLength3, 3, melody_V2_33a, noteLength2, 4, melody_B_33a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* tie you up   ~");
    play(melody_V1_33b, noteLength1, 2, melody_V2_33b, noteLength2, 4, melody_B_33b, noteLength2, 4);


  // M50 (34)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displayL2("* with her     ~");
    play(melody_V1_34a, noteLength2, 4, melody_V2_34a, noteLength2, 4, melody_B_34a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* pet ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("  ");
    play(melody_V1_34b, noteLength0, 1, melody_V2_34b, noteLength0, 1, melody_B_34b, noteLength0, 1);
}

void displayM51_M66() {

  // M51
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpiderL2b();
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpiderL2c();
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);

  // M52 (51)
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpiderL2b();
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpiderL2c();
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);

  // M53
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpiderL2a();
  displayHeartL2Space();
    play(melody_V1_53a, noteLength7, 4, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayHeartL2Fulla();
    play(melody_V1_53b, noteLength5, 3, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);

  // M54 (53)
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpiderL2a();
  displayHeartL2Space();
    play(melody_V1_54a, noteLength0, 1, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayHeartL2Fulla();
    play(melody_V1_54b, noteLength0, 1, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);

  // M55 (51)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* All the      ~");
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* spiders      ~");
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);

  // M56 (51)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* want you     ~");
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* to know      ~");
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);

  // M57 (53)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* how          ~");
    play(melody_V1_57a, noteLength1, 2, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* thankful     ~");
    play(melody_V1_57b, noteLength2, 4, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);  

  // M58 (54)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* they are!     ");
    play(melody_V1_54a, noteLength0, 1, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
    play(melody_V1_54b, noteLength0, 1, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);


  // M59 (51)
  displaySpiderL1a();
  displayHeartL1Right();
    displayL2("=====");
  lcd.write("=<<@");
  displaySpider(9, 1);
  lcd.write("@>>===");
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
    displayL2("===");
  lcd.write("<<@");
  displaySpider(6, 1);
  lcd.write("@>>=======");
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);
    
  // M60 (51)
  displaySpiderL1a();
  displayHeartL1Right();
    displayL2("=====");
  lcd.write("=<<@");
  displaySpider(9, 1);
  lcd.write("@>>===");
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
    displayL2("===");
  lcd.write("<<@");
  displaySpider(6, 1);
  lcd.write("@>>=======");
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);

  // M61 (53)
   displaySpiderL1a();
  displayHeartL1Right();
  displaySpider(0, 1);
  lcd.write(" We are       ~");
    play(melody_V1_53a, noteLength7, 4, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" in your      ~");
    play(melody_V1_53b, noteLength5, 3, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);

  // M62 (54)
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpider(0, 1);
  lcd.write(" debt,        ~");
    play(melody_V1_54a, noteLength0, 1, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" deary ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("      ");
    play(melody_V1_54b, noteLength0, 1, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);


  // M63 (55)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("================");
  displayHeartL2Space();
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
  displayHeartL2a();
  displaySpiderL2f();
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);


  // M64 (56)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("================");
  displayHeartL2Space();
    play(melody_V1_51a, noteLength5, 3, melody_V2_51a, noteLength2, 4, melody_B_51a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" Would you    ~");
    play(melody_V1_51b, noteLength2, 4, melody_V2_51b, noteLength2, 4, melody_B_51b, noteLength2, 4);

  // M65 (57)
   displaySpiderL1a();
  displayHeartL1Right();
  displaySpider(0, 1);
  lcd.write(" also like    ~");
    play(melody_V1_57a, noteLength1, 2, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" something for~");
    play(melody_V1_57b, noteLength2, 4, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);  

  // M66 (58)
  displaySpiderL1a();
  displayHeartL1Right();
  displaySpider(0, 1);
  lcd.write("     DINNER    ");
    play(melody_V1_54a, noteLength0, 1, melody_V2_53a, noteLength2, 4, melody_B_53a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" ??? DINNER ???");
    play(melody_V1_54b, noteLength0, 1, melody_V2_53b, noteLength2, 4, melody_B_53b, noteLength2, 4);

}

void displayM67_M78() {
  // M67
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("================");
  displaySpiderL2f();
    play(melody_V1_67a, noteLength5, 3, melody_V2_67a, noteLength2, 4, melody_B_67a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("================");
  displaySpiderL2e();
    play(melody_V1_67b, noteLength2, 4, melody_V2_67b, noteLength2, 4, melody_B_67b, noteLength2, 4);

  // M68 (67)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* Muffet said  ~");
    play(melody_V1_67a, noteLength5, 3, melody_V2_67a, noteLength2, 4, melody_B_67a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* all of the   ~");
    play(melody_V1_67b, noteLength2, 4, melody_V2_67b, noteLength2, 4, melody_B_67b, noteLength2, 4);

  // M69
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* spider donuts~");
    play(melody_V1_69a, noteLength7, 4, melody_V2_69a, noteLength2, 4, melody_B_69a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* are made with~");
    play(melody_V1_69b, noteLength5, 3, melody_V2_69b, noteLength2, 4, melody_B_69b, noteLength2, 4);

  // M70 (69)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* ciders       ~");
    play(melody_V1_70a, noteLength0, 1, melody_V2_69a, noteLength2, 4, melody_B_69a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* in the batter.");
    play(melody_V1_70b, noteLength0, 1, melody_V2_69b, noteLength2, 4, melody_B_69b, noteLength2, 4);

  // M71 (67)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* You paid     ~");
    play(melody_V1_67a, noteLength5, 3, melody_V2_67a, noteLength2, 4, melody_B_67a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* another      ~");
    play(melody_V1_67b, noteLength2, 4, melody_V2_67b, noteLength2, 4, melody_B_67b, noteLength2, 4);

  // M72 (67)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* 99           ~");
    play(melody_V1_67a, noteLength5, 3, melody_V2_67a, noteLength2, 4, melody_B_67a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* 9999         ~");
    play(melody_V1_67b, noteLength2, 4, melody_V2_67b, noteLength2, 4, melody_B_67b, noteLength2, 4);


  // M73 (69)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* 9999G on a   ~");
    play(melody_V1_73a, noteLength1, 2, melody_V2_69a, noteLength2, 4, melody_B_69a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* freshly backe~");
    play(melody_V1_73b, noteLength2, 4, melody_V2_69b, noteLength2, 4, melody_B_69b, noteLength2, 4);

  // M74 (70)
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("* d spider     ~");
    play(melody_V1_70a, noteLength0, 1, melody_V2_69a, noteLength2, 4, melody_B_69a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* donut!        ");
    play(melody_V1_70b, noteLength0, 1, melody_V2_69b, noteLength2, 4, melody_B_69b, noteLength2, 4);

  // M75 (67)
  displaySpiderL1a();
  displayHeartL1Right();
  displayHeartL2Fulla();
    play(melody_V1_67a, noteLength5, 3, melody_V2_67a, noteLength2, 4, melody_B_67a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayHeartL2Fullb();
    play(melody_V1_67b, noteLength2, 4, melody_V2_67b, noteLength2, 4, melody_B_67b, noteLength2, 4);

  // M76 (67)
  displaySpiderL1a();
  displayHeartL1Right();
  displayHeartL2Fulla();
    play(melody_V1_67a, noteLength5, 3, melody_V2_67a, noteLength2, 4, melody_B_67a, noteLength2, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayHeartL2Fullb();
    play(melody_V1_67b, noteLength2, 4, melody_V2_67b, noteLength2, 4, melody_B_67b, noteLength2, 4);


  // M77
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("*      YOUR     ");
    play(melody_V1_77a, noteLength1, 2, melody_V2_77a, noteLength1, 2, melody_B_77a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("*  AWESOMENESS  ");
    play(melody_V1_77b, noteLength6, 3, melody_V2_77b, noteLength6, 3, melody_B_77b, noteLength6, 3);


  // M78
  displaySpiderL1a();
  displayHeartL1Right();
  displayL2("*   IS MAXED  ");
    play(melody_V1_78a, noteLength8, 5, melody_V2_78a, noteLength8, 5, melody_B_78a, noteLength10, 4);
  displaySpiderL1b();
  displayHeartL1Left();
  displayL2("* !!!! OUT !!!! ");
    play(melody_V1_78b, noteLength9, 8, melody_V2_78b, noteLength9, 8, melody_B_78b, noteLength0, 1);

}
void displayM78_M86() {
   //M79 (19)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" Ahuhu         ~");
    play(melody0, noteLength0, 1, melody_V1_79a, noteLength1, 2, melody_B_79a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" Ahuhuhuhu ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("      ");
    play(melody0, noteLength0, 1, melody_V1_79b, noteLength1, 2, melody_B_79b, noteLength0, 1);


  //M80 (20)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" That was     ~");
    play(melody0, noteLength0, 1, melody_V1_80a, noteLength5, 3, melody_B_80a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" FUN ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("        ");
    play(melody0, noteLength0, 1, melody_V1_80b, noteLength1, 2, melody_B_80b, noteLength0, 1);

  //M81 (21)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write(" I think      ~");
    play(melody0, noteLength0, 1, melody_V1_81a, noteLength5, 3, melody_B_81a, noteLength1, 2);
  displaySpiderL1b();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write(" I would      ~");
    play(melody0, noteLength0, 1, melody_V1_81b, noteLength1, 2, melody_B_81b, noteLength0, 1);

  //M82 (22)
  displaySpiderL1a();
  displayHeartL1Right(); 
  displaySpider(0, 1);
  lcd.write("   SPARE YOU    ");
    play(melody_V1_22a, noteLength5, 3, melody0, noteLength0, 1, melody_B_82a, noteLength1, 2);
  displaySpiderL1d();
  displayHeartL1Left();
  displaySpider(0, 1);
  lcd.write("   ");
  displayHeartL2(4, 5);
  lcd.write(" NOW ");
  displayHeartL2(11, 12);
  lcd.write("      ");
    play(melody_V1_22b, noteLength2, 4, melody0, noteLength0, 1, melody_B_82b, noteLength1, 2);

  //M83 (31)
  displaySpider(0, 1);
  lcd.write(" Come         ~");
    play(melody_V1_31a, noteLength1, 2, melody0, noteLength0, 1, melody_B_83a, noteLength1, 2);
  displaySpider(0, 1);
  lcd.write(" back at      ~");
    play(melody_V1_31b, noteLength1, 2, melody0, noteLength0, 1, melody_B_83b, noteLength0, 1);

  // M84 (32)
  displaySpider(0, 1);
  lcd.write(" any          ~");
    play(melody_V1_32a, noteLength6, 3, melody0, noteLength0, 1, melody_B_84a, noteLength1, 2);
  displaySpider(0, 1);
  lcd.write(" anytime!      ");
    play(melody_V1_32b, noteLength5, 3, melody0, noteLength0, 1, melody_B_84b, noteLength0, 1);

  // M85 (33)
  displaySpider(0, 1);
  lcd.write("   SEE YOU     ");
    play(melody_V1_33a, noteLength3, 3, melody0, noteLength0, 1, melody_B_85a, noteLength1, 2);
  displaySpider(0, 1);
  lcd.write("    AGAIN     ");

    play(melody_V1_33b, noteLength1, 2, melody0, noteLength0, 1, melody_B_85a, noteLength1, 2);

  // M86 (84)
  displaySpider(0, 1);
  lcd.write("    DEARY ");
  lcd.write(byte(4));
  lcd.write(byte(4));
  lcd.write("      ");
    play(melody_V1_34a, noteLength2, 4, melody0, noteLength0, 1, melody_B_86a, noteLength1, 2);

    play(melody_V1_34b, noteLength0, 1, melody0, noteLength0, 1, melody_B_86b, noteLength0, 1);
}

void displayEnding() {
  delay(3000);
   lcd.clear();

   playClick();
   lcd.home();
   lcd.write("* Muffet spared");
   lcd.setCursor(0, 1);
   lcd.write("  you!");

   delay(3000);
   lcd.clear();
   playClick();
   lcd.home();
   lcd.write("* You earned ");
   lcd.setCursor(0, 1);
   lcd.write("  0 XP ");

   delay(3000);
   lcd.clear();
   playClick();
   lcd.home();
   lcd.write("* and ");
   lcd.setCursor(0, 1);
   lcd.write("  -19998G ");

   delay(3000);
   lcd.clear();
   playClick();
   lcd.setCursor(15, 1);
   lcd.write(".");
   delay(1000);

   for(int i = 15; i > 8; i--) {
    delay(1000);
    lcd.scrollDisplayLeft();
   }

    delay(3000);
    
   for(int i = 15; i > 11; i--) {
    delay(600);
    lcd.scrollDisplayRight();
   }

   for(int i = 11; i > 7; i--) {
    delay(300);
    lcd.scrollDisplayRight();
   }

  delay(2000);
   lcd.clear();
   playClick();
   lcd.home();
   lcd.write("   THANKS FOR   ");
   lcd.setCursor(0, 1);
   lcd.write("    WATCHING    ");
   delay(5000);
}
