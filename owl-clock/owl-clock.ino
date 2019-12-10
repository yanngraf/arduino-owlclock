/*
 * Owl Clock 
 * A clock tell you switches state every third day * 
 * Made for Tim *
 */

// Inclusion of all the different libraries needed
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <TimeLib.h> // the time library 
//#include <TimeAlarms.h> 
#include <Wire.h>
#include <Servo.h>

// Needs to be updated each time the code is complied again.
// not sure what daysSinceStart, and the 2 other variables are for anymore...
int daysSinceStart = 0; // day since the 
int dosisInterval = 3; // days between the different Euthyrox dosage. 3 = every third day the dosage is a half
int dosisStatus =  0; // 0 = half-dosis ; 1 = full dosis

// Definition of the first day from which to count a half dosis
// In the exampel (2018-1-1) we assume the first January is a half dosis, the 2nd and the 3rd a full one
// And then it starts over
int dosisStartYear = 2019;  // Specify year (ex: 2018)
int dosisStartMonth = 12; // Specify Month (ex: 1 (January)) 
int dosisStartDay = 9; // Specify Day (ex: 1 (First day of month))

// Servo-related vars
int angle;

Servo myServo;


//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------


void setup()  {

  Serial.begin(9600);
  
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");   
  
  // Alarms
  // create the 3 daily Cortisol alarms 
  //Alarm.alarmRepeat(6,30,00, MorningAlarm);  // 6:30am every day
  //Alarm.alarmRepeat(14,00,00, NoonAlarm);  // 2pm every day
  //Alarm.alarmRepeat(22,00,00, EveningAlarm);  // 10pm every day
  //Alarm.alarmRepeat(00,00,01, PinAlarm);  // 0am every day (to define the eutyrox pin)
 
  // Test alarm
  //Alarm.timerRepeat(15, Repeats);  
  //Alarm.timerRepeat(100, blinkEyes);       
  
  // Initialize Pins
  pinMode(3, OUTPUT); // Blue LED blinking

  // Calculate the days since started counting
  daysSinceStart = rdn(year(), month(), day()) - rdn(dosisStartYear, dosisStartMonth, dosisStartDay);
  Serial.print("Days since start counting euthyrox: ");
  Serial.println(daysSinceStart);
   
  // initializes the Euthyroy Pin position (Full dosis or Half Dose)
  MoveServo(getDosisDayStatus(daysSinceStart));
}


//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------

void loop()
{
  if (timeStatus() == timeSet) {
    digitalClockDisplay();
    //Serial.println("The time is set TETESTESTEST");
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    //delay(4000);
  }

  //Alarm.delay(1000); // wait one second between clock display
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}


int rdn(int y, int m, int d) { /* Rata Die day one is 0001-01-01 */
  // Calculating the number of days between 2 dates
  if (m < 3)
    y--, m += 12;
    return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + d - 306;
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void MoveServo (int pinStatus) { 
  // Servor mouvement function (on and off)
  // Accepts 0 = Half-dose OR everything else = Full Dose
  myServo.attach(9);
  
  if (pinStatus == 0) {
    myServo.write(0);
    Serial.println("Servo Off - 1/2 Eutyrox");
  } else {
     myServo.write(180);
     Serial.println("Servo On - Full Eutyrox");
  }
  delay(1000);
  myServo.detach();
  
}

// functions to be called when an alarm triggers:
//void MorningAlarm(){
//  Serial.println("Alarm: Morning 6:30"); 
//}

//void NoonAlarm(){
//  Serial.println("Alarm: Afternoon"); 
//}

//void EveningAlarm(){
//  Serial.println("Alarm: Evening"); 
//}

// Move the Arrow to the "1 or 1/2 euthyrox)
void PinAlarm() {

  // Update the count of days since start 
  daysSinceStart = rdn(year(), month(), day()) - rdn(dosisStartYear, dosisStartMonth, dosisStartDay);
 
  MoveServo(getDosisDayStatus(daysSinceStart));
}


// 15 seconds timer
void Repeats() {
  Serial.println("15 second timer");
  delay(100);
}


void blinkLed (int pin, int duration, int loops) {

  for (int i=0; i <= loops; i++){
    digitalWrite (pin, HIGH);
    delay(duration);
    digitalWrite (pin, LOW);
  }
}


int getDosisDayStatus (int dosisDay) {
  // Test if it is a Euthyorx half dosis days
  // Returns 0 if it is a half dosis day
  dosisDay = dosisDay % dosisInterval;
  return dosisDay;
}
