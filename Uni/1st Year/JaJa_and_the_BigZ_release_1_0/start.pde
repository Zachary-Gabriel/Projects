void starting() {
  if (frameCount%10==0) {
    //changes stars every 10 frames
      for (int i = 0; i<1000; i++)
      {
        xStars[i] = random(0, width);
        yStars[i] = random(0, width);
      }
    }
    noStroke();
    //sky
    fill(#1E628B);
    rect(0, 0, width, height);
    //moon
    fill(255);
    ellipse(width*0.8, height*0.2, width/8, width/8);
    fill(#1E628B);
    ellipse(width*0.8-((width/8)/2), height*0.2, width/8, width/8);
    //stars
    fill(#FFD900);
    for (int i = 0; i<1000; i++)
    {
      rect(xStars[i], yStars[i], width/1080, width/1080);
    }
    //grass
    fill(#1E8B1E);
    rect(0, height*0.68, width, height);
    //cloud
    fill(#505E6A, 150);
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
    rect(width*0.14, height*0.82, -width/18, -height/3);
    fill(#44744B);
    ellipse(width*0.14, height*0.45, width/6, width/6);
    ellipse(width*0.1, height*0.42, width/6, width/6);
    ellipse(width*0.12, height*0.33, width/6, width/8);
    fill(0, 50);
    quad(width*0.14, height*0.82, 0+(width/18)/1.5, height, 0-(width/18)/1.5, height, (width*0.14)-(width/18), height*0.82);
    //hole
    fill(#552C17);
    ellipse(width*0.9, height*0.85, width*0.15, width*0.1);
    fill(#341A0E);
    ellipse(width*0.9, height*0.86, width*0.14, width*0.09);
    //sign
    fill(#8E6D4A);
    rect(width*0.8, height*0.82, width/50, height*0.15);
    fill(0, 50);
    quad(width*0.8, (height*0.82)+(height*0.15), (width*0.8)-(width*0.001), height, ((width*0.8)+(width/50))-(width*0.001), height, (width*0.8)+(width/50), (height*0.82)+(height*0.15));
    fill(#7C6042);
    rect(width*0.785, height*0.835, width/20, height/15);
    textSize(width*0.009);
    fill(0);
    text("WATCH", width*0.81, height*0.857);
    text("OUT!", width*0.81, height*0.871);
    text("DUNGEON", width*0.81, height*0.885);

    //movement tutorial
    textSize(width*0.009);
    fill(0);
    text("MOVEMENT CONTROLS", width*0.33, height*0.735);
    fill(255, 150);
    rect(width*0.305, height*0.75, width*0.05, width*0.05, 5);
    rect(width*0.31, height*0.85, width*0.05, width*0.05, 5);
    rect(width*0.366, height*0.85, width*0.05, width*0.05, 5);
    rect(width*0.254, height*0.85, width*0.05, width*0.05, 5);
    fill(0);
    textSize(width*0.03);
    text("w", width*0.33, height*0.81);
    text("s", width*0.335, height*0.91);
    text("a", width*0.28, height*0.91);
    text("d", width*0.39, height*0.91);
    //weapon tutorial
    textSize(width*0.009);
    fill(0);
    text("WEAPON CONTROLS", width*0.665, height*0.735);
    fill(255, 150);
    rect(width*0.69, height*0.75, -width*0.05, width*0.05, 5);
    rect(width*0.69, height*0.85, -width*0.05, width*0.05, 5);
    rect(width*0.634, height*0.85, -width*0.05, width*0.05, 5);
    rect(width*0.746, height*0.85, -width*0.05, width*0.05, 5);
    fill(0);
    textSize(width*0.03);
    text("^", width*0.665, height*0.81);
    text("", width*0.665, height*0.91);
    text("<", width*0.61, height*0.91);
    text(">", width*0.72, height*0.91);
    //draws the title
    textSize(width*0.07);
    textAlign(CENTER);
    fill(255);
    text("Jaja and the Big Z", width/2, height/3);
    fill(#F2DD3B);
    text("start", width/2, 2*height/3);
    if (mouseX>width*0.424 && mouseX<width*0.579 && mouseY>height*0.58 && mouseY<height*0.68) {
      fill(255, 0);
      stroke(255, 0, 0);
      strokeWeight(4);
      rect(width*0.424, height*0.58, width*0.155, height*0.1);
      if (mousePressed) {
        start=false;
      }
    }
}
