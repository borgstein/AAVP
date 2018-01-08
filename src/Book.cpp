//
//  Book.cpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#include "Book.hpp"

void Book::setupBook(){
    
    book.load("book.png");
    book.setAnchorPoint(book.getWidth()/2, book.getHeight()/2);
    bookCollected = false;
    
    
}

void Book::drawBook(int posX, int posY){
    pos = ofVec2f(posX, posY);
    
    
    if(!bookCollected){
        book.draw(pos.x, pos.y);
    }
}


bool Book::playerBookCollision(float playerX, float playerY){
    
    if (playerX + 10 >= pos.x && playerX - 10 <= pos.x &&
        playerY + 20 >= pos.y && playerY - 20 <= pos.y)
        return true;
    else
        return false;
}
