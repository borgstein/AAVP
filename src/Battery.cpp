//
//  Battery.cpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#include "Battery.hpp"

void Battery::setupBattery(){
    
    battery.load("charge.png");
    battery.setAnchorPoint(battery.getWidth()/2, battery.getHeight()/2);
    batteryUsed = false;
    
    
}

void Battery::drawBattery(int posX, int posY){
    pos = ofVec2f(posX, posY);
   

    if(!batteryUsed){
    battery.draw(pos.x, pos.y);
    }
    
}


bool Battery::playerBatteryCollision(float playerX, float playerY){
    
    if (playerX + 10 >= pos.x && playerX - 10 <= pos.x &&
        playerY + 20 >= pos.y && playerY - 20 <= pos.y)
        return true;
    else
        return false;
}
