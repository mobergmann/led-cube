
class Cube {
  
  int[][][] layers;

  Cube() {
    for (int k = 0; k < 5; k++) {
      for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
          
          layers[i][j][k] = 0;
      
        }
      }
    }
  
  }
  
  public int led(int x, int y, int z) {
     return layers[z][y][z];
  }
  
  
  
}
