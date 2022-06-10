void roomClear() {
  //checks if all enemies have been killed then saves it to a boolean
  if (hpE1<=0 &&hpE2<=0 &&hpE3<=0 &&hpE4<=0) {
    if (up) {
      roomup=true;
    } else if (down) {
      roomdown=true;
    } else if (right) {
      roomright=true;
    } else if (left) {
      roomleft=true;
    } else {
      roommid=true;
    }
  }
}
