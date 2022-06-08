# import wiringpi
from gpiozero import LED

my_pin = 26
led = LED(my_pin)

mode = False


def toggle_pin():
    global led
    global mode

    if mode:
        led.on()
        print("Set to on")

    else:
        led.off()
        print("Set to off")

    mode = not mode
    
    print(mode)
    print()

def main():
    while True:
        input("Enter smth. ")
        toggle_pin()


if __name__ == '__main__':
    main()
