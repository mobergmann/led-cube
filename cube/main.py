import json
from ursina import *
from cube import Cube
import os
import numpy as np


window.title = "3D LED CUBE"
app = Ursina(borderless=False)


cube = Cube()
frames = [cube.leds]

selected = 0

def select_frame(frame):
    print(frame)

model_dict = {name : Func(select_frame, name) for name in ['Frame_0']}

bl = None

def add_frame():
    global frames
    global selected

    a = list(frame_selection.options)
    a.append(f"Frame {len(a)}")

    if len(a) > 20:
        frame_selection.scripts[0].min += 0.05
        frame_selection.scripts[0].max -= 0.05

    n = list()

    for i in frames[-1]:
        if i == 1:
            n.append(0.5)
        else:
            n.append(0)

    cube.leds = n
    selected = len(frames)
    frames.append(n)
    frame_selection.options = tuple(a)

def delete_frame():
    pass

def save_frames():
    global frames
    global deltaTime

    path = f"../files/{fileName.text}.json"

    json_frames = list()
    json_frame = {}


    for key in list(frame_selection.options):
        for value in frames:
            if (deltaTime.text == "Delta Time" or deltaTime.text == ''):
                json_frame["frame-time"] = 1
            else:
                json_frame["frame-time"] = float(deltaTime.text)

            tmp_list = list()
            tmp = np.array_split(value, 5)

            for i in tmp:

                sub = []

                for j in i:
                    sub.append(bool(j == 1))

                tmp_list.append(sub)

            json_frame["data"] = tmp_list

        json_frames.append(json_frame)

    meta = {"frames": json_frames}

    with open(path, 'w') as f:
        f.write(json.dumps(meta, indent=4,))


def update():
    window.size = (1250,750)

    cube.update()
    frames[selected] = cube.leds

frame_selection = ButtonGroup(("Frame 0",), position=(.88, .454))
frame_selection.add_script(Scrollable(target_value=0, min=-.05, max=.05)) # TODO ???

def on_value_changed():
    global selected
    selected = int(frame_selection.value[6:])

    cube.apply(frames[selected])

frame_selection.on_value_changed = on_value_changed

b = Button(text='New Frame', color=color.azure, scale_x=.2, scale_y=.05, text_origin=(0,0), position = (.75, .43))
b.on_click = add_frame # assign a function to the button.

b = Button(text='Delete Frame', color=color.red, scale_x=.2, scale_y=.05, text_origin=(0,0), position = (.75, .37))
b.on_click = delete_frame # assign a function to the button.

b = Button(text='Save', color=color.blue, scale_x=.2, scale_y=.05, text_origin=(0,0), position = (.75, .17))
b.on_click = save_frames # assign a function to the button.

deltaTime = InputField(default_value="Delta Time", position=(.57, .3), limit_content_to='0123456789.')
fileName = InputField(default_value="File Name", position=(.57, .24))

window.color = color._32
app.run()