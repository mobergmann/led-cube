from gpiozero import LED
import time
import re

NUM_PINS = 24


pin_datain = LED(5)
pin_shift_clock = LED(17) # lila
pin_store_clock = LED(27) # grün


def shift():
    global pin_shift_clock
    pin_shift_clock.on()
    pin_shift_clock.off()


def store():
    global pin_store_clock
    pin_store_clock.on()
    pin_store_clock.off()


def reset():
    global NUM_PINS
    for i in range(NUM_PINS):
        pin_datain.off()
        shift()


def apply(sequence: list[str]):
    for i in sequence:
        if i == "0":
            pin_datain.off()
            shift()
        elif i == "1":
            pin_datain.on()
            shift()
    store()


if __name__ == "__main__":
    while True:
        sequence = input("enter sequence: (format: [01]*): ")
        
        # # check input valid
        # if not re.match("[01]{1,"+str(NUM_PINS)+"}", sequence):
        #     print("Wrong format")
        #     continue
        
        reset()
        
        apply(sequence)

