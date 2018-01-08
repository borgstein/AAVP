//
//  Player.cpp
//  creativePresentaion
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#include "Player.hpp"
#include <math.h>

void Player::setupPlayer() {
    
    UP = 0;
    LEFT = 1;
    DOWN = 2;
    RIGHT = 3;
    pos.x = ofGetWidth()/2;
    pos.y = ofGetHeight()/2;
    inMotion = false;
    death = false;
    light = 0;  //0 = fully lit, 1 = torch off, 2 = torch on
    currentDirection = 1;
    currentFrame = 0;
    deathFrame = 0;
    trans = 255;
    playerSprites.load("inspector.png");
    deathSprites.load("deathSprite.png");
    darkSprite.load("dark.png");
    lightSprite.load("light.png");
    setupSprites();

}

void Player::setupSprites() {
    
    //Each image is loaded into the movement matrix
    for (int i = 0; i < 9; i++) {
        movement[0][i].cropFrom(playerSprites, 16 + 64 * i, 8, 32, 56);
        movement[1][i].cropFrom(playerSprites, 16 + 64 * i, 72, 32, 56);
        movement[2][i].cropFrom(playerSprites, 16 + 64 * i, 136, 32, 56);
        movement[3][i].cropFrom(playerSprites, 16 + 64 * i, 200, 32, 56);
        movement[0][i].setAnchorPoint(movement[0][i].getWidth()/2, movement[0][i].getHeight()/2);
        movement[1][i].setAnchorPoint(movement[1][i].getWidth()/2, movement[1][i].getHeight()/2);
        movement[2][i].setAnchorPoint(movement[2][i].getWidth()/2, movement[2][i].getHeight()/2);
        movement[3][i].setAnchorPoint(movement[3][i].getWidth()/2, movement[3][i].getHeight()/2);

    }
    
   for (int i = 8; i < 12; i++){
        playerDeath[i].cropFrom(deathSprites, 0, 0 + 59 * (i-8), deathSprites.getWidth(), 59);
       playerDeath[i].setAnchorPoint(playerDeath[i].getWidth()/2, playerDeath[i].getHeight()/2);
    }
    
    darkSprite.resize(darkSprite.getWidth()*(ofGetWidth()*2/darkSprite.getWidth()),darkSprite.getHeight()*(ofGetWidth()*2/darkSprite.getWidth()));
    darkSprite.setAnchorPoint(darkSprite.getWidth()/2, darkSprite.getHeight()/2);
  
    lightSprite.resize(lightSprite.getWidth()*(ofGetWidth()*2/lightSprite.getWidth()),lightSprite.getHeight()*(ofGetWidth()*2/lightSprite.getWidth()));
    lightSprite.setAnchorPoint(lightSprite.getWidth()/2, lightSprite.getHeight()/2);

   }

void Player::drawPlayer(){
    
    //If statements to determine whether to draw player in motion or stationary
    if (inMotion && !death){
        ofSetColor(255,255,255,trans);
        movement[currentDirection][int(currentFrame)].draw (pos.x, pos.y);
    }
    //sprites for right direction in different order to other directions so seperate if statement required
    else if (!inMotion && currentDirection == RIGHT &&  !death){
        ofPushMatrix();
            ofSetColor(255,255,255,trans);
            movement[currentDirection][8].draw (pos.x, pos.y);
        ofPopMatrix();
    }
    else if (!death)   {
        ofSetColor(255,255,255,trans);
        movement[currentDirection][0].draw (pos.x, pos.y);
    }
    if (death){
        for(int i = 0; i < 8; i++){
            playerDeath[i].cropFrom(movement[currentDirection][0], 0, 0, movement[currentDirection][0].getWidth(),movement[currentDirection][0].getHeight());
            playerDeath[i].setAnchorPoint(playerDeath[i].getWidth()/2, playerDeath[i].getHeight()/2);
        }
        if (deathFrame < 200){
            playerDeath[deathFrame/20].draw(pos.x, pos.y);
            deathFrame++;
        }

    }
    
    if(light == 1){
        darkSprite.draw(pos.x,pos.y);
    }
    else if(light == 2){
        lightSprite.draw(pos.x, pos.y);
    }
 
}

void Player::updatePlayer(int xDelta, int yDelta){
    
    //sets program to display images in order from 0-8
    //has effect of looping 'frames' of sprites and animating movement

    
    float count = currentFrame + 0.5;
    currentFrame = fmod(count, 8);
    inMotion = true;
    
    if (xDelta == 0 && yDelta == 0)
        inMotion = false;
    else if (xDelta == -2 || xDelta == -1)
        currentDirection = LEFT;
    else if (xDelta == 2 || xDelta == 1)
        currentDirection = RIGHT;
    else if (yDelta == -2 || yDelta == -1)
        currentDirection = UP;
    else if (yDelta == 2 || yDelta == 1)
        currentDirection = DOWN;
    
    //stops player movement when dead
    if (!death) {
        pos.x = pos.x + xDelta;
        pos.y = pos.y + yDelta;
    }
    
    //stops player from walking off screen
    if (isPlayerOffScreen()) {
        pos.x = pos.x - xDelta;
        pos.y = pos.y - yDelta;
    }
    
}

//boolean to test if player if player is on screen

bool Player::isPlayerOffScreen() {
    if (pos.x < 24 || pos.x > ofGetWidth() - 24 ||
        pos.y < 32 || pos.y > ofGetHeight() - 32)
        return true;
    return false;
}


int Player::whatEdge(){
    
    //Function to determine which edge of the window the player is currently at
    
    //TL = 0
    //TM = 1
    //TR = 2
    //ML = 3
    //MR = 4
    //BL = 5
    //BM = 6
    //BR = 7
    //MM = 8 - no action
    if(pos.y < 40){
        if(pos.x < 25)
            return 0; //TL
        if(pos.x >= 25 && pos.x <= ofGetWidth() - 25)
            return 1; //TM
        if(pos.x > ofGetWidth() - 25)
            return 2; //TR
            }
    if(pos.y >= 40 && pos.y <= ofGetHeight()-40){
        if(pos.x < 25)
            return 3; //ML
        if(pos.x > ofGetWidth()-25)
            return 4; //MR
            }
    if(pos.y > ofGetHeight() - 40){
        if(pos.x < 25)
            return 5; //BL
        if(pos.x >= 25 && pos.x <= ofGetWidth() - 25)
            return 6; //BM
        if(pos.x > ofGetWidth() - 25) //
            return 7; //BR
    }
    return 8;
}




