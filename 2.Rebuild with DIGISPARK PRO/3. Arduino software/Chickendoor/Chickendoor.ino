//***********SECTION ECRAN OLED*************
#include <DigisparkOLED.h>
#include <Wire.h>
#include "chickendoor_128x64c1.h"
//***********SECTION RTC DS3231 ************
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//***********SECTION SERVOMOTEUR ***********
  #include <SimpleServo.h>
  SimpleServo servo;
  
//***********VARIABLE**********************
int TSTART=10;//horaire ouverture par defaut
int TEND=20;//horaire fermeture par defaut
int TEMPSREST=0;//temps restant avant prochaine action.
long TEMPSRESTT=0;//millis temps delay avant control.
int MINREST=0;//minutes restant
bool PORTEOUV=0;//0:fermé - 1:ouvert


//***********LANCEMENT PROG***************
void setup() {

//*******ACTIVATION MOTEUR +RELAIS *******
  pinMode(3, OUTPUT); // moteur relai sur PIN 3
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
        oled.setCursor(0, 2);
        oled.setFont(FONT8X16);
        oled.print(F("RTC OK"));
        if (rtc.lostPower()) {
                            oled.print(F("MISE A JOUR RTC OK !!"));
                            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
                              }
        delay(1000);
		}
		
}
void loop() {
  DateTime now = rtc.now();
  if (now.hour() <  TSTART) {
							TEMPSREST=TSTART-now.hour();
							TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
							MINREST=(60-now.minute());
							ecran();
							if( TEMPSRESTT >= 10800000) {
                            TEMPSRESTT=10800000;
                            }                                                            
                           delay (TEMPSRESTT);
							
  } else {      
        if ((TSTART == now.hour()) && (PORTEOUV == 0)) {
          ouvrir();
		  TEMPSRESTT=3660000;
		  delay (TEMPSRESTT);
        } else {
                if(( now.hour() > TSTART) && ( now.hour() < TEND)) {
                            TEMPSREST=TEND-now.hour();
                            TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
							MINREST=(60-now.minute());
							ecran();
							if( TEMPSRESTT >= 10800000) {
                            TEMPSRESTT=10800000;
                            }                                                            
                            delay (TEMPSRESTT);
							
                } else {
                        if ((TEND == now.hour()) && (PORTEOUV == 1)) {
							fermer();
							changh();
							TEMPSRESTT=3660000;
							delay (TEMPSRESTT);
                            } else {
                              if (now.hour() > TEND) {
                                    TEMPSREST=(24-now.hour())+TSTART;
                                    TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
                                    MINREST=(60-now.minute());
									ecran();
									if( TEMPSRESTT >= 10800000) {
									TEMPSRESTT=10800000;
									}                                                            
									delay (TEMPSRESTT);
													}
									}
                      }
    
              }
        }
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
  oled.begin();
  oled.clear();			
  

  oled.setCursor(0, 2);
  oled.setFont(FONT8X16);
  
  //AFFICHE LE TEMPS RESTANT AVANT PROCHAINE ACTION
  oled.print(F("RESTANT:"));
  oled.print(TEMPSREST-1);
  oled.print(F("h"));    
  oled.print(MINREST);           
    oled.println();
 //AFFICHE L'heure actuelle
  DateTime now = rtc.now();
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
  
delay(1000);
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
