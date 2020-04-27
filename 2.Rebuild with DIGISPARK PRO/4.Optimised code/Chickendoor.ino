//***********SECTION ECRAN OLED*************
#include <DigisparkOLED.h>
#include <Wire.h>
#include "chickendoor_128x64c1.h"
//***********SECTION RTC DS3231 ************
#include "RTClib.h"
RTC_DS3231 rtc;
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
int sensorValue = 0; //Capteur solaire
bool DEBUG= 1;//mode test rafraichit l ecran tres regulierement.

//***********LANCEMENT PROG***************
void setup() {

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
        if(DEBUG) {rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  };
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
  sensorValue = digitalRead(3); // NUIT=1 JOUR=0
  DateTime now = rtc.now();
  if (now.hour() <  TSTART) { //AVANT HEURE CALCUL TEMPS AVANT OUVERTURE
              TEMPSREST=TSTART-now.hour();
              TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
              MINREST=(60-now.minute());
              ecran();
              if( TEMPSRESTT >= 10800000) {
                            TEMPSRESTT=10800000;
                            }                                                            
                           if(!DEBUG) {delay (TEMPSRESTT);  };
              
  } else {      
        if ((TSTART == now.hour()) && (PORTEOUV == 0)) { //Heure ouverture
                            ouvrir();
                            TEMPSRESTT=3660000;
                            if(!DEBUG) {delay (TEMPSRESTT); };
        } else {
        if(( now.hour() > TSTART) && ( now.hour() < TEND)) {
                            TEMPSREST=TEND-now.hour();
                            TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
                            MINREST=(60-now.minute());
                            ecran();
                            if( TEMPSRESTT >= 10800000) {
                              TEMPSRESTT=10800000;
                              }                                                            
                            if(!DEBUG) {delay (TEMPSRESTT); };
        } else {
       if ((TEND == now.hour()) && (PORTEOUV == 1)) {//Heure fermeture
                          if (!sensorValue) {//NUIT
                            fermer();
                            changh();
                            TEMPSRESTT=3660000;
                          } else {//Attente 10min
                            TEMPSRESTT=600000;
                          }
                          if(!DEBUG) {delay (TEMPSRESTT); };

        } else {
        if ((now.hour() > TEND) && (PORTEOUV == 1)) { //au cas ou le delay depasse l'heure de fermeture peut importe etat lumiere ex panne
                          fermer();
                          changh();
                          TEMPSRESTT=3660000;
                          if(!DEBUG) {delay (TEMPSRESTT); };
                          
                          
        } else {
        if (now.hour() > TEND) {//APRES L'heure. calcul temps avant ouverture
                          TEMPSREST=(24-now.hour())+TSTART;
                          TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
                          MINREST=(60-now.minute());
                          ecran();
                          if( TEMPSRESTT >= 10800000) {
                              TEMPSRESTT=10800000;
                              }                                                            
                          if(!DEBUG) {delay (TEMPSRESTT);};
                            }
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
  

  //oled.setCursor(0, 2);
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
//AFFICHE LA VALEUR CAPTEUR SOLAIRE
  oled.println();
  oled.print(F("ETAT:"));
  if(sensorValue) {
       oled.print(F("NUIT"));
   } else  {
      oled.print(F("JOUR"));
  };
  //oled.print(sensorValue);
  
delay(1000);
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
