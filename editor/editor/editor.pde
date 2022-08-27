

import controlP5.*;

ControlP5 cp5;

ScrollableList list;

int tx = 0;
int ty = 0;

float xpos= 0;
float ypos= 0;
float zpos= 0;

int last_mouseX = 0;
int last_mouseY = 0;

float i = 0;

ArrayList<Cube> frames = new ArrayList<Cube>();
int selected_frame;

int from_x, from_y = 0;
int to_x, to_y = 0;

boolean mouse_pressed = false;

void setup() {
  size(1250, 780, P3D);
  surface.setResizable(true);
  camera(0, 0, 0, width*3/5, height/2-30, 0.0, 
       0.0, 1.0, 0.0);
  
  
  //translateMore(width*3/5, height/2,0);
  
  frames.add(new Cube());
  selected_frame = 0;
  
  UI();
}

void draw() {
  tx = 0;
  ty = 0;
  background(50);
  lights();
  
  pushMatrix();
  
  translateMore(width*3/5, height/2-70,0);
  rotateX(xpos);
  rotateY(ypos);
  rotateZ(zpos);
  
  frames.get(selected_frame).update();
  
  
  translateMore(-width*3/5, -height/2-70,0);
  
  popMatrix();
  
  pushMatrix();
  
  
  
  translateMore(width-60, height-60, 0);
  rotateX(xpos);
  rotateY(ypos);
  rotateZ(zpos);
  
  // making toggle cube ## experimental
  pushMatrix();
  fill(255, 255, 255, 150);
  box(40);
  translateMore(-20, -20, 0);
  fill(255, 0, 0);
  box(10, 10, 40);
  translateMore(0, 0, -20);
  rotateX(PI/2);
  box(15, 15, 15);
  popMatrix();
  // end toggle cube
  
  popMatrix();
  
  //i += 0.01;
  
  line(from_x, from_y, to_x, to_y);
  
  
  //camera(xpos, ypos, zpos, width/2, height/2, 0, 0, -1, 0);
  
  //rotation++;
  
  // cube rotation
  if (mousePressed) { 
    float orbitRadius= (last_mouseX-mouseX)/2;
    xpos += (float(last_mouseY-mouseY)/3)/100;
    ypos += (cos(radians(0))*(-1)*orbitRadius)/100;
    zpos = (sin(radians(0))*orbitRadius)/100;
  }
  
  last_mouseX = mouseX;
  last_mouseY = mouseY;
}

void mousePressed(){
   if (mouse_pressed) {
     
   }
}

void UI() {
  cp5 = new ControlP5(this);
  var arial = createFont("arial",20);
  
  // ListVIew for the Frames
  ArrayList l = new ArrayList();
  
  for (int i = 0; i < 80; i++) {
    l.add(str(i));
  }
  
  list = cp5.addScrollableList("Layers")
     .setPosition(5, 5)
     .setSize(150, height-5)
     .setBarHeight(40)
     .setItemHeight(30)
     .addItems(l)
     ;
     
  list.onRelease(new CallbackListener() {
  public void controlEvent(CallbackEvent theEvent) {
    list.setOpen(true);
    }
  });
  
  // new Frame Button
  cp5.addButton("New Frame")
     .setValue(0)
     .setPosition(165, 5)
     .setSize(150,40)
     ;
     
  // delete Frame Button
  cp5.addButton("Delete Frame")
     .setValue(0)
     .setPosition(165, 55)
     .setSize(150,40)
     ;
     
  // set FrameTime
  cp5.addTextfield("Frame Time")
     .setPosition(165, 105)
     .setSize(150,40)
     .setFont(arial)
     .setColor(color(255,0,0))
     ;
     
   // set FrameTime
  cp5.addTextfield("File Name")
     .setPosition(165, height-155)
     .setSize(150,40)
     .setFont(arial)
     .setColor(color(255,0,0))
     ;
     
  // delete Frame Button
  cp5.addButton("Save File")
     .setValue(0)
     .setPosition(165, height-95)
     .setSize(150,40)
     //.setColorBackground(color(255, 255, 255))
     ////.setColorForeground(color(0, 0, 0))
     //.setColorActive(color(10, 10, 10))
     ;
     
  // delete Frame Button
  cp5.addButton("Open File")
     .setValue(0)
     .setPosition(165, height-45)
     .setSize(150,40)
     ;
     
   //cp5.addTextlabel("label")
   //                 .setText("A single ControlP5 textlabel, in yellow.")
   //                 .setPosition(100,50)
   //                 .setColorValue(0xffffff00)
   //                 .setFont(createFont("Georgia",20))
   //                 ;
                    
  
}

void updateFrames() {
  list.clear();
  int i = 0;
  ArrayList new_frames = new ArrayList();
  for (Cube frame : frames) {
    new_frames.add("Frame: " + str(i));
  }
  list.addItems(new_frames);
}

void translateMore(float x, float y, float z) {
  
  translate(x, y, z);
  
  tx += x;
  ty += y;

}
