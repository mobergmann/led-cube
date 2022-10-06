# Assemble


## Components

- Soldering iron and solder
- vice
- 1x PCB board (plan available in the appendix)
- 1x Raspberry Pi 2 or better
- 1x Micro SD card
- 125x LED 20mA, 3.3V
- 1x LED 20mA, 3.3V (different color)
- 5x rolls of wire 1.0mm
- 25x 100 Ohm resistors
- 5x 1k Ohm resistors
- 1x 10k Ohm resistors
- 3x 74HC595 shift register
- 3x matching sockets
- 3x Ceramic Disk Capacitor 0.1 microfarad
- 1x multilayer ceramic capacitor (MLCC) 220 nanofarad, 50 volts
- 1x Tantalum electrolytic capacitor 2.2 microfarad, 35 volts +
- 5x Transistor 2N2222A H331
- 5x 2N222A transistors
- 1x Female Pinheader 1x14
- 1x Male Pinheader 1x6
- 1x Female Pinheader 2x20
- 1x LF 33 CV voltage converter
- 4x Raspberry Pi spacer
- 3x Brass Spacer M2*20
- 4x Pushbuttons
- 1x plexiglass plate 30cm x 30cm
- 8x M2 screws and nuts
- ribbon cable
- (nail polish)


## Assembly

### PCB

The PCB was designed so that you can usually tell which components go where. 
Additional reference: The components come to the following places

1. 100 Ohm resistors
2. 74HC595 shift register and matching sockets
3. 2N222A transistors
4. 1k Ohm resistors
5. Ceramic Disk Capacitor 0.1 microfarad
6. 10k Ohm resistors
7. LF 33 CV voltage converter
8. multilayer ceramic capacitor (MLCC) 220 nanofarad, 50 volts
9. Tantalum electrolytic capacitor 2.2 microfarad, 35 volts +
10. Female Pinheader 2x20 and Raspberry Pi 2
11. Female Pinheader 1x14, Male Pinheader 1x6 for the buttons

### Wire Mesh

The wire mesh consists of 125 LEDs and a lot of wire. Each layer contains 25 LEDs connected with wire. We would recommend using a wood template or something similar. Sold 5 LEDs to a wire, make sure that you only connect the annodes OR cathodes.
Connect the LED-wires with 2 additional wires to form a square. Depending on the used LED, it might be necessary to cover the LEDs bottom with (nail) paint to avoid lower LEDs light shining through. Repeat this step 5 times.
Thread 30 wires through PCB and printed board and solder them to the PCB. Use spacers to ensure even spacing. Sold the remaining annode or cathode to the respective upwards pointing wire.
Each layer needs to be connected with it's corresponding layer wire in the back right. The most left to layer one, the most right wire to the last (upper) layer.
The image in the editor can be used as a reference.


### Buttons

3D print the case and screw the buttons and the "connection LED" to the intended places. Attach a ribbon cable to each button and one of the led wires.
Sold the male pinheader to the other ends of the ribbon cables. Use an additional wire to connect the remaining pins and connect an additional ribbon cable for ground.
Plug the male pinheader in the female pinheader as following:

- black button to GPIO 07
- green button to GPIO 04
- yellow button to GPIO 11
- LED to GPIO05
- blue button to GPIO 06
- ground wire to GND

### Last steps

Use the screws to assemble the case. Use the Plexiglas to seal the case as a see through bottom. The edge of the Plexiglas pane can be worn down so that the light of the Raspberry Pi does not show through.
