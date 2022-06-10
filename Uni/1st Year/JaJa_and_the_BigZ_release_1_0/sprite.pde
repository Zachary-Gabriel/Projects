void Sprite() {
  //draws sprite up
  if (keys[0]) {
    stroke(0);
    strokeWeight(2);
    fill(#FFDBAF);
    rectMode(CENTER);
    rect(x, y, 50, 50, 50);
    rect(x, y+40, 60, 10);
    fill(0);
    strokeWeight(5);
    fill(#C129B2);
    rect(x, y+50, 20, 50);
    fill(#E0E0E0);
    quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
    quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
    rectMode(CORNER);
    noStroke();
  }
  //draws sprite right
  else if (keys[1]) {
      stroke(0);
      strokeWeight(2);
      fill(#FFDBAF);
      rectMode(CENTER);
      rect(x, y, 50, 50, 50);
      fill(0);
      ellipse(x+10, y, 2, 2);
      strokeWeight(5);
      fill(#C129B2);
      rect(x, y+50, 20, 50);
      fill(#E0E0E0);
      quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
      quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
      strokeWeight(2);
      fill(#FFDBAF);
      rect(x, y+40, 10, 30);
      rectMode(CORNER);
      noStroke();
    }
  //draws sprite down
  else if (keys[2]) {
      stroke(0);
      strokeWeight(2);
      fill(#FFDBAF);
      rectMode(CENTER);
      rect(x, y, 50, 50, 50);
      fill(0);
      ellipse(x-10, y, 2, 2);
      strokeWeight(5);
      fill(#C129B2);
      rect(x, y+50, 20, 50);
      fill(#E0E0E0);
      quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
      quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
      strokeWeight(2);
      fill(#FFDBAF);
      rect(x, y+40, 10, 30);
      rectMode(CORNER);
      noStroke();
    } 
    // draws sprite left
    else if (keys[3]) {
    stroke(0);
    strokeWeight(2);
    fill(#FFDBAF);
    rectMode(CENTER);
    rect(x, y, 50, 50, 50);
    rect(x, y+40, 60, 10);
    fill(0);
    ellipse(x-10, y, 2, 2);
    ellipse(x+10, y, 2, 2);
    strokeWeight(5);
    fill(#C129B2);
    rect(x, y+50, 20, 50);
    fill(#E0E0E0);
    quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
    quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
    rectMode(CORNER);
    noStroke();
  }else {
    //draws sprite when no keys are pressed
     stroke(0);
      strokeWeight(2);
      fill(#FFDBAF);
      rectMode(CENTER);
      rect(x, y, 50, 50, 50);
      rect(x, y+40, 60, 10);
      fill(0);
      ellipse(x-10, y, 2, 2);
      ellipse(x+10, y, 2, 2);
      strokeWeight(5);
      fill(#C129B2);
      rect(x, y+50, 20, 50);
      fill(#E0E0E0);
      quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
      quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
      rectMode(CORNER);
      noStroke();
    }
  if ((keys[4] || keys[5] || keys[6] || keys[7]) && !(keys[0] || keys[1] || keys[2] || keys[3]))
  {
    //draws the sprite when laser is being used
    stroke(0);
    strokeWeight(2);
    fill(#FFDBAF);
    rectMode(CENTER);
    rect(x, y, 50, 50, 50);
    rect(x, y+40, 60, 10);
    fill(0);
    ellipse(x-10, y, 2, 2);
    ellipse(x+10, y, 2, 2);
    strokeWeight(5);
    fill(#C129B2);
    rect(x, y+50, 20, 50);
    fill(#E0E0E0);
    quad(x-10, y+75, x-20, y+100, x-10, y+105, x, y+75);
    quad(x+10, y+75, x+20, y+100, x+10, y+105, x, y+75);
    rectMode(CORNER);
    noStroke();
  }
}
