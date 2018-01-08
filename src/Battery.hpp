//
//  Battery.hpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#ifndef Battery_hpp
#define Battery_hpp
#include "ofMain.h"
#include <stdio.h>


class Battery {
    
public:
    
    //methods
    void setupBattery();
    void drawBattery(int posX, int posY);
    bool playerBatteryCollision(float playerX, float playerY);

    //variables
    ofVec2f pos;
    ofImage battery;
    bool batteryUsed;

    
};
#endif /* Battery_hpp */
