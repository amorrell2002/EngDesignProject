
#define LED1PIN 2
#define LED2PIN 5

#define LIGHTSON true;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);

  if(LIGHTSON)
  {
    digitalWrite(LED1PIN, HIGH);
    digitalWrite(LED2PIN, HIGH);
  }
  else
  {
    digitalWrite(LED1PIN, LOW);
    digitalWrite(LED2PIN, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
