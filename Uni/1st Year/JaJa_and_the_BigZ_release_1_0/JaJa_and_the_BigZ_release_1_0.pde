float x;
float y;
float x1;
float y1;
float xE1, yE1, xE2, yE2, xE3, yE3, xE4, yE4, xd, yd;
int hpE1=200;
int hpE2=200;
int hpE3=200;
int hpE4=200;
int hpEB=400;
int hp=100;
int difficulty=100;
boolean[] keys;
boolean down, left, right, up;
boolean laserA=true;
boolean start=true;
boolean sprite;
boolean bullet=true;
int laserCharge=0;
int laserCool=0;
boolean laserCharged=true;
float[] xStars= new float[1000];
float[] yStars= new float[1000];
boolean roomup, roomleft, roomright, roommid, roomdown;
float size=40;
float a=1;
enemies h1, h2, h3, h4;
laser h6; 

void setup() {
  frameRate(50);
  noStroke();
  fullScreen();
  //writes every boolean in array to false
  keys=new boolean[8];
  for (int i=0; i<8; i++) {
    keys[i]=false;
  }
  
  //sets initial conditions
  x=2.5*width/4;
  y=1.5*height/4;
  x1=width/4.5;
  y1=height-height/5;
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
  h6 = new laser(width/2, height/2);
  //stars
}

// overly complicated movement so we can go diagonal
void keyPressed() {
  //movement
  if (key=='w')
    keys[0]=true;
  sprite=true;
  if (key=='d')
    keys[1]=true;
  sprite=true;
  if (key=='a')
    keys[2]=true;
  sprite=true;
  if (key=='s')
    keys[3]=true;
  sprite=true;
  
  // cheat code
  if (key=='u') {
    hpE1=-1;
    hpE2=-1;
    hpE3=-1;
    hpE4=-1;
    roomup=true;
    roomdown=true;
    roomleft=true;
    roomright=true;
  }
  //weapons aim
  if (key==CODED) {
    if (keyCode == UP) {
      keys[4]=true;
      sprite=true;
    }
    if (keyCode == RIGHT) {
      keys[5]=true;
      sprite=true;
    }
    if (keyCode == DOWN) {
      keys[6]=true;
      sprite=true;
    }
    if (keyCode == LEFT) {
      keys[7]=true;
      sprite=true;
    }
  }
}

void keyReleased() {
  //movement
  if (key=='w')
    keys[0]=false;
  sprite=false;
  if (key=='d')
    keys[1]=false;
  sprite=false;
  if (key=='a')
    keys[2]=false;
  sprite=false;
  if (key=='s')
    keys[3]=false;
  sprite=false;

  //gun aim
  if (key==CODED) {
    if (keyCode == UP) {
      keys[4]=false;
      sprite=false;
    }
    if (keyCode == RIGHT) {
      keys[5]=false;
      sprite=false;
    }
    if (keyCode == DOWN) {
      keys[6]=false;
      sprite=false;
    }
    if (keyCode == LEFT) {
      keys[7]=false;
      sprite=false;
    }
  }
}

void draw() {

// sets vector co-ordinates for the start of every bullet
  xd=x;
  yd=y;
  player = new PVector(xE1+80, yE1+80);
  player1 = new PVector(xE2+80, yE2+80);
  player2 = new PVector(xE3+80, yE3+80);
  player3 = new PVector(xE4+80, yE4+80);
  playerB1 = new PVector(width*0.4652777, height/2);
  playerB2 = new PVector(width*0.5347222, height/2);

  if (start) {
    // runs start screen object
    starting();
  } else {
    // checks if room is clear
    roomClear();
    
    //background
    background(#A78986);
    noStroke();
    
    //playable area
    fill(#472607);
    rect(width*0.0347222, height*0.055555, width*0.9305555, height*0.88889);
    
    //door blocks
    fill(#472607);
    noStroke();
    rect(1.5*width/4, 0, width/4, height);
    rect(0, height/3, width, height/3);
    stroke(0);

    //tracks location of the enemies objects (used to stop laser at enemy)
    if (dist(xE1, yE1, x, y)<width/3.5) {
      yE1+=(y-(yE1+100))/difficulty; 
      xE1+=((x-50)-(xE1+50))/difficulty;
    } else {
      yE1+=((height/6)-(yE1+100))/difficulty; 
      xE1+=(((width/6)-50)-(xE1+50))/difficulty;
    }

    if (dist(xE2, yE2, x, y)<width/3.5) {
      yE2+=(y-(yE2+100))/difficulty; 
      xE2+=((x-50)-(xE2+50))/difficulty;
    } else {
      yE2+=((5*height/6)-(yE2+100))/difficulty; 
      xE2+=(((width/6)-50)-(xE2+50))/difficulty;
    }

    if (dist(xE3, yE3, x, y)<width/3.5) {
      yE3+=(y-(yE3+100))/difficulty; 
      xE3+=((x-50)-(xE3+50))/difficulty;
    } else {
      yE3+=((height/6)-(yE3+100))/difficulty; 
      xE3+=(((5*width/6)-50)-(xE3+50))/difficulty;
    }

    if (dist(xE4, yE4, x, y)<width/3.5) {
      yE4+=(y-(yE4+100))/difficulty; 
      xE4+=((x-50)-(xE4+50))/difficulty;
    } else {
      yE4+=((5*height/6)-(yE4+100))/difficulty; 
      xE4+=(((5*width/6)-50)-(xE4+50))/difficulty;
    }
    //moves the enemy objects
    if (hpE1>0) {
      h1.update();
    }
    if (hpE2>0) {
      h2.update();
    }
    if (hpE3>0) {
      h3.update();
    }
    if (hpE4>0) {
      h4.update();
    }


    //bullets
    player.add(playerSpeed);
    PVector mouse = new PVector(x, y);

    if (frameCount%20==0 && dist(xE1+80, yE1+80, x, y)<width/4 && hpE1>0) {
      PVector dir = PVector.sub(mouse, player);
      dir.normalize();
      dir.mult(maxSpeed*1);
      Bullet b = new Bullet(player, dir);
      bullets.add(b);
    }
    if (frameCount%20==0 && dist(xE2+80, yE2+80, x, y)<width/4 && hpE2>0) {
      PVector dir1 = PVector.sub(mouse, player1);
      dir1.normalize();
      dir1.mult(maxSpeed*1);
      Bullet c = new Bullet(player1, dir1);
      bullets.add(c);
    }
    if (frameCount%20==0 && dist(xE3+80, yE3+80, x, y)<width/4 && hpE3>0) {
      PVector dir2 = PVector.sub(mouse, player2);
      dir2.normalize();
      dir2.mult(maxSpeed*1);
      Bullet d = new Bullet(player2, dir2);
      bullets.add(d);
    }
    if (frameCount%20==0 && dist(xE4+80, yE4+80, x, y)<width/4 && hpE4>0) {  
      PVector dir3 = PVector.sub(mouse, player3);
      dir3.normalize();
      dir3.mult(maxSpeed*1);
      Bullet e = new Bullet(player3, dir3);
      bullets.add(e);
    }
    if (roommid && roomleft && roomright && roomup && roomdown && hpEB>0 && up==false && down==false && right== false && left==false) {
      if (frameCount%12==0 ) {  
        PVector dir4 = PVector.sub(mouse, playerB1);
        dir4.normalize();
        dir4.mult(maxSpeed*1.5);
        Bullet f = new Bullet(playerB1, dir4);
        bullets.add(f);
      }
      if (frameCount%12==0 ) {  
        PVector dir5 = PVector.sub(mouse, playerB2);
        dir5.normalize();
        dir5.mult(maxSpeed*1.5);
        Bullet g = new Bullet(playerB2, dir5);
        bullets.add(g);
      }
      boss();
    }
    for (Bullet b : bullets) {
      b.update();
      b.display();
    }
    for (Bullet c : bullets) {
      c.update();
      c.display();
    }
    for (Bullet d : bullets) {
      d.update();
      d.display();
    }
    for (Bullet e : bullets) {
      e.update();
      e.display();
    }
    for (Bullet f : bullets) {
      f.update();
      f.display();
    }
    for (Bullet g : bullets) {
      g.update();
      g.display();
    }

    // runs sprite function if hp>0
    if (hp>0) {
      if (sprite) {
        Sprite();
      } else {
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
    } else {
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
      stroke(255, 0, 0);
      line(x-15, y-5, x-5, y+5);
      line(x-5, y-5, x-15, y+5);
      line(x+15, y-5, x+5, y+5);
      line(x+5, y-5, x+15, y+5);
      noStroke();
    }


    //limits
    limits();
    
    //player hp bar
    noStroke();
    textSize(width*0.010416);
    fill(255);
    text("Health: "+(hp)+"%", 5.4*(width/6.5), 8.8*height/10);
    fill(0, 50);
    rect(7.95*(width/10), 8*height/9, 200, 30);
    fill(255, 0, 0);
    rect(7.95*(width/10), 8*height/9, (hp*2), 30);

    //door teleport
    if (x<0)
    {
      left=true;
      up=false;
      down=false;
      x = width;
      if (left && right==false) {
        if (roomleft==false) {
          initial();
        }
      }
    }
    if (x>width)
    {
      right=true;
      up=false;
      down=false;
      x = 0;
      if (right && left==false) {
        if (roomright==false) {
          initial();
        }
      }
    }
    if (y<0)
    {
      left=false;
      right=false;
      up=true;
      y = height;
      if (up && down==false) {
        if (roomup==false) {
          initial();
        }
      }
    }
    if (y>height)
    { 
      left=false;
      right=false;
      down=true;
      y = 0;
      if (down && up==false) {
        if (roomdown==false) {
          initial();
        }
      }
    }
    
    //changes the room depending on where you are
    if (left) {
      if (y<height*0.083333) {
        y=height*0.083333;
      }
      if (x<width*0.05208333) {
        x=width*0.05208333;
      }
      if (y>=height*0.873333) {
        y=height*0.873333;
      }
      if (right) {
        left=false;
        right=false;
      } else {
        noStroke();
        fill(#A78986);
        rect(0, 0, width/28.8, height);
        rect(0, 0, width, height/18);
        rect(0, height*0.94444, width, height*0.0555555);
      }
    }
    if (right) {
      if (y<height*0.083333) {
        y=height*0.083333;
      }
      if (y>=height*0.873333) {
        y=height*0.873333;
      }
      if (x>width*0.947916667) {
        x=width*0.947916667;
      }
      if (left) {
        right=false;
        left=false;
      } else {

        noStroke();
        fill(#A78986);
        rect(width*0.96527777, 0, width*0.0347222, height);
        rect(0, 0, width, height*0.05555);
        rect(0, height*0.94444, width, 100);
      }
    }

    
    if (up) {
      if (y<height*0.083333) {
        y=height*0.083333;
      }
      if (x<width*0.05208333) {
        x=width*0.05208333;
      }
      if (x>width*0.8333333) {
        x=width*0.8333333;
      }
      if (down) {
        up=false;
        down=false;
      } else {
        noStroke();
        fill(#A78986);
        rect(width*0.9652777, 0, width*0.0347222, height);
        rect(0, 0, width, height*0.055555);
        rect(0, 0, width*0.0347222, height);
      }
    }
    if (down) {
      if (y>=height*0.873333) {
        y=height*0.873333;
      }
      if (x<width*0.05208333) {
        x=width*0.05208333;
      }
      if (x>width*0.9479166) {
        x=width*0.9479166;
      }
      if (up) {
        up=false;
        down=false;
      } else {
        noStroke();
        fill(#A78986);
        rect(width*0.9652777, 0, width*0.0347222, height);
        rect(0, height*0.94444, width, height*0.055556);
        rect(0, 0, width*0.0347222, height);
      }
    }
    
    //movement controls
    if (hp>0) {
      if (keys[0]) {
        y-=10;
      }
      if (keys[1]) {
        x+=10;
      }
      if (keys[2]) {
        x-=10;
      }
      if (keys[3]) {
        y+=10;
      }
      textSize(50);
      h6.update();
      noStroke();
    }
  }
  //death screen
  if (hp<=0)
  {
    death();
  }
  //win screen
  if (hpEB<=0) {
    winScreen();
  }
}
