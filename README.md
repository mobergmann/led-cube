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

Eine Programmeingabe, die den Cube eine sekunde voll aufleuchten lässt und danach eine Sekunde dunkel bleibt, z.B. wie folgt aussehen:
```json

```