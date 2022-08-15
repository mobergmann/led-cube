# led-cube

## Dependencies
Diese Programme müssen installiert sein um das Projekt zu kompilieren.
- libgpiod-dev
- cmake
- g++


## Pins
### User Pins
Eine Liste von Pins, die dem Benutzer zur Verfügung stehen.
- 8
- 7
- 2
- 3
- 4
- 11
- 5
- 6
- 19
- 26
- 16

### Belegung
| Bezeichnung      | Pin | Notizen | Ort       |
|------------------|-----|---------|-----------|
| Data_In          | 12  |         | Verbaut   |
| Pin_25           | 13  |         | Verbaut   |
| Shift_Clock      | 14  | lila    | Verbaut   |
| Store_Clock      | 15  | grün    | Verbaut   |
| Reset_Pin        | 18  |         | Verbaut   |
| Layer_1          | 20  |         | Verbaut   |
| Layer_2          | 21  |         | Verbaut   |
| Layer_3          | 23  |         | Verbaut   |
| Layer_4          | 24  |         | Verbaut   |
| Layer_5          | 25  |         | Verbaut   |
| pairing led      |     |         | User Pins |
| bluetooth button |     |         | User Pins |
| next button      |     |         | User Pins |
| previous button  |     |         | User Pins |
| power button     |     |         | User Pins |


## Programm
### Dateiformat
Die Programmeingaben sind im [JSON](https://www.json.org/json-de.html) Format.  
Es wird jeder Frame mit der anzuzeigenden Zeit angegeben.  
Die Daten werden als 3D-Array angegeben.  

Eine Programmeingabe, die den Cube eine Sekunde voll aufleuchten lässt und danach eine Sekunde dunkel bleibt, kann z.B. wie folgt aussehen:
```json
{
  "frames": [
    {
      "frame-time": 1000,
      "layers": [
        [
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true]
        ],
        [
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true]
        ],
        [
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true]
        ],
        [
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true]
        ],
        [
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true],
          [true, true, true, true, true]
        ]
      ]
    },
    {
      "frame-time": 1000,
      "layers": [
        [
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false]
        ],
        [
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false]
        ],
        [
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false]
        ],
        [
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false]
        ],
        [
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false],
          [false, false, false, false, false]
        ]
      ]
    }
  ]
}
```

In der Eingabe muss es ein Attribut geben vom Typen Array, welches den namen `frames` hat. 
Dies hält eine Menge von Frame Objekten.

Jedes Frame Objekt hat ein Integer Attribut `frame-time`. Dieses gibt in millisekunden an, wie lange der Frame sichtbar sein soll.

In dem Attribut `layers` ist ein 3D Array, von der Größe 5x5x5.  
Die erste Dimension stellt die Layer dar (die Schichten von unten nach oben).
Jeder Layer, somit die zweite Dimension, besteht aus 5 sogenannten Lines.
Jede Line besteht wiederrum aus 5 boolean Werten. Diese kodieren, ob eine LED an, oder aus sein soll.

Die LED an der Position layers[1][2][3] stellt somit die LED dar, die im ersten Layer (von unten nach oben), in der Zweiten Line (von vorne nach hinten) und an der dritten Stelle (von links nach rechts) ist.
