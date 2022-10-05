# Main Program

## Dependencies
These dependencies must be installed to compile the project
- libgpiod-dev
- cmake
- g++

## Install
For an installation guide see the [documentation](../doc/README.md#compile--install).

## Pins
### User Pins
A list of pins available to the user.
The order of this list represents the available pins from top to bottom.
- 8
- 7
- 2 (does not work)
- 3 (does not work)
- 4
- 11
- 5
- 6
- 19
- 26
- 16

### Assignment
| Designation | Pin | Power-on Pull | Location |
|------------------|-----|---------------|-----------|
| Datain | 12 | Pull Down | Installed |
| Special Pin | 13 | Pull Down | Built-in |
| Shift Clock | 14 | Pull Down | Built-in |
| Store Clock | 15 | Pull Down | Built |
| Reset Pin | 18 | Pull Down | Built |
| Layer 1 | 20 | Pull Down | Built |
| Layer 2 | 21 | Pull Down | Built |
| Layer 3 | 23 | Pull Down | Built |
| Layer 4 | 24 | Pull Down | Built |
| Layer 5 | 25 | Pull Down | Built |
| Pairing LED | 11 | Pull Down | User Pins |
| Bluetooth Button | 6 | Pull Up | User Pins |
| Next Button | 4 | Pull Up | User Pins |
| Previous Button | 5 | Pull Up | User Pins |
| Power Button | 7 | Pull Up | User Pins |


## Program
### file format
The configurations are in [JSON](https://www.json.org/json-de.html) format.  
Each frame is specified with the time to be displayed.  
The data is specified as a 3D array.

For example, a configurations that causes the cube to light up fully for one second and then remain dark for one second might look like the following:
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

In the input there must be an attribute of type array, which has the name `frames`.
This holds a set of frame objects.

Each frame object has an integer attribute `frame-time`. This specifies in milliseconds how long the frame should be visible.

In the attribute `layers` is a 3D array, of size 5x5x5.  
The first dimension represents the layers (the layers from bottom to top).
Each layer, thus the second dimension, consists of 5 so-called lines.
Each line consists of 5 boolean values. 
These code whether an LED should be on or off.

The LED at the position 'layers[1][2][3]` thus represents the LED that is in the first layer (from bottom to top), in the second line (from front to back) and in the third position (from left to right).

### Loading configurations
The configuration (the `json` files) that a user wants to display in the cube must be present in the folder `~/.led-cube/custom`.
