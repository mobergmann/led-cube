from gpiozero import LED

pin = LED(14)

pin.off()

pin.on()
input()
pin.off()
input()
# pins = []

# # init all off
# for i in range(2, 26):
#         pin = LED(i)
#         pin.off()

#         pins.append(pin)


# mode = input("Mode (1|2): ")
# if mode == "1":
#     while True:
#         p = int(input("pin: "))
#         pin = pins[p-2]

#         # pin.off()
#         input("Turn ON:")
#         pin.on()
#         input("Next Pin")
#         pin.off()
#         print()
# elif mode == "2":
#     for i in range(40):
#         pin = LED(i)

#         pin.off()
#         input("Turn ON:")
#         pin.on()
#         input("Next Pin")
#         pin.off()
#         print()
# else:
#     print("Choose 1 or 2")