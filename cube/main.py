from ursina import *
from cube import Cube

app = Ursina()

scale = 2

cube = Cube()

def update():
    cube.update()


app.run()