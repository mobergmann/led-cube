# led-cube

## Build
### Dependencies
Diese Programme müssen installiert sein um das Projekt zu kompilieren.
- build-essential 
- cmake 
- libgpiod-dev

### Build
Führe das `configure` skript mit root rechten aus. Dies übernimmt die kompilierung (für debian/ raspi Systeme) und die installation.
```
./configure
```

### Pack
Soll das Projekt durch einen Installer/ Packet installiert werden, dann geht dies mit CPack:
```
cpack -C "Release" -G "STGZ;TGZ"
```

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
| Bezeichnung      | Pin | Power-on Pull | Ort       |
|------------------|-----|---------------|-----------|
| Datain           | 12  | Pull Down     | Verbaut   |
| Special Pin      | 13  | Pull Down     | Verbaut   |
| Shift Clock      | 14  | Pull Down     | Verbaut   |
| Store Clock      | 15  | Pull Down     | Verbaut   |
| Reset Pin        | 18  | Pull Down     | Verbaut   |
| Layer 1          | 20  | Pull Down     | Verbaut   |
| Layer 2          | 21  | Pull Down     | Verbaut   |
| Layer 3          | 23  | Pull Down     | Verbaut   |
| Layer 4          | 24  | Pull Down     | Verbaut   |
| Layer 5          | 25  | Pull Down     | Verbaut   |
| Pairing LED      | 11  | Pull Down     | User Pins |
| Bluetooth Button | 6   | Pull Up       | User Pins |
| Next Button      | 4   | Pull Up       | User Pins |
| Previous Button  | 5   | Pull Up       | User Pins |
| Power Button     | 7   | Pull Up       | User Pins |


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

### Programme laden
Die Programme (die `json` Dateien), die ein Benutzer im Cube anzeigen lassen möchte, müssen in dem Ordner `~/.led-cube/custom` vorhanden sein.
