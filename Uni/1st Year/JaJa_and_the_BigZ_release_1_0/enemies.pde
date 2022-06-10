class enemies { 
  float ya, xa, a, b;

  enemies(float y, float x) {  
    ya = y; 
    xa = x;
    a=ya;
    b=xa;
  } 
  void update() { 
    if (dist(xa, ya, x, y)<width/3.5) {
      ya+=(y-(ya+100))/difficulty; 
      xa+=((x-50)-(xa+50))/difficulty;
    } else {
      ya+=(a-(ya+100))/difficulty; 
      xa+=((b-50)-(xa+50))/difficulty;
    }
    //draws the enemy
    strokeWeight(1);
    fill(#4987F0, 150);
    rect(xa+50, ya+50, 100, 100, 100, 100, 25, 25);
    fill(0, 150);
    ellipse(xa+80, ya+90, 10, 10);
    ellipse(xa+120, ya+80, 7, 7);
    fill(255, 150);
    ellipse(xa+80, ya+90, 10, 10);
    ellipse(xa+120, ya+80, 7, 7);
    fill(255, 0, 0, 150);
    quad(xa+70, ya+120, xa+110, ya+110, xa+115, ya+115, xa+75, ya+130);
    //hp bar E1
    if (hpE1>0) {
      noStroke();
      fill(0, 255, 0);
      rect(xE1+(width*0.025), yE1+height*0.03, 100, 10);
      fill(255, 0, 0);
      rect(xE1+(width*0.025), yE1+height*0.03, (hpE1)/2, 10);
    }
    //hp bar E2
    if (hpE2>0) {
      noStroke();
      fill(0, 255, 0);
      rect(xE2+(width*0.025), yE2+height*0.03, 100, 10);
      fill(255, 0, 0);
      rect(xE2+(width*0.025), yE2+height*0.03, (hpE2)/2, 10);
    }
    //hp bar E3
    if(hpE3>0) {
    noStroke();
    fill(0, 255, 0);
    rect(xE3+(width*0.025), yE3+height*0.03, 100, 10);
    fill(255, 0, 0);
    rect(xE3+(width*0.025), yE3+height*0.03, (hpE3)/2, 10);
    }
    //hp bar E4
    if(hpE4>0) {
    noStroke();
    fill(0, 255, 0);
    rect(xE4+(width*0.025), yE4+height*0.03, 100, 10);
    fill(255, 0, 0);
    rect(xE4+(width*0.025), yE4+height*0.03, (hpE4)/2, 10);
    }
  }
}
