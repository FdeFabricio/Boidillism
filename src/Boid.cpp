#include "Boid.h"

void Boid::draw()
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
    
        ofSetColor(color);
    ofCircle(0,0,size);
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
    /********************************/
    ofVec2f sep = separation(boids);
    ofVec2f ali = alignment(boids);
    ofVec2f coh = cohesion(boids);
    
    sep *= weightSeparation;
    ali *= weightAlignment;
    coh *= weightCohesion;
    
    acceleration += sep;
    acceleration += ali;
    acceleration += coh;
    /********************************/
    
    if (gravity) pullToCenter(ofVec2f(ofGetWidth()/2, ofGetHeight()/2));
    
    //followMouse();
    borders();
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
	float maxDistance = ofGetHeight()/3;
	
	if (distToCenter > maxDistance ){
		dirToCenter.normalize();
		float pullStrength = gravityForce;
		velocity -= dirToCenter * ( ( distToCenter - maxDistance ) * pullStrength );
	}
}

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