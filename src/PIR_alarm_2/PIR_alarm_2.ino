//******************* HEADER **************
/*
  Name : PIR_ALARM_2
  Title : PIR Alarm
  Description : An Arduino based passive infrared red (PIR) Alarm. Inputs from Jumpers and PIR sensor.
  Output to Buzzer, Relay ,  and Status LED's.
  Author: Gavin Lyons
  MC: ATMega328p stand-only also tested on arduino Uno and Nano
  URL:https://github.com/gavinlyonsrepo/PIR_Alarm
  Date: 12-03-19
  IDE: Arduino v 1.8.9
*/

//********************** GLOBALS ***************

uint8_t  mode = 0; //var to hold mode based on jumper input
uint8_t  AlarmCount = 0;

//**********************DEFINES ***************
// GPIO pins
#define Jumper1_pin 6
#define Jumper2_pin 7
#define Buzzer_pin 10
#define Relay_pin 9
#define Led_pin 8
#define PIR_pin 2

// delays
#define myDelay 1000
#define InitDelay 15 // number of (myDelays*2) for PIR init delay
#define AlarmDelay 120 //number of myDelays for alarm state max 256
#define AlarmMAX 10 //alarm go off X times

//********************** SETUP ************************
void setup() {
  GPIO_Setup();
  Serial_Setup();
  Check_Jumper();
  Init_Delay();
}

//******************* MAIN LOOP *****************
void loop() {
  Check_Alarm();
}
// ********************* END OF MAIN **************

// ********************* FUNCTION SPACE ****************

// Function check jumper ,Run at Setup
// check  if switch/jumpers are set for modes setup.
void Check_Jumper()
{
  boolean JP1;
  boolean JP2;
  JP1 = digitalRead(Jumper1_pin);
  JP2 = digitalRead(Jumper2_pin);
  Serial.print("JP1: ");
  Serial.println(JP1);
  Serial.print("JP2: ");
  Serial.println(JP2);

  // Jumper truth table
  //00 = All on
  //01 = relay & LED on
  //10 = buzzer & LED  on
  //11 = LED only

  if (JP1 == 1 && JP2 == 1)
  {
    mode = 0; //LED only
  }
  else  if  (JP1 == 1 && JP2 == 0)
  {
    mode = 1; //buzzer and LED
  }
  else if  (JP1 == 0 && JP2 == 1)
  {
    mode = 2;// Relay and LED
  }
  else if (JP1 == 0 && JP2 == 0)
  {
    mode = 3;//everything
  }
  Serial.print("mode: ");
  Serial.println(mode);
}

//Function GPIO setup , run at setup setups the GPIO pins
void GPIO_Setup(void)
{
  //Pir pin
  pinMode(PIR_pin, INPUT);

  ////Internal pull-up resistors for jumpers
  pinMode(Jumper1_pin, INPUT);
  digitalWrite(Jumper1_pin, HIGH);
  pinMode(Jumper2_pin, INPUT);
  digitalWrite(Jumper1_pin, HIGH);

  // initialize the LED pin as an output:
  pinMode(Led_pin, OUTPUT);
  digitalWrite(Led_pin, LOW);

  // initialize the Relay pin as an output:
  pinMode(Relay_pin, OUTPUT);
  digitalWrite(Led_pin, LOW);

  // initialize the Buzzer pin as an output:
  pinMode(Buzzer_pin, OUTPUT);
  digitalWrite(Buzzer_pin, LOW);
}

// Function Serial_setup , run at setup,  setups serial comms
//only needed for debug.
void Serial_Setup()
{
  // Serial debug
  Serial.begin(9600);
  Serial.println("---- PIR ALARM Start ----");
}

//Function to Check the alarm Condition
void Check_Alarm(void)
{
  if (digitalRead(PIR_pin) == HIGH) {
    delay(25); //check again to check for a glitch in voltage line
    if (digitalRead(PIR_pin) == HIGH) {
      Output_Alarm();
    }
  }
  else if (digitalRead(PIR_pin) == LOW)
  {
    Serial.println("Did not detect movement.");
    delay(myDelay); // Stop serial monitor getting flooded
  }
}

//Function: OutputAlarm , If alarm condition detected , actions.
void Output_Alarm(void)
{
  Serial.println("Movement detected.");
  AlarmCount++;
  switch (mode)
  {
    case 0:
      //LED only
      digitalWrite(Led_pin, HIGH);
      for (uint8_t i = 0; i < AlarmDelay ; i++)
      {
        delay(myDelay);
      }
      break;

    case 1: //buzzer and LED only
      digitalWrite(Led_pin, HIGH);
      if (AlarmCount < AlarmMAX) // Stop false triggers causing buzzer
      {
        digitalWrite(Buzzer_pin, HIGH);
      }
      for (uint8_t i = 0; i < AlarmDelay ; i++)
      {
        delay(myDelay);
      }
      digitalWrite(Buzzer_pin, LOW);
      digitalWrite(Led_pin, LOW);
      break;
    case 2: // relay and LED only
      digitalWrite(Led_pin, HIGH);
      if (AlarmCount < AlarmMAX) // Stop false triggers causing buzzer/relay
      {
        digitalWrite(Relay_pin, HIGH);
      }
      for (uint8_t i = 0; i < AlarmDelay ; i++)
      {
        delay(myDelay);
      }
      digitalWrite(Relay_pin, LOW);
      digitalWrite(Led_pin, LOW);
      break;

    case 3:
      digitalWrite(Led_pin, HIGH);
      if (AlarmCount < AlarmMAX) // Stop false triggers causing buzzer/relay
      {
        digitalWrite(Relay_pin, HIGH);
        digitalWrite(Buzzer_pin, HIGH);
      }
      for (uint8_t i = 0; i < AlarmDelay ; i++)
      {
        delay(myDelay);
      }
      digitalWrite(Buzzer_pin, LOW);
      digitalWrite(Relay_pin, LOW);
      digitalWrite(Led_pin, LOW);
      break;
  }
}

//inital delay at setup, PIR sensor needs this.
void Init_Delay(void)
{
  digitalWrite(Led_pin, HIGH);

  for (uint8_t i = 0; i < InitDelay ; i++)
  {
    digitalWrite(Led_pin, LOW);
    delay(myDelay);
    digitalWrite(Led_pin, HIGH);
    delay(myDelay);
  }
  digitalWrite(Led_pin, LOW);
}
//*************************** EOF *****************
