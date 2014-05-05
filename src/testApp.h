#pragma once

#include "ofMain.h"
#include "Boid.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        Flock boids;	
    
        ofTrueTypeFont ver;
    
    bool isFullScreen;
    
    void drawFPS(vector<Boid*> boids);
    
    
    
    
    bool debugDraw;
    ofxPanel gui;
    ofxLabel gui_boids;
    ofxFloatSlider gui_size;
    ofxFloatSlider gui_gravity;
    ofxFloatSlider gui_forceMax;
    ofxFloatSlider gui_speedMax;
    ofxToggle gui_drawArrow;
    ofxToggle gui_applyGravity;
    ofxToggle gui_move2origin;
    ofxToggle gui_showFPS;
    
    ofxLabel gui_alignment;
    ofxToggle gui_drawAlignment;
    ofxFloatSlider gui_weightAlignment;
    ofxFloatSlider gui_radiusAlignment;
    
    ofxLabel gui_cohesion;
    ofxToggle gui_drawCohesion;
    ofxFloatSlider gui_weightCohesion;
    ofxFloatSlider gui_radiusCohesion;
    
    ofxLabel gui_separation;
    ofxToggle gui_drawSeparation;
    ofxFloatSlider gui_weightSeparation;
    ofxFloatSlider gui_radiusSeparation;
    
    ofxLabel gui_debug;
    
    
    
    
    ofImage img;
    ofImage blob;
    int *mat;
    int auxaux;
    
    linkedList targets;
   
    
};
