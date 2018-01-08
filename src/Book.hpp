//
//  Book.hpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#ifndef Book_hpp
#define Book_hpp
#include "ofMain.h"
#include <stdio.h>
class Book {
    
public:
    
    //methods
    void setupBook();
    void drawBook(int posX, int posY);
    bool playerBookCollision(float playerX, float playerY);
    
    //variables
    ofVec2f pos;
    ofImage book;
    bool bookCollected;

    
};
#endif /* Book_hpp */
