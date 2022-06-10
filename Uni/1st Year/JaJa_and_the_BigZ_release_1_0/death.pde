void death() {
//draws the death screen 
  textAlign(CENTER);
  textSize(width*0.073);
  fill(#29190A);
  noStroke();
  rect(0, 0, width, height);
  fill(255, 0, 0);
  text("YOU DIED", width/2, height/3);
  strokeWeight(14);
  stroke(255, 0, 0);
  line(width*0.349, height*0.3, width*0.349, height);
  line(width*0.568, height*0.3, width*0.568, height);
  rect(width*0.589, height*0.33, width*0.024, height);
  rect(width*0.381, height*0.34, width*0.039, height);
  triangle(width*0.381, height*0.34, width*0.381, height*0.315, width*0.4, height*0.34);
  triangle((width*0.381)+(width*0.039), height*0.34, (width*0.381)+(width*0.039), height*0.315, width*0.4, height*0.34);
  rect(width*0.44, height*0.34, width*0.03, height);
  triangle(width*0.44, height*0.34, width*0.44, height*0.315, width*0.45, height*0.34);
  triangle((width*0.44)+(width*0.03), height*0.34, (width*0.44)+(width*0.03), height*0.315, width*0.465, height*0.34);
  rect(width*0.5135, height*0.335, width*0.036, height);
  triangle((width*0.5135)+(width*0.036), height*0.335, (width*0.5135)+(width*0.036), height*0.3, width*0.539, height*0.335);
  rect(width*0.6285, height*0.335, width*0.036, height);
  triangle((width*0.6285)+(width*0.036), height*0.335, (width*0.6285)+(width*0.036), height*0.3, width*0.656, height*0.335);
  stroke(0);
  strokeWeight(2);
  fill(#FFDBAF);
  rectMode(CENTER);
  rect(x, y, 50, 50, 50);
  rect(x, y+40, 60, 10);
  fill(0);
  line(x-8, y-2, x-12, y+2);
  line(x-8, y+2, x-12, y-2);
  line(x+8, y-2, x+12, y+2);
  line(x+8, y+2, x+12, y-2);
  strokeWeight(5);
  fill(#C129B2);
  rect(x, y+50, 20, 50);
  fill(#E0E0E0);
  quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
  quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
  rectMode(CORNER);
  stroke(#270E3B, 150);
  fill(#411664, 150);
  quad(x-60, y+10, x-20, y-50, x+20, y-50, x+60, y+10);
  quad(x-60, y+10, x+60, y+10, x+40, y+120, x-40, y+120);
  line(x, y-50, x, y+120);
  noStroke();
  fill(#F2DD3B);
  textSize(width*0.07);
  text("restart", width/2, 2*height/3);
  if (mouseX>width*0.385 && mouseX<width*0.620 && mouseY>height*0.58 && mouseY<height*0.68) {
    fill(0, 0);
    stroke(0);
    strokeWeight(4);
    rect(width*0.385, height*0.58, width*0.235, height*0.1);
    if (mousePressed) {
      start=false;
    }
  }
  textAlign(CENTER);
  textSize(width*0.073);
  fill(#29190A);
  noStroke();
  rect(0, 0, width, height);
  fill(255, 0, 0);
  text("YOU DIED", width/2, height/3);
  strokeWeight(14);
  stroke(255, 0, 0);
  line(width*0.349, height*0.3, width*0.349, height);
  line(width*0.568, height*0.3, width*0.568, height);
  rect(width*0.589, height*0.33, width*0.024, height);
  rect(width*0.381, height*0.34, width*0.039, height);
  triangle(width*0.381, height*0.34, width*0.381, height*0.315, width*0.4, height*0.34);
  triangle((width*0.381)+(width*0.039), height*0.34, (width*0.381)+(width*0.039), height*0.315, width*0.4, height*0.34);
  rect(width*0.44, height*0.34, width*0.03, height);
  triangle(width*0.44, height*0.34, width*0.44, height*0.315, width*0.45, height*0.34);
  triangle((width*0.44)+(width*0.03), height*0.34, (width*0.44)+(width*0.03), height*0.315, width*0.465, height*0.34);
  rect(width*0.5135, height*0.335, width*0.036, height);
  triangle((width*0.5135)+(width*0.036), height*0.335, (width*0.5135)+(width*0.036), height*0.3, width*0.539, height*0.335);
  rect(width*0.6285, height*0.335, width*0.036, height);
  triangle((width*0.6285)+(width*0.036), height*0.335, (width*0.6285)+(width*0.036), height*0.3, width*0.656, height*0.335);
  stroke(0);
  strokeWeight(2);
  fill(#FFDBAF);
  rectMode(CENTER);
  rect(x, y, 50, 50, 50);
  rect(x, y+40, 60, 10);
  fill(0);
  line(x-8, y-2, x-12, y+2);
  line(x-8, y+2, x-12, y-2);
  line(x+8, y-2, x+12, y+2);
  line(x+8, y+2, x+12, y-2);
  strokeWeight(5);
  fill(#C129B2);
  rect(x, y+50, 20, 50);
  fill(#E0E0E0);
  quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
  quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
  rectMode(CORNER);
  stroke(#270E3B, 150);
  fill(#411664, 150);
  quad(x-60, y+10, x-20, y-50, x+20, y-50, x+60, y+10);
  quad(x-60, y+10, x+60, y+10, x+40, y+120, x-40, y+120);
  line(x, y-50, x, y+120);
  noStroke();
  fill(#F2DD3B);
  textSize(width*0.07);
  text("restart", width/2, 2*height/3);
  if (mouseX>width*0.385 && mouseX<width*0.620 && mouseY>height*0.58 && mouseY<height*0.68) {
    fill(0, 0);
    stroke(0);
    strokeWeight(4);
    rect(width*0.385, height*0.58, width*0.235, height*0.1);
  }
}
void mouseClicked() {
  if (hpEB<=0) {
    // sets initial conditions from the win screen
    if (mouseX>width*0.385 && mouseX<width*0.620 && mouseY>height*0.58 && mouseY<height*0.68) {
      left=false;
      right=false;
      up=false;
      down=false;
      start=true;
      roomup=false;
      roomdown=false;
      roommid=false;
      roomleft=false;
      roomright=false;
      laserCharge=0;
      laserCool=0;
      laserCharged=true;
      hpE1=200;
      hpE2=200;
      hpE3=200;
      hpE4=200;
      hpEB=400;
      hp=100;
      x=width/2;
      y=height/2;
      xE1=width/6;
      yE1=height/6;
      xE2=width/6;
      yE2=5*height/6;
      xE3=5*width/6;
      yE3=height/6;
      xE4=5*width/6;
      yE4=5*height/6;
      playerSpeed = new PVector();
      h1 = new enemies(height/6, width/6); 
      h2 = new enemies(5*height/6, width/6);
      h3 = new enemies(height/6, 5*width/6); 
      h4 = new enemies(5*height/6, 5*width/6);
    }
  }
  if (hp<0) {
    //sets initial conditions of the death screen
    if (mouseX>width*0.385 && mouseX<width*0.620 && mouseY>height*0.58 && mouseY<height*0.68) {
      left=false;
      right=false;
      up=false;
      down=false;
      start=true;
      roomup=false;
      roomdown=false;
      roommid=false;
      roomleft=false;
      roomright=false;
      laserCharge=0;
      laserCool=0;
      laserCharged=true;
      hpE1=200;
      hpE2=200;
      hpE3=200;
      hpE4=200;
      hpEB=400;
      hp=100;
      x=width/2;
      y=height/2;
      xE1=width/6;
      yE1=height/6;
      xE2=width/6;
      yE2=5*height/6;
      xE3=5*width/6;
      yE3=height/6;
      xE4=5*width/6;
      yE4=5*height/6;
      playerSpeed = new PVector();
      h1 = new enemies(height/6, width/6); 
      h2 = new enemies(5*height/6, width/6);
      h3 = new enemies(height/6, 5*width/6); 
      h4 = new enemies(5*height/6, 5*width/6);
    }
  }
}
