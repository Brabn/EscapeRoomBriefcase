# EscapeRoomBriefcase
Briefcase with riddles to use in escape room amusement. 

According to the plot of the game, the briefcase is part of the puzzle and must be used in a chain of tasks to open the exit door.

The briefcase can be connected to a stationary connection console from which the door lock is unlocked.

All actions are accompanied by messages on numerical display, character LCD screen  and sound signals from a buzzer.

The control elements inside the briefcase are:

* Card reader for card reading
* Standard full-size computer keyboard for entering passwords and numeric codes
* 8-digit display for displaying digital codes.
* 32-character LCD screen for displaying password and explanations
* Buzzer for sound signals
* Electrical connector for connecting the diplomat to the remote control
  
On the stationary console there are:
* Two buttons for opening doors, covered with a transparent cover
* Electric lock that locks the transparent cover
* Electric lock that blocks the opening of the exit door
* Signal LED

## Operation logic
The system includes several “riddles” that must be solved sequentially:

1.	The briefcase must be found in a secret place inside the room
2.	The unlock card must be found in another secret location
3.	The suitcase must be connected to the stationary console with a cable, after which a sound signal will sound and the `Insert card` request will appear on the character screen.
4.	The player inserts a card	
    - Card inserted – The character screen displays `password:` and the cursor flashes. The numerical display displays a random set of numbers (changing every 0.3-0.5 seconds). The digital block does not respond to pressing;
    - Card removed – The symbol screen displays `Insert card`;
5.	The player begins to enter the code from the keyboard
    - Entering a symbol or number – displayed on the screen;
    - `del”, “backspace” – deleting the last character;
    - “esc” – deletes all entered characters;
    - “enter” – enter a password;
    - e. The rest of the buttons - nothing happens and a short buzzer;
6.	The player entered the password and pressed “enter”
    - Wrong password – long buzzer, “wrong password” flashes on the screen three times with a pause of 0.5 seconds. Then everything goes back to the beginning of entering the password;
    - The password is correct – “correct password” is displayed on the character screen for 10 seconds. The change of numbers on the numerical display stops - a certain sequence of 8 digits is displayed. The character screen displays “code decrypted” The LED next to the number pad begins to blink. The alphabetic keyboard stops responding to keystrokes;
7.	The player starts typing the numbers on the number pad
    - Each press is accompanied by a flash on the numeric display and a short buzzer tone.
8.	All 8 digits entered and presseed `#` (`ok` or as it will be signed as `ok` on the numeric keypad.
    - The code is incorrect 	– the buzzer is long, the numbers on the numerical scoreboard flash twice and light up again;
    - The code is correct – the numbers on the numerical board are extinguished. The			 `correct code` is displayed on the symbol screen. A signal is given to unlock the magnetic lock of the button cover on the main stationary console;
9.	The electric lock, which locks the transparent lid, is unlocked and two buttons become available
    - The player presses one of the buttons – nothing happens, a short buzzer;
    - The player presses two buttons at the same time – the lamp between the buttons lights up, the long buzzer comes on, the electric lock of the exit door is unlocked;

## System parameters:
* Main controller		– Arduino Uno 
* Processor 			– 16 MGh, ATmega328P
* Controller memory		– 32 KB Flash + 2 kB SRAM + 1kB EEPROM
* LCD screen			– LCD 1602
* Screen size			– 64.5x13.8 mm
* Character size			– 4.35x2.95 mm
* Pixel size			– 0.5x0.5 mm
* Seven-segment display	– 8-digit
* Dispay size			– 100x19mm
* Digit size			– 8x14.2mm
* Power supply 			– 12V 2A
* Briefcase dimensions		– 440x320x120 mm 

## Components
* Controller Arduino UNO 
* Full-size computer keyboard with PS2 connector
* Seven-segment numerical display (8-digit)
* 32-character LCD screen LCD1602 
* 5V buzzer
* Card reader 
* Standard buttons – 2pcs
* Signal led
* Electric lock – 2pcs

## Wiring diagram

![Escape Room Briefcase wiring diagram](https://github.com/Brabn/EscapeRoomBriefcase/blob/main/Wiring_diagram/EscapeRoomBriefcase.WiringDiagram.jpg)

## Further development of the system
- [ ] Improvements to puzzle plots; 
- [ ] Adding internal power supply and wireless connection;
- [ ] Adding speakers and audio feedback in the form of recorded audio messages;
- [ ] Adding a full TFT display to display graphics;


