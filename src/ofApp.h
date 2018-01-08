#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "Player.hpp"
#include "ofxOpenCv.h"
#include "ofxTextSuite.h"
#include "Battery.hpp"
#include "Librarian.hpp"
#include "Book.hpp"
#include "Card.hpp"
#include <iostream>
#include <fstream>
#include <string>



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //---------Variables for Games ------------//
    
    //functions in main class
    void loadGameState();
    void moveMaze();
    void charge();
    void inRange();
    void bookScore();
    void audioOut(float *output, int bufferSize, int nChannels);
    
    //Load graphical variables
    ofImage maze1, map1;
    ofColor white, black;
    bool collisionMap [4*500][4*500];
    Player player;
    ofImage unlock;
    ofImage battery, click, unclick;
    ofImage exit;
    float anchorX, anchorY;
    Librarian librarian [2];
    Battery batteries [15];
    Book book [5];
    Card card [2];
    
    
    //Text and Sound Variables
    ofSoundPlayer point;
    ofxTextBlock  myFont [50];

    
    //Generative Audio Variables
    maxiOsc osc [5][8], oldOsc[7][5][8];  //Osc contains 5 prime harmonic frequencies with 7 harmonics for each of them
                                        //Old osc contains previous 7 chords and carries on playing them quiter and quiter each iteration
    maxiFilter filter;
    
    
    unsigned int m_sample_rate;
    unsigned int m_buffer_size;
    
    int audioStartTime, audioEndTime, audioCurrentTime, filtFreq;
    
    bool oddSeries, filtRise;
    
    int harmonicSeries;
    
    float frequency[5], harmonics[5][7], oldFrequency[7][5], oldHarmonics[7][5][7];
    float midi[128];
    int a;
    
    
    //Gameplay related variables
    bool torch;
    int score;
    bool keys [128];
    string scoreString;
    bool exitOpen;
    int librarianDeath;
    
    //0 = INTRO, 1 = GAMEPLAY, 2 = DEATH, 3 - WIN
    int gameState;
    
    
    //Game Timer Variables
    float startTime;
    float pauseTime;
    bool timerEnd;
    float timer;
    float absStart;


};
