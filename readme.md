A Mega32u4 based SDVX/K-Shoot Mania Controller

Description:
SDVX/K-Shoot Mania program for the Sparkfun Pro Micro and official Arduino Micro and Leonardo microcontroller boards. Also compatible with other Atmel Mega32u4 based microcontroller boards if pins are remapped.

Program emulates DJDAO SDVX/K-Shoot Mania style input modes. Arcade buttons send keystrokes. Rotary encoders emulate X/Y axis mouse movement.

Required Software:  
Arduino IDE

Required Libraries:  
Keyboard  
Mouse

Features:  
7 Push button inputs  
2 Interrupt based full read resolution rotary encoders  
Serial debug mode  
Program selectable keybindings  
Program selectable controller board presets  

Limitatioons:  
Due to I/O constraints of the Pro Micro, all digital/analog pins are occupied. UART pins are also unavailable.  
Due to I/O constraints of the Arduino Micro, UART and I2C are unavailable.  

Features to be implemented:  
Software reversible X/Y axis movement  
Escape/Back button key or keycombo  

For latest revision of code: https://github.com/Reyn-Mukai/SDVX

Required Parts:  
1x Mega32u4 based microcontroller board (4 interrupts minimum)  
4x 60mm x 60mm Square illuminated arcade buttons  
1x 33mm x 33mm Square illuminated arcade button  
2x 50mm x 33mm Rectangular illuminated arcade buttons  
2x 24PPR Rotary encoder (No detents preferred)  
2x 30mm Diameter knobs  
