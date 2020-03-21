# ChickenDoor
Systeme d'ouverture et de fermeture automatique du poulailler

For this version i have used:

ATTiny167 Digispark Pro
RTC DS3231 (Real Time clock)
Relay for Servomotor
Servomotor MG995
Battery holder 4 battery 1,5 v (x2)
Oled screen 128x64 I2C SPI serial.

TARGET:
Move the chicken house doors 2 times per day.
Posibble to connect the Oled screen sometimes to have information.
Estimate battery lifetime: 8 month with 460mA battery.

How it's work:
Push software to Digispark.
If the RTC have power lost, software update it automaticaly.
Oled screen & RTC use the same SDA and SCL connection to DIGISPARK.
RTC DS3231 allow to have Temp information.

NOTE: Software is release, test is done.
But mounting test awaiting parts delivery.

I have put an relay to avoid battery consuming to maintain the servomotor position.
Becarefull Servomotor have only 180° angle, i have create a 3D print wheel to open the door.
