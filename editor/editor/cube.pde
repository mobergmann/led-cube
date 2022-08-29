private enum LED {
  OFF,
  ON,
  SELECTED,
}
  

class Cube {
  
  LED[][][] layers = new LED[5][5][5];
  
  // variables for cube rotation
  boolean mousePress = false;
  boolean cubeMoving = false;
  
  // spacing between leds
  int spacing = 90;

  public Cube() {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 5; k++) {
          
          layers[i][j][k] = LED.OFF;
      
        }
      }
    }
  }
  
  // create from JSON
  public Cube(JSONArray json) {
    
    for (int i = 0; i < 5; i++) {
      JSONArray y = json.getJSONArray(i);
      for (int j = 0; j < 5; j++) {
        JSONArray x = y.getJSONArray(j);
        for (int k = 0; k < 5; k++) {
          
          if (x.getBoolean(k))
            layers[i][j][k] = LED.ON;
          else 
            layers[i][j][k] = LED.OFF;
            
      
        }
      }
    }
  }
  
  public void update() {
    
    // list of leds that are within the hitbox of the mouse
    ArrayList<PVector> possiblePressedLeds = new ArrayList<PVector>();
  
    for (int i = 0; i < 5 ; i++) { // layer
      
      for (int j = 0; j < 5; j++) { // y
      
        for (int k = 0; k < 5; k++) { // x
        
          pushMatrix();
        
          translate((k-2)*spacing, (j-2)*spacing, (i-2)*spacing);
        
          // check collision with mouse
          if (mouseCollision((k-2), (j-2), (i-2))) {
            
            possiblePressedLeds.add(new PVector(i, j, k));
            
          }
          popMatrix();
        }
      }
    }
    
    // handle led collisions
    if (possiblePressedLeds.size() != 0){
      float cameraX = width/2.0;
      float cameraY = height/2.0;
      float cameraZ = (height/2.0) / tan(PI*30.0 / 180.0);
      float minDist = 100000;
      PVector minPoint = new PVector();
      
      for (PVector ppl : possiblePressedLeds) {
        float newMinDist = dist(ppl.x, ppl.y, ppl.z, cameraX, cameraY, cameraZ);
        
        if (newMinDist < minDist) {
          minDist = newMinDist;
          minPoint = ppl;
        }
      }
      
      if (mousePressed && !mousePress && !cubeMoving){
                  
        mousePress = true;
        
        layers[(int)minPoint.x][(int)minPoint.y][(int)minPoint.z] = layers[(int)minPoint.x][(int)minPoint.y][(int)minPoint.z] == LED.OFF ? LED.ON : LED.OFF;
      }
    }
    
    
    
    if (!mousePressed) {
      mousePress = false;
    
    }
    
    drawCube();
  }
  
  private void drawCube() {
    
    for (int i = 0; i < 5 ; i++) { // layer
      
      for (int j = 0; j < 5; j++) { // y
      
        for (int k = 0; k < 5; k++) { // x
          
          pushMatrix();
          
          translate((k-2)*spacing, (j-2)*spacing, (i-2)*spacing);
          
          // draw wires
          if (k == 2) {
            
            fill(255,255,255,70);
            box((4)*spacing, 1, 1);
          }
          
          if (i == 2) {
            
            fill(255,255,255,70);
            box(1, 1, (4)*spacing);
            
          }
          if (j == 2) {
            
            fill(255,255,255,70);
            box(1, (4)*spacing, 1);
          }
          
          // draw the leds
          if (layers[i][j][k] == LED.OFF) {
            noStroke();
            fill(100,100,100,70);
            sphere(width/100);
            
          } else if (layers[i][j][k] == LED.ON) {
            noStroke();
            fill(0,0,255,128);
            sphere(width/100);
          }          
          
          popMatrix();
        
        }
      }
    }
    
    // box below
    translate(0, 0, -2.5*spacing);
    noStroke();
    fill(0,0,0,128);
    box(400, 400, 50);
    
    // front
    fill(0,0,255, 128);
    translate(0,200, 0);
    box(100, 3, 30);
  }
  
  private boolean mouseCollision(int x, int y, int z) {
     return (mousePressed && dist(mouseX, mouseY, screenX(x, y, z), screenY(x, y, z)) < 15);
  }
  
  public boolean LedSelected() {
    return mousePress;
  }
  
  public void cubeMoving(boolean state){
    cubeMoving = state;
  }
  
  public JSONArray toJSON(){
    
    JSONArray layer = new JSONArray();
    
    for (int i = 0; i < 5 ; i++) { // layer
      JSONArray y = new JSONArray();
      
      for (int j = 0; j < 5; j++) { // y
      
        JSONArray x = new JSONArray();
      
        for (int k = 0; k < 5; k++) { // x
          
          if (layers[i][j][k] == LED.ON)
            x.setBoolean(k, true);
          else
            x.setBoolean(k, false);
          
        }
        y.setJSONArray(j, x);
      }
      layer.setJSONArray(i, y);
    }
    
    return layer;
  }
}
