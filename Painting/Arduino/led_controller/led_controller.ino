int ledPin =  8;// LED_BUILTIN
int ledState = 0; 

void setup() 
{
  pinMode(ledPin, OUTPUT); // Assign LED pin
  Serial.begin(9600);
}

void loop() 
{
  if (Serial.available() > 0)
  {
    ledState = Serial.read(); // used to read incoming data
      
    switch(ledState)// see what was sent to the board
    {
      case '1': // if the the one was sent
        digitalWrite(ledPin,HIGH);
        break;
      case '0': // if 0 was sent
        digitalWrite(ledPin,LOW);
        break;
      default:
        break; 
    }
  }
}
