#include <stdbool.h>

#define RNGSEED 2002

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

#define MODNUM 2

#define NEWLEDINTERVAL 2000

#define GAMEDURATION 45000


int buttonPins [MODNUM] = {MOD1PINBUT, MOD2PINBUT};

int ledPins [MODNUM] = {MOD1PINLED, MOD2PINLED};

bool ledValues[MODNUM] = {false};



unsigned long currTime = millis();



unsigned int score = 0;

bool ledEnable = true;



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
  
  while(millis() < GAMEDURATION)
  {
    gameLoop();
  }

  Serial.print("Points Scored: ");
  Serial.println(score);
  while(true)
  {
    delay(5);
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
  int ledList [MODNUM + 1] = {0};
  getFalseLEDList(ledList);
  int ledCount = ledList[0];
  Serial.print("LED OFF COUNT: ");
  Serial.println(ledCount);
  int chosenLED = random(1, ledCount + 1);
  Serial.print("Chosen LED: ");
  Serial.println(chosenLED);
  ledValues[ledList[chosenLED]] = true;
  
}

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

//We may need some modifications depending on the button final pinout
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
        Serial.println("Point Scored!");
    }

  }
}

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