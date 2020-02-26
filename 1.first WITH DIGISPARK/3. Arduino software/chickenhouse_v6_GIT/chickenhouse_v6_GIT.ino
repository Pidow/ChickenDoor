  #include <TinyWireM.h>
  #include "TinyRTClib.h"

  RTC_DS1307 RTC;
  #define DS1307_ADDR   0x68
  #include <SimpleServo.h>
  SimpleServo servo;
//VARIABLE
int TSTART=10;
int TEND=20;
int CLIGNO=0;
int MOT=0;
bool INITIA=0;
int TEMPSREST=0;
int TSUPP=0;
long TEMPSRESTT=0;
bool PORTEOUV=0;
int var=0;
bool DEBUG=1; //Active les lumieres de communication.

void setup () {
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(1, OUTPUT);
  servo.attach(1);
  servo.setMillisPer60Degrees(390); 
  servo.setMinimumPulse(400); 
  servo.setMaximumPulse(2800); 
  servo.setPulseMillis(20);


   Serial.begin(115200);
    TinyWireM.begin(); 
    RTC.begin();
    RTC.adjust(DateTime(__DATE__, __TIME__));
    if (! RTC.isrunning()) {
      RTC.adjust(DateTime(__DATE__, __TIME__));
                            }
  TinyWireM.beginTransmission(DS1307_ADDR); // reset DS1307 register pointer 
  TinyWireM.send(0);
  //TinyWireM.requestFrom(DS1307_ADDR, 7);
  
  changh();
}
 
void loop() {
    DateTime now = RTC.now();

    if(INITIA ==0) {
                    initia();
                    CLIGNO=1;//TEST LED
                    cligno();
                    delay (5000);
                    changh();
                    

                        if(( now.hour() >= TSTART) && ( now.hour() < TEND)) { ouvrir(); }
                        if(( now.hour() < TSTART) or ( now.hour() > TEND)) { fermer(); }
                                            
                    if(DEBUG) { //ON AFFICHE L'HEURE PAR CLIGNOTEMENT
                               CLIGNO=now.hour();               
                                cligno();
                                delay (5000);
                              }
                  }

    if( PORTEOUV == 0) { 
                              if(( now.hour() >= TSTART) && ( now.hour() < TEND)) {
                                                      ouvrir();
                                                      } else {                                                                            

                                                              TSUPP = 0;
                                                              if( now.hour() > TSTART) { TSUPP = ((24-now.hour())+TSTART); }
                                                              
                                                              TEMPSREST = (TSTART-now.hour());
                                                              TEMPSREST = (TSUPP+TEMPSREST);
                                                              
                                                   if(DEBUG) {                                                      
                                                              CLIGNO=TEMPSREST;               
                                                              cligno();
                                                              delay (5000);
                                                             }
                                                              TEMPSRESTT=((TEMPSREST*60)+(now.minute())*60000);
                                                               if( TEMPSRESTT >= 10800000) {
                                                                                            TEMPSRESTT=10800000;
                                                                                            }                                                            
                                                              delay (TEMPSRESTT);
                                                             }

                        }
                        


    if( PORTEOUV == 1) {
                         if(( now.hour() < TSTART) or ( now.hour() >= TEND)) {
                                                      fermer();
                                                      changh();
                                                   } else {                                                   
                                                              TEMPSREST=(TEND-now.hour());
                                                              
                                                  if(DEBUG) {                                                             
                                                              CLIGNO=TEMPSREST;               
                                                              cligno();
                                                              delay (5000);
                                                            }

                                                              TEMPSRESTT=(((TEMPSREST-1)*60)+((60-now.minute()))*60000);
                                                               
                                                               if( TEMPSRESTT >= 10800000) {
                                                                                            TEMPSRESTT=10800000;
                                                                                            }
                                                                
                                                              delay (TEMPSRESTT);
                                                             }


                      }
}

void initia() {
              digitalWrite(3, HIGH);
              delay(100);
              fermer();
              delay(4000);
              ouvrir();
              INITIA=1;
              digitalWrite(3, LOW);
              delay(100);
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
               
void cligno() {
               var = 0;
              while(var < CLIGNO){
              digitalWrite(4, HIGH);
              delay(1000);
              digitalWrite(4, LOW);
              delay(100);
              var++;
              }
              return;
}

void changh() {
  DateTime now = RTC.now();
  if(now.month() == 1) { 
                        TSTART=8;
                        TEND=18; 
                        }
  if(now.month() == 2) { 
                        TSTART=8;
                        TEND=19; 
                        }
  if(now.month() == 3) { 
                        TSTART=8;
                        TEND=20;//20h 
                        }
  if(now.month() == 4) { //+1h
                        TSTART=7;
                        TEND=21; 
                        }
  if(now.month() == 5) { 
                        TSTART=7;
                        TEND=22; 
                        }
  if(now.month() == 6) { 
                        TSTART=7;
                        TEND=22; 
                        }
  if(now.month() == 7) { 
                        TSTART=7;
                        TEND=22; 
                        }
  if(now.month() == 8) { 
                        TSTART=7;
                        TEND=22; 
                        }
  if(now.month() == 9) { 
                        TSTART=8;
                        TEND=21; 
                        }
  if(now.month() == 10) { 
                        TSTART=8;
                        TEND=19; 
                        }
  if(now.month() == 11) { //-1h
                        TSTART=8;
                        TEND=18; 
                        }
  if(now.month() == 12) { 
                        TSTART=8;
                        TEND=18; 
                        } 
}
