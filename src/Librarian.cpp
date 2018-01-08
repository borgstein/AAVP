//
//  Librarian.cpp
//  darkMazeMac
//
//  Created by Leo Petrokofsky on 05/01/2018.
//
//

#include "Librarian.hpp"

void Librarian::setupLibrarian() {
    
    speed = 0;
    attack = false;
    death = false;
    currentDirection = 0;
    moveFrame = 0;
    attackFrame = 0;
    deathFrame = 0;
    lightFrame = 0;
    librarianSprites.load("gilbert.png");
    lightning.load("smallLight.png");
    setupSprites();
}

void Librarian::setupSprites(){
    
    //Gets sprites for librarian lightning attack
    lightAttack[0].cropFrom(lightning, 0, 0, 24, lightning.getHeight());
    lightAttack[1].cropFrom(lightning, 29, 0, 49, lightning.getHeight());
    lightAttack[2].cropFrom(lightning, 79, 0, 78, lightning.getHeight());
    lightAttack[3].cropFrom(lightning, 158, 0, 66, lightning.getHeight());
    lightAttack[4].cropFrom(lightning, 227, 0, 35, lightning.getHeight());
    lightAttack[5].cropFrom(lightning, 264, 0, 11, lightning.getHeight());
    
    //EACH SPRITE IS CONTAINED IN BOX 48 x 48
    //Sets librarian movement sprites
    for (int i = 0; i < 5; i ++){
        librarianMovement[0][i].cropFrom(librarianSprites, 59 + 48 * i, 9, 25, 39);
        librarianMovement[1][i].cropFrom(librarianSprites, 59 + 48 * i, 57, 25, 39);
        
    }
    //sets sprites for Librarian Death
    for (int i = 0; i < 8; i++) {
        librarianDeath[i].cropFrom(librarianSprites, 48 * i, 9 * 48, 48, 48);
    }
    
    //Sets sprites for Librarian Attack
    for(int i = 0; i < 4; i++){
        librarianAttack[i].cropFrom(librarianSprites, 48 * i, 7 * 48, 48, 48);
    }
    
}

void Librarian::drawLibrarian(int posX, int posY){
    
    pos.x = posX;
    pos.y = posY;
    
    updateLibrarian();
    
    //Calls librarian movement
    if(!attack && !death) {
        librarianMovement[currentDirection][moveFrame/10].draw (pos.x, pos.y);
        
    }
    //Calls librarian attack
   else if (attack == true  && death == false) {
        librarianAttack[attackFrame/50].draw (pos.x + speed, pos.y + speed/2);
    }
    //Calls librarian death
    else {
        librarianDeath[deathFrame/25].draw (pos.x + speed, pos.y + speed/2);
    }
}

void Librarian::updateLibrarian(){
    
    //PACING
    if(!attack && !death){
        pos.x = pos.x + speed;
        pos.y = pos.y + speed/4;
        int count = moveFrame + 1;
        moveFrame = fmod(count, 40);
    }
    
    //ATTACKING
     else if (attack && !death) {
        if(attackFrame < 140)
            attackFrame++;
        if(attackFrame >= 140){
            attackFrame = 141;
        if(lightFrame < 50){
            lightAttack[lightFrame/10].draw(pos.x + speed - lightAttack[lightFrame/10].getWidth(), pos.y + 10 + speed/3);
            lightFrame++;
          }
        }
    }
    //Animates librarian DEATH
    else {
        if(deathFrame < 180)
        deathFrame++;
        if(deathFrame >= 180)
            deathFrame = 181;
    }
    //Movement speed and positions for librarian
if(!attack && ! death) {
    if (speed < 120 && currentDirection == 0){
        speed+=2;
        }
    if (speed >= 120){
        currentDirection = 1;
        speed-=2;
        }
    if (speed >= -120 && currentDirection == 1){
        speed-=2;
        }
    if (speed <= -120){
        currentDirection = 0;
        speed+=2;
        }
    }
}







