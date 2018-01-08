#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    gameState = 0;


  //-------------------------                   Game Variables                 --------------------///
    
    //LOAD CHARACTERS, CARDS AND LIGHT
    player.setupPlayer();
    player.light = 0;
    librarian[0].setupLibrarian();
    librarian[1].setupLibrarian();
    librarianDeath = false;
    card[0].setupCard();
    card[1].setupCard();
    torch = false;
    
    //SOUND VARIABLES -----------------------------------------------------------
       point.load("Coin.mp3");
    
    //Midi to Frequency
    a = 440;
    for (int i = 0; i < 128; i++) {
        
        midi[i] = 13.75 * pow(2.0,(i - 9.0) / 12.0);
        
    }
    
    
    //3 Available harmonic series' - harmonics 1-8 if 0, harmonics 1-15 (only odds) if 1, harmonics 1-14 (only evens excluding primary harmonic) if 3
    harmonicSeries = 0;
    oddSeries = false;
    
    m_sample_rate = 44100;
    m_buffer_size = 512;
    
    
    ofSoundStreamSetup(2, 0, m_sample_rate, m_buffer_size, 4);
    
    audioStartTime = ofGetElapsedTimeMillis();
    audioEndTime = 7000;
    
    for(int i = 0; i < 5; i++){
        
        frequency[i] = midi[(int)ofRandom(1, 127)];
        
        for(int j = 0; j < 7; j++){
            
            if(harmonicSeries <= 33){
                harmonics[i][j] = frequency[i] * (j+2);
            }
            else if(harmonicSeries > 33 && harmonicSeries <= 66){
                harmonics[i][j] = frequency[i] * ((2*j) + 3);
            }
            else {
                harmonics[i][j] = frequency[i] * ((2*j) + 2);
            }
        }
        
    }
    
    
    filtRise = true;
    filtFreq = 501;

  //----------------------------------
    //TIMER VARIABLES
    timerEnd = false;
    startTime = ofGetElapsedTimeMillis();
    timer = 0;
    absStart = ofGetElapsedTimeMillis();

    for(int i = 0; i < 50; i ++){
        
        myFont[i].init("Crow.ttf",20);
    }

    //LOAD MAZE
    maze1.load("newMaze.jpg");  //Plain black and white maze image created with recursive algorithm in processing
    map1.load("newMazeTex.png"); //Textured image layered on top of maze - created using photoshop
    map1.setAnchorPoint(map1.getWidth()/2, map1.getHeight()/2);
    maze1.setAnchorPoint(maze1.getWidth()/2, maze1.getHeight()/2);
    
    //DYNAMIC BACKGROUND VARIABLES
    anchorX = 0;
    anchorY = 0;
    exit.load("open.png");
    exitOpen = true;

    //MAZE POINTS
    white = (255); //FREE SPACE
    black = (0); //WALL
    
    
    //BATTERY AND BOOK VARIABLES
    battery.load("smallBattery.png");
    click.load("clickS.png");
    unclick.load("unclickS.png");
    for (int i = 0; i < 15; i++){
        batteries[i].setupBattery();
    }
    for (int i = 0; i < 5; i++){
        book[i].setupBook();
    }

    
    //SET MAZE WALLS
    for (int i = 0; i < maze1.getWidth(); i++){
        for (int j = 0; j < maze1.getHeight(); j++) {
            ofColor c = maze1.getColor(i,j);
            if(c == black){
                //can't pass through black lines
                collisionMap[i][j] = false;
            }
            else if(c == white){
                //can pass through white space
                collisionMap[i][j] = true;
            }
            else {
                //shouldn't be other colours in background maze
                maze1.setColor(i, j, black);
                collisionMap[i][j] = false;  //Only white space is able to be passed through
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    audioCurrentTime = ofGetElapsedTimeMillis() - audioStartTime;
    
    if(audioCurrentTime >= audioEndTime){
        
        harmonicSeries = ofRandom(100);
        
        
        //Loop backwards through array to set most recently selected frequencies last
        //Looping forwards would set every old frequency to the current frequency.
        //First set the prime frequency
        for(int i = 6; i > 0;  i--){
            
            for(int j = 0; j < 5; j++){
                
                oldFrequency[i][j] = oldFrequency[i-1][j];
                
                if(i == 1){
                    oldFrequency[0][j] = frequency[j];
                }
                
                
                //Set the harmonics
                for(int k = 0; k < 7; k++){
                    
                    if(harmonicSeries <= 33){
                        oldHarmonics[i][j][k] = oldFrequency[i][j] * (k+2);
                    }
                    else if(harmonicSeries > 33 && harmonicSeries <= 66) {
                        oldHarmonics[i][j][k] = oldFrequency[i][j] * ((2*k) + 3);
                    }
                    else
                        oldHarmonics[i][j][k] = oldFrequency[i][j] * ((2*k) + 2);
                }
            }
            
            
        }
        
        for (int i = 0; i < 5; i ++){
            
            //Change the current frequencies selected
            frequency[i] = midi[(int)ofRandom(1, 127)];
            
            //Set the harmonics
            for(int j = 0; j < 7; j++){
                
                if(harmonicSeries <= 33 ){
                    harmonics[i][j] = frequency[i] * (j+2);
                }
                else if(harmonicSeries > 33 && harmonicSeries <= 66){
                    harmonics[i][j] = frequency[i] * ((2*j) + 3);
                }
                else {
                    harmonics[i][j] = frequency[i] * ((2*j) + 2);
                    
                }
            }
            
            
        }
        
        audioStartTime = ofGetElapsedTimeMillis();
        
    }
    
    //Shift the lores filter frequency between 500 and 10000 Hz
    if(filtFreq <= 500){
        filtRise = true;
    }
    if(filtFreq >= 10000){
        filtRise = false;
    }
    if(filtRise == true){
        filtFreq +=3;
    }
    if(filtRise == false){
        filtFreq-=3;;
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    loadGameState();
    
}
//----------------------------------------------------------------------------------///

void ofApp::inRange(){
    
    //If player is in range of librarians' sight and doesn't have a library card then they are taken out
    
    librarian[0].drawLibrarian(ofGetWidth()/2 - 420 + anchorX, ofGetHeight()/2 - 920 + anchorY);
    librarian[1].drawLibrarian(ofGetWidth()/2 - 292 + anchorX, ofGetHeight()/2 + 840 + anchorY);

    card[0].drawCard(ofGetWidth()/2 + 690 + anchorX, ofGetHeight()/2 + 540 + anchorY);
    card[1].drawCard(ofGetWidth()/2 + 50 + anchorX, ofGetHeight()/2 - 900 + anchorY);
    
    for (int i = 0; i < 15; i++){
        if(card[i].playerCardCollision(player.pos.x, player.pos.y))
            card[i].cardCollected = true;
        }
    //Librarians can only attack to the left
    for(int i = 0; i < 2; i++){
    if (player.pos.x >= librarian[i].pos.x - 80 && player.pos.x <= librarian[i].pos.x && player.pos.y >= librarian[i].pos.y
        && player.pos.y <= librarian[i].pos.y + 40 && librarian[i].currentDirection == 1){
        librarian[i].attack = true;
        if(card[0].cardCollected == false && card[1].cardCollected == false)
        player.death = true;
        if(player.deathFrame >= 199){
            gameState = 2;
        }
    }
        if(librarian[i].lightFrame >= 49 && player.death == false)
            librarian[i].death = true;
        }
   
}

void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    
    
    //Play sound during gameplay and when player dead
    if(gameState == 1 || gameState == 2){
    
    double harmonicMix[5], oldHarmonicMix[7][5];
    double mixL[8], oldMixL[7], currentMixL, filtMixL;
    double mixR[8], oldMixR[7], currentMixR, filtMixR;
    double osc_value[5][8];
    double oldOsc_value[7][5][8];
    
    for (unsigned int i = 0; i < bufferSize; ++i) {
        
        for(int l = 0; l < 7; l++){
            for(int j = 0; j < 5; j++){
                
                //First the prime harmonic is set
                //Ensures osc_value is only set once
                if(l==0){
                    osc_value[j][0] = osc[j][0].sinewave(frequency[j]);
                }
                oldOsc_value[l][j][0] = oldOsc[l][j][0].sinewave(oldFrequency[l][j]);
                
                //Then the other harmonics in the selected series are set
                for(int k = 1; k < 8; k++){
                    
                    if(l == 0){
                        osc_value[j][k] = osc[j][k].sinewave(harmonics[j][k-1]);
                    }
                    oldOsc_value[l][j][k] = oldOsc[l][j][k].sinewave(oldHarmonics[l][j][k-1]);
                }
                
            }
        }
        
        for(int l = 0; l < 7; l++){
            for(int j = 0; j < 5; j++){
                //Harmonic mix of chord weights the prime frequency the highest
                if(l == 0){
                    harmonicMix[j] = osc_value[j][0] * 0.25; + osc_value[j][1] * 0.2 + osc_value[j][2] * 0.1 + osc_value[j][3] * 0.2 + osc_value[j][4] * 0.05 + osc_value[j][5] * 0.1 + osc_value[j][6] * 0.05 + osc_value[j][7] * 0.05;
                }
                
                oldHarmonicMix[l][j] = oldOsc_value[l][j][0] * 0.25; + oldOsc_value[l][j][1] * 0.2 + oldOsc_value[l][j][2] * 0.1 + oldOsc_value[l][j][3] * 0.2 + oldOsc_value[l][j][4] * 0.05 + oldOsc_value[l][j][5] * 0.1 + oldOsc_value[l][j][6] * 0.05 + oldOsc_value[l][j][7] * 0.05;
            }
        }
        
        for(int j = 1; j < 8; j++){
            //Equal mix of all sine tones in the mix
            mixL[j] = oldHarmonicMix[j][0] * 0.2 + oldHarmonicMix[j][1] * 0.2 + oldHarmonicMix[j][2] * 0.2 + oldHarmonicMix[j][3] * 0.2 + oldHarmonicMix[j][4] * 0.2;
            
            mixR[j] = oldHarmonicMix[j][0] * 0.2 + oldHarmonicMix[j][1] * 0.2 + oldHarmonicMix[j][2] * 0.2 + oldHarmonicMix[j][3] * 0.2 + oldHarmonicMix[j][4] * 0.2;
            
        }
        
        
        mixL[0] = harmonicMix[0] * 0.2 + harmonicMix[1] * 0.2 + harmonicMix[2] * 0.2 + harmonicMix[3] * 0.2 + harmonicMix[4] * 0.2;
        mixR[0] = harmonicMix[0] * 0.2 + harmonicMix[1] * 0.2 + harmonicMix[2] * 0.2 + harmonicMix[3] * 0.2 + harmonicMix[4] * 0.2;
        
        
        //Each frequency that gets selected plays a total of 8 times - getting quiter each iteration
        currentMixL = mixL[0] * 0.3 + mixL[1] * 0.25 + mixL[2] * 0.2 + mixL[3] * 0.1 + mixL[4] * 0.07 + mixL[5] * 0.04 + mixL[6] * 0.025 + mixL[7] * 0.015;
        currentMixR = mixR[0] * 0.3 + mixR[1] * 0.25 + mixR[2] * 0.2 + mixR[3] * 0.1 + mixR[4] * 0.07 + mixR[5] * 0.04 + mixR[6] * 0.025 + mixR[7] * 0.015;
        
        
        filtMixL = filter.lores(currentMixL, filtFreq, 5);
        filtMixR = filter.lores(currentMixR, filtFreq, 5);
        
        output[i * nChannels] = filtMixL;
        output[i * nChannels + 1] = filtMixR;
        
        }
    }
    
}

//----------------------------------------------------------------------------------///
void ofApp::charge(){
    
//BATTERY POSITIONS ======> (OFGETWIDTH()/2, OFGETHEIGHT()/2) = (1000,1000) on maze <========
    
    batteries[0].drawBattery(ofGetWidth()/2 - 140 + anchorX, ofGetHeight()/2 - 900 + anchorY); // (860, 100) y
    batteries[1].drawBattery(ofGetWidth()/2 - 150 + anchorX, ofGetHeight()/2 - 580 + anchorY); // (850, 420) y
    batteries[2].drawBattery(ofGetWidth()/2 + 530 + anchorX, ofGetHeight()/2 - 580 + anchorY); //(1530, 420) y
    batteries[3].drawBattery(ofGetWidth()/2 + 845 + anchorX, ofGetHeight()/2 - 460 + anchorY); // (1845, 540) y
    batteries[4].drawBattery(ofGetWidth()/2 - 600 + anchorX, ofGetHeight()/2 - 260 + anchorY); // (400, 740) y
    batteries[5].drawBattery(ofGetWidth()/2 + 375 + anchorX, ofGetHeight()/2 - 250 + anchorY); // (1375, 750) y
    batteries[6].drawBattery(ofGetWidth()/2 - 110 + anchorX, ofGetHeight()/2 - 125 + anchorY); // (890, 875) y
    batteries[7].drawBattery(ofGetWidth()/2 - 760 + anchorX, ofGetHeight()/2 + 190 + anchorY); // (240, 1190) y
    batteries[8].drawBattery(ofGetWidth()/2 - 440 + anchorX, ofGetHeight()/2 + 190 + anchorY); // (560, 1190) y
    batteries[9].drawBattery(ofGetWidth()/2 + 545 + anchorX, ofGetHeight()/2 + 350 + anchorY); // (1545, 1350) y
    batteries[10].drawBattery(ofGetWidth()/2 + 220 + anchorX, ofGetHeight()/2 + 660 + anchorY); // (1220, 1660) y
    batteries[11].drawBattery(ofGetWidth()/2 - 760 + anchorX, ofGetHeight()/2 + 860 + anchorY); // (240, 1860) y
    batteries[12].drawBattery(ofGetWidth()/2 + 360 + anchorX, ofGetHeight()/2 + 860 + anchorY); // (1360, 1860) y
    batteries[13].drawBattery(ofGetWidth()/2 + 850 + anchorX, ofGetHeight()/2 + 880 + anchorY); // (1850, 1880) y
    batteries[14].drawBattery(ofGetWidth()/2 - 450 + anchorX, ofGetHeight()/2 + 725 + anchorY); // (550, 1725) y
    
    
    //When Player collect's battery charge
    for (int i = 0; i < 15; i++){
    if(batteries[i].playerBatteryCollision(player.pos.x, player.pos.y)){
        batteries[i].batteryUsed = true;
        startTime = ofGetElapsedTimeMillis();
        point.play();
        
        if(timerEnd == true){
            timerEnd = false;
        }
        
        }
    }
    

    if(torch == true){
        click.draw(3, ofGetHeight() - 45);
        timer = ofGetElapsedTimeMillis() - startTime - pauseTime;
        if (timerEnd == false)
            player.light = 2;      }
    else {
        unclick.draw(3, ofGetHeight() - 45);
        pauseTime = ofGetElapsedTimeMillis() - timer;
        player.light = 1;
    }
    
    if (timer >= 30000){
        timerEnd = true;
        player.light = 1;
    }
    
    float timerBar = ofMap(timer, 0.0, 30000, -1.0, 0.0, true);

//Battery Timer Bar
    ofPushMatrix();
    if (timerBar < -0.6)
        ofSetColor(0,255,0);
    else if(timerBar >= -0.6 && timerBar < -0.25)
        ofSetColor(255, 150, 0);
    else
        ofSetColor(255, 0, 0);
    
    ofDrawRectangle(8, 34, 16, 25*timerBar);
    ofSetColor(255);
    ofPopMatrix();
    
    //PLayer dies if in the maze longer than 10 minutes
    int absTime = ofGetElapsedTimeMillis()/1000 - absStart;
    if(absTime >= 600){
        player.death = true;
        if(player.deathFrame >= 199)
            gameState = 2;
        
    }
    
}
//---------------------------------------------------------------------------------///
void ofApp::bookScore(){
    
    book[0].drawBook(ofGetWidth()/2 + 680 + anchorX, ofGetHeight()/2 - 760 + anchorY);
    book[1].drawBook(ofGetWidth()/2 + 820 + anchorX, ofGetHeight()/2 - 100 + anchorY);
    book[2].drawBook(ofGetWidth()/2 - 755 + anchorX, ofGetHeight()/2 + 550 + anchorY);
    book[3].drawBook(ofGetWidth()/2 + 40 + anchorX, ofGetHeight()/2 + 850 + anchorY);
    book[4].drawBook(ofGetWidth()/2 + 690 + anchorX, ofGetHeight()/2 + 855 + anchorY);
    
    
    for (int i = 0; i < 5; i++){
        if(book[i].playerBookCollision(player.pos.x, player.pos.y)){
            book[i].bookCollected = true;
            
        }
    }

    //If statement to open Exit if any 2 (or more) books have been collected
    if((book[0].bookCollected && book[1].bookCollected) || (book[0].bookCollected && book[2].bookCollected) || (book[0].bookCollected && book[3].bookCollected) || (book[0].bookCollected && book[4].bookCollected) || (book[1].bookCollected && book[2].bookCollected) || (book[1].bookCollected && book[3].bookCollected) || (book[1].bookCollected && book[4].bookCollected) || (book[2].bookCollected && book[3].bookCollected) || (book[2].bookCollected && book[4].bookCollected) || (book[3].bookCollected && book[4].bookCollected)) {
            exit.load("open.png");
        exitOpen = true;
    }
   exit.draw(ofGetWidth()/2 - 516 + anchorX, ofGetHeight()/2 - 827 + anchorY);
    if(exitOpen && player.pos.x - 10 >= ofGetWidth()/2 - 516 + anchorX  && player.pos.x + 10 <= ofGetWidth()/2 - 516 + anchorX + exit.getWidth() && player.pos.y - 20 >= ofGetHeight()/2 - 827 + anchorY && player.pos.y + 20 <= ofGetHeight()/2 - 827 + anchorY + exit.getHeight()){
        player.trans-=2;
    //Reset Player and Map Positions, Reclose exit and Load Win Screen.
    if(player.trans <= 0){
        anchorX = 0;
        anchorY = 0;
        player.pos = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
        exitOpen = false;
            gameState = 3;
        }
    }
}
//---------------------------------------------------------------------------------///
void ofApp::moveMaze(){
    
    int  xDelta = 0;
    int  yDelta = 0;
    //Using boolean array for movement allows multiple keys to be registered simultaneuosly
    //Benefit of this is diagonal movement enabled
    
    int allTrue = 0;
    
    //W to move up
    if (keys ['w']){
        
        //nested for loops ensure movement is only possible if, after movement, all values of player position are in white space.
        //Lines of maze are black and the 0 pixel registers as a solid 'wall' that the player can't cross
        for(int i = (int)player.pos.x - 10  + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i < (int)player.pos.x + 10 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i++){
            for(int j = (int)player.pos.y - 22 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j < (int)player.pos.y + 18 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j++){
                if(collisionMap[i][j]){
                    allTrue++;
                }
                else if(collisionMap[i][j] == false){
                    allTrue = 0;
                    break;
                }
            }
        }
        if(allTrue >= 100){
            yDelta-=2;
            allTrue = 0;
        }
    }
    
    //S to move down
    if (keys['s']){
        
        for(int i = (int)player.pos.x - 10 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i < (int)player.pos.x + 10 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i++){
            for(int j = (int)player.pos.y - 18 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j < (int)player.pos.y + 22 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j++){
                if(collisionMap[i][j]){
                    allTrue++;
                }
                else if(collisionMap[i][j] == false){
                    allTrue = 0;
                    break;
                }
            }
        }
        if(allTrue >= 100){
            yDelta+=2;
            allTrue = 0;
        }
    }
    
    //A to Move left
    if (keys['a']){
        for(int i = (int)player.pos.x - 12 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i < (int)player.pos.x + 8 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i++){
            for(int j = (int)player.pos.y - 20 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j < (int)player.pos.y + 20 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j++){
                
                if(collisionMap[i][j]){
                    allTrue++;
                }
                else if(collisionMap[i][j] == false){
                    allTrue = 0;
                    break;
                }
            }
        }
        if(allTrue >= 100){
            xDelta-=2;
            allTrue = 0;
        }
    }
    
    //D to move right
    if (keys['d']){
        
        for(int i = (int)player.pos.x - 8 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i < (int)player.pos.x + 12 + maze1.getWidth()/2 - ofGetWidth()/2 - anchorX; i++){
            for(int j = (int)player.pos.y - 20 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j < (int)player.pos.y + 20 + maze1.getHeight()/2 - ofGetHeight()/2 - anchorY; j++){
                if(collisionMap[i][j]){
                    allTrue++;
                }
                else if(collisionMap[i][j] == false){
                    allTrue = 0;
                    break;
                }
            }
        }
        if(allTrue >= 100){
            xDelta+=2;
            allTrue = 0;
        }
    }
    player.updatePlayer(xDelta, yDelta);
    
    //Maze is larger than window size - Below are some if statements to move the window if the player is moving and also close to the window edge
     if(player.inMotion){
        if(player.whatEdge() == 0 && (ofGetHeight()/2 + anchorY < 1000 && ofGetWidth()/2 + anchorX < 1000)) { //TL
            anchorX+=2;
            anchorY+=2;
        }
        if((player.whatEdge() == 1 && ofGetHeight()/2 + anchorY < 1000) ||
           (player.whatEdge() == 0 && ofGetWidth()/2 + anchorX > 1000) || (player.whatEdge() == 2 &&  ofGetWidth()/2 +anchorX < -600)){ //TM
            if(player.currentDirection == 0){
            anchorY+=2;
            }
        }
        if(player.whatEdge() == 2 && (ofGetHeight()/2 + anchorY < 1000 && ofGetWidth()/2 + anchorX > -600)){   //TR
            anchorX-=2;
            anchorY+=2;
        }
        if((player.whatEdge() == 3 && ofGetWidth()/2 + anchorX < 1000) ||
           (player.whatEdge() == 0 && ofGetHeight()/2 + anchorY > 1000) || (player.whatEdge() == 5 &&  ofGetHeight()/2 +anchorY < -850)){ //ML
            if(player.currentDirection == 1){
            anchorX+=2;
            }
        }
        if((player.whatEdge() == 4 && ofGetWidth()/2 + anchorX > -600) ||
           (player.whatEdge() == 2 && ofGetHeight()/2 + anchorY > 1000) || (player.whatEdge() == 7 &&  ofGetHeight()/2 +anchorY < -850)){//MR
            if(player.currentDirection == 3){
            anchorX-=2;
            }
        }
        if(player.whatEdge() == 5 && (ofGetHeight()/2 + anchorY > -850 && ofGetWidth()/2 + anchorX < 1000)){ //BL
            anchorX+=2;
            anchorY-=2;
        }
        if((player.whatEdge() == 6 && ofGetHeight()/2 + anchorY > -850) ||
           (player.whatEdge() == 5 && ofGetWidth()/2 + anchorX < -600) || (player.whatEdge() == 7 &&  ofGetWidth()/2 +anchorX > 1000)){//BM
            if(player.currentDirection == 2){
            anchorY-=2;
            }
        }
        if(player.whatEdge() == 7 && (ofGetHeight()/2 + anchorY > -850 && ofGetWidth()/2 + anchorX > -600)){ //BR
            anchorX-=2;
            anchorY-=2;
        }
     }
}

//---------------------------------------------------------------------------------///
void ofApp::loadGameState(){
  
   // 0 - Camera
    // 1 - Menu
    // 2 - GW Intro
    // 3 - GW Game
    // 4 - GW Death
    // 5 - GW Win
    // 6 - DM Intro - 0
    // 7 - DM Game - 1
    // 8 - DM Death - 2
    // 9 - DM Win  - 3

    //Quickly Select between intro and gameplay for testing
        if(keys ['0'])
            gameState = 0;
        if(keys['1'])
            gameState = 1;
    

    
    //DARK MAZE INTRO
    if(gameState == 0){
        int h = 5;
        myFont[0].setText("You are Stuck in a Labyrinthine Library");
        myFont[1].setText("It Is Closing In 10 Minutes");
        myFont[2].setText("Find 2 of the Books you Borrowed");
        myFont[3].setText("Use Space to Operate Your Torch");
        myFont[4].setText("Don't Run Out of Battery");
        myFont[5].setText("Collect Charges to Keep Your Torch Alive");
        myFont[6].setText("Avoid the Librarians");
        myFont[7].setText("Unless you Have your Library Card");
        myFont[8].setText("Press M to Begin");

        for (int i = 0; i < 8; i++){
            
        myFont[i].drawCenter(ofGetWidth()/2, h);
            
            h+=40;
        }
    }
    
    //DARK MAZE GAMEPLAY
    if(gameState == 1){
        
        map1.draw(ofGetWidth()/2 + anchorX, ofGetHeight()/2 + anchorY);
        inRange();
        player.drawPlayer();
        charge();
        battery.draw(0,2);
        moveMaze();
        bookScore();

    }
    //DM DEATH
    if (gameState == 2) {
        int h = 5;
        myFont[0].setText("You Did Not Make It Out");
        myFont[1].setText("GAME OVER");
        myFont[2].setText("Did You Lose Your Way?");
        myFont[3].setText("Did You Forget Your Card?");
        myFont[4].setText("Press M to Try Again");

        for(int i = 0; i < 5; i++){
        myFont[i].drawCenter(ofGetWidth()/2, h);
            h+=40;
        }
    }
    //DM WIN
    if (gameState == 3){
        myFont[0].setText("GAMESTATE 3");
        myFont[0].drawCenter(ofGetWidth()/2, ofGetHeight()/2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    keys[key] = true;
    
    if(keys [' '] && gameState == 1){
            torch = !torch;
        }
    //Press M at anytime to reload the gameplay portion
    if(keys ['m']){
        gameState = 1;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    keys[key] = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
