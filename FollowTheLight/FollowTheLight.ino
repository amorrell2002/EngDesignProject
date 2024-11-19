#include <stdbool.h>


#define RNGSEED 2002

//Button Pin Macros for prototyping
#define MOD1PINBUT 0
#define MOD1PINLED 2

#define MOD2PINBUT 3
#define MOD2PINLED 5
/*
#define MOD3PINBUT 5
#define MOD3PINLED 6

#define MOD4PINBUT 4
#define MOD4PINLED 4

#define MOD5PINBUT 5
#define MOD5PINLED 5

#define MOD6PINBUT 4
#define MOD6PINLED 5
*/

#define BUZZPIN 11

//Number of modules currently connected
#define MODNUM 2

//How frequently (in ms) a new led is chosen to be lit
#define NEWLEDINTERVAL 2000
//Game duration (in ms)
#define GAMEDURATION 25000

//Data Structures 
//Associating the index(module) with a pin for the button
int buttonPins [MODNUM] = {MOD1PINBUT, MOD2PINBUT};
//Same but leds
int ledPins [MODNUM] = {MOD1PINLED, MOD2PINLED};

//This datastructure contatins which leds are currenlty lit(stored as true) or unlit(stored as false)
bool ledValues[MODNUM] = {false};


//Initialize the currTime variable for timing
unsigned long currTime = millis();


//Points scored by player
unsigned int score = 0;

//Toggle to turn on and off all LED's reguardless of ledValue status
bool ledEnable = true;

//A wrapper fuction that introduces a blocking delay for easier music programming
void blockingBuzz(unsigned int duration, int freq)
{
  //magic num "1.3" is from https://docs.arduino.cc/built-in-examples/digital/toneMelody/
  tone(BUZZPIN, freq, duration);
  delay(duration * 1.3);
  noTone(BUZZPIN);
}

//Called when a point is scored by the player
void buzzPointScored()
{
  noTone(BUZZPIN);
  delay(100);
  tone(BUZZPIN, 440, 200);
}

//Played at the end of a game before the players score is announced
void buzzEndSong()
{
  blockingBuzz(500, 523);
  blockingBuzz(500, 440);
  blockingBuzz(500, 329);
  delay(1000);
}

//Intoduction song, plays when game begins
void buzzStartSong()
{
  for(int i = 0; i < 3; i++)
  {
  blockingBuzz(500, 329);
  }
  /*
  const int N2DUR = 1000;
  tone(BUZZPIN, 329, 1000);
  delay(N2DUR * 1.3);
  noTone(BUZZPIN);
  */
  blockingBuzz(800, 440);
}

//Annouces player score with a note that increases by 1 semitone for each point, topping out at 880 hz
void buzzScore()
{
  unsigned int frequency = 440;
  for(int i = 0; i < score; i++)
  {
    //1.0595 multiplied by current frequency should yield the next semitone
    frequency = frequency * 1.0595;
    if(frequency < 880)
    {
      blockingBuzz(500, frequency);
    }
    else
    {
      blockingBuzz(500, 880);
    }
  }
}

//Setter function for pin data stuctures
void setPinMode(int* pins, int numOfPins, uint8_t mode)
{
  for(int i = 0; i < numOfPins; i++)
    {
        pinMode(pins[i], mode);
    }
}




void setup()
{
    Serial.begin(9600);
    Serial.println("Serial Start...");
    //Set LED pins to output
    setPinMode(ledPins, MODNUM, OUTPUT);

    //Set button pins to input (floating input, needs pullup?)
    setPinMode(buttonPins, MODNUM, INPUT_PULLUP);

    //Random Seed
    randomSeed(RNGSEED);

}



void loop()
{
  buzzStartSong();
  //Ennter gameloop until we surpass game duration
  while(millis() < GAMEDURATION)
  {
    gameLoop();
  }

  Serial.print("Points Scored: ");
  Serial.println(score);

  //turn off leds
  ledEnable = false;
  writeLights();


  buzzEndSong();
  buzzScore();
  //Idle loop, require player reset
  while(true)
  {
    delay(100);
  }
}

void gameLoop()
{
    //Choose led's to light up

    //RNG light picking every NEWLEDINTERVAL milliseconds, will only flip "false" ledValues 
    if(millis() > currTime + NEWLEDINTERVAL)
    {
      ledRNG();
      currTime = millis();
    }

    //Write ledValues to lights if ledEnable == true
    writeLights();

    //Listen for player input
    buttonListen(LOW);

    //update leds
    writeLights();
}

void ledRNG()
{
  //Create ledList and get currently unlit leds to choose one to be lit
  int ledList [MODNUM + 1] = {0};
  getFalseLEDList(ledList);
  
  int ledCount = ledList[0];
  Serial.print("LED OFF COUNT: ");
  Serial.println(ledCount);
  //RNG function random() is used to pick one of the leds in ledList
  int chosenLED = random(1, ledCount + 1);
  Serial.print("Chosen LED: ");
  Serial.println(chosenLED);

  //Set chosen led value to true in ledValues
  ledValues[ledList[chosenLED]] = true;
  
}

//Modifies the passed list pointer. ledList[0] contains number of currently unlit leds.
//Every index after is the pin of that unlit led
void getFalseLEDList(int* ledList)
{
  int falseCount = 0;
  for(int i = 0; i < MODNUM; i++)
  {
    if(!ledValues[i])
    {
      falseCount++;
      ledList[falseCount] = i;
    }
  }
  ledList[0] = falseCount;
  return ledList;
}

//Check all connected buttons for player input
//If input is detected, and the button's associated led is also lit, then record point scored
void buttonListen(uint8_t value)
{
  for(int i = 0; i < MODNUM; i++)
  {
    if(digitalRead(buttonPins[i]) == value && ledValues[i])
    {
        //Point scored TODO
        score++;
        //Disable Light
        ledValues[i] = false;
        //Output score
        Serial.println("Point Scored!");
        buzzPointScored();
    }

  }
}

//Iterate over ledValues and output the correct volatge for each bool value
//Turns all LEDs off when ledEnable == false
void writeLights()
{
    for(int i = 0; i < MODNUM; i++)
    {
        if(ledValues[i] && ledEnable)
        {
          digitalWrite(ledPins[i], HIGH);
        }
        else
        {
          digitalWrite(ledPins[i], LOW);
        }
    }
}