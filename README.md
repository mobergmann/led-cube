# led-cube

## Dependencies
Diese Programme müssen installiert sein um das Projekt zu kompilieren.
- libgpiod-dev
- cmake
- g++


## Pins
### User Pins
Eine Liste von Pins, die dem Benutzer zur Verfügung stehen.
Die Reihenfolge dieser Liste stellt die verfügbaren Pins von oben nach unten da.
- 8
- 7
- 2 (funktioniert nicht)
- 3 (funktioniert nicht)
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
| Datain           | 12  |         | Verbaut   |
| Special Pin      | 13  |         | Verbaut   |
| Shift Clock      | 14  | lila    | Verbaut   |
| Store Clock      | 15  | grün    | Verbaut   |
| Reset Pin        | 18  |         | Verbaut   |
| Layer 1          | 20  |         | Verbaut   |
| Layer 2          | 21  |         | Verbaut   |
| Layer 3          | 23  |         | Verbaut   |
| Layer 4          | 24  |         | Verbaut   |
| Layer 5          | 25  |         | Verbaut   |
| Pairing LED      | 16  |         | User Pins |
| Bluetooth Button | 26  |         | User Pins |
| Next Button      | 19  |         | User Pins |
| Previous Button  | 6   |         | User Pins |
| Power Button     | 5   |         | User Pins |


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

Die LED an der Position `layers[1][2][3]` stellt somit die LED dar, die im ersten Layer (von unten nach oben), in der Zweiten Line (von vorne nach hinten) und an der dritten Stelle (von links nach rechts) ist.
