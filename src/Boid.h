#include "ofMain.h"
#include "linkedlist.h"

class Boid {
private:
    float size;
    ofVec2f position;
    ofVec2f origin;
    ofVec2f velocity;
    ofVec2f acceleration;
    float angle;
    float radius;
    float maxForce;
    float maxSpeed;
    ofColor color;
    ofTrueTypeFont 	verdana;
    
public:
    
    bool debug_drawArrow, debug_drawRadiusSeparation, debug_drawRadiusAlignment, debug_drawRadiusCohesion, gravity, move2origin;
    float radiusSeparation, radiusAlignment, radiusCohesion;
    float weightSeparation, weightAlignment, weightCohesion;
    float gravityForce;
    
    Boid(float _px = ofGetWidth()/2,
         float _py = ofGetHeight()/2,
         float _ox = 0,
         float _oy = 0,
         ofColor _color = ofColor(0),
         float _size = 3.0f,
         float _maxSpeed = 2.0f,
         float _maxForce = 0.05f)
    {
        angle = ofRandom(TWO_PI);
        position.set(_px, _py);
        origin.set(_ox, _oy);
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
        
        move2origin = false;
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
    void draw(ofImage *img, int *mat, linkedList targets);
    ofVec2f seek(ofVec2f);
    ofVec2f separation(vector <Boid *>);
    ofVec2f alignment(vector <Boid *>);
    ofVec2f cohesion(vector <Boid *>);
    
    void borders();
    void pullToCenter(ofVec2f);
    void pull2point(ofVec2f, float);
    
    
    // to implement
    void update(vector <Boid *>); // update velocity, position, acceleration, direction
    
    
    // tests
    void followMouse();
    
    
    //optmization
    void forces(vector <Boid *>, ofVec2f *, ofVec2f *, ofVec2f *);
    ofVec2f sep, ali, coh;
    
    void kill(vector <Boid *> boids, ofImage img);
};

class Flock {
public:
    
    vector <Boid *> boids; // an list of all the boids
    
    void addBoid(float px, float py, float ox, float oy)
    {
        Boid * b = new Boid(px, py, ox, oy);
        boids.push_back(b);
    }
    
    void addBoid(float px, float py, float ox, float oy, ofColor color)
    {
        Boid * b = new Boid(px, py, ox, oy, color);
        boids.push_back(b);
    }
    
    void addBoid(float px, float py, ofColor color)
    {
        Boid * b = new Boid(px, py, 0, 0, color);
        boids.push_back(b);
    }
    
    void draw(ofImage img, int *mat)
    {
        for (int i = 0; i < boids.size(); i++) boids[i]->draw(&img, mat);
    }
    
    void update()
    {
        for (int i = 0; i < boids.size(); i++) boids[i]->update(boids);
    }
    
    void gui_update(float _size, float newforce, float newspeed, bool _arrow, bool _separation, bool _alignment, bool _cohesion, float _wSeparation, float _wAlignment, float _wCohesion, float _radiusSeparation, float _radiusAlignment, float _radiusCohesion, bool _gravity, float _gravityForce, bool _move2origin)
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
            
            boids[i]->move2origin = _move2origin;
        }
    }
};