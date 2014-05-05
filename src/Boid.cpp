#include "Boid.h"

void Boid::draw(ofImage *img, int *mat, linkedList targets)
{
    ofPushMatrix();
    
        ofTranslate(position.x, position.y);
        ofRotate(-ofRadToDeg(velocity.angleRad(ofVec2f(1, 0)) - ofDegToRad(90)));
    
    
    
    

    

        if (debug_drawRadiusCohesion)
        {
            ofPushStyle();
            ofSetColor(255, 0, 0, 100);
            ofCircle(0, 0, radiusCohesion);
            ofPopStyle();
        }
        if (debug_drawRadiusAlignment)
        {
            ofPushStyle();
            ofSetColor(0, 255, 0, 100);
            ofCircle(0, 0, radiusAlignment);
            ofPopStyle();
        }
        if (debug_drawRadiusSeparation)
        {
            ofPushStyle();
            ofSetColor(0, 0, 255, 100);
            ofCircle(0, 0, radiusSeparation);
            ofPopStyle();
        }
    
        ofPushStyle();
        if (!(position.x < 0 || position.x > ofGetHeight() * img->width/img->height || position.y < 0 || position.y > ofGetHeight()))
        {
            color = img->getColor(ofMap(position.x, 0, ofGetHeight() * img->width/img->height, 0, img->width), ofMap(position.y, 0, ofGetHeight(), 0, img->height));
            color = ofColor(0);
            int index = (int)ofMap(position.x, 0, ofGetHeight() * img->width/img->height, 0, img->width) + (int)ofMap(position.y, 0, ofGetHeight(), 0, img->height) * img->width;
            mat[index] = 1;
            targets.deleteMatch(index);
//            ofPushStyle();
//            ofSetColor(img.getColor(i, j));
//            ofSetRectMode(OF_RECTMODE_CENTER);
//            ofRect(ofMap(i, 0, img.width, 0, ofGetHeight() * img.width/img.height), ofMap(j, 0, img.height, 0, ofGetHeight()),10, 10);
//            //ofCircle(ofMap(i, 0, img.width, 0, ofGetHeight() * img.width/img.height), ofMap(j, 0, img.height, 0, ofGetHeight()), 5);
//            ofPopStyle();
        }
    
    
        else color = ofColor(0);
        ofSetColor(color);
        //ofCircle(0,0,size);
        ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(0, 0, size, size);
//        ofBeginShape();
//            ofVertex(0, -size * 1.3f);
//            ofVertex(-size, size * 1.3f);
//            ofVertex(size, size * 1.3f);
//        ofEndShape();
    
        ofPopStyle();
    
        ofPushStyle();
        ofSetColor(0);
    
        if (debug_drawArrow)
        {
            ofLine(0, 0, 0, -size*5);
            
            ofPushMatrix();
            ofTranslate(0, -size*5);
            ofBeginShape();
            ofVertex(0, -size/2);
            ofVertex(-size/4, size/2);
            ofVertex(size/4, size/2);
            ofEndShape();
            ofPopMatrix();
        }
    
        ofPopStyle();

    ofPopMatrix();
}

void Boid::update(vector <Boid *> boids)
{

    if (move2origin)
    {
        if ((position-origin).length() <= size)
        {
            position = origin;
            velocity.set(0);
        }
        else pull2point(origin, 1);
    }
    else
    {
        /********************************/
        ofVec2f sep, ali, coh;
        forces(boids, &sep, &ali, &coh);
        //sep = separation(boids);
        //ali = alignment(boids);
        //coh = cohesion(boids);
        
        sep *= weightSeparation;
        ali *= weightAlignment;
        coh *= weightCohesion;
        
        acceleration += sep;
        acceleration += ali;
        acceleration += coh;
        /********************************/
        
        //if (gravity) pullToCenter(ofVec2f(ofGetWidth()/2, ofGetHeight()/2));
        if (gravity) pullToCenter(ofVec2f(ofGetMouseX(), ofGetMouseY()));
        //followMouse();
        borders();
    }
    
    // Update velocity
    velocity += acceleration;
    // Limit speed
    velocity.limit(maxSpeed);
    position += velocity;
    // Reset accelertion to 0 each cycle
    acceleration *= 0;
}

void Boid::borders()
{
    if (position.x <  -size) position.x = ofGetWidth() + size;
    if (position.y <  -size) position.y = ofGetHeight() + size;
    if (position.x > ofGetWidth() + size) position.x =  -size;
    if (position.y > ofGetHeight() + size) position.y =  -size;
}

void Boid::followMouse()
{
    acceleration =  ofVec2f(ofGetMouseX(), ofGetMouseY()) - position;
}

// A method that calculates and applies a steering force towards a target
// STEER = DESIRED MINUS VELOCITY
ofVec2f Boid::seek(ofVec2f target)
{
    ofVec2f desired = target - getPosition();  // A vector pointing from the location to the target
    // Scale to maximum speed
    //desired.normalize();
    //desired *= maxSpeed;
    
    // Above two lines of code below could be condensed with new PVector setMag() method
    // Not using this method until Processing.js catches up
     desired.getScaled(maxSpeed);
    
    // Steering = Desired minus Velocity
    ofVec2f steer = desired - velocity;
    steer.limit(maxForce);  // Limit to maximum steering force
    return steer;
}


// Separation
// Method checks for nearby boids and steers away
ofVec2f Boid::separation (vector <Boid *> boids) {
    float desiredseparation = radiusSeparation;
    ofVec2f steer = ofVec2f(0, 0);
    int count = 0;
    // For every boid in the system, check if it's too close
    for (Boid *other : boids)
    {
        float d = position.distance(other->getPosition());
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        if ((d > 0) && (d < desiredseparation))
        {
            // Calculate vector pointing away from neighbor
            ofVec2f diff = position - other->getPosition();
            diff.normalize();
            diff /= d;        // Weight by distance
            steer += diff;
            count++;            // Keep track of how many
        }
    }
    // Average -- divide by how many
    if (count > 0) {
        steer /= (float)count;
    }
    
    // As long as the vector is greater than 0
    if (steer.length() > 0) {
        // First two lines of code below could be condensed with new PVector setMag() method
        // Not using this method until Processing.js catches up
         steer = steer.getScaled(maxSpeed);
        
        // Implement Reynolds: Steering = Desired - Velocity
        //steer.normalize();
        //steer *= maxSpeed;
        steer -= velocity;
        steer.limit(maxForce);
    }
    return steer;
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
ofVec2f Boid::alignment(vector <Boid *> boids)
{
    float neighbordist = radiusAlignment;
    ofVec2f sum = ofVec2f(0, 0);
    int count = 0;
    for (Boid *other : boids) {
        float d = position.distance(other->getPosition());
        if ((d > 0) && (d < neighbordist)) {
            sum += other->getVelocity();
            count++;
        }
    }
    if (count > 0) {
        sum /= (float)count;
        // First two lines of code below could be condensed with new PVector setMag() method
        // Not using this method until Processing.js catches up
         sum.getScaled(maxSpeed);
        
        // Implement Reynolds: Steering = Desired - Velocity
        //sum.normalize();
        //sum.mult(maxspeed);
        ofVec2f steer = sum - velocity;
        steer.limit(maxForce);
        return steer;
    }
    else {
        return ofVec2f(0, 0);
    }
}

// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
ofVec2f Boid::cohesion (vector <Boid *> boids) {
    float neighbordist = radiusCohesion;
    ofVec2f sum = ofVec2f(0, 0);   // Start with empty vector to accumulate all locations
    int count = 0;
    for (Boid *other : boids)
    {
        float d = position.distance(other->getPosition());
        if ((d > 0) && (d < neighbordist)) {
            sum += other->getPosition(); // Add location
            count++;
        }
    }
    if (count > 0) {
        sum /= count;
        return seek(sum);  // Steer towards the location
    }
    else {
        return ofVec2f(0, 0);
    }
}

void Boid::pullToCenter(ofVec2f center)
{
	ofVec2f dirToCenter = position - center;
	float distToCenter = dirToCenter.length();
	float maxDistance = ofGetHeight()/4;
	
	if (distToCenter > maxDistance ){
		dirToCenter.normalize();
		float pullStrength = gravityForce;
		velocity -= dirToCenter * ( ( distToCenter - maxDistance ) * pullStrength );
	}
}

void Boid::pull2point(ofVec2f p, float maxDistance)
{
	ofVec2f dirToPoint = position - p;
	float distToPoint = dirToPoint.length();
	
	if (distToPoint > maxDistance)
    {
		dirToPoint.normalize();
		float pullStrength = 0.05f;
		velocity -= dirToPoint * ( ( distToPoint - maxDistance ) * pullStrength );
	}
}

//void Boid::kill(vector <Boid *> boids)
//{
//    this->~Boid();
//}

ofVec2f Boid::getPosition()
{
    return position;
}

ofVec2f Boid::getVelocity()
{
    return velocity;
}

void Boid::setSize(float newSize)
{
    size = newSize;
}

void Boid::setMaxForce(float newMaxForce)
{
    maxForce = newMaxForce;
}

void Boid::setMaxSpeed(float newMaxSpeed)
{
    maxSpeed = newMaxSpeed;
}

void Boid::forces(vector <Boid *> boids, ofVec2f *sep, ofVec2f *ali, ofVec2f *coh)
{
    ofVec2f steerSep = ofVec2f(0, 0);
    ofVec2f steerAli = ofVec2f(0, 0);
    ofVec2f steerCoh = ofVec2f(0, 0);
    
    int countSep = 0;
    int countAli = 0;
    int countCoh = 0;
    
    // For every boid in the system, check if it's too close
    for (Boid *other : boids)
    {
        float d = position.distance(other->getPosition());
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        
        
        /** Separation **/
        if ((d > 0) && (d < radiusSeparation))
        {
            // Calculate vector pointing away from neighbor
            ofVec2f diff = position - other->getPosition();
            diff.normalize();
            diff /= d;        // Weight by distance
            steerSep += diff;
            countSep++;            // Keep track of how many
        }
        
        /** Alignment **/
        if ((d > 0) && (d < radiusAlignment)) {
            steerAli += other->getVelocity();
            countAli++;
        }
        
        /** Cohesion **/
        if ((d > 0) && (d < radiusCohesion)) {
            steerCoh += other->getPosition(); // Add location
            countCoh++;
        }
    }
    
    /** Separation **/
    // Average -- divide by how many
    if (countSep > 0) {
        steerSep /= (float)countSep;
    }
    
    // As long as the vector is greater than 0
    if (steerSep.length() > 0) {
        // First two lines of code below could be condensed with new PVector setMag() method
        // Not using this method until Processing.js catches up
        steerSep = steerSep.getScaled(maxSpeed);
        
        // Implement Reynolds: Steering = Desired - Velocity
        //steer.normalize();
        //steer *= maxSpeed;
        steerSep -= velocity;
        steerSep.limit(maxForce);
    }
    *sep = steerSep;
    
    /** Alignment **/
    if (countAli > 0) {
        steerAli /= (float)countAli;
        // First two lines of code below could be condensed with new PVector setMag() method
        // Not using this method until Processing.js catches up
        steerAli.getScaled(maxSpeed);
        
        // Implement Reynolds: Steering = Desired - Velocity
        //sum.normalize();
        //sum.mult(maxspeed);
        steerAli -= velocity;
        steerAli.limit(maxForce);
        *ali = steerAli;
    }
    else *ali = ofVec2f(0, 0);
    
    /** Cohesion **/
    if (countCoh > 0) *coh = seek(steerCoh/countCoh);  // Steer towards the location
    else *coh = ofVec2f(0, 0);


    
}