import numpy as np
import math
from ursina import *

class Cube:
    
    def __init__(self):

        self._leds = list()
        self.leds = list()

        # mouse pressed states
        self._mouse_pressed = False
        self.mouse_last_pos = Vec3(0, 0, 0)
        self.led_clicked = False

        for i in range(125):
            self.leds.append(0)

        camera.x = 4
        camera.y = -1.5
        camera.z = -35

        self.wires = list()

        self.bottom = Entity(model=Mesh(vertices = [
            (4.5, -4.5, 4.5),
            (4.5, -4.5, -4.5),
            (-4.5, -4.5, -4.5),
            (-4.5, -4.5, 4.5),
            (4.5, -4.5, 4.5),
            ], mode='line', colors=(color.gray, color.gray, color.gray, color.gray, color.gray)))

        # create spheres
        for x in range(-2, 3):
            for y in range(-2, 3):
                for z in range(-2, 3):

                    led = Entity(
                        model="sphere", 
                        color=color.black50, 
                        scale=.5, 
                        position=(x*2, y*2, z*2))

                    self._leds.append(led)


        for x in range(-2, 3):
            for y in range(-2, 3):

                pos1 = (x*2, y*2, -2*2)
                pos2 = (x*2, y*2, 2*2)

                e = Entity(model=Mesh(vertices = [pos1, pos2], mode='line', colors=(color.green, color.green)))

                self.wires.append(e)

                pos1 = (-2*2, x*2, y*2)
                pos2 = (2*2, x*2, y*2)

                e = Entity(model=Mesh(vertices = [pos1, pos2], mode='line', colors=(color.green, color.green)))

                self.wires.append(e)

                pos1 = (x*2, -2*2, y*2)
                pos2 = (x*2, 2*2, y*2)

                e = Entity(model=Mesh(vertices = [pos1, pos2], mode='line', colors=(color.green, color.green)))

                self.wires.append(e)
        
        self._leds[0].color = color.yellow
        self._leds[-1].color = color.orange

    def update(self):

        # led clicked
        if self.mouse_pressed():

            self.mouse_last_pos = mouse.position

            for i, led in enumerate(self._leds):

                if distance(mouse.position, led.screen_position) < 0.02:
                    self.toggle_led(led)
                    self.leds[i] = -self.leds[i] + 1
                    self.led_clicked = True
        
        if mouse.left and self.led_clicked == False:

            self.rotate_cube()


        if mouse.left == False:
            self.led_clicked = False

    def apply(self, leds):
        self.leds = leds

        for i, led in enumerate(self._leds):
            if leds[i] == 0:
                led.color = color.black50
            else:
                led.color = color.blue

    def invisible(self):
        
        for i in self.wires:
            i.enabled = False

        for i in self._leds:
            i.enabled = False

        self.bottom.enabled = False


    def visible(self):

        for i in self.wires:
            i.enabled = True

        for i in self._leds:
            i.enabled = True

        self.bottom.enabled = True


    def rotate_cube(self):

        # region rotations matrix
        vec = self.mouse_last_pos - mouse.position
        
        self.mouse_last_pos = mouse.position

        drag_vec = np.array(list(vec))

        angleX = -drag_vec[1]
        angleY = drag_vec[0]
        # angleZ = math.acos(drag_vec.dot(self.last_drag)/self.abs(drag_vec) * self.abs(self.last_drag))/1000
        
        rotX = np.array([
            [1, 0, 0],
            [0, math.cos(angleX), -math.sin(angleX)],
            [0, math.sin(angleX), math.cos(angleX)]
            ])

        rotY = np.array([
            [math.cos(angleY), 0, math.sin(angleY)],
            [0, 1, 0],
            [-math.sin(angleY), 0, math.cos(angleY)]
            ])

        # rotZ = np.array([
        #     [math.cos(angleZ), -math.sin(angleZ), 0],
        #     [math.sin(angleZ), math.cos(angleZ), 0],
        #     [0, 0, 1]
        #     ])
        
        rot = rotX.dot(rotY)

        # endregion

        for led in self._leds:

            new_pos = rot.dot(np.array(list(led.position)))

            led.x = new_pos[0]
            led.y = new_pos[1]
            led.z = new_pos[2]

        vertices = []

        for vertice in self.bottom.model.vertices:

            new_pos = rot.dot(np.array(list(vertice)))

            vertices.append(new_pos)

        self.bottom.model = Mesh(vertices = vertices, mode='line', colors=(color.gray, color.gray, color.gray, color.gray, color.gray))

        for wire in self.wires:

            new_pos1 = rot.dot(np.array(list(wire.model.vertices[0])))
            new_pos2 = rot.dot(np.array(list(wire.model.vertices[1])))

            wire.model = Mesh(vertices = [
                Vec3(new_pos1[0], new_pos1[1], new_pos1[2]), 
                Vec3(new_pos2[0], new_pos2[1], new_pos2[2])
                ], mode='line', colors=(color.gray, color.gray))

                
    def abs(self, np_array):
        return math.sqrt(math.pow(np_array[0], 2) + math.pow(np_array[1], 2) + math.pow(np_array[2], 2))

    def mouse_pressed(self):
        if mouse.left and not self._mouse_pressed:
            self._mouse_pressed = True
            return True

        if not mouse.left:
            self._mouse_pressed = False

        return False

    def toggle_led(self, led):
        
        if led.color == color.black50:
            led.color = color.blue
        else:
            led.color = color.black50
