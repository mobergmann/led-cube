from gpiozero import LED
import time
import re

pin_datain = LED(5)
pin_shift_clock = LED(17) # lila
pin_store_clock = LED(27) # grün
layer_1 = LED(18)
layer_2 = LED(23)
reset_pin = LED(24)

special = LED(22)

layer_1.on()

special.on()

input()

special.off()

input()
