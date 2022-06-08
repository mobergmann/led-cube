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

reset_pin.on()

input()