//***********SECTION ECRAN OLED*************
#include <DigisparkOLED.h>
#include <Wire.h>
#include "chickendoor_128x64c1.h"

//***********WATCHDOG SLEEP MODE ***********
// using watch dog instead of delay
// Soft with delay: Power consumption: 0.108-0.120W
// Soft with Watchdog: Power Consumption: 0.038-0.051W
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

//***********SECTION RTC DS3231 ************
#include "RTClib.h"
RTC_DS3231 rtc;

//***********SECTION SERVOMOTEUR ***********
#include <SimpleServo.h>
SimpleServo servo;
  
//***********VARIABLE**********************
int TSTART=10;//horaire ouverture par defaut
int TEND=20;//horaire fermeture par defaut
int MEND=00;//Min fermeture par defaut
bool PORTEOUV=0;//0:fermé - 1:ouvert
bool TIME= 0;//Force MAJ RTC

//***********LANCEMENT PROG***************
void setup() {

//***********WATCH DOG ********************
  // Power Saving setup
  for (byte i = 0; i < 6; i++) {
    pinMode(i, INPUT);      // Met tous les ports en INPUT pour economiser l'energie
    digitalWrite (i, LOW);  //
  }
  adc_disable();          // Desactive le convertisseur Analog-to-Digital
  wdt_reset();            // RESET Watchdog
  wdt_enable(WDTO_8S);    // Temps du Watchdog: 15MS, 30MS, 60MS, 120MS, 250MS, 500MS, 1S, 2S, 4S, 8S
  WDTCR |= _BV(WDIE);     // Permettre l'interruption du watchdog.
  sei();                  // Permettre les interruptions
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Sleep Mode: maximum

//*******ACTIVATION MOTEUR +RELAIS *******
  //pinMode(3, OUTPUT); // moteur relai sur PIN 3
  pinMode(5, OUTPUT); //MOTEUR sur PIN5
  servo.attach(5);
  ouvrir(); //On ouvre la porte par defaut

//*******ACTIVATION ECRAN OLED *******
  oled.begin();
  oled.clear();
  oled.bitmap(0, 0, 128, 8, Chickendoorlogo);
  delay(8000);
  oled.clear();
  
//******VERIF RTC + MAJ *************  
    if (! rtc.begin()) {
          oled.println(F("Connection RTC Impossible"));
      } else {
          if(TIME) {
         oled.print(F("DEBUG:MISE A JOUR RTC OK !"));
         rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
         delay(1000);
         };
          oled.setCursor(0, 2);
          oled.setFont(FONT8X16);
          oled.print(F("RTC OK"));
          if (rtc.lostPower()) {
                            oled.print(F("MISE A JOUR RTC OK !!"));
                            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
                              }
        delay(1000);
		}
   changh();
		
}
void loop() {
  DateTime now = rtc.now();
  
if ((TSTART == now.hour()) && (MEND == now.minute()) && (PORTEOUV == 0)) { //Heure ouverture
                            ouvrir();
                                                }
                                                
if ((TEND == now.hour()) && (MEND == now.minute()) && (PORTEOUV == 1)) {//Heure fermeture
                            fermer();
                            changh();
                            }
ecran();
attendre();                          
}

void attendre() {
                sleep_enable();
                sleep_cpu();
                }

ISR (WDT_vect) {
                WDTCR |= _BV(WDIE);
                }
                
void ouvrir() {
              digitalWrite(3, HIGH);
              delay(100);
              PORTEOUV=1;
              servo.write(180);
              delay(2000);
              digitalWrite(3, LOW);
              delay(100);              
              }
              
void fermer() {
              digitalWrite(3, HIGH);
              delay(100);
              PORTEOUV=0;
              servo.write(0);
              delay(2000);
              digitalWrite(3, LOW);
              delay(100);              
               }
			   
void ecran() {
                DateTime now = rtc.now();
                oled.begin();
                oled.clear();			
                //oled.setCursor(0, 2);
                oled.setFont(FONT8X16);
  
  //AFFICHE LA PROCHAINE ACTION
                  oled.print(F("CLOSE:"));
                oled.print(TEND);
                oled.print(F(":"));
                oled.print(MEND);         
                oled.println();
                
 //AFFICHE L'heure actuelle
                oled.print(now.hour(), DEC);
                oled.print(F(":"));
                oled.print(now.minute(), DEC);
                oled.print(F(":"));
                oled.print(now.second(), DEC);
                
//AFFICHE LA TEMPERATURE
                oled.println();
                oled.print(F("Temp:"));
                oled.print(rtc.getTemperature());
                oled.print(F(" C"));
                
  }

void changh() {
  DateTime now = rtc.now();

if(now.month() == 1) {
    if(now.day() < 9) {  TEND=17; MEND=30;  return;}
    if(now.day() < 20) {  TEND=17;  MEND=45;  return;}
    if(now.day() < 32) {  TEND=18;  MEND=00;  return;}
}

if(now.month() == 2) {
    if(now.day() < 12) {  TEND=18;  MEND=15;  return;}
    TEND=18;  MEND=30;  return;

}

if(now.month() == 3) {
    if(now.day() < 5) {  TEND=18; MEND=45;  return;}
    if(now.day() < 18) {  TEND=19;  MEND=00;  return;}
    if(now.day() < 30) {  TEND=19;  MEND=15;  return;}
    if(now.day() > 29) {  TEND=20;  MEND=15;  return;}
}

if(now.month() == 4) {
    if(now.day() < 12) {  TEND=20;  MEND=30;  return;}
    TEND=20;  MEND=45;  return;

}

if(now.month() ==5) {
    if(now.day() < 7) {  TEND=21; MEND=00;  return;}
    TEND=21;  MEND=15;  return;

}

if(now.month() ==6) {

    TEND=21;  MEND=35;  return;

}

if(now.month() ==7) {
    if(now.day() > 29) {  TEND=21;  MEND=15;  return;}
    TEND=21;  MEND=30;  return;

}

if(now.month() ==8) {
    if(now.day() < 11) {  TEND=21;  MEND=00;  return;}
    if(now.day() < 21) {  TEND=20;  MEND=45;  return;}
    TEND=20;  MEND=30;  return;

}

if(now.month() ==9) {
    if(now.day() < 07) {  TEND=20;  MEND=15;  return;}
    if(now.day() < 16) {  TEND=20;  MEND=00;  return;}
    TEND=19;  MEND=45;  return;

}

if(now.month() ==10) {
    if(now.day() < 02) {  TEND=19;  MEND=30;  return;}
    if(now.day() < 11) {  TEND=19;  MEND=15;  return;}
    if(now.day() < 20) {  TEND=19;  MEND=00;  return;}
    if(now.day() < 27) {  TEND=18;  MEND=00;  return;}
    TEND=17;  MEND=45;  return;

}

if(now.month() ==11) {
    if(now.day() < 10 ) {  TEND=17; MEND=30;  return;}
    TEND=17;  MEND=15;  return;

}

if(now.month() ==12) {
    if(now.day() < 19 ) {  TEND=17; MEND=15;  return;}
    TEND=17;  MEND=30;  return;

}
}
