

import controlP5.*;

// UI
ControlP5 cp5;

ScrollableList list;

// rotation
float xpos= 0;
float ypos= 0;
float zpos= 0;

// mouse values for rotation
int last_mouseX = 0;
int last_mouseY = 0;

// frames
ArrayList<Cube> frames = new ArrayList<Cube>();
int selectedFrame;


void setup() {
  // meta setup
  size(1250, 780, P3D);
  surface.setResizable(true);  
  
  UI();
  
  // initial frame
  frames.add(new Cube());
  selectedFrame = 0;
}

void draw() {
  background(50);
  lights();
  
  // cube
  pushMatrix();
  
  translate(width*3/5, height/2-70,0);
  rotateX(xpos-0.3);
  rotateY(ypos-PI/5);
  rotateZ(zpos);
  
  frames.get(selectedFrame).update();
  
  popMatrix();
  
  // making toggle cube ## experimental
  pushMatrix();
  
  translate(width-60, height-60, 0);
  rotateX(xpos-0.3);
  rotateY(ypos-PI/5);
  rotateZ(zpos);
  
  fill(255, 255, 255, 150);
  box(40);
  translate(-20, -20, 0);
  fill(255, 0, 0);
  box(10, 10, 40);
  translate(0, 0, -20);
  rotateX(PI/2);
  box(15, 15, 15);
  
  popMatrix();
  
  // cube rotation
  frames.get(selectedFrame).cubeMoving(false);
  
  if (mousePressed && !frames.get(selectedFrame).LedSelected()) { 
    frames.get(selectedFrame).cubeMoving(true);
    
    float orbitRadius= (last_mouseX-mouseX)/2;
    xpos += (float(last_mouseY-mouseY)/3)/100;
    ypos += (cos(radians(0))*(-1)*orbitRadius)/100;
    zpos = (sin(radians(0))*orbitRadius)/100;
  }
  
  last_mouseX = mouseX;
  last_mouseY = mouseY;
}

// UI
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
