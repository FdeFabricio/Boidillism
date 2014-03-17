#include "ofMain.h"



class Boid {
private:
    float size;
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f acceleration;
    float angle;
    float radius;
    float maxForce;
    float maxSpeed;
    ofColor color;
    ofTrueTypeFont 	verdana;
    
public:
    
    bool debug_drawArrow, debug_drawRadiusSeparation, debug_drawRadiusAlignment, debug_drawRadiusCohesion, gravity;
    float radiusSeparation, radiusAlignment, radiusCohesion;
    float weightSeparation, weightAlignment, weightCohesion;
    float gravityForce;
    
    Boid(float _x = ofGetWidth()/2,
         float _y = ofGetHeight()/2,
         ofColor _color = ofColor(0),
         float _size = 3.0f,
         float _maxSpeed = 2.0f,
         float _maxForce = 0.05f)
    {
        angle = ofRandom(TWO_PI);
        position.set(_x, _y);
        color.set(_color);
        velocity.set(cos(angle), sin(angle));
        acceleration.set(0.0f);
        size = _size;
        radius = 3*size;
        maxSpeed = _maxSpeed;
        maxForce = _maxForce;
        
        radiusSeparation = 32.0f;
        radiusAlignment = 60.0f;
        radiusCohesion = 80.0f;
        
        weightSeparation = 1.0f;
        weightAlignment = 1.0f;
        weightCohesion = 1.0f;
        
        debug_drawArrow = false;
        debug_drawRadiusSeparation = false;
        debug_drawRadiusAlignment = false;
        debug_drawRadiusCohesion = false;
        
        
    }
    
    ofVec2f getPosition();
    ofVec2f getVelocity();
    
    
    
    void setSize(float newSize);
    void setMaxForce(float newMaxForce);
    void setMaxSpeed(float newMaxSpeed);
    
    // implemented
    void draw();
    ofVec2f seek(ofVec2f);
    ofVec2f separation(vector <Boid *>);
    ofVec2f alignment(vector <Boid *>);
    ofVec2f cohesion(vector <Boid *>);
    void borders();
    void pullToCenter(ofVec2f);
    
    
    // to implement
    void update(vector <Boid *>); // update velocity, position, acceleration, direction
    
    
    // tests
    void followMouse();
};

class Flock {
public:
    vector <Boid *> boids; // an list of all the boids
    
    void addBoid(float x, float y)
    {
        Boid * b = new Boid(x, y);
        boids.push_back(b);
    }
    
    void addBoid(float x, float y, ofColor color)
    {
        Boid * b = new Boid(x, y, color);
        boids.push_back(b);
    }
    
    void draw()
    {
        for (int i = 0; i < boids.size(); i++) boids[i]->draw();
    }
    
    void update()
    {
        for (int i = 0; i < boids.size(); i++) boids[i]->update(boids);
    }
    
    void gui_update(float _size, float newforce, float newspeed, bool _arrow, bool _separation, bool _alignment, bool _cohesion, float _wSeparation, float _wAlignment, float _wCohesion, float _radiusSeparation, float _radiusAlignment, float _radiusCohesion, bool _gravity, float _gravityForce)
    {
        for (int i = 0; i < boids.size(); i++)
        {
            boids[i]->setSize(_size);
            boids[i]->setMaxForce(newforce);
            boids[i]->setMaxSpeed(newspeed);
            
            boids[i]->debug_drawArrow = _arrow;
            boids[i]->debug_drawRadiusSeparation = _separation;
            boids[i]->debug_drawRadiusAlignment= _alignment;
            boids[i]->debug_drawRadiusCohesion = _cohesion;
            
            boids[i]->weightSeparation = _wSeparation;
            boids[i]->weightAlignment = _wAlignment;
            boids[i]->weightCohesion = _wCohesion;
            
            boids[i]->radiusSeparation = _radiusSeparation * _size;
            boids[i]->radiusAlignment = _radiusAlignment * _size;
            boids[i]->radiusCohesion = _radiusCohesion * _size;
            
            boids[i]->gravity = _gravity;
            boids[i]->gravityForce = _gravityForce;
        }
    }
};