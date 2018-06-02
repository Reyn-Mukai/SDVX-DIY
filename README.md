# A DIY SDVX/K-Shoot Mania Controller w/RGB Underglow

This program was made for the Sound Voltex/K-Shoot Mania DIY guide at https://sdvx-diy.github.io/.

## Customizing the Code
Depending on the parts or configuration used in the construction and wiring of the controller, modifications to the code may have to be made. Instead of implementing each hardware pin, character and timer as a separate variable, these variables were implemented as arrays. All arrays have a standard order which is BT-A, BT-B, BT-C, BT-D, FxL, FxR and Start.

##### Software Debounce Interval
Software based debounce is implemented using timers. The default value is 10ms however you may need to increase this value if multiple keystrokes are registered per button press. This value can be changed by adjusting the following define:
```
/* Software Debounce Interval */
#define DEBOUNCE 10
```

##### Hardware Pin Assignments
Pins for the button inputs, LED outputs and encoder inputs are all declared using a global define. If you decide to change the ports, modify the following defines:
```
/* Button Input Pins */
#define BTA_PIN A2
#define BTB_PIN A3
#define BTC_PIN A4
#define BTD_PIN A5
#define FxL_PIN A0
#define FxR_PIN A1
#define START_PIN 7

/* LED Output Pins */
#define BTA_LED 9
#define BTB_LED 10
#define BTC_LED 11
#define BTD_LED 12
#define FxL_LED 5
#define FxR_LED 6
#define START_LED 13

/* Encoder */
#define ENC_1_PIN_A 0
#define ENC_1_PIN_B 1
#define ENC_2_PIN_A 2
#define ENC_2_PIN_B 3
```

##### Using Normally Open Limit switches
If you are using limit switches without normally closed contacts, change the bool values for the corresponding buttons to false:
```
bool switchType[7] = {true, true, true, true, true, true, true};
```

##### Changing The Keybindings
The button assignments for the controller are by default d, f, j, k, c, m and 1. If you would like to change these values, adjust the corresponding ascii values (note: ascii values are substituted with char at compile time):
```
char asciiKey[7] = {0x64, 0x66, 0x6A, 0x6B, 0x6D, 0x63, 0x31};
```

##### Credits
Code for the encoder library was sourced from this [repository](https://github.com/PaulStoffregen/Encoder).
