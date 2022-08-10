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
const int pickupRange=15;
const int hitboxSize=5;
bool gameRunning=0;

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

class Pillar{
  private:
  int x=ScreenW,y;
  public:
  const int width=50;
  const int gap=70;
  Pillar(){
    y=rand()*1.0/RAND_MAX*100+80;
  }
  void draw(){
    Brain.Screen.drawRectangle(x, y, width, ScreenH-y);
    Brain.Screen.drawRectangle(x, 0, width, y-gap);
  }
  void Update(){
    x-=scrollSpeed;
    draw();
  }

  int getx(){return x;}
  int gety(){return y;}
};

std::vector<Pillar*> pillars;

bool isColliding(Pillar p){ // checks if the bird is colliding with Pillar p
  if(bird.getx()>p.getx()-hitboxSize && bird.getx()<p.getx()+p.width+hitboxSize){
    if(bird.gety()>p.gety()-hitboxSize || bird.gety()<p.gety()-p.gap+hitboxSize) return 1;
  }
  return 0;
}

int cnt=0;

void Update(){
  Brain.Screen.clearScreen();
  if(cnt==0){
    pillars.push_back(new Pillar);
  }
  if(cnt%30==0) bird.score++;
  cnt++;
  cnt%=90;
  // Brain.Screen.printAt(1, 60, "number of coins in the vector: %d", coins.size());
  for(int i=0; i<pillars.size(); i++){
    if(pillars[i]->getx() < -60){
      delete pillars[i];
      pillars.erase(pillars.begin()+i);
    }
    if(isColliding(*pillars[i])){
      for(int i=0; i<pillars.size(); i++){
        delete pillars[i];
        pillars.erase(pillars.begin()+i);
      }
      gameRunning=0;
    }
    pillars[i]->Update();
  }
  bird.Update();
}

void titleScreen(){
  Brain.Screen.clearScreen();
  int midy=ScreenH/2, midx=ScreenW/2;
  Brain.Screen.printAt(midx-45, midy+10, "Press A to start");
  wait(1, sec);
  while(1){
    if(Controller1.ButtonA.pressing()) return;
    wait(50, msec);
  }
}

void gameOver(){
  bird.score=0;
  Brain.Screen.clearScreen();
  Brain.Screen.setFont(mono30);
  Brain.Screen.printAt(1, ScreenH/2, "GAME OVER. Press A to continue...");
  Brain.Screen.setFont(mono20);
  wait(1, sec);
  while(1){
    if(Controller1.ButtonA.pressing()) return;
    wait(50, msec);
  }
}

void gameLoop(){
  while(gameRunning){
    Update();
    if(Controller1.ButtonA.pressing()){
      bird.jump();
    }
    wait(20, msec);
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  while(1){
    titleScreen();
    gameRunning=1;
    gameLoop();
    gameOver();
  }

}
