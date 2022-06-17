from gpiozero import LED
import time

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


# def apply(sequence: list[str]):
#     for i in sequence:
#         if i == "0":
#             pin_datain.off()
#             shift()
#         elif i == "1":
#             pin_datain.on()
#             shift()
#     store()

def iterate():
    WAITING_TIME = 0.25
    special.on()
    time.sleep(WAITING_TIME)
    special.off()
    
    
    for i in range(0, NUM_PINS):
        # init
        pin_datain.on()
        shift()
        pin_datain.off()
        store()
        time.sleep(0.25)


if __name__ == "__main__":
    while True:
        iterate()
