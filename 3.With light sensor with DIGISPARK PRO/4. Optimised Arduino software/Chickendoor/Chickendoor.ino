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
bool PORTEOUV=0;//0:fermé - 1:ouvert
int sensorValue = 0; //Capteur solaire: 1:Nuit 0:Jour
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

//***********CAPTEUR LUMIERE***************
pinMode(3,INPUT);

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
  sensorValue = digitalRead(3); // NUIT=1 JOUR=0
  DateTime now = rtc.now();
  
     
if ((TSTART == now.hour()) && (PORTEOUV == 0)) { //Heure ouverture
                            ouvrir();
                                                }                                                
if(( now.hour() > TSTART) && (PORTEOUV == 0)) { //Depassement on ouvre
                            ouvrir();
                                                }
if ((TEND == now.hour()) && (PORTEOUV == 1)) {//Heure fermeture
							            if (!sensorValue) {//NUIT
							              fermer();
							              changh(); }
                                                }
if ((now.hour() > TEND) && (PORTEOUV == 1)) { //au cas ou le delay depasse l'heure de fermeture peut importe etat lumiere ex panne
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
                  oled.print(F("PROCHAIN:"));
                if (now.hour() < TSTART) {  oled.print(TSTART); }
                if ( ( ( now.hour() > TSTART) || (now.hour() == TSTART)) && ( now.hour() < TEND)) {  oled.print(TEND); }                                          
                if (( now.hour() > TEND) || (now.hour() == TEND)) {  oled.print(TSTART); }
                oled.print(F("h"));         
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
                
//AFFICHE LA VALEUR CAPTEUR SOLAIRE
                oled.println();
                oled.print(F("ETAT:"));
                if(sensorValue) {
                                oled.print(F("NUIT"));
                } else  {
                                oled.print(F("JOUR"));
                };
  }

void changh() {
  DateTime now = rtc.now();
  if(now.month() == 1) { 
                        TSTART=9;
                        TEND=18; 
                        }
  if(now.month() == 2) { 
                        TSTART=9;
                        TEND=19; 
                        }
  if(now.month() == 3) { 
                        TSTART=9;
                        TEND=20;//20h 
                        }
  if(now.month() == 4) { //+1h
                        TSTART=9;
                        TEND=21; 
                        }
  if(now.month() == 5) { 
                        TSTART=9;
                        TEND=22; 
                        }
  if(now.month() == 6) { 
                        TSTART=9;
                        TEND=22; 
                        }
  if(now.month() == 7) { 
                        TSTART=9;
                        TEND=22; 
                        }
  if(now.month() == 8) { 
                        TSTART=9;
                        TEND=22; 
                        }
  if(now.month() == 9) { 
                        TSTART=9;
                        TEND=21; 
                        }
  if(now.month() == 10) { 
                        TSTART=9;
                        TEND=19; 
                        }
  if(now.month() == 11) { //-1h
                        TSTART=9;
                        TEND=18; 
                        }
  if(now.month() == 12) { 
                        TSTART=9;
                        TEND=18; 
                        } 
}
