//
//  Card.hpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#ifndef Card_hpp
#define Card_hpp
#include "ofMain.h"
#include <stdio.h>

class Card {
    
public:
    
    //methods
    void setupCard();
    void drawCard(int posX, int posY);
    bool playerCardCollision(float playerX, float playerY);
    
    //variables
    ofVec2f pos;
    ofImage card;
    bool cardCollected;
    
    
};
#endif /* Card_hpp */
