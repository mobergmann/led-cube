
import controlP5.*;

// UI
ControlP5 cp5;

ScrollableList frameList;
Button newFrameButton;
Button deleteFrameButton;
Button saveFileButton;
Button openFileButton;
Button playButton;
Textfield frameTimeTf;
Textfield fileNameTf;
Button pushX;
Button popX;
Button pushY;
Button popY;
Button pushZ;
Button popZ;
Button copy;
Button paste;

// play sequence
boolean playSequence = false;
int time = millis();

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
Cube copyFrame = new Cube();

void setup() {
  // meta setup
  size(1250, 780, P3D);
  surface.setResizable(true);  
  
  UI();
  
  // initial frame
  frames.add(new Cube());
  selectedFrame = 0;
  
  updateFrames();
}

void draw() {
  background(50);
  lights();
  
  // cube
  pushMatrix();
  
  translate(width*3/5, height/2-70,0);
  rotateX(xpos+1.56-0.3);
  rotateY(ypos+0.64-PI/5);
  rotateZ(zpos);
  
  frames.get(selectedFrame).update();
  
  popMatrix();
  
  // making toggle cube ## experimental
  //pushMatrix();
  
  //translate(width-60, height-60, 0);
  //rotateX(xpos+1.56-0.3);
  //rotateY(ypos+0.64-PI/5);
  //rotateZ(zpos);
  
  //fill(255, 255, 255, 150);
  //box(40);
  //translate(-20, -20, 0);
  //fill(255, 0, 0);
  //box(10, 10, 40);
  //translate(0, 0, -20);
  //rotateX(PI/2);
  //box(15, 15, 15);
  
  //popMatrix();
  
  // cube rotation
  frames.get(selectedFrame).cubeMoving(false);
  
  if (mousePressed && !frames.get(selectedFrame).LedSelected()) { 
    frames.get(selectedFrame).cubeMoving(true);
    
    float orbitRadius= (last_mouseX-mouseX)/2;
    xpos += (float(last_mouseY-mouseY)/3)/100;
    zpos += (cos(radians(0))*orbitRadius)/100;
    ypos = (sin(radians(0))*orbitRadius)/100;
  }
  
  last_mouseX = mouseX;
  last_mouseY = mouseY;
  
  // handle play button
  try {
    if (playSequence) {
      if (millis()-time >= int(frameTimeTf.getText())) {
        selectedFrame++;
        selectedFrame = selectedFrame % frames.size();
        time = millis();
        updateFrames();
      }
    }
  }
  catch(Exception e) {}
  
  
  frameList.setSize(150, height-5);
  fileNameTf.setPosition(5, height-55);
  saveFileButton.setPosition(5, height-105);
  openFileButton.setPosition(5, height-155);
}

// UI
void UI() {
  cp5 = new ControlP5(this);
  var arial = createFont("arial",20);
  
  // ListVIew for the frame
  
  frameList = cp5.addScrollableList("Layers")
     .setPosition(165, 5)
     .setSize(150, height-5)
     .setBarHeight(40)
     .setItemHeight(30)
     .onRelease(new CallbackListener() {
  public void controlEvent(CallbackEvent theEvent) {
    frameList.setOpen(true);
    
    if (playSequence)
      return;
    
    selectedFrame = (int)frameList.getValue();
    }
  });
  
  // new frame button
  newFrameButton = cp5.addButton("New Frame")
     .setValue(0)
     .setPosition(5, 5)
     .setSize(150,40)
     .onRelease(new CallbackListener() {
  void controlEvent(CallbackEvent theEvent) {
    
      if (playSequence)
        return;
        
      selectedFrame++;
      frames.add(selectedFrame, new Cube());
      updateFrames();
    }
  });
  
  // delete frame button
  deleteFrameButton = cp5.addButton("Delete Frame")
     .setValue(0)
     .setPosition(5, 105)
     .setSize(150,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      
      if (playSequence)
        return;
        
      frames.remove(frames.get(selectedFrame));
      
      if (selectedFrame >= frames.size()) {
        selectedFrame = frames.size()-1;
      }
      if (frames.size() == 0) {
        frames.add(new Cube());
        selectedFrame = 0;
      }
      updateFrames();
    }
  });
     
  // copy frame button
  copy = cp5.addButton("Copy Frame")
     .setValue(0)
     .setPosition(5, 55)
     .setSize(70,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      if (playSequence)
        return;
        
      copyFrame = new Cube(frames.get(selectedFrame));
    }
  });
     
  // paste frame button
  paste = cp5.addButton("Paste Frame")
     .setValue(0)
     .setPosition(85, 55)
     .setSize(70,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      if (playSequence)
        return;
        
      selectedFrame++;
      frames.add(selectedFrame, new Cube(copyFrame));
      updateFrames();
    }
  });
     
  // place random button
  paste = cp5.addButton("Place RND")
     .setValue(0)
     .setPosition(5, 360)
     .setSize(70,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      if (playSequence)
        return;
        
      frames.get(selectedFrame).placeRnd();
    }
  });
     
  // remove random button
  paste = cp5.addButton("Remove RND")
     .setValue(0)
     .setPosition(85, 360)
     .setSize(70,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      if (playSequence)
        return;
        
      frames.get(selectedFrame).removeRnd();
    }
  });
  
  // Magic button
  paste = cp5.addButton("Magic Button")
     .setValue(0)
     .setPosition(5, 410)
     .setSize(150,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      if (playSequence)
        return;
        
      // magic code here
      
      
    }
  });
     
  // set frametime
  frameTimeTf = cp5.addTextfield("Frame Time (ms)")
     .setPosition(5, 155)
     .setSize(150,40)
     .setFont(arial)
     .setColor(color(255,255,255))
     .setValue("1000")
     ;
     
   // play sequence button
   playButton = cp5.addButton("Play")
     .setValue(0)
     .setPosition(5, 210)
     .setSize(150,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      
      if (playSequence) {
        playSequence = false;
        playButton.setLabel("Play");
      }
      else {
        playSequence = true;
        playButton.setLabel("Pause");
        time = millis();
      }
    }
  });
  
  // shift cube buttons
   pushX = cp5.addButton("+X")
     .setValue(0)
     .setPosition(5, 260)
     .setSize(45,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      frames.get(selectedFrame).shift(true, "x");
    }
  });
  
   popX = cp5.addButton("-X")
     .setValue(0)
     .setPosition(5, 310)
     .setSize(45,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      frames.get(selectedFrame).shift(false, "x");
      
    }
  });
  
   pushY = cp5.addButton("+Y")
     .setValue(0)
     .setPosition(55, 260)
     .setSize(45,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      frames.get(selectedFrame).shift(true, "y");
    }
  });
  
   popY = cp5.addButton("-Y")
     .setValue(0)
     .setPosition(55, 310)
     .setSize(45,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      frames.get(selectedFrame).shift(false, "y");
      
    }
  });
  
   pushZ = cp5.addButton("+Z")
     .setValue(0)
     .setPosition(105, 260)
     .setSize(45,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      frames.get(selectedFrame).shift(true, "z");
    }
  });
  
   popZ = cp5.addButton("-Z")
     .setValue(0)
     .setPosition(105, 310)
     .setSize(45,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      frames.get(selectedFrame).shift(false, "z");
      
    }
  });
  
  
     
   // set file name
  fileNameTf = cp5.addTextfield("File Name")
     .setPosition(5, height-55)
     .setSize(150,40)
     .setFont(arial)
     .setColor(color(255,255,255))
     ;
     
  // save file button
  saveFileButton = cp5.addButton("Save File")
     .setValue(0)
     .setPosition(5, height-105)
     .setSize(150,40)
     //.setColorBackground(color(255, 255, 255))
     ////.setColorForeground(color(0, 0, 0))
     //.setColorActive(color(10, 10, 10))
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      
      selectFolder("Select a folder to process:", "saveJSONFile");
    }
  });
     
  // open file button
  openFileButton = cp5.addButton("Open File")
     .setValue(0)
     .setPosition(5, height-155)
     .setSize(150,40)
     .onRelease(new CallbackListener() {
    void controlEvent(CallbackEvent theEvent) {
      
      if (playSequence){
      
        return;
      }
      
      selectInput("Select a file to process:", "openJSONFile");
      
    }
  });
}

// updates the UI frame list
void updateFrames() {
  frameList.clear();
  int i = 1;
  ArrayList new_frames = new ArrayList();
  for (Cube frame : frames) {
    new_frames.add("Frame: " + str(i));
    i++;
  }
  frameList.addItems(new_frames);
  frameList.setLabel("Frame: " + str(selectedFrame+1));
}

void saveJSONFile(File file) {
  
  if (file == null) {
    println("Window was closed or the user hit cancel.");
    return;
  }
  
  if (fileNameTf.getText().length() == 0) {
    println("Empty Filename.");
    return;
  }
  
  try {
    
    JSONObject json = new JSONObject();
    
    JSONArray values = new JSONArray();
    
    int i = 0;
    for (Cube frame : frames) {
  
      JSONObject _frame = new JSONObject();
      
      _frame.setInt("frame-time", int(frameTimeTf.getText()));
      
      _frame.setJSONArray("layers", frame.toJSON());
  
      values.setJSONObject(i++, _frame);
    }
    
    json.setJSONArray("frames", values);
  
    saveJSONObject(json, file.getAbsolutePath()+"/"+fileNameTf.getText()+".json");
    
  } catch(Exception e) {
    println("file save exception");
  }
}

void openJSONFile(File file) {
  
  if (file == null) {
    println("Window was closed or the user hit cancel.");
    return;
  }
  
  try {
    
    JSONObject json = loadJSONObject(file);
    
    JSONArray _frames = json.getJSONArray("frames");
    
    frames.clear();
    selectedFrame = 0;
    
    for (int i = 0; i < _frames.size(); i++) {
      
      JSONObject _frame = _frames.getJSONObject(i);
      
      frameTimeTf.setText(str(_frame.getInt("frame-time")));
      
      JSONArray layers = _frame.getJSONArray("layers");
      
      frames.add(new Cube(layers));
      
    }
    
    String[] path = file.getAbsolutePath().split("/");
    String name = path[path.length-1];
    
    fileNameTf.setText(name.substring(0, name.length()-5));
    
    updateFrames();
    
    
  } catch(Exception e) {
    println("file load exception " + e);
  }
}
