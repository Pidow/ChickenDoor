# ChickenDoor
![SCREENLOGO](https://github.com/Pidow/ChickenDoor/blob/master/2.Rebuild%20with%20DIGISPARK%20PRO/1.Assy%20view/1.Setup.png?raw=true)
Two version actualy existing (see info file in each folder)

TARGET:
Move the chicken house doors 2 times per day.
Estimate battery lifetime: 8 month with AAA battery(x8).


## FIRST VERSION cost: less 15€
![FIRSTVERSION](https://github.com/Pidow/ChickenDoor/blob/master/1.first%20WITH%20DIGISPARK/1.Assy%20view/REAL%20VIEW.png?raw=true)
*   ATTINY85 Digispark
*   RTC DS1307 (Real Time clock)
*   Relay for Servomotor
*   Servomotor MG995
*   Battery holder 4 battery 1,5 v (x2)
*   LED (to have some information

## SECOND VERSION cost: less 30€
![SECOND VERSION](https://github.com/Pidow/ChickenDoor/blob/master/2.Rebuild%20with%20DIGISPARK%20PRO/1.Assy%20view/0.Definition.jpg?raw=true)
*   ATTiny167 Digispark Pro
*   RTC DS3231 (Real Time clock)
*   Relay for Servomotor
*   Servomotor MG995
*   Battery holder 4 battery 1,5 v (x2)
*   Oled screen 128x64 I2C SPI serial.

```
How it's work:
Push software to Digispark, connect all component following instructions.
The system is ready !

Note:
If the RTC have power lost, software update it time automaticaly.
Oled screen & RTC use the same SDA and SCL connection to DIGISPARK.
RTC DS3231 allow to have Temp information.
I have put an relay to avoid battery consuming to maintain the servomotor position.
FYI Servomotor have only 180° angle, i have create a 3D print wheel to open the door.
```

> I'm available if any question.
> 
> Have fun !
> 
>    ARNAUD TOCABENS.
