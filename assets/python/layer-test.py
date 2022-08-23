from gpiozero import LED
import time


pin_datain = LED(12)
pin_25 = LED(13)

pin_shift_clock = LED(14) # lila
pin_store_clock = LED(15) # grün

pin_reset = LED(18)

layers = [
    LED(20),
    LED(21),
    LED(23),
    LED(24),
    LED(25)
]


def shift():
    global pin_shift_clock
    pin_shift_clock.on()
    pin_shift_clock.off()


def store():
    global pin_store_clock
    pin_store_clock.on()
    pin_store_clock.off()


def reset():
    pin_reset.off()
    pin_reset.on()
    pin_25.off()


if __name__ == "__main__":
    while True:
        for layer in layers:
            layer.on()
            for i in range(25):
                pin_datain.on()
                shift()
                pin_datain.off()
            input("Press enter to continue...")
            layer.off()
