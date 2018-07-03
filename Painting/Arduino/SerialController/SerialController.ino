// Digital pin for casefan and turntable
const int Pin_CaseFan = 11;
const int Pin_TurnableTable = 12;
const int RotateTime = 12200;
const int DelayTime = 3000;

int state = 0;

void setup() 
{
  // Setup serial communication
  Serial.begin(9600);  
  
  // Declare pin in use
  pinMode(Pin_CaseFan, OUTPUT);
  pinMode(Pin_TurnableTable, OUTPUT);

  // Setup defaule state
  // digitalWrite(Pin_CaseFan, LOW);
  // digitalWrite(Pin_TurnableTable, LOW);
}

void loop() 
{
  if (Serial.available() > 0)
  {
    // read incoming data
    state = Serial.read();
    Serial.println(state);

    switch(state)
    {
      case '0':
        digitalWrite(Pin_CaseFan, LOW);
        digitalWrite(Pin_TurnableTable, LOW);
        break;
      case '1':
        digitalWrite(Pin_CaseFan, HIGH);
        digitalWrite(Pin_TurnableTable, LOW);
        break;
      case '2':
        digitalWrite(Pin_CaseFan, LOW);      
        digitalWrite(Pin_TurnableTable, HIGH);
        delay(RotateTime);
        digitalWrite(Pin_TurnableTable, LOW);
        delay(DelayTime);
        break;    
      case '3':
        digitalWrite(Pin_CaseFan, HIGH);
        // -- First turn
        digitalWrite(Pin_TurnableTable, HIGH);
        delay(RotateTime);
        digitalWrite(Pin_TurnableTable, LOW);
        delay(DelayTime);
        // -- Second turn
        digitalWrite(Pin_TurnableTable, HIGH);
        delay(RotateTime);
        digitalWrite(Pin_TurnableTable, LOW);
        delay(DelayTime);
        // -- Third turn
        digitalWrite(Pin_TurnableTable, HIGH);
        delay(RotateTime);
        digitalWrite(Pin_TurnableTable, LOW);
        delay(DelayTime);
        // -- Final turn
        digitalWrite(Pin_TurnableTable, HIGH);
        delay(RotateTime);
        digitalWrite(Pin_TurnableTable, LOW);
        delay(DelayTime);
        break; 
      case '4':
        digitalWrite(Pin_CaseFan, LOW);
        digitalWrite(Pin_TurnableTable, HIGH);
        break;
      case '5':
        digitalWrite(Pin_CaseFan, HIGH);
        digitalWrite(Pin_TurnableTable, HIGH);
        break;
      default:
        break;
    }
  }
}
