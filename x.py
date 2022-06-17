from gpiozero import LED
import time
import re

NUM_PINS = 24


pin_datain = LED(5)
pin_shift_clock = LED(17) # lila
pin_store_clock = LED(27) # grün
layer_1 = LED(18)
layer_2 = LED(23)
reset_pin = LED(24)
special = LED(22)


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


# for i in range(0, NUM_PINS):
#     pin_datain.on()
#     shift()
#     store()


# layer_1.on()
# special.on()

# store()
# reset()
time.
reset_pin.on()
input()
