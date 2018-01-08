//
//  Player.hpp
//  creativePresentaion
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#ifndef Player_hpp
#define Player_hpp
#include "ofMain.h"
#include "ofxOpenCv.h"
#include <math.h>

class Player {
    
public:
    
    //methods
    void setupPlayer();
    void setupSprites();
    void drawPlayer();
    void updatePlayer(int xDelta, int yDelta);
    bool isPlayerOffScreen();
    int whatEdge();
    
    //variables for player
    ofVec2f pos;
    ofImage playerSprites;
    ofImage movement [4][9];
    ofImage deathSprites;
    ofImage playerDeath [12];
    bool inMotion;
    bool death;
    int currentDirection;
    float currentFrame;
    float trans;
    int deathFrame;
    int UP;
    int LEFT;
    int DOWN;
    int RIGHT;

    //variables for light
    ofImage darkSprite, lightSprite;
    int light;

    
    
};

#endif /* Player_hpp */
