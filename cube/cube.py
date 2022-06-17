import numpy as np
import math
from ursina import *

class Cube:
    
    def __init__(self):

        self.leds = list()

        self._mouse_pressed = False

        self.mouse_last_pos = Vec3(0, 0, 0)


        self.bottom = Entity(model="cube", color=color.black90, scale_x = 8, scale_z = 8, scale_y = 0.1, position=(0, -4.2, 3))

        for x in range(-2, 3):
            for y in range(-2, 3):
                for z in range(5):

                    e = Entity(model="sphere", color=color.black50, scale=.4, position=(x*2, y*2, z*2))

                    self.leds.append(e)

    def update(self):


        if self.mouse_pressed():

            self.mouse_last_pos = mouse.position

            for led in self.leds:

                if distance(mouse.position, led.screen_position) < 0.02:
                    self.toggle_led(led)
        
        if (mouse.left):

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

            for led in self.leds:

                led.z -= 3

                new_pos = rot.dot(np.array(list(led.position)))

                led.x = new_pos[0]
                led.y = new_pos[1]
                led.z = new_pos[2] + 3
            
            self.bottom.z -= 3

            new_pos = rot.dot(np.array(list(self.bottom.position)))

            self.bottom.x = new_pos[0]
            self.bottom.y = new_pos[1]
            self.bottom.z = new_pos[2] + 3

            self.bottom.world_rotation_x = angleX
            self.bottom.world_rotation_y = angleY
            # self.bottom.rotation_Z += angleZ

                
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
