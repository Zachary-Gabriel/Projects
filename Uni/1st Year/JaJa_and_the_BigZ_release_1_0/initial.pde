void initial() {
  //sets initial conditions as you walk through a door
  hpE1=200;
  hpE2=200;
  hpE3=200;
  hpE4=200;
  if(hp<85) {
  hp+=15;
  }
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
