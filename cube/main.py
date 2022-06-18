from ursina import *
from cube import Cube

window.title = "3D LED CUBE"
app = Ursina(borderless=False)

frames = [Cube()]

selected = 0

model_names = ['Frame_0']

def select_frame(frame):
    print(frame)

model_dict = {name : Func(select_frame, name) for name in model_names}

def add_frame():
    global selected
    global frames
    global model_dict
    print(model_dict)
    
    frames.append(frames[selected])
    selected = len(model_dict)
    model_dict[f'Frame_{len(model_dict)}'] = select_frame

    bl = ButtonList(model_dict, font='VeraMono.ttf', width=.15, position = (.8, .34))

def delete_frame():
    pass

def update():
    window.size = (1250,750)
    frames[selected].update()

b = Button(text='New Frame', color=color.azure, scale_x=.2, scale_y=.05, text_origin=(0,0), position = (.9, .43))
b.on_click = add_frame # assign a function to the button.

b = Button(text='Delete Frame', color=color.red, scale_x=.2, scale_y=.05, text_origin=(0,0), position = (.9, .37))
b.on_click = delete_frame # assign a function to the button.

bl = ButtonList(model_dict, font='VeraMono.ttf', width=.15, position = (.8, .34))

app.run()