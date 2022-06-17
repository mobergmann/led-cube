from gpiozero import LED
import time
import re

NUM_PINS = 24

pin_datain = LED(12)
pin_25 = LED(13)

pin_shift_clock = LED(14) # lila
pin_store_clock = LED(15) # grün

reset_pin = LED(18)

layer_1 = LED(20)
layer_2 = LED(21)
layer_3 = LED(23)
layer_4 = LED(24)
layer_5 = LED(25)



def shift():
    global pin_shift_clock
    pin_shift_clock.on()
    pin_shift_clock.off()


def store():
    global pin_store_clock
    pin_store_clock.on()
    pin_store_clock.off()


def reset():
    global reset_pin
    reset_pin.off()
    reset_pin.on()


def apply(sequence: list[str]):

    pin_25.off()
    for i, a in enumerate(sequence):
        if i == NUM_PINS:
            pin_25.on()
            break
        elif a == "0":
            pin_datain.off()
            shift()
        elif a == "1":
            pin_datain.on()
            shift()
    store()


if __name__ == "__main__":

    reset_pin.on()
    layer_1.on()

    while True:

        for i in range(33554432):

            reset()
            apply(sequence=format(i, 'b'))

            time.sleep(.25)

        print("finish") 


        
