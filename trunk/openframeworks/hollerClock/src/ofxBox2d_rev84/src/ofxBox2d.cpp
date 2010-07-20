#include "ofxBox2d.h"

// ------------------------------------------------------ 
ofxBox2d::ofxBox2d() {
	
}
// ------------------------------------------------------ init
void ofxBox2d::init() {
	
	//settings
	bHasContactListener = false;
	bCheckBounds		= false;
	bEnableGrabbing		= true;
	bWorldCreated		= false;
	scale				= OFX_BOX2D_SCALE;
	doSleep				= true;
	
	//gravity
	gravity.set(0, 5.0f);
	setFPS(60.0);
	velocityIterations = 40;
	positionIterations = 20;
	
	//mouse grabbing
	mouseJoint = NULL;
	ground	   = NULL;
	
	//debug drawer
	debugRender.setScale(scale);
	debugRender.SetFlags(1);
	
	worldAABB.lowerBound.Set(-100.0f, -100.0f);
	worldAABB.upperBound.Set(100.0f, 100.0f);
	
	world = new b2World(worldAABB, b2Vec2(gravity.x, gravity.y), doSleep);
	world->SetDebugDraw(&debugRender);
	
	ofLog(OF_LOG_NOTICE, "- Box2D Created -\n");
	
	bWorldCreated = true;
	
}


// ------------------------------------------------------ grab shapes 
void ofxBox2d::setContactListener(ofxBox2dContactListener * listener) {
	
	if(world != NULL) {
		bHasContactListener = true;
		world->SetContactListener(listener);
	}
	else {
		printf("--- you need a world ---\n");	
	}
}

// ------------------------------------------------------ grab shapes Events
void ofxBox2d::registerGrabbing() {
#ifdef TARGET_OF_IPHONE
	ofAddListener(ofEvents.touchDown, this, &ofxBox2d::touchDown);
	ofAddListener(ofEvents.touchMoved, this, &ofxBox2d::touchMoved);
	ofAddListener(ofEvents.touchUp, this, &ofxBox2d::touchUp);
#else
	ofAddListener(ofEvents.mousePressed, this, &ofxBox2d::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &ofxBox2d::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxBox2d::mouseReleased);
#endif
}

#ifdef TARGET_OF_IPHONE
void ofxBox2d::touchDown(ofTouchEventArgs &touch) {
	grabShapeDown(touch.x, touch.y);
}
void ofxBox2d::touchMoved(ofTouchEventArgs &touch) {
	grabShapeDragged(touch.x, touch.y);
}
void ofxBox2d::touchUp(ofTouchEventArgs &touch) {
	grabShapeUp(touch.x, touch.y);
}
#else
void ofxBox2d::mousePressed(ofMouseEventArgs &e) {
	grabShapeDown(e.x, e.y);
}
void ofxBox2d::mouseDragged(ofMouseEventArgs &e) {
	grabShapeDragged(e.x, e.y);
}
void ofxBox2d::mouseReleased(ofMouseEventArgs &e) {
	grabShapeUp(e.x, e.y);
}
#endif;

// ------------------------------------------------------ 
void ofxBox2d::grabShapeDown(float x, float y) {
	if(bEnableGrabbing) {
		b2Vec2 p(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
		
		if (mouseJoint != NULL) {
			return;
		}
		
		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;
		
		// Query the world for overlapping shapes.
		const int32 k_maxCount = 10000;
		b2Shape* shapes[k_maxCount];
		int32 count = world->Query(worldAABB, shapes, k_maxCount);
		b2Body* body = NULL;
		
		for (int32 i = 0; i < count; ++i) {
			
			b2Body* shapeBody = shapes[i]->GetBody();
			if (shapeBody->IsStatic() == false && shapeBody->GetMass() > 0.0f) {
				bool inside = shapes[i]->TestPoint(shapeBody->GetXForm(), p);
				if (inside) {
					body = shapes[i]->GetBody();
					break;
				}
			}
		}
		
		if (body) {
			
			b2MouseJointDef md;
			md.body1 = world->GetGroundBody();
			md.body2 = body;
			md.target = p;
#ifdef TARGET_FLOAT32_IS_FIXED
			md.maxForce = (body->GetMass() < 16.0)? 
			(1000.0f * body->GetMass()) : float32(16000.0);
#else
			md.maxForce = 1000.0f * body->GetMass();
#endif
			mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
			body->WakeUp();
			
		}
	}
}
void ofxBox2d::grabShapeUp(float x, float y) {
	
	if(mouseJoint && bEnableGrabbing) {
		world->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}
}
void ofxBox2d::grabShapeDragged(float x, float y) {
	b2Vec2 p(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
	if (mouseJoint && bEnableGrabbing) mouseJoint->SetTarget(p);
}


// ------------------------------------------------------ set gravity
void ofxBox2d::setGravity(float x, float y) {
	world->SetGravity(b2Vec2(x, y));
}
void ofxBox2d::setGravity(ofPoint pt) {
	world->SetGravity(b2Vec2(pt.x, pt.y));
}

// ------------------------------------------------------ set bounds
void ofxBox2d::setBounds(ofPoint lowBounds, ofPoint upBounds) {
	// still need to work on this...
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createFloor(float floorWidth, float bottom) {
	
	if(!bWorldCreated) return;
	
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = world->CreateBody(&bd);	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	
	sd.density = 0.0f;
	sd.restitution = 0.0f;
	sd.friction = 0.6;
	float thick = 5/OFX_BOX2D_SCALE;
	
	//bottom
	sd.SetAsBox((floorWidth/OFX_BOX2D_SCALE)/2, thick, 
				b2Vec2((floorWidth/OFX_BOX2D_SCALE)/2, (bottom-thick)/OFX_BOX2D_SCALE), 0.0);
	ground->CreateShape(&sd);
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createBounds(float x, float y, float w, float h) {
	
	if(!bWorldCreated) return;
	
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = world->CreateBody(&bd);	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	
	sd.density = 0.0f;
	sd.restitution = 0.0f;
	sd.friction = 0.6;
	float thick = 10/OFX_BOX2D_SCALE;
	
	// w h x y r 
	//right
	sd.SetAsBox(thick, (h/OFX_BOX2D_SCALE)/2, b2Vec2((w/OFX_BOX2D_SCALE), (h/OFX_BOX2D_SCALE)/2), 0.0);
	ground->CreateShape(&sd);
	//left
	sd.SetAsBox(thick, (h/OFX_BOX2D_SCALE)/2, b2Vec2(0, (h/OFX_BOX2D_SCALE)/2), 0.0);
	ground->CreateShape(&sd);
	//top
	sd.SetAsBox((w/OFX_BOX2D_SCALE)/2, thick, b2Vec2((w/OFX_BOX2D_SCALE)/2, 0), 0.0);
	ground->CreateShape(&sd);
	//bottom
	sd.SetAsBox((w/OFX_BOX2D_SCALE)/2, thick, b2Vec2((w/OFX_BOX2D_SCALE)/2, h/OFX_BOX2D_SCALE), 0.0);
	ground->CreateShape(&sd);
}

// ------------------------------------------------------ check if shapes are out of bounds
void ofxBox2d::checkBounds(bool b) {
	bCheckBounds = b;
}

// ------------------------------------------------------ 
void ofxBox2d::setIterations(int velocityTimes, int positionTimes) {
	velocityIterations = velocityTimes;
	positionIterations = positionTimes;
}


// ------------------------------------------------------ 
void ofxBox2d::update() {
	
	// destroy the object if we are out of the bounds
	if(bCheckBounds) {
		/*
		 float top = 0;
		 float bottom = ofGetHeight();
		 float right = ofGetWidth();
		 float left = 0;
		 
		 b2Body* node = world->GetBodyList();
		 while(node) {
		 b2Body* b = node;
		 node = node->GetNext();
		 b2Vec2 p = b->GetPosition();
		 ofxBox2dBaseShape* base = (ofxBox2dBaseShape*)b->GetUserData();
		 if(base) {
		 //printf("dead:%i\n", base->dead);
		 
		 if(p.y*OFX_BOX2D_SCALE > bottom) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.y*OFX_BOX2D_SCALE < top) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.x*OFX_BOX2D_SCALE > right) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.x*OFX_BOX2D_SCALE < left) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 */
		
		
	}
	
	float	timeStep		   = (1.0f / fps);
	world->Step(timeStep, velocityIterations, positionIterations);
	world->Validate();
}

// ------------------------------------------------------ 
void ofxBox2d::drawGround() {
	
	//draw the ground
	if(ground != NULL) {
		for(b2Shape* s=ground->GetShapeList(); s; s=s->GetNext()) {
			
			const b2XForm& xf = ground->GetXForm();		
			b2PolygonShape* poly = (b2PolygonShape*)s;
			int count = poly->GetVertexCount();
			const b2Vec2* verts = poly->GetVertices();
			ofEnableAlphaBlending();
			ofFill();
			ofSetColor(90, 90, 90, 100);
			ofBeginShape();
			for(int j=0; j<count; j++) {
				
				b2Vec2 pt = b2Mul(xf, verts[j]);
				
				ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
			}
			ofEndShape();
			ofDisableAlphaBlending();
		}
	}
	
}

// ------------------------------------------------------ 
void ofxBox2d::draw() {
	
	if(mouseJoint) {
		b2Body* mbody = mouseJoint->GetBody2();
		b2Vec2 p1 = mbody->GetWorldPoint(mouseJoint->m_localAnchor);
		b2Vec2 p2 = mouseJoint->m_target;
		
		p1 *= OFX_BOX2D_SCALE;
		p2 *= OFX_BOX2D_SCALE;
		
		//draw a line from touched shape
		ofEnableAlphaBlending();
		ofSetLineWidth(2.0);
		ofSetColor(200, 200, 200, 200);
		ofLine(p1.x, p1.y, p2.x, p2.y);
		ofNoFill();
		ofSetLineWidth(1.0);
		ofCircle(p1.x, p1.y, 2);
		ofCircle(p2.x, p2.y, 5);
		ofDisableAlphaBlending();
	}
	
	drawGround();
}

// -------------------------------------------------------

void ofxBox2d :: raycast ( const ofPoint &p1, const ofPoint &p2, int maxHits, vector<ofPoint>* hitPoints )
{
	bool bHitPoints;
	bHitPoints = ( hitPoints != NULL );

	b2Vec2 sp1	= b2Vec2( p1.x / OFX_BOX2D_SCALE, p1.y / OFX_BOX2D_SCALE );
	b2Vec2 sp2	= b2Vec2( p2.x / OFX_BOX2D_SCALE, p2.y / OFX_BOX2D_SCALE );
	b2Vec2 dir	= sp2 - sp1;
	
	b2Segment segment;
	segment.p1	= sp1;
	segment.p2	= sp2;
	
	float segmentLength;
	segmentLength = ofDist( sp1.x, sp1.y, sp2.x, sp2.y );
	
	for( int i=0; i<maxHits; i++ )
	{
		float lambda = 1;
		b2Vec2 normal;
		
		b2Shape* shape = NULL;
		shape = world->RaycastOne( segment, &lambda, &normal, false, NULL );
	
		if( shape != NULL )
		{
			if( bHitPoints )
			{
				ofPoint hitPoint;
				hitPoint.x = ( ( 1 - lambda ) * segment.p1.x + lambda * segment.p2.x ) * OFX_BOX2D_SCALE;
				hitPoint.y = ( ( 1 - lambda ) * segment.p1.y + lambda * segment.p2.y ) * OFX_BOX2D_SCALE;
				
				hitPoints->push_back( hitPoint );
				
				return;		// testing with one point for the moment.
			}
		}
		else
		{
			return;		// nothing hit.
		}
		
		segmentLength *= ( 1 - lambda );

		if( segmentLength <= B2_FLT_EPSILON )
			break;
		
		sp1 = ( 1 - lambda ) * segment.p1 + lambda * segment.p2;
		dir	= segment.p2 - segment.p1;
		dir.Normalize();
		dir = dir - 2 * b2Dot( dir, normal ) * normal;
		
		segment.p1 = sp1 - 0.1 * dir;
		segment.p2 = sp1 + segmentLength * dir;
	}
}

/**
 
void Step(Settings* settings)
{
	Test::Step(settings);
	
	float32 segmentLength = 30.0f;
	
	b2Segment segment;
	b2Vec2 laserStart(5.0f-0.1f,0.0f);
	b2Vec2 laserDir(segmentLength,0.0f);
	segment.p1 = laserBody->GetWorldPoint(laserStart);
	segment.p2 = laserBody->GetWorldVector(laserDir);
	segment.p2+=segment.p1;
	
	for(int32 rebounds=0;rebounds<10;rebounds++){
		
		float32 lambda=1;
		b2Vec2 normal;
		b2Shape* shape = m_world->RaycastOne(segment,&lambda,&normal,false,NULL);
		
		b2Color laserColor(255,0,0);
		
		if(shape)
		{
			m_debugDraw.DrawSegment(segment.p1,(1-lambda)*segment.p1+lambda*segment.p2,laserColor);
		}
		else
		{
			m_debugDraw.DrawSegment(segment.p1,segment.p2,laserColor);
			break;
		}
		//Bounce
		segmentLength *=(1-lambda);
		if(segmentLength<=B2_FLT_EPSILON)
			break;
		laserStart = (1-lambda)*segment.p1+lambda*segment.p2;
		laserDir = segment.p2-segment.p1;
		laserDir.Normalize();
		laserDir = laserDir -2 * b2Dot(laserDir,normal) * normal;
		segment.p1 = laserStart-0.1f*laserDir;
		segment.p2 = laserStart+segmentLength*laserDir;
	}
}
 
*/