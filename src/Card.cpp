//
//  Card.cpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#include "Card.hpp"

void Card::setupCard(){
    
    card.load("card1.png");
    card.setAnchorPoint(card.getWidth()/2, card.getHeight()/2);
    cardCollected = false;
    
    
}

void Card::drawCard(int posX, int posY){
    pos = ofVec2f(posX, posY);
    
    
    if(!cardCollected){
        card.draw(pos.x, pos.y);
    }
}


bool Card::playerCardCollision(float playerX, float playerY){
    
    if (playerX + 10 >= pos.x && playerX - 10 <= pos.x &&
        playerY + 20 >= pos.y && playerY - 20 <= pos.y)
        return true;
    else
        return false;
}
