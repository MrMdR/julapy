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
	
	for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchJoints[ i ] = NULL;
	
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
	grabShapeDown( touch.x, touch.y, touch.id );
}
void ofxBox2d::touchMoved(ofTouchEventArgs &touch) {
	grabShapeDragged( touch.x, touch.y, touch.id );
}
void ofxBox2d::touchUp(ofTouchEventArgs &touch) {
	grabShapeUp( touch.x, touch.y, touch.id );
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
void ofxBox2d::grabShapeDown(float x, float y, int id )
{
	if( !bEnableGrabbing )
		return;
	
	if( id == -1 )							// check the grab is valid.
	{
		if( mouseJoint != NULL )			// mouse joint already registered.
		{
			grabShapeUp( x, y, id );		// destroy the mouse joint before replacing it.
		}
	}
	else if( id >= 0 && id < OF_MAX_TOUCH_JOINTS )
	{
		if( touchJoints[ id ] != NULL )		// touch joint already registered.
		{
			grabShapeUp( x, y, id );		// destroy the touch joint before replacing it.
		}
	}
	else 
	{
		return;								// invalid mouse / touch id.
	}

	
	b2Vec2 p( x / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE );
	
	b2AABB aabb;									// Make a small box.
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;
	
	const int32 k_maxCount = 10000;					// Query the world for overlapping shapes.
	b2Shape* shapes[ k_maxCount ];
	int32 count = world->Query( worldAABB, shapes, k_maxCount );
	b2Body* body = NULL;
	
	for( int32 i=0; i<count; ++i )
	{
		b2Body* shapeBody = shapes[ i ]->GetBody();
		
		if( shapeBody->IsStatic() == false && shapeBody->GetMass() > 0.0f )
		{
			bool inside;
			inside = shapes[ i ]->TestPoint( shapeBody->GetXForm(), p );
			
			if( inside )
			{
				body = shapes[ i ]->GetBody();
				break;
			}
		}
	}
	
	if( body )
	{
		b2MouseJointDef md;
		md.body1 = world->GetGroundBody();
		md.body2 = body;
		md.target = p;
#ifdef TARGET_FLOAT32_IS_FIXED
		md.maxForce = ( body->GetMass() < 16.0 )? 
		( 1000.0f * body->GetMass() ) : float32( 16000.0 );
#else
		md.maxForce = 1000.0f * body->GetMass();
#endif

		if( id == -1 )
		{
			mouseJoint = (b2MouseJoint*)world->CreateJoint( &md );
		}
		else if( id >= 0 )
		{
			touchJoints[ id ] = (b2MouseJoint*)world->CreateJoint( &md );
		}
		
		body->WakeUp();
	}
}

void ofxBox2d :: grabShapeUp( float x, float y, int id )
{
	if( !bEnableGrabbing )
		return;

	if( id == -1  )
	{
		if( mouseJoint != NULL )
		{
			world->DestroyJoint( mouseJoint );
			mouseJoint = NULL;
		}
	}
	else if( id >= 0 && id < OF_MAX_TOUCH_JOINTS )
	{
		if( touchJoints[ id ] != NULL )
		{
			world->DestroyJoint( touchJoints[ id ] );
			touchJoints[ id ] = NULL;
		}
	}
}

void ofxBox2d :: grabShapeDragged( float x, float y, int id )
{
	if( !bEnableGrabbing )
		return;
	
	b2Vec2 p( x / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE );

	if( id == -1  )
	{
		if( mouseJoint != NULL )
		{
			mouseJoint->SetTarget( p );
		}
	}
	else if( id >= 0 && id < OF_MAX_TOUCH_JOINTS )
	{
		if( touchJoints[ id ] != NULL  )
		{
			touchJoints[ id ]->SetTarget( p );
		}
	}
}

//---------------------------------------------------MAJOR HACK!!! - i hate myself right now.
//---------------------------------------------------MUST REMOVE FOR ANY SORT OF RELEASE.

void ofxBox2d :: grabShapeDown2 (float x, float y, int id, b2Body* body )
{
	if( id == -1 )							// check the grab is valid.
	{
		if( mouseJoint != NULL )			// mouse joint already registered.
		{
			grabShapeUp( x, y, id );		// destroy the mouse joint before replacing it.
		}
	}
	else if( id >= 0 && id < OF_MAX_TOUCH_JOINTS )
	{
		if( touchJoints[ id ] != NULL )		// touch joint already registered.
		{
			grabShapeUp( x, y, id );		// destroy the touch joint before replacing it.
		}
	}
	else 
	{
		return;								// invalid mouse / touch id.
	}
	
	if( body )
	{
		b2MouseJointDef md;
		md.body1	= world->GetGroundBody();
		md.body2	= body;
		md.target	= body->GetWorldCenter();
#ifdef TARGET_FLOAT32_IS_FIXED
		md.maxForce = ( body->GetMass() < 16.0 )? 
		( 1000.0f * body->GetMass() ) : float32( 16000.0 );
#else
		md.maxForce = 1000.0f * body->GetMass();
#endif
		
		if( id == -1 )
		{
			mouseJoint = (b2MouseJoint*)world->CreateJoint( &md );
		}
		else if( id >= 0 )
		{
			touchJoints[ id ] = (b2MouseJoint*)world->CreateJoint( &md );
		}
		
		body->WakeUp();
	}
}

void ofxBox2d :: grabShapeUp2 ( float x, float y, int id )
{
	if( id == -1  )
	{
		if( mouseJoint != NULL )
		{
			world->DestroyJoint( mouseJoint );
			mouseJoint = NULL;
		}
	}
	else if( id >= 0 && id < OF_MAX_TOUCH_JOINTS )
	{
		if( touchJoints[ id ] != NULL )
		{
			world->DestroyJoint( touchJoints[ id ] );
			touchJoints[ id ] = NULL;
		}
	}
}

void ofxBox2d :: grabShapeDragged2 ( float x, float y, int id )
{
	b2Vec2 p( x / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE );
	
	if( id == -1  )
	{
		if( mouseJoint != NULL )
		{
			mouseJoint->SetTarget( p );
		}
	}
	else if( id >= 0 && id < OF_MAX_TOUCH_JOINTS )
	{
		if( touchJoints[ id ] != NULL  )
		{
			touchJoints[ id ]->SetTarget( p );
		}
	}
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
	
	if( mouseJoint != NULL )
		drawMouseJoint( mouseJoint );
	
	for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
	{
		if( touchJoints[ i ] != NULL )
			drawMouseJoint( touchJoints[ i ] );
	}
	
	drawGround();
}

void ofxBox2d :: drawMouseJoint ( b2MouseJoint* mj )
{
	b2Body* mbody = mj->GetBody2();
	b2Vec2 p1 = mbody->GetWorldPoint( mj->m_localAnchor);
	b2Vec2 p2 = mj->m_target;
	
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

// -------------------------------------------------------

void ofxBox2d :: raycast ( const ofPoint &p1, const ofPoint &p2, int maxHits, vector<ofPoint>* hitPoints, vector<b2Shape*>* shapes )
{
	b2Segment segment;
	segment.p1	= b2Vec2( p1.x / OFX_BOX2D_SCALE, p1.y / OFX_BOX2D_SCALE );
	segment.p2	= b2Vec2( p2.x / OFX_BOX2D_SCALE, p2.y / OFX_BOX2D_SCALE );
	
	for( int i=0; i<maxHits; i++ )
	{
		float lambda = 1;
		b2Vec2 normal;
		
		b2Shape* shape = NULL;
		shape = world->RaycastOne( segment, &lambda, &normal, false, NULL );
	
		if( shape != NULL )
		{
			if( shapes != NULL )
			{
				shapes->push_back( shape );
			}
			
			if( hitPoints != NULL )
			{
				b2Vec2 hit;
				hit = ( 1 - lambda ) * segment.p1 + lambda * segment.p2;
				
				ofPoint hitPoint;
				hitPoint.x = hit.x * OFX_BOX2D_SCALE;
				hitPoint.y = hit.y * OFX_BOX2D_SCALE;
				
				hitPoints->push_back( hitPoint );
			}
			
			lambda = MIN( lambda + 0.001, 1.0 );	// nudge lambda a little so it doesn't find the same shape twice.
			
			segment.p1 = ( 1 - lambda ) * segment.p1 + lambda * segment.p2;
		}
		else
		{
			return;		// nothing hit.
		}
		
		float segmentLength;
		segmentLength = ofDist( segment.p1.x, segment.p1.y, segment.p2.x, segment.p2.y );

		if( segmentLength <= B2_FLT_EPSILON )
			return;
	}
}