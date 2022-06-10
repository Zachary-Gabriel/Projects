void limits() {
  //sets the playable area for the game
  if (x<=width/28.8 || x>width*0.965277) {
    if (y<=height*0.34722) {
      y=height*0.34722;
    } else if (y>=height*0.611111) {
      y=height*0.611111;
    }
  }
  if (y<=height/18 || y>height*0.888889) {
    if (x<=width*0.38368) {
      x=width*0.38368;
    } else if (x>=width*0.6163194) {
      x=width*0.6163194;
    }
  }
  if (y<=height/3 || y>=2*height/3) {
    if (x<=width*0.0520833) {
      x=width*0.0520833;
    }
    if (x>=width*0.9479166) {
      x=width*0.9479166;
    }
  }
  if (x<=1.5*width/4 || x>=2.5*width/4) {
    if (y<=height*0.071111) {
      y=height*0.071111;
    }
    if (y>=height*0.87333) {
      y=height*0.87333;
    }
  }
  //wont let you leave till enemies are dead
  if (hpE1>0 || hpE2>0 || hpE3> 0 || hpE4>0) {
    if (x<width*0.043403) {
      x=width*0.043403;
    }
    if (x>width*0.956598) {
      x=width*0.956598;
    }
    if (y<height*0.069444444) {
      y=height*0.069444444;
    }
    if (y>height*0.886111) {
      y=height*0.886111;
    }
  }
}
