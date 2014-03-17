#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    
    
    ofBackground(255);
    //load our typeface
    ver.loadFont("verdana.ttf", 10);
    
    for (int i = 0; i < 900; i++)
    {
        boids.addBoid(ofGetWidth()/2, ofGetHeight()/2);
    }
    
    
    gui.setup();
    gui.add(gui_boids.setup("Boids", ""));
    gui.add(gui_applyGravity.setup("Gravity", true));
    gui.add(gui_gravity.setup("Gravity Force", 0.0004f, 0.0001f, 0.0005f));
    gui.add(gui_size.setup("Size", 5, 2, 7));
    gui.add(gui_forceMax.setup("Max Force", 0.35f, 0.01f, 1));
    gui.add(gui_speedMax.setup("Max Speed", 4.6f, 1, 10));
   
    gui.add(gui_separation.setup("", ""));
    gui.add(gui_separation.setup("Separation", ""));
    gui.add(gui_drawSeparation.setup("draw radius", false));
    gui.add(gui_weightSeparation.setup("weight", 0.6f, 0, 1));
    gui.add(gui_radiusSeparation.setup("radius", 5.5f, 0, 25));
    
    gui.add(gui_alignment.setup("", ""));
    gui.add(gui_alignment.setup("Alignment", ""));
    gui.add(gui_drawAlignment.setup("draw radius", false));
    gui.add(gui_weightAlignment.setup("weight", 0.15f, 0, 1));
    gui.add(gui_radiusAlignment.setup("radius", 12, 0, 25));
    
    gui.add(gui_cohesion.setup("", ""));
    gui.add(gui_cohesion.setup("Cohesion", ""));
    gui.add(gui_drawCohesion.setup("draw radius", false));
    gui.add(gui_weightCohesion.setup("weight", 0.4f, 0, 1));
    gui.add(gui_radiusCohesion.setup("radius", 19, 0, 25));
    
    gui.add(gui_debug.setup("", ""));
    gui.add(gui_debug.setup("Debug", ""));
    gui.add(gui_showFPS.setup("Show FPS", true));
    gui.add(gui_drawArrow.setup("Direction arrow", false));
    

    
    
    

    
}

//--------------------------------------------------------------
void testApp::update()
{
    boids.gui_update(gui_size, gui_forceMax, gui_speedMax, gui_drawArrow, gui_drawSeparation, gui_drawAlignment, gui_drawCohesion, gui_weightSeparation, gui_weightAlignment, gui_weightCohesion, gui_radiusSeparation, gui_radiusAlignment, gui_radiusCohesion, gui_applyGravity, gui_gravity);
    boids.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    boids.draw();
    
    
    //ver.drawString(ofToString(ofGetMouseY()), 100, 100);
    
    gui.draw();
    
    if (gui_showFPS) drawFPS(boids.boids);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if(key == 'f')
    {
		isFullScreen = !isFullScreen;
		if(!isFullScreen) ofSetFullscreen(false);
        else ofSetFullscreen(true);
	}
    
    if (key == 'a')
    {
        boids.addBoid(mouseX, mouseY, ofColor(0));
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    boids.addBoid(mouseX, mouseY, ofColor(0));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::drawFPS(vector<Boid*> boids)
{
    ofPushStyle();
    
    int w = 150;
    int h = 40;
    ofPoint p1;
    
    p1.set(ofGetWidth() - w + 5, ofGetHeight() - h + 14);
    
    ofSetColor(0, 100);
    ofRect(ofGetWidth() - w - 3, ofGetHeight() - h - 3, w, h);
    ofSetColor(255);
    ver.drawString("FPS: " + ofToString(ofGetFrameRate()), p1.x, p1.y);
    ver.drawString("No. boids: "+ ofToString(boids.size()), p1.x, p1.y+13);
    
    ofPopStyle();
}
