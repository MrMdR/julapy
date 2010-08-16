#pragma once

#include "ofxBox2dUtils.h"
#include "ofxBox2dBaseShape.h"

#include "ofxBox2dCircle.h"
#include "ofxBox2dPolygon.h"
#include "ofxBox2dRect.h"
#include "ofxBox2dLine.h"

#include "ofxBox2dSoftBody.h"
#include "ofxBox2dJoint.h"
#include "ofxBox2dRender.h"

#include "ofxBox2dContactListener.h"
#include "ofxBox2dContactReceiver.h"

#define OF_MAX_TOUCH_JOINTS		5			// max number of touch points on iPhone + iPad (this may change in the future though).

class ofxBox2d {
	
private:
	
	float				fps;
	int					velocityIterations;
	int					positionIterations;
public:
	
	b2AABB				worldAABB;
	b2World *			world;
	ofxBox2dRender		debugRender;
	
	float				scale;
	bool				doSleep;
	bool				bWorldCreated;
	bool				bEnableGrabbing;
	bool				bCheckBounds;
	bool				bHasContactListener;
	
	ofPoint				gravity;
	b2BodyDef			bd;
	
	b2Body*				m_bomb;
	b2MouseJoint*		mouseJoint;
	b2MouseJoint*		touchJoints[ OF_MAX_TOUCH_JOINTS ];
	b2Body*				ground;
	b2Body*				mainBody;

	// ------------------------------------------------------ 
	ofxBox2d();
	void		init();
	void		setFPS(float theFps) { fps = theFps; }
	
#ifdef TARGET_OF_IPHONE
	void		touchDown(ofTouchEventArgs &touch);
	void		touchMoved(ofTouchEventArgs &touch);
	void		touchUp(ofTouchEventArgs &touch);
#else
	void		mousePressed(ofMouseEventArgs &e);
	void		mouseDragged(ofMouseEventArgs &e);
	void		mouseReleased(ofMouseEventArgs &e);
#endif
	
	void		registerGrabbing	();
	void		grabShapeDown		( float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void		grabShapeUp			( float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void		grabShapeDragged	( float x, float y, int id = -1 );		// -1 is reserved for mouse.
	
	void		grabShapeDown2		( float x, float y, int id = -1, b2Body* body = NULL );	// when you want to force grab.
	void		grabShapeUp2		( float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void		grabShapeDragged2	( float x, float y, int id = -1 );		// -1 is reserved for mouse.

	b2World*	getWorld()		  { return world;				   }
	int			getBodyCount()    { return world->GetBodyCount();  }
	int			getJointCount()   { return world->GetJointCount(); }
	
	void		enableGrabbing()  { bEnableGrabbing = true;  };
	void		disableGrabbing() { bEnableGrabbing = false; };
	
	void		setContactListener(ofxBox2dContactListener * listener);
	
	void setIterations(int velocityTimes, int positionTimes);
	void setGravity(float x, float y);
	void setGravity(ofPoint pt);
	void setBounds(ofPoint lowBounds, ofPoint upBounds);
	void createBounds(float x=0, float y=0, float w=ofGetWidth(), float h=ofGetHeight());
	void createFloor(float floorWidth=ofGetWidth(), float bottom=ofGetHeight());
	void checkBounds(bool b);
	
	void update(); 
	void draw();
	void drawMouseJoint( b2MouseJoint* mj );
	void drawGround();
	
	void raycast ( const ofPoint &p1, const ofPoint &p2, int maxHits = 1, vector<ofPoint>* hitPoints = NULL, vector<b2Shape*>* shapes = NULL );
};
