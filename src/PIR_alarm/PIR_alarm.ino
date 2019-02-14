//******************* HEADER ***********************************
/*
 Name : PIR_ALARM
 Title : PIR Alarm
 Description : An Arduino based passive infared red(PIR) Alarm. Input from Jumpers and PIR sensor.
 Ouptu to Buzzer , relay circuit and Status LED. Sleep Mode wakes on interrupt.
 Author: Gavin Lyons
 URL: 
*/

//******************** LIBRARIES*****************


//********************** GLOBALS ********************
char JumperInputs = 0;

//********************** SETUP ************************
void setup() {
  
 //Internal resistors for buttons
  digitalWrite(12,HIGH);
  digitalWrite(11,HIGH);
  
  //debug
  Serial.begin(9600);

  }

//******************* MAIN LOOP *****************
void loop() {

 CheckJumpers();
 CheckAlarm();
 delay(100);
}
// ********************* END OF MAIN **************

// ********************* FUNCTIONS ****************

void CheckJumpers()
{
  
}

//*************************** EOF *****************
