class laser { 
  float ypos, xpos; 
  float damage=2;
  laser(float xa, float ya) {  
    ypos = ya; 
    xpos = xa;
  } 
  void update() { 
    strokeWeight(5);
    stroke(255, 0, 0);
    
    //overly complicated laser code that checks enemy location then fires to there
    if (laserCharged) {
      if (keys[4]) {
        laserCharge+=1;
        if (x>=xE1+50 && x<=xE1+150  && (yE1>=yE2 || (y<yE2 || hpE2<=0))  &&  hpE1>0 && y>yE1) {
          hpE1-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE1+100);
          line(x+10, y, x+10, yE1+100);
        } else if (x>=xE2+50 && x<=xE2+150 && (yE2>=yE1 || hpE1<=0) && (yE2>=yE3 || hpE3<=0) && y>=yE2 && hpE2>0) {
          hpE2-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE2+100);
          line(x+10, y, x+10, yE2+100);
        } else if (x>=xE3+50 && x<=xE3+150  && (yE3>=yE4 || (y<yE4 || hpE4<=0) ) && hpE3>0 && y>yE3 ) {
          hpE3-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE3+100);
          line(x+10, y, x+10, yE3+100);
        } else if (x>=xE4+50 && x<=xE4+150 && (yE4>=yE1 || hpE1<=0) && (yE4>=yE3 || hpE3<=0)  && y>=yE4 && hpE4>0) {
          hpE4-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE4+100);
          line(x+10, y, x+10, yE4+100);
        } else if(hpEB>0 && y>height/2+height/10 && x>width/2-height/10 && x<width/2+height/10 &&roommid && roomleft && roomright && roomup && roomdown && up==false && down==false && right== false && left==false) {
          hpEB-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, height/2+height/11);
          line(x+10, y, x+10, height/2+height/11);
        }else {
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, height*0.05555556);
          line(x+10, y, x+10, height*0.05555556);
        }
      } else if (keys[5]) {
        laserCharge+=1;
        if (y>=yE1 && y<=yE1+150 && (xE1<=xE2 || hpE2<=0) && (xE1<=xE3 || hpE3<=0) && (xE1<=xE4 || hpE4<=0) && hpE1>0 && x<=xE1) {
          hpE1-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, xE1+100, y);
        } else if (y>=yE2 && y<=yE2+150 && (xE2<=xE3 || hpE3<=0) && (xE2<=xE4 || hpE4<=0) && (xE2<=xE1 || hpE1<=0) && hpE2>0 && x<=xE2) {
          hpE2-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, xE2+100, y);
        } else if (y>=yE3 && y<=yE3+150  && (xE3<=xE1 || (x>xE1 || hpE1<=0)) && (xE3<=xE4 || hpE4<=0) && hpE3>0 && x<=xE3) {
          hpE3-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, xE3+100, y);
        } else if (y>=yE4 && y<=yE4+150  && (xE4<=xE2 || (x>xE1 || hpE2<=0)) && (xE4<=xE3 || hpE3<=0) && hpE4>0 && x<=xE4) {
          hpE4-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, xE4+100, y);
        } else if(hpEB>0 && x<width/2-height/10 && y>height/2-height/10 && y<height/2+height/10&&roommid && roomleft && roomright && roomup && roomdown && up==false && down==false && right== false && left==false) {
          hpEB-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, width/2-height/11, y);
        }else {
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, width*0.9652777, y);
        }
      } else if (keys[6]) {
        laserCharge+=1;
        if (x>=xE1+50 && x<=xE1+150  && (yE1<=yE2 || hpE2<=0)  &&  hpE1>0 && y<yE1) {
          hpE1-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE1+100);
          line(x+10, y, x+10, yE1+100);
        } else if (x>=xE2+50 && x<=xE2+150 && (yE2<=yE1 || (y>yE2 || hpE1<=0))  && (yE2<=yE4 || hpE4<=0) && y<=yE2 && hpE2>0) {
          hpE2-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE2+100);
          line(x+10, y, x+10, yE2+100);
        } else if (x>=xE3+50 && x<=xE3+150  && (yE3<=yE4 || hpE4<=0)  && hpE3>0 && y<yE3 ) {
          hpE3-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE3+100);
          line(x+10, y, x+10, yE3+100);
        } else if (x>=xE4+50 && x<=xE4+150  && (yE4<=yE3 || (y>yE3 || hpE3<=0))&& (yE4<=yE2 || hpE2<=0) && y<=yE4 && hpE4>0) {
          hpE4-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, yE4+100);
          line(x+10, y, x+10, yE4+100);
        } else if(hpEB>0 && y<height/2-height/10 && x>width/2-height/10 && x<width/2+height/10 &&roommid && roomleft && roomright && roomup && roomdown && up==false && down==false && right== false && left==false) {
          hpEB-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, height/2-height/11);
          line(x+10, y, x+10, height/2-height/11);
        }else {
          strokeWeight((100-laserCharge)/5);
          line(x-10, y, x-10, height*0.944444);
          line(x+10, y, x+10, height*0.944444);
        }
      } else if (keys[7]) {
        laserCharge+=1;
        if (y>=yE1 && y<=yE1+150 &&  (xE1>=xE3 || (x<xE3 || hpE3<=0)) && hpE1>0 && x>=xE1) {
          hpE1-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x+10, y, xE1+100, y);
        } else if (y>=yE2 && y<=yE2+150  && (xE2>=xE4 || (x<xE4 || hpE4<=0)) && hpE2>0 && x>=xE2) {
          hpE2-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x+10, y, xE2+100, y);
        } else if (y>=yE3 && y<=yE3+150  && (xE3>=xE1 || hpE1<=0) && (xE3<=xE4 || hpE4<=0) && hpE3>0 && x>=xE3) {
          hpE3-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x+10, y, xE3+100, y);
        } else if (y>=yE4 && y<=yE4+150  && (xE4>=xE2 || hpE2<=0) &&  hpE4>0 && x>=xE4) {
          hpE4-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x+10, y, xE4+100, y);
        } else if(hpEB>0 && x>width/2+height/10 && y>height/2-height/10 && y<height/2+height/10 &&roommid && roomleft && roomright && roomup && roomdown&& up==false && down==false && right== false && left==false) {
          hpEB-=damage;
          strokeWeight((100-laserCharge)/5);
          line(x+10, y, width/2+height/11, y);
        }else {
          strokeWeight((100-laserCharge)/5);
          line(x+10, y, width*0.03472222, y);
        }
      }
    }
    if (laserCharge>=100) {
      laserCharged=false;
      laserCool+=1;
    }

    if (laserCool>=100) {
      laserCool=0;
      laserCharge=0;
      laserCharged=true;
    }
    
    if (100-laserCharge==0) {
      noStroke();
      textSize(width*0.010416);
      text("Charge: "+laserCool+"%", width/6.5, 8.8*height/10);
      fill(0, 50);
      rect(width/10, 8*height/9, 200, 30);
      fill (255, 0, 0);
      rect(width/10, 8*height/9, (laserCool)*2, 30);
    } else {
      noStroke();
      textSize(width*0.010416);
      text("Charge: "+(100-laserCharge)+"%", width/6.5, 8.8*height/10);
      fill(0, 50);
      rect(width/10, 8*height/9, 200, 30);
      fill(255);
      rect(width/10, 8*height/9, (100-laserCharge)*2, 30);
    }
}
}
