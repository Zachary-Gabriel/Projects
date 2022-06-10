void boss() {
  
  //draws the boss
  rectMode(CENTER);
  strokeWeight(1);
  fill(#0CAD33, 150);
  rect(width/2, height/2, height/5, height/5, width*0.104, width*0.104, width/28.8, width/28.8);
  fill(0, 150);
  ellipse(width*0.46527, height*0.46111, height*0.033333, height*0.033333);
  ellipse(width*0.53472, height*0.45555, height*0.033333, height*0.033333);
  ellipse(width*0.46527, height*0.50555, height*0.033333, height*0.033333);
  ellipse(width*0.53472, height*0.49444, height*0.033333, height*0.033333);
  fill(255, 150);
  ellipse(width*0.46527, height*0.46111, height*0.02777778, height*0.02777778);
  ellipse(width*0.53472, height*0.45555, height*0.02777778, height*0.02777778);
  ellipse(width*0.46527, height*0.50555, height*0.02777778, height*0.02777778);
  ellipse(width*0.53472, height*0.49444, height*0.02777778, height*0.02777778);
  fill(#0D0C0C);
  shapeMode(CENTER);
  quad(width*0.458333, height*0.566667, width*0.46527, height*0.555555, width*0.53993, height*0.56666, width*0.52604, height*0.57222);
  
  rectMode(CORNER);
  
  //boss health bar
  if (hpEB>0) {
    noStroke();
    fill(0, 255, 0);
    rect(width/2+(width*0.025), height/2-height/9, 200, 20);
    fill(255, 0, 0);
    rect(width/2+(width*0.025), height/2-height/9, (hpEB)/2, 20);
  }
  
  //limits when fighting boss
  if (roommid && roomleft && roomright && roomup && roomdown && up==false && down==false && right== false && left==false && hpEB>0) {
    if (x<width*0.043403) {
      x=width*0.043403;
    }
    if (x>width*0.956598) {
      x=width*0.956598;
    }
    if (y<height*0.069444444) {
      y=height*0.069444444;
    }
    if (y>height*0.916667) {
      y=height*0.916667;
    }
  }
}
