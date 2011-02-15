/*
 *  Clock.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Clock.h"

Clock :: Clock ()
{
	box2d		= NULL;
	softBody	= NULL;
	
	secTwoSound	= NULL;
	secOneSound	= NULL;
	
	texBg		= NULL;
	texInfo		= NULL;
	texCells	= NULL;
	texCellAnim	= NULL;
	texLines	= NULL;
	texMembrane	= NULL;
	texDigits	= NULL;
	
	clockMode = CLOCK_MODE_1;
	
	digits[ 0 ].valueMax = 2;
	digits[ 1 ].valueMax = 9;
	digits[ 2 ].valueMax = 5;
	digits[ 3 ].valueMax = 9;
	digits[ 4 ].valueMax = 5;
	digits[ 5 ].valueMax = 9;
	
	float x ,d, g;
	
//	x = 0.05;
	x = 0.14;
	d = 0.022;
//	g = 0.05;
	g = 0.04;
	
	digits[ 0 ].p.x = digits[ 0 ].p1.x	= x;
	digits[ 1 ].p.x = digits[ 1 ].p1.x	= x += d;
	digits[ 2 ].p.x = digits[ 2 ].p1.x	= x += g;
	digits[ 3 ].p.x = digits[ 3 ].p1.x	= x += d;
	digits[ 4 ].p.x = digits[ 4 ].p1.x	= x += g;
	digits[ 5 ].p.x = digits[ 5 ].p1.x	= x += d;

//	x = 0.102;
	x = 0.16;
	d = 0.075;
//	g = 0.26;
	g = 0.215;
	
	digits[ 0 ].p2.x = x;
	digits[ 1 ].p2.x = x += d;
	digits[ 2 ].p2.x = x += g;
	digits[ 3 ].p2.x = x += d;
	digits[ 4 ].p2.x = x += g;
	digits[ 5 ].p2.x = x += d - 0.02;
	
	forceCenterPull	= 30;
	forceCenterPush = 30;
	rayBlobPad		= 0.07;
	rayBlobEase		= 0.4;

	bMouseDown		= false;
	mouseDownCount	= 0;
	mouseDownLimit	= 30;
	
	setSize( ofGetWidth(), ofGetHeight() );
	setGravity( 0, 0 );
	setScreenScale( 1.0 );

#ifdef TARGET_OF_IPHONE	
//	ofAddListener( ofEvents.touchDown,		this, &Clock::touchDown		);
//	ofAddListener( ofEvents.touchMoved,		this, &Clock::touchMoved	);
//	ofAddListener( ofEvents.touchUp,		this, &Clock::touchUp		);
#else
	ofAddListener( ofEvents.mousePressed,	this, &Clock::mousePressed	);
	ofAddListener( ofEvents.mouseMoved,		this, &Clock::mouseMoved	);
	ofAddListener( ofEvents.mouseDragged,	this, &Clock::mouseDragged	);
	ofAddListener( ofEvents.mouseReleased,	this, &Clock::mouseReleased	);
#endif
}

Clock :: ~Clock()
{
	//
}

///////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////

void Clock :: setup ()
{
	if( box2d == NULL )
		return;
	
	createBounds();
	createCircles();
	createLines();
	createInfoScreen();
//	createLabels();
//	creatFreeCircles();
}

///////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////

void Clock :: setBox2d ( ofxBox2d *box2d )
{
	this->box2d = box2d;
}

void Clock :: setSize ( ofRectangle &size )
{
	setSize( size.width, size.height );
}

void Clock :: setSize ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
	screenMinLength	= MIN( w, h );
	screenMaxLength = MAX( w, h );
	screenTotal		= w * h;
	
	screenWidthScale	= screenWidth / 1280.0;
	screenHeightScale	= screenHeight / 720.0;
	
	screenCenter.x	= screenWidth * 0.5;
	screenCenter.y	= screenHeight * 0.5;
	
	screenDiagonal	= ofDist( 0, 0, screenCenter.x, screenCenter.y );
	
	//-- update circles.
	
	for( int i=0; i<circlesAll.size(); i++ )
	{
		circlesAll[ i ]->setSize( w, h );
	}
	
	infoScreen.setSize( w, h );
}

void Clock :: setGravity( float x, float y )
{
	gravity.x += ( x - gravity.x ) * 0.6;
	gravity.y += ( y - gravity.y ) * 0.6;
}

void Clock :: setScreenScale ( float scale )
{
	screenScale = scale;
}

//-- SOUND.

#ifdef TARGET_OF_IPHONE	
void Clock :: setSound ( ofxALSoundPlayer* secTwoSound, ofxALSoundPlayer* secOneSound )
{
	this->secTwoSound = secTwoSound;
	this->secOneSound = secOneSound;
}
#else
void Clock :: setSound ( ofSoundPlayer* secTwoSound, ofSoundPlayer* secOneSound )
{
	this->secTwoSound = secTwoSound;
	this->secOneSound = secOneSound;
}
#endif

//-- TEXTURES.

void Clock :: setBgTexture ( ofTexture* tex )
{
	texBg = tex;
}

void Clock :: setInfoTexture ( ofTexture* tex )
{
	texInfo = tex;
}

void Clock :: setCellTexture ( ofTexture* tex, int numOfTextures )
{
	texCells = tex;
	texCellsNum = numOfTextures;
}

void Clock :: setCellAnimTex ( vector<ofTexture*>* tex )
{
	texCellAnim = tex;
}

void Clock :: setLineTexture ( ofTexture* tex, int numOfTextures )
{
	texLines = tex;
	texLinesNum = numOfTextures;
}

void Clock :: setMembraneTex ( ofTexture* tex )
{
	texMembrane = tex;
}

void Clock :: setDigitTexture ( ofTexture* tex, int numOfTextures )
{
	texDigits = tex;
	texDigitsNum = numOfTextures;
}

//-- CREATE.

void Clock :: createBounds ()
{
	b2BodyDef bd;
	bd.position.Set( 0, 0 );
	ground = box2d->ground = box2d->world->CreateBody( &bd );
	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	sd.density		= 0.0f;
	sd.restitution	= 0.0f;
	sd.friction		= 0.6;
	
	float thick		= 0.02 * screenHeight;
	
	int w = screenWidth;
	int h = screenHeight;
	
	sd.SetAsBox		//-- right		( float32 hx, float32 hy, const b2Vec2& center, float32 angle )
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2,
		b2Vec2( ( w + thick ) / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape( &sd );
	
	sd.SetAsBox		//-- left
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2, 
		b2Vec2( -thick / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- top
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, -thick /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- bottom
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, ( h + thick ) /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
}

void Clock :: createCircles ()
{
	float area	= 0.015;		// biggest area as start.
	float dec	= 0.5;		// decrease in area for the next batch of circles.
	
//	int colors[] = { 0x6b007e, 0xf51d2a, 0xf6009d, 0x8c162f, 0xc96dfd, 0xf7719a };
	int colors[] = { 0xa65eb3, 0xf17a81, 0xf169c0, 0xa94f62, 0xdca6fb, 0xf8a7c0 };
//	int colors[] = { 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff };
	
	createCircle( hrsOne, digits[ 0 ].valueMax, 0, areaToRadius( area ),		colors[ 0 ], 0.14 );
	createCircle( hrsTwo, digits[ 1 ].valueMax, 1, areaToRadius( area *= dec ),	colors[ 1 ], 0.26 );
	createCircle( minOne, digits[ 2 ].valueMax, 2, areaToRadius( area *= dec ),	colors[ 2 ], 0.475 );
	createCircle( minTwo, digits[ 3 ].valueMax, 3, areaToRadius( area *= dec ),	colors[ 3 ], 0.55 );
	createCircle( secOne, digits[ 4 ].valueMax, 4, areaToRadius( area *= dec ),	colors[ 4 ], 0.75 );
	createCircle( secTwo, digits[ 5 ].valueMax, 5, areaToRadius( area *= dec ),	colors[ 5 ], 0.82 );
}

void Clock  :: createCircle ( vector<ClockCircle*> &circlesVec, int numOfCircle, int texIndex, float radius, int color, float lx )
{
	int t = numOfCircle;
	
	for( int i=0; i<t; i++ )
	{
		ClockCircle* circle;
		circle = new ClockCircle( radius, color );
		
		//-- define line up point.
		
		float rx = radius / (int)screenWidth;
		float ry = radius / (int)screenHeight;
		
		float lineX;
		lineX = lx;
		lineX += ( i > 4 ) ? rx * 2.2 : 0;

		int j = i % 5;
		
		float lineY;
		lineY = 1.0;
		lineY -= 0.14;
		lineY -= ry;
		lineY -= j * ry * 2;
		lineY -= j * ( ry * 0.2 );
		
		circle->lineUpPoint.set( lineX, lineY );
		circle->setSize( screenWidth, screenHeight );
		circle->setTexture( &texCells[ MIN( texIndex, texCellsNum - 1 ) ] );
		circle->setTextureAnim( texCellAnim );
		circle->setForceScale( screenScale );
		circle->setDigitIndex( i );
		circle->init();

		//-- define circles physics.
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		
		circle->enableGravity( false );
		circle->setPhysics( mass, bounce, friction );
		circle->setup( box2d->getWorld(), ofRandom( 0, screenWidth ), ofRandom( 0, screenHeight ), radius, false );
		circle->setRotationFriction( 1.0 );
		circle->setDamping( 1.0 );
		
		//-- add to vectors.
		
		circlesVec.push_back( circle );
		circlesAll.push_back( circle );
	}
}

float Clock :: areaToRadius ( float area )
{
	float r;
	r = sqrt( area * screenTotal );
	r *= 0.5;
	
	return r;
}

void Clock :: createSoftBody ()
{
	float mass		= 3.0;
	float bounce	= 0.53;
	float friction	= 0.1;
	
	softBody = new ofxBox2dSoftBody();
	softBody->setPhysics( mass, bounce, friction );
	softBody->setup( box2d->getWorld(), ofRandom( 0, screenWidth ), ofRandom( 0, screenHeight ) );
}

void Clock :: createLines ()
{
	int res = 100;
#ifdef TARGET_OF_IPHONE
	res = 70;
#endif
	
	for( int i=0; i<res; i++ )
	{
		float p = i / (float)res;
		int r = ofRandom( 0, texLinesNum );
		
		lines.push_back( ClockLine() );
		ClockLine& line = lines.back();

		if( texLines != NULL )
			line.setTexture( &texLines[ r ] );
		
		line.size.width		= screenWidth;
		line.size.height	= screenHeight;
		line.angle			= p * 360;
		line.setup();
	}
}

void Clock :: createInfoScreen ()
{
	if( texInfo != NULL )
		infoScreen.setTexture( texInfo );
}

void Clock :: createLabels ()
{
	ClockLabel* label;
	
	float w = screenHeight * 0.15;
	float h = screenHeight * 0.10;
	float x = screenWidth - w * 2;
	float y = 0.3 * screenHeight;

	float mass			= 3.0;
	float bounce		= 0.53;
	float friction		= 0.1;
	float rotFriction	= 0.5;
	float damping		= 1.5;
	
	//-- label one.
	
	label = new ClockLabel();
	label->setSize( screenWidth, screenHeight );
	label->setForceScale( screenScale );
	label->init();
	
	label->setPhysics( mass, bounce, friction );
	label->setup( box2d->getWorld(), x, y, w, h, false );
	label->setRotationFriction( rotFriction );
	label->setDamping( damping );
	
	labels.push_back( label );
	
	//-- label two.
	
	y += 0.15 * screenHeight;
	
	label = new ClockLabel();
	label->setSize( screenWidth, screenHeight );
	label->setForceScale( screenScale );
	label->init();
	
	label->setPhysics( mass, bounce, friction );
	label->setup( box2d->getWorld(), x, y, w, h, false );
	label->setRotationFriction( rotFriction );
	label->setDamping( damping );
	
	labels.push_back( label );
}

void Clock :: creatFreeCircles ()
{
	int t = 20;
	for( int i=0; i<t; i++ )
	{
		ClockFreeCircle* circle;
		circle = new ClockFreeCircle();
		
//		circle->setSize( screenWidth, screenHeight );
//		circle->setTexture( &texCells[ 0 ] );
//		circle->setForceScale( screenScale );
//		circle->init();
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		
		float radius;
		radius = screenMinLength * 0.015;
		radius *= ofRandom( 0.5, 1.0 );
		
		circle->setPhysics( mass, bounce, friction );
		circle->setup( box2d->getWorld(), ofRandom( 0, screenWidth ), ofRandom( 0, screenHeight ), radius, false );
		circle->setRotationFriction( 1.0 );
		circle->setDamping( 1.0 );
		
		freeCircles.push_back( circle );
	}
}

///////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////

void Clock :: update ( int hrs, int min, int sec )
{
	int hrsOneNew = hrs / 10;
	int hrsTwoNew = hrs % 10;
	
	int minOneNew = min / 10;
	int minTwoNew = min % 10;
	
	int secOneNew = sec / 10;
	int secTwoNew = sec % 10;
	
	if( digits[ 5 ].value != secTwoNew )
	{
		if( secTwoNew == 0 )
		{
			playSecOneSound();
		}
		else 
		{
			playSecTwoSound();
		}
	}
	
	digits[ 0 ].value = hrsOneNew;
	digits[ 1 ].value = hrsTwoNew;
	
	digits[ 2 ].value = minOneNew;
	digits[ 3 ].value = minTwoNew;
	
	digits[ 4 ].value = secOneNew;
	digits[ 5 ].value = secTwoNew;

	//--
	
//	updateInfoScreen();
	updateText();
	updateForces();
	
	box2d->update();
	
	for( int i=0; i<circlesAll.size(); i++ )
	{
		circlesAll[ i ]->update();
	}
	
	for( int i=0; i<freeCircles.size(); i++ )
	{
		freeCircles[ i ]->update();
	}
	
	updateConvexBlob();
	updateDelaunay();
	
//	infoScreen.update();
}

void Clock :: playSecTwoSound ()
{
	if( secTwoSound != NULL )
		secTwoSound->play();
}

void Clock :: playSecOneSound ()
{
	if( secOneSound != NULL )
		secOneSound->play();
}

void Clock :: updateText ()
{
	float ease = 0.3;
	
	if( clockMode == CLOCK_MODE_1 )
	{
		for( int i=0; i<6; i++ )
		{
			digits[ i ].p.x += ( digits[ i ].p1.x - digits[ i ].p.x ) * ease;
		}
	}
	else if ( clockMode == CLOCK_MODE_2 )
	{
		for( int i=0; i<6; i++ )
		{
			digits[ i ].p.x += ( digits[ i ].p2.x - digits[ i ].p.x ) * ease;
		}
	}
}

void Clock :: updateInfoScreen ()
{
	if( bMouseDown && !infoScreen.bShowing )
	{
		mouseDownCount += 1;
		
		if( mouseDownCount > mouseDownLimit )
		{
			infoScreen.show();
			
			bMouseDown = false;
		}
	}
	else if( bMouseDown && infoScreen.bShowing )
	{
		infoScreen.hide();
		
		bMouseDown = false;
	}
}

void Clock :: toggleInfoScreen ()
{
	if( infoScreen.bShowing )
	{
		infoScreen.hide();
	}
	else
	{
		infoScreen.show();
	}
}

void Clock :: checkLabelPress ( int x, int y )
{
	float d = ofDist( mouseDownPoint.x, mouseDownPoint.y, x, y );
	
	if( d < 30 )	// little movement, will count as click.
	{
		b2Vec2 p( x / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE );
		
		for( int i=0; i<labels.size(); i++ )
		{
			ClockLabel* label = labels[ i ];
			
			for( b2Shape* s=label->body->GetShapeList(); s; s=s->GetNext() )
			{
				b2Body* shapeBody = s->GetBody();
				
				bool inside;
				inside = s->TestPoint( shapeBody->GetXForm(), p );
				
				if( inside )
				{
					if( i == 0 )
					{
						toggleInfoScreen();
					}
					else if( i == 1 )
					{
						toggleClockMode();
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////
//	PREPARE CIRCLES.
///////////////////////////////////////////////

void Clock :: initModeOne ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		circle.destroyJoint();						// destroy all joints.
	}
}

void Clock :: initModeTwo ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		circle.destroyJoint();						// destroy all joints.
	}
}

///////////////////////////////////////////////
//	CLOCK MODE 1.
///////////////////////////////////////////////

void Clock :: updateForces ()
{
	circlesActive.clear();
	circlesInactive.clear();
	
	updateForcesVec( hrsOne, digits[ 0 ].value );
	updateForcesVec( hrsTwo, digits[ 1 ].value );
	updateForcesVec( minOne, digits[ 2 ].value );
	updateForcesVec( minTwo, digits[ 3 ].value );
	updateForcesVec( secOne, digits[ 4 ].value );
	updateForcesVec( secTwo, digits[ 5 ].value );
}

void Clock :: updateForcesVec ( vector<ClockCircle*> &circlesVec, int count )
{
	for( int i=0; i<circlesVec.size(); i++ )
	{
		ClockCircle& circle	= *circlesVec[ i ];
		
		if( i < count )
		{
			if( clockMode == CLOCK_MODE_1 )
			{
				if( !circle.hasCenterJoint() )
					circle.createCenterJoint();
				
				spinInner( circle );		// spin force.
			}
			else if( clockMode == CLOCK_MODE_2 )
			{
				if( !circle.hasLineupJoint() )
					circle.createLineupJoint();
			}

			circle.active = true;
			circlesActive.push_back( &circle );
		}
		else 
		{
			if( clockMode == CLOCK_MODE_1 )
			{
				if( !circle.hasOuterJoint() )
					circle.createOuterJoint();
				
//				tilt( circle );
				spinOuter( circle );		// spin force.
			}
			else if( clockMode == CLOCK_MODE_2 )
			{
				if( circle.hasLineupJoint() )
					circle.destroyJoint();
				
				floatUp( circle );
			}
			
			circle.active = false;
			circlesInactive.push_back( &circle );
		}
		
		circle.update();
	}
}

//-- CM1 FORCES.

void Clock :: spinInner ( ClockCircle& circle )
{
	ofxVec2f v;
	v.set( ofGetWidth() * 0.5, ofGetHeight() * 0.5 );
	v -= circle.getPosition();
	v.perpendicular();
	v *= circle.spinFrc;
	v *= ( gravity.x > 0 ) ? -1.0 : 1.0;
	v *= 0.8;
	v *= screenScale;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

void Clock :: spinOuter ( ClockCircle& circle )
{
	ofxVec2f v;
	v.set( ofGetWidth() * 0.5, ofGetHeight() * 0.5 );
	v -= circle.getPosition();
	v.perpendicular();
	v *= circle.spinFrc;
	v *= ( gravity.x < 0 ) ? -1.0 : 1.0;
	v *= screenScale;
	v *= 0.5;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

void Clock :: pushFromCenter ( ClockCircle& circle )
{
	ofxVec2f v;
	v.set( circle.getPosition() );
	v -= screenCenter;
	
	//-- distance to keep from center, follow the shape of the inner blob.

	float d;
	d = 1 - ( v.length() / ( screenMinLength * 0.4 ) );
	
//	float ang;
//	ang = v.angle( ofxVec2f( 0, -1 ) );
//	ang *= -1;
//	ang += 180;
//	ang /= 360.0;
//
//	int ri = (int)( ang * RAY_BLOB_LO_RES );
//	ri = MAX( MIN( ri, RAY_BLOB_LO_RES - 1 ), 0 );
//
//	ofxVec2f rb;
//	rb.set( rayBlob[ ri ] );
//	rb -= screenCenter;
//	
//	ofxVec2f rv;
//	rv.set( 0, -1 );
//	rv.rotateRad( TWO_PI - v.angleRad( ofxVec2f( 0, -1 ) ) );
//	rv *= rb.length();
//	rv *= 1.5;
//	
//	rayPoints.push_back( rv + screenCenter );
//	
//	d = 1 - ( v.length() / ( rv.length() ) );

	//-- perpendicular force to center.
	
	ofxVec2f perp;							// spinning force.
	perp = v.getPerpendicular();
	perp *= circle.spinFrc;
	perp *= circle.spinDir;
	perp *= 3;
	
	//-- add forces.
	
	v.normalize();
	v *= d;
	v *= forceCenterPull;
//	v += perp;
	v *= screenScale;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

void Clock :: tilt ( ClockCircle& circle )
{
	float s = 0.3;
	
	b2Vec2 v = b2Vec2( gravity.x, gravity.y );
	if( ofDist( 0, 0, v.x, v.y ) < s )
	{
		v.x = 0;
		v.y = 0;
	}
	v *= 0.2;
	
	circle.body->ApplyImpulse( v, circle.body->GetWorldCenter() );
}

//-- CM2 FORCES.

void Clock :: floatUp ( ClockCircle& circle )
{
	float r;
	r = circle.getRadius();
	
	float s;
	s = ( circle.getPosition().y - r ) / ( screenHeight - ( r * 2 ) );
	
	float gx = gravity.x * 2;
	float gy = -30 * s;
	
	b2Vec2 up = b2Vec2( gx, gy );
	up *= screenScale;
	
	circle.body->ApplyImpulse( up, circle.body->GetWorldCenter() );
}

void Clock :: lineUp ( ClockCircle& circle )
{
	ofxVec2f p1;
	ofxVec2f p2;
	ofxVec2f v;
	
	p1.set( circle.lineUpPoint.x * screenWidth, circle.lineUpPoint.y * screenHeight );
	p2.set( circle.getPosition() );
	v = p1 - p2;
	
	float d;
	d = v.length() / (float)screenMaxLength;
	
	v.normalize();
	v *= d;
	v *= 30;
	v *= screenScale;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

//-- RAY BLOB.

void Clock :: updateRayBlob ()
{
	int h = RAY_BLOB_HI_RES;	// high res.
	int l = RAY_BLOB_LO_RES;	// low res - down sample.
	int s = h / l;				// sample.
	
	bool  bVerbose	= false;
	float blobDist	= screenMaxLength * 0.5;
	
	ofxVec2f p1;
	ofxVec2f p2;
	ofxVec2f hitPoint;
	ofxVec2f rayPoint;
	
	p2.set( screenWidth * 0.5, screenHeight * 0.5 );
	
	for( int i=0; i<h; i+=s )
	{
		if( bVerbose && ofGetFrameNum() == 0 )
			cout << "new loop" << endl;
		
		int count			= 0;
		float length		= 0;
		float lengthMax		= 0;
		float lengthTotal	= 0;
		
		for( int j=0; j<s; j++ )
		{
			//-------						// works out the index.
			
			int k = i + j;
			int m = k - (int)( s * 0.5 );
			
			if( m < 0 )
				m += h;
			
			if( m > h - 1 )
				break;
			
			//-------						// prints index for debugging.
			
			if( bVerbose && ofGetFrameNum() == 0 )
				cout << m << endl;
			
			//-------						// works out average length of samples.
			
			float p;
			p = m / (float)h;
			
			p1.set( 0, -1 );
			p1.rotate( p * TWO_PI * RAD_TO_DEG );
			p1 *= blobDist;
			p1 += p2;
			
			hitPoint.set( 0, 0 );
			
			for( int c=0; c<circlesActive.size(); c++ )
			{
				bool bHit;
				
				ClockCircle& circle = *circlesActive[ c ];
				bHit = circle.raycast( p1, p2, &rayPoint );
				
				if( bHit )
				{
					rayPoint -= p2;
					
					if( rayPoint.length() > hitPoint.length() )
					{
						hitPoint.set( rayPoint );
					}
				}
			}
			
			hitPoint += hitPoint.getNormalized() * ( rayBlobPad * screenHeight );
			
			length		= hitPoint.length();
			lengthMax	= ( length > lengthMax ) ? length : lengthMax;
			lengthTotal	+= length;
			count		+= 1;
		}
		
		//-------							// calc average.
		
		float lengthAvg;
		lengthAvg = length / (float)count;

		float p;
		p = i / (float)h;
		
		p1.set( 0, -1 );
		p1.rotate( p * TWO_PI * RAD_TO_DEG );
//		p1 *= lengthAvg;
		p1 *= lengthMax;
		p1 += p2;
		
		int n = i / s;
		ofPoint& rayPoint = rayBlob[ n ];
		
		rayPoint.x += ( p1.x - rayPoint.x ) * rayBlobEase;
		rayPoint.y += ( p1.y - rayPoint.y ) * rayBlobEase;
	}
}

void Clock :: updateConvexBlob()
{
	convexBlobInner.clear();
	convexBlobOuter.clear();

	vector<ClockCircle*>* circles;
	
	//-- inner blob.
	
	circles = &circlesActive;
	
	for( int i=0; i<circles->size(); i++ )
	{
		ClockCircle& circle = *circles->at( i );
		
		ofPoint p1 = circle.getPosition();
		
		for( int j=0; j<circle.pointsTotal; j++ )
		{
			const ofPoint& p2 = circle.points1[ j ];
			
			convexBlobInner.push_back( ofPoint() );
			ofPoint& p3 = convexBlobInner.back();
			
			p3.x = p1.x + p2.x;
			p3.y = p1.y + p2.y;
		}
	}
	
	contourUtil.convexHull( convexBlobInner );
	
	//-- outer blob.
	
	if( clockMode == CLOCK_MODE_1 )
	{
		circles = &circlesAll;
	}
	else
	{
		circles = &circlesInactive;
	}


	for( int i=0; i<circles->size(); i++ )
	{
		ClockCircle& circle = *circles->at( i );
		
		ofPoint p1 = circle.getPosition();
		
		for( int j=0; j<circle.pointsTotal; j++ )
		{
			ofPoint p2;
			
			if( clockMode == CLOCK_MODE_1 )
			{
				p2 = circle.points2[ j ];
			}
			else
			{
				p2 = circle.points1[ j ];
			}

			
			convexBlobOuter.push_back( ofPoint() );
			ofPoint& p3 = convexBlobOuter.back();
			
			p3.x = p1.x + p2.x;
			p3.y = p1.y + p2.y;
		}
	}

	contourUtil.convexHull( convexBlobOuter );
}

void Clock :: updateDelaunay ()
{
	//-- OUTER CIRCLE.
	
	delaunay.reset();
	
//	delaunay.addPoints( convexBlobOuter );				//-- add outer convex points.
	
	for( int i=0; i<circlesInactive.size(); i++ )		//-- add intactive circle center points.
		delaunay.addPoint( circlesInactive.at( i )->getPosition() );
	
	if( clockMode == CLOCK_MODE_1 )						//-- add center point.
		delaunay.addPoint( ofPoint( screenWidth * 0.5, screenHeight * 0.5 ) );
	
	delaunay.triangulate();
	delaunayTrg1 = delaunay.triangles;
	
	//-- INNER CIRCLE.
	
	delaunay.reset();
	
//	delaunay.addPoints( convexBlobInner );				//-- add outer convex points.
	
	for( int i=0; i<circlesActive.size(); i++ )			//-- add active circle center points.
		delaunay.addPoint( circlesActive.at( i )->getPosition() );
	
	delaunay.triangulate();
	delaunayTrg2 = delaunay.triangles;
}

//-- CLOCK MODE.

void Clock :: toggleClockMode ()
{
	if( clockMode == CLOCK_MODE_1 )
	{
		clockMode = CLOCK_MODE_2;
		
		initModeTwo();
	}
	else if( clockMode == CLOCK_MODE_2 )
	{
		clockMode = CLOCK_MODE_1;
		
		initModeOne();
	}
}

///////////////////////////////////////////////
//	CONTACT HANDLER.
///////////////////////////////////////////////

void Clock :: box2dContactEventHandler ( const b2ContactPoint* p )
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		
		for( b2Shape* s=circle.body->GetShapeList(); s; s=s->GetNext() )
		{
			if( p->shape1 == s || p->shape2 == s )
			{
				float vel;								// weight colour change by velocity.
				vel = p->velocity.Length();
				vel /= 10;
				vel = ofClamp( vel, 0, 1 );
				
				if( vel > 0.5 )
				{
					circle.colorTrgt.r = 255;
					circle.colorTrgt.g = 255;
					circle.colorTrgt.b = 255;
					
//					circle.colorCurr.r = circle.colorTrgt.r;
//					circle.colorCurr.g = circle.colorTrgt.g;
//					circle.colorCurr.b = circle.colorTrgt.b;
				}
			}
		}
	}
}

///////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////

void Clock :: draw ( int x, int y )
{
	bool bMove = ( x != 0 || y != 0 );
	
	if( bMove )
	{
		glPushMatrix();
		glTranslatef( x, y, 0 );
	}
	
	ofSetColor( 255, 255, 255 );
	drawBg();
	
	drawTime();
	drawLines();

//	drawFreeCircles();
	
	ofSetColor( 95, 63, 34, 30 );
	drawConvexBlob( convexBlobOuter );
	drawConvexBlobRim( convexBlobOuter );
	drawDelaunay( delaunayTrg1 );
	drawCircles( circlesInactive );
	
	ofSetColor( 95, 63, 34, 30 );
	drawConvexBlob( convexBlobInner );
	drawConvexBlobRim( convexBlobInner );
	drawDelaunay( delaunayTrg2 );
	drawCircles( circlesActive );
	
	if( clockMode == CLOCK_MODE_1 )
	{
		drawCircleNumbers();
	}
	
//	drawLabels();
	
	if( bMove )
	{
		glPopMatrix();
	}
}

void Clock :: drawBg ()
{
	if( texBg == NULL )
		return;
	
	texBg->draw( 0, 0, screenWidth, screenHeight );
}

void Clock :: drawLines ()
{
	ofEnableAlphaBlending();
	
	glPushMatrix();
	glTranslatef( screenWidth * 0.5, screenHeight * 0.5, 0 );
	
	for( int i=0; i<lines.size(); i++ )
	{
		lines[ i ].draw( screenDiagonal );
	}
	
	glPopMatrix();
	
	ofDisableAlphaBlending();
}

void Clock :: drawCircles ( vector<ClockCircle*>& circles )
{
	for( int i=0; i<circles.size(); i++ )
	{
		circles[ i ]->draw();
	}
}

void Clock :: drawCircleLines ( vector<ClockCircle*>& circles )
{
	for( int i=0; i<circles.size(); i++ )
	{
		drawCircleLine( *circles[ i ] );
	}
}

void Clock :: drawCircleLine ( ClockCircle &circle )
{
	ofEnableAlphaBlending();
	
	ofSetColor( 255, 255, 255, 30 );
	ofLine( screenCenter.x, screenCenter.y, circle.eye.x, circle.eye.y );
	
	ofDisableAlphaBlending();
}

void Clock :: drawCircleNumbers ()
{
//	for( int i=0; i<circlesInactive.size(); i++ )
//	{
//		ClockCircle* circle = circlesInactive[ i ];
//		circle->drawInfoLine();
//	}

	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 0, 0, 0, 95 );

	float size = screenHeight * 0.025;
	
	for( int i=0; i<circlesInactive.size(); i++ )
	{
		ClockCircle* circle = circlesInactive[ i ];
		
		float x = circle->linePoint2.x - size * 0.5;
		float y = circle->linePoint2.y - size * 0.5;
		
		texDigits[ circle->digitIndex ].draw( x, y, size, size );
	}
	
	ofDisableAlphaBlending();
}

void Clock :: drawFreeCircles ()
{
	ofEnableAlphaBlending();
	
	for( int i=0; i<freeCircles.size(); i++ )
	{
		freeCircles[ i ]->draw();
	}
	
	ofDisableAlphaBlending();
}

void Clock :: drawLabels ()
{
	for( int i=0; i<labels.size(); i++ )
	{
		labels[ i ]->draw();
	}
}

void Clock :: drawTime ()
{
	if( texDigits == NULL )
		return;
	
	ofEnableAlphaBlending();

//	ofSetColor( 95, 63, 34, 180 );		// brownie.
	ofSetColor( 76, 66, 54, 150 );
	
	float size	= 65 * MIN( screenWidthScale, screenHeightScale );
	float x		= 0;
	float y		= screenHeight * 0.905;
	
	for( int i=0; i<6; i++ )
	{
		x = digits[ i ].p.x * screenWidth;
		texDigits[ digits[ i ].value ].draw( x, y, size, size  );
	}
	
	ofDisableAlphaBlending();
}

void Clock :: drawRayCasts ()
{
	int t = RAY_BLOB_LO_RES;
	
	ofEnableAlphaBlending();

	for( int i=0; i<t; i++ )
	{
		float p;
		p = i / (float)t;
		
		if( p > 1.0 )	// just to test its being drawn correctly.
			return;
		
		ofxVec2f v;
		v.set( 0, -1 );
		v.rotate( p * TWO_PI * RAD_TO_DEG );
		v *= screenMinLength * 0.4;
		
		ofxVec2f cp;
		cp.set( screenWidth * 0.5, screenHeight * 0.5 );

		ofNoFill();
		ofSetColor( 255, 0, 0, 100 );
		ofLine( cp.x, cp.y, cp.x + v.x, cp.y + v.y );
		
		ofPoint& rayPoint = rayBlob[ i ];
		
		ofFill();
		ofSetColor( 0, 255, 0, 100 );
		ofCircle( rayPoint.x, rayPoint.y, 4 );
	}
	
	ofDisableAlphaBlending();
}

void Clock :: drawRayBlob ()
{
	int t = RAY_BLOB_LO_RES;

	ofNoFill();
	ofSetColor( 255, 255, 255, 100 );
	ofSetLineWidth( 2 );
	ofEnableAlphaBlending();
	
	ofBeginShape();
	
	for( int i=0; i<t+3; i++ )
	{
		int j = i % t;
		
		ofPoint& rayPoint = rayBlob[ j ];
		
		ofCurveVertex( rayPoint.x, rayPoint.y );
	}
	
	ofEndShape( true );
	
	ofSetLineWidth( 1 );
	ofDisableAlphaBlending();
}

void Clock :: drawConvexBlob ( const vector<ofPoint>& points )
{
	ofFill();
	ofEnableAlphaBlending();
	
	ofBeginShape();
	
	bool bCurve = false;

	int t = points.size();
	
	if( bCurve )
		t += 3;
	
	for( int i=0; i<t; i++ )
	{
		int j = i % t;
		
		if( bCurve )
		{
			ofCurveVertex( points[ j ].x, points[ j ].y );
		}
		else
		{
			ofVertex( points[ j ].x, points[ j ].y );
		}
	}
	
	ofEndShape( true );
	
	ofDisableAlphaBlending();
}

void Clock :: drawConvexBlobRim	( const vector<ofPoint>& points )
{
	if( texMembrane == NULL )
		return;

	vector<ofPoint> normals;
	geom.createNormals( points, normals );
	
	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, 100 );
	
	//--- draw rim.
	
	texMembrane->bind();
	
	int t;
	t = points.size() - 1;

	float circ	= 0;				// circumference.
	float c		= 0;
	for( int i=0; i<t; i++ )
	{
		int j	= ( i + 1 ) % t;
		float d = ofDist( points[ i ].x, points[ i ].y, points[ j ].x, points[ j ].y );
		c += d;
	}
	circ = c;
	c = 0;
	
	float p;
	
	GLfloat* ver_coords = new GLfloat[ ( t + 1 ) * 4 ];
	GLfloat* tex_coords = new GLfloat[ ( t + 1 ) * 4 ];
	
	for( int i=0; i<( t + 1 ); i++ )
	{
		int j = i % t;
		int k = ( i - 1 ) % t;
		
		if( i > 0 )
			c += ofDist( points[ j ].x, points[ j ].y, points[ k ].x, points[ k ].y );
		
		p = c / circ;
		
		ver_coords[ i * 4 + 0 ] = points[ j ].x;
		ver_coords[ i * 4 + 1 ] = points[ j ].y;
		ver_coords[ i * 4 + 2 ] = points[ j ].x - normals[ j ].x * 20;
		ver_coords[ i * 4 + 3 ] = points[ j ].y - normals[ j ].y * 20;
		
		ofPoint tx1 = texMembrane->getCoordFromPercent( p, 0 );
		ofPoint tx2 = texMembrane->getCoordFromPercent( p, 1 );
		
		tex_coords[ i * 4 + 0 ] = tx1.x;
		tex_coords[ i * 4 + 1 ] = tx1.y;
		tex_coords[ i * 4 + 2 ] = tx2.x;
		tex_coords[ i * 4 + 3 ] = tx2.y;
	}
	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );		
	
	glTexCoordPointer( 2, GL_FLOAT, 0, tex_coords );
	glVertexPointer( 2, GL_FLOAT, 0, ver_coords );
	
	glDrawArrays( GL_TRIANGLE_STRIP, 0, ( t + 1 ) * 2 );
	
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	
	delete[] ver_coords;
	delete[] tex_coords;
	
	texMembrane->unbind();
	
	ofDisableAlphaBlending();
}

void Clock :: drawDelaunay ( vector<ofxDelaunayTriangle>& triangles )
{
	ofNoFill();
	ofSetColor( 0, 0, 0, 20 );
	
	ofEnableAlphaBlending();

	for( int i=0; i<triangles.size(); i++ )
	{
		ofxDelaunayTriangle& triangle = triangles[ i ];
		
		ofTriangle
		(
			triangle.points[ 0 ].x,
			triangle.points[ 0 ].y,
			triangle.points[ 1 ].x,
			triangle.points[ 1 ].y,
			triangle.points[ 2 ].x,
			triangle.points[ 2 ].y
		);
	}
	
	ofDisableAlphaBlending();
}

///////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////

#ifdef TARGET_OF_IPHONE

void Clock :: touchDown	( int x, int y, int id )
{
	bMouseDown		= true;
	mouseDownCount	= 0;
	mouseDownPoint.set( x, y );
}

void Clock :: touchMoved ( int x, int y, int id )
{
	bMouseDown		= false;
}

void Clock :: touchUp ( int x, int y, int id )
{
	bMouseDown		= false;
	
	if( infoScreen.bShowing )
	{
		toggleInfoScreen();
	}
	else
	{
//		checkLabelPress( touch.x, touch.y );
	}
}

#else

void Clock :: mouseMoved( ofMouseEventArgs &e )
{
	//
}

void Clock :: mousePressed( ofMouseEventArgs &e )
{
	bMouseDown		= true;
	mouseDownCount	= 0;
	mouseDownPoint.set( e.x, e.y );
}

void Clock :: mouseDragged( ofMouseEventArgs &e )
{
	bMouseDown		= false;
}

void Clock :: mouseReleased( ofMouseEventArgs &e )
{
	bMouseDown		= false;
	
	if( infoScreen.bShowing )
	{
		toggleInfoScreen();
	}
	else
	{
//		checkLabelPress( e.x, e.y );
	}
}

#endif