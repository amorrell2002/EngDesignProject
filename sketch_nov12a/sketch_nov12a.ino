int lights [6];

#define MOD1PINBUT 1
#define MOD1PINLED 2

#define MOD2PINBUT 4
#define MOD2PINLED 4

#define MOD3PINBUT 4
#define MOD3PINLED 4

#define MOD4PINBUT 4
#define MOD4PINLED 4

#define MOD5PINBUT 5
#define MOD5PINLED 5

#define MOD6PINBUT 4
#define MOD6PINLED 5

#include <stdbool.h>

int buttonPins [6] = {MOD1PINBUT, MOD2PINBUT, MOD3PINBUT, MOD4PINBUT, MOD5PINBUT, MOD6PINBUT};

int ledPins [6] = {MOD1PINLED, MOD2PINLED, MOD3PINLED, MOD4PINLED, MOD5PINLED, MOD6PINLED};

bool ledValues[6] = {false};


bool ledEnable = true;

void setup()
{
    //Set LED pins to output
    setPinMode(ledPins, 6, OUTPUT);

    //Set button pins to input (floating input, needs pulldown?)
    setPinMode(buttonPins, 6, INPUT);


}

void setPinMode(int* pins, int numOfPins, uint8_t mode)
{
  for(int i = 0; i < numOfPins; i++)
    {
        pinMode(pins[i], mode);
    }
}



int main()
{
  while(true)
  {
    gameLoop();
  }
}

void gameLoop()
{
    //Choose led's to light up

    //RNG light picking

    //Write new list to lights
    writeLights();

    //Listen for player input
    buttonListen(HIGH);

    //update leds
    writeLights();
}

void buttonListen(uint8_t value)
{
  //Return buttonNum pressed
  for(int i = 0; i < 6; i++)
  {
    if(digitalRead(buttonPins[i]) == value && ledValue[i])
    {
        //Point scored TODO
        //Disable Light
        ledValue[i] = false;
    }

  }
}

void writeLights()
{
    for(int i = 0; i < 6; i++)
    {
        if(ledValues[i] && ledEnable)
        {
          digitalWrite(ledPin[i], HIGH)
        }
        else
        {
          digitalWrite(ledPin[i], LOW)
        }
    }
}