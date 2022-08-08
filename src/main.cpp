/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       xingrui                                                   */
/*    Created:      Sun Aug 07 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <vector>

using namespace vex;

#define ScreenH 272
#define ScreenW 480

const int scrollSpeed=2;
const int pickupRange=13;

class Bird{
  private:
  const int x=50;
  double y=ScreenH/2;
  const int w=10,h=10;
  const int jumpH=4;
  double vel=0.0; // vel stands for velocity
  const double gravity=0.5;
  int jumpCD=0;
  public:
  int score=0;
  void draw(){
    Brain.Screen.drawRectangle(x,(int)y,w,h);
  }
  void jump(){
    if(jumpCD==0){
      vel=jumpH;
      jumpCD=5;
    }
  }
  void printScore(){
    Brain.Screen.printAt(200, 20, "score: %d                    ", score);
  }
  void Update(){
    jumpCD=fmax(jumpCD-1,0);
    printScore();
    y-=vel;
    vel-=gravity;
    draw();
  }

  int getx(){return x;}
  int gety(){return y;}
}bird;

class Coin{
  private:
  int x=ScreenW,y;
  const int r=10;
  public:
  Coin(){
    y=rand()*1.0/RAND_MAX*100+80;
  }
  void draw(){
    Brain.Screen.drawCircle(x, y, r);
  }
  void Update(){
    x-=scrollSpeed;
    draw();
  }

  int getx(){return x;}
  int gety(){return y;}
};

std::vector<Coin*> coins;

bool isColliding(Coin c){ // checks if the bird is colliding with Coin c
  // Brain.Screen.printAt(20, 40, "bird x=%d y=%d", bird.getx(), bird.gety());
  if(abs(bird.getx()-c.getx())<pickupRange && abs(bird.gety()-c.gety())<pickupRange) return 1;
  return 0;
}

int cnt=0;

void Update(){
  Brain.Screen.clearScreen();
  bird.Update();
  if(cnt==0){
    coins.push_back(new Coin);
  }
  cnt++;
  cnt%=30;
  // Brain.Screen.printAt(1, 60, "number of coins in the vector: %d", coins.size());
  for(int i=0; i<coins.size(); i++){
    if(coins[i]->getx()<0){
      delete coins[i];
      coins.erase(coins.begin()+i);
    }
    if(isColliding(*coins[i])){
      bird.score++;
      delete coins[i];
      coins.erase(coins.begin()+i);
    }
    coins[i]->Update();
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  while(1){
    Update();
    if(Controller1.ButtonA.pressing()){
      bird.jump();
    }
    wait(20, msec);
  }

}
