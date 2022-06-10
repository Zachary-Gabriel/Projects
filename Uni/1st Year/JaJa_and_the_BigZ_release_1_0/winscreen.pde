void winScreen() {
  noStroke();
  //sky
  fill(#0FADFA);
  rect(0, 0, width, height);
  //sun
  fill(#F9FA0F);
  ellipse(width*0.8, height*0.2, width/8, width/8);
  //grass
  fill(#07A50F);
  rect(0, height*0.68, width, height);
  //clouds
  fill(255);
  ellipse(width*0, height*0.3, width/6, height/6);
  ellipse(width*0.1, height*0.2, width/6, height/6);
  ellipse(width*0.2, height*0.3, width/6, height/6);
  ellipse(width*0.3, height*0.4, width/6, height/6);
  ellipse(width*0.35, height*0.15, width/15, height/10);
  ellipse(width*0.37, height*0.07, width/15, height/10);
  ellipse(width*0.31, height*0.1, width/15, height/10);
  ellipse(width*0.4, height*0.3, width/6, height/6);
  ellipse(width*0.5, height*0.2, width/6, height/6);
  ellipse(width*0.6, height*0.3, width/6, height/6);
  ellipse(width*0.5, height*0.3, width/6, height/6);
  ellipse(width*0.7, height*0.4, width/6, height/6);
  ellipse(width*0.8, height*0.3, width/6, height/6);
  ellipse(width*0.9, height*0.2, width/6, height/6);
  ellipse(width, height*0.3, width/6, height/6);
  //tree
  fill(#745344);
  rect(width-width*0.14, height*0.82, width/18, -height/3);
  fill(#44744B);
  ellipse(width-width*0.14, height*0.45, -width/6, width/6);
  ellipse(width-width*0.1, height*0.42, -width/6, width/6);
  ellipse(width-width*0.12, height*0.33, -width/6, width/8);
  fill(0, 50);
  quad(width-width*0.14, height*0.82, (width-width/18)/1.2, height, (width-width/18)/1.1, height, (width-width*0.14)+(width/18), height*0.82);
  //hole
  fill(#552C17);
  ellipse(width-width*0.9, height*0.85, width*0.15, width*0.1);
  fill(#341A0E);
  ellipse(width-width*0.9, height*0.86, width*0.14, width*0.09);
  //sign
  fill(#8E6D4A);
  rect(width-width*0.8, height*0.82, -width/50, height*0.15);
  fill(0, 50);
  quad(width-width*0.8, (height*0.82)+(height*0.15), (width-width*0.8)-(width*0.001), height, ((width-width*0.8)-(width/50))-(width*0.001), height, (width-width*0.8)-(width/50), (height*0.82)+(height*0.15));
  fill(#7C6042);
  rect(width-width*0.785, height*0.835, -width/20, height/15);
  textSize(width*0.009);
  fill(0);
  text("WATCH", width-width*0.81, height*0.857);
  text("OUT!", width-width*0.81, height*0.871);
  text("DUNGEON", width-width*0.81, height*0.885);
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(size);
  
  //you Win comes in and out
  text("You Win", width/2, height/2);
  size+=3*a;
  if (size>=200) {
    a=a*-1;
  }
  if (size<=40) {
    a=a*-1;
  }
  //attempts to animate the guy walking off screen
  if(frameCount%6==0 || frameCount%8==0) {
  stroke(0);
  strokeWeight(2);
  fill(#FFDBAF);
  rectMode(CENTER);
  rect(x1, y1, 50, 50, 50);
  fill(0);
  ellipse(x1+10, y1, 2, 2);
  strokeWeight(5);
  fill(#C129B2);
  rect(x1, y1+50, 20, 50);
  fill(#E0E0E0);
  quad(x1-10, y1+75, x1-20, y1+100, x1-10, y1+105, x1, y1+75);
  quad(x1+10, y1+75, x1+20, y1+100, x1+10, y1+105, x1, y1+75);
  strokeWeight(2);
  fill(#FFDBAF);
  rect(x1, y1+40, 10, 30);
  rectMode(CORNER);
  noStroke();
  }else if(frameCount%7==0 || frameCount%5==0 ){
    stroke(0);
  strokeWeight(2);
  fill(#FFDBAF);
  rectMode(CENTER);
  rect(x1, y1, 50, 50, 50);
  fill(0);
  ellipse(x1+10, y1, 2, 2);
  strokeWeight(5);
  fill(#C129B2);
  rect(x1, y1+50, 20, 50);
  fill(#E0E0E0);
  rect(x1, y1+90, 10, 25);
  strokeWeight(2);
  fill(#FFDBAF);
  rect(x1, y1+40, 10, 30);
  rectMode(CORNER);
  noStroke();
  }else {
  stroke(0);
  strokeWeight(2);
  fill(#FFDBAF);
  rectMode(CENTER);
  rect(x1, y1, 50, 50, 50);
  fill(0);
  ellipse(x1+10, y1, 2, 2);
  strokeWeight(5);
  fill(#C129B2);
  rect(x1, y1+50, 20, 50);
  fill(#E0E0E0);
  //rect(x1, y1+90, 10, 25);
  strokeWeight(2);
  fill(#FFDBAF);
  rect(x1, y1+40, 10, 30);
  rectMode(CORNER);
  noStroke();
  }
  x1+=9;
  
  //draws return button
  noStroke();
  fill(#F2DD3B);
  textSize(width*0.07);
  text("Menu", width/2, height*0.62);
  if (mouseX>width*0.385 && mouseX<width*0.620 && mouseY>height*0.58 && mouseY<height*0.68) {
    fill(0, 0);
    stroke(0);
    strokeWeight(4);
    rect(width*0.385, height*0.58, width*0.235, height*0.1);
  }
}
