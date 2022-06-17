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
reset_pin2 = LED(25)
reset_pin3 = LED(8)
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
        pin_datain.on()
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
    # layer_1.on()
    # for i in range(NUM_PINS):
    #     pin_datain.on()
    #     shift()
    #     store()

    # special.on()
    
    # input()

    # reset_pin.off()
    # store()
    # reset_pin.on()

    # input()
    # reset_pin2.off()
    # store()
    # reset_pin2.on()

    # input()
    # reset_pin3.off()
    # store()
    # reset_pin3.on()

    # input()

    # reset()
    special.on()
    reset_pin.on()
    layer_1.on()

    sequence = input("enter sequence: (format: [01]*): ")
    apply(sequence)
    reset_pin.on()
    input()
    reset_pin.off()
    store()
    input()
    layer_1.on()


    reset_pin.on()
    reset()
    sequence = input("enter sequence: (format: [01]*): ")
    apply(sequence)
    reset_pin.on()
    input()
    reset_pin.off()
    store()
    input()
    # layer

    pin_datain.on()
    shift()
    shift()
    shift()
    shift()
    pin_datain.off()
    store()


    reset_pin.off()
    while True:
        layer_2.off()
        layer_1.on()
        time.sleep(0.5)
        layer_1.off()
        time.sleep(0.5)
        layer_2.on()
        time.sleep(0.5)
        # layer_2.off()
        # time.sleep(0.02)

        # # check input valid
        # if not re.match("[01]{1,"+str(NUM_PINS)+"}", sequence):
        #     print("Wrong format")
        #     continue
        
        

