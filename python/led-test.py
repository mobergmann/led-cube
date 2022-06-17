from gpiozero import LED
import time

pos = 0
# 0000000000000000

pin_datain = LED(2)
pin_shift_clock = LED(3)
pin_store_clock = LED(4)


def print_gpio_status():
    pass


def toggle_pins(binary_sequence):
    global pin_datain
    global pin_shift_clock
    global pin_store_clock

    for i in reversed(binary_sequence):
        pin_mode = int(i)
        if pin_mode == 1:
            pin_datain.on()
        else:
            pin_datain.off()

        pin_shift_clock.on()
        pin_shift_clock.off()

    pin_store_clock.on()
    pin_store_clock.off()


def main():
    global pos
    
    max = 8

    while True:
        # input_pins = input("Enter Pin Sequence: ")
        input_pins = (max-pos)*"0" + "1" + (pos-1)*"0"
        # input_pins = input_pins[::-1]
        print(input_pins)

        toggle_pins(input_pins)
        pos = (pos + 1) % max

        # time.sleep(0.5)
        input("next entry")

if __name__ == '__main__':
    main()
