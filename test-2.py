from gpiozero import LED
import time

MAX_PINS = 25

pin_datain = LED(5)
pin_shift_clock = LED(17) # lila
pin_store_clock = LED(27) # grün
single_pin = LED(22) # 25er pin
# pin_reset = LED(5)

count = 0

def reset():
    # global pin_reset
    # pin_reset.off()
    # pin_reset.on()

    for i in range(MAX_PINS):
        pin_datain.off()
        pin_shift_clock.on()
        pin_shift_clock.off()
    pin_store_clock.on()
    pin_store_clock.off()

    single_pin.off()

reset()


single_pin.on()
time.sleep(.25)
# single_pin.off()




pin_datain.on()
pin_shift_clock.on()
pin_shift_clock.off()

pin_store_clock.on()
pin_store_clock.off()
pin_datain.off()

for i in range(MAX_PINS):
    pin_datain.on()

    # input("shift")
    pin_shift_clock.on()
    pin_shift_clock.off()

    # input("store")
    pin_store_clock.on()
    pin_store_clock.off()
    
    time.sleep(.25)


# while True:    
#     input("shift")
#     pin_shift_clock.on()
#     pin_shift_clock.off()
# 
#     input("store")
#     pin_store_clock.on()
#     pin_store_clock.off()
