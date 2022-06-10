ArrayList <Bullet> bullets = new ArrayList <Bullet> ();
PVector player,player1,player2,player3, playerB1,playerB2, playerSpeed;
float maxSpeed = 3;
 
class Bullet extends PVector {
  PVector vel;
  int i=10000;
  Bullet(PVector loc, PVector vel) {
    super(loc.x, loc.y);
    this.vel = vel.get();
  }
 
  void update() {
    add(vel);
    
  }
 
  void display() {
    if(x>xd-50 && x<xd+50 && y>yd-50 && y<yd+120) {
     if(y!=yd && x!=xd){
      hp--;
      }
      //flicks bullets off screen cause idk how to delete them
      x=xd;
      y=i*i;
      i++;
    }
    if(i==1000000){
      i=10000;
    }
    //draws the bullet ellipse
    fill(0, 0, 255);
    ellipse(x, y, 30, 30);
  }
}
