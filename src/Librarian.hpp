//
//  Librarian.hpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#ifndef Librarian_hpp
#define Librarian_hpp

#include <stdio.h>
#include "ofMain.h"
#include <math.h>
#include <iostream>

class Librarian {
    
public:
    
    //methods
    void setupLibrarian();
    void setupSprites();
    void drawLibrarian(int posX, int posY);
    void updateLibrarian();
    
    //variables
    ofVec2f pos, initPos;
    ofImage librarianSprites;
    ofImage librarianMovement [2][6];
    ofImage librarianDeath [8];
    ofImage librarianAttack [4];
    ofImage lightning;
    ofImage lightAttack [7];
    bool attack;
    bool death;
    float speed;
    int currentDirection;
    int moveFrame, attackFrame, deathFrame, lightFrame;
};
#endif /* Librarian_hpp */
