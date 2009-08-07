
#include "testApp.h"

///////////////////////////////////////////////////////////
// SETUP.
///////////////////////////////////////////////////////////

void testApp::setup()
{
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(true);
	ofSetFrameRate(60);
	ofDisableArbTex();
	ofEnableAlphaBlending();
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	ofxAccelerometer.setup();

	ofxMultiTouch.addListener(this);
	
	initGeom();
	initBg();
	initEarth();
	initLocations();
	initRibbons();
	initRays();
	
	initTrackball();
	mouseSphereInit();
//	initLighting();
}

void testApp :: initGeom()
{
	planeBottomCenter[ 0 ]  = -0.5f;
	planeBottomCenter[ 1 ]  =  0.0f;
	planeBottomCenter[ 2 ]  =  0.0f;
	planeBottomCenter[ 3 ]  =  0.5f;
	planeBottomCenter[ 4 ]  =  0.0f;
	planeBottomCenter[ 5 ]  =  0.0f;
	planeBottomCenter[ 6 ]  = -0.5f;
	planeBottomCenter[ 7 ]  =  1.0f;
	planeBottomCenter[ 8 ]  =  0.0f;
	planeBottomCenter[ 9 ]  =  0.5f;
	planeBottomCenter[ 10 ] =  1.0f;
	planeBottomCenter[ 11 ] =  0.0f;
	
	planeCenter[ 0 ]  = -0.5f;
	planeCenter[ 1 ]  = -0.5f;
	planeCenter[ 2 ]  =  0.0f;
	planeCenter[ 3 ]  =  0.5f;
	planeCenter[ 4 ]  = -0.5f;
	planeCenter[ 5 ]  =  0.0f;
	planeCenter[ 6 ]  = -0.5f;
	planeCenter[ 7 ]  =  0.5f;
	planeCenter[ 8 ]  =  0.0f;
	planeCenter[ 9 ]  =  0.5f;
	planeCenter[ 10 ] =  0.5f;
	planeCenter[ 11 ] =  0.0f;
	
	planeTexture[ 0 ] = 0.0f;
	planeTexture[ 1 ] = 1.0f;
	planeTexture[ 2 ] = 1.0f;
	planeTexture[ 3 ] = 1.0f;
	planeTexture[ 4 ] = 0.0f;
	planeTexture[ 5 ] = 0.0f;
	planeTexture[ 6 ] = 1.0f;
	planeTexture[ 7 ] = 0.0f;
	
	planeTextureInv[ 0 ] = 0.0f;
	planeTextureInv[ 1 ] = 0.0f;
	planeTextureInv[ 2 ] = 1.0f;
	planeTextureInv[ 3 ] = 0.0f;
	planeTextureInv[ 4 ] = 0.0f;
	planeTextureInv[ 5 ] = 1.0f;
	planeTextureInv[ 6 ] = 1.0f;
	planeTextureInv[ 7 ] = 1.0f;
}

void testApp :: initBg()
{
//	earthBg.loadImage( "bg01_512x512.jpg" );
//	earthBg.loadImage( "bg02_512x512.jpg" );
//	earthBg.loadImage( "bg03_512x512.jpg" );
//	earthBg.loadImage( "bg04_512x512.jpg" );
	earthBg.loadImage( "bg05_512x512.jpg" );
}

void testApp :: initEarth()
{
	ofxVec3f right;
	ofxVec3f up;
	ofxVec3f out;
	
	right = ofxVec3f(  1,  0,  0 );
	up    = ofxVec3f(  0,  0, -1 );
	out   = ofxVec3f(  0, -1,  0 );
	
	sphereOrientation[ 0 ]  = right.x;
	sphereOrientation[ 1 ]  = right.y;
	sphereOrientation[ 2 ]  = right.z;
	sphereOrientation[ 3 ]  = 0;
	sphereOrientation[ 4 ]  = up.x;
	sphereOrientation[ 5 ]  = up.y;
	sphereOrientation[ 6 ]  = up.z;
	sphereOrientation[ 7 ]  = 0;
	sphereOrientation[ 8 ]  = out.x;
	sphereOrientation[ 9 ]  = out.y;
	sphereOrientation[ 10 ] = out.z;
	sphereOrientation[ 11 ] = 0;
	sphereOrientation[ 12 ] = 0;
	sphereOrientation[ 13 ] = 0;
	sphereOrientation[ 14 ] = 0;
	sphereOrientation[ 15 ] = 1;
	
//	earthMap.loadImage( "earth_texture01_1024x512.jpg" );
//	earthMap.loadImage( "earth_texture02_1024x512.jpg" );
//	earthMap.loadImage( "earth_texture03_1024x512.jpg" );
	earthMap.loadImage( "earth_texture04_1024x512.jpg" );
}

void testApp :: initLocations()
{
	locationIndex	= 0;
	locationsTotal	= 0;
	locationsMax	= 200;
	locations		= new float[ locationsMax * 2 ];	// latitude and longitude pairs.
	
	addLocation( -33.87784462833714, 151.2179660797119 );	// Holler Sydney
	addLocation( 35.689488, 139.691706 );					// Tokyo
	addLocation( 9.009697, -79.603243 );					// Panama City
	addLocation( 40.756054, -73.986951 );					// New York
	addLocation( 48.856667, 2.350987 );						// Paris
	addLocation( 25.271139, 55.307485 );					// Dubai
	addLocation( 51.500152, -0.126236 );					// London

	ofxVec3f p = sphericalToCartesian( 55.676294, 12.568116, 1 );
	copenhagen[ 0 ] = p.x;
	copenhagen[ 1 ] = p.y;
	copenhagen[ 2 ] = p.z;
	
//	for( int i=0; i<locationsMax; i++ )
//	{
//		float rndLat = ofRandom( -90, 90 );
//		float rndLon = ofRandom( -180, 180 );
//		
//		addLocation( rndLat, rndLon );
//	}
}

void testApp :: addLocation( float lat, float lon )
{
	locations[ locationsTotal * 2 + 0 ] = lat;
	locations[ locationsTotal * 2 + 1 ] = lon;
	
	++locationsTotal;
}

void testApp :: initRibbons()
{
	ribbonsTotal = 10;
	ribbons		 = new RibbonInfo[ ribbonsTotal ];
	
	for( int i=0; i< ribbonsTotal; i++ )
	{
		float progressStep	= ofRandom( 0.005, 0.01 );
		float curveWidth	= 0.02;
		float curveHeight	= ofRandom( 0.2, 0.5 );
		int locationIndex	= (int)( ofRandom( 0, locationsTotal - 1 ) );
		
//		ribbons[ i ].ribbon = new SphereRibbon();
		ribbons[ i ].ribbon = new QuatRibbon();
//		ribbons[ i ].ribbon = new CurveHop();
		ribbons[ i ].ribbon->setProgressStep( progressStep );
		ribbons[ i ].ribbon->setCurveWidth( curveWidth );
		ribbons[ i ].ribbon->setCurveHeight( curveHeight );
		ribbons[ i ].locationIndex = locationIndex;
		
		updateRibbonLocation( &ribbons[ i ] );
	}
}

void testApp :: initRays()
{
	raysTotal = locationsTotal;
	rays = new Ray[ raysTotal ];
	
	for( int i=0; i<raysTotal; i++ )
	{
		float lat = locations[ i * 2 + 0 ];
		float lon = locations[ i * 2 + 1 ];
		
		rays[ i ].p.set( sphericalToCartesian( lat, lon, 1 ) );
		rays[ i ].p.normalize();
	}
	
	rayTarget		= new float[ 2 ];
	rayTarget[ 0 ]	= 0;
	rayTarget[ 1 ]	= 0;

	rayImage.loadImage( "white01.png" );
	rayBaseImage.loadImage( "white_base01.png" );
}

void testApp :: initLighting()
{
//	lightAmbient[ 0 ] = 0.8f;
//	lightAmbient[ 1 ] = 0.8f;
//	lightAmbient[ 2 ] = 0.8f;
//	lightAmbient[ 3 ] = 1.0f;
//	
//	lightDiffuse[ 0 ] = 0.2f;
//	lightDiffuse[ 1 ] = 0.2f;
//	lightDiffuse[ 2 ] = 0.2f;
//	lightDiffuse[ 3 ] = 1.0f;
//	
//	matAmbient[ 0 ] = 0.6f;
//	matAmbient[ 1 ] = 0.6f;
//	matAmbient[ 2 ] = 0.6f;
//	matAmbient[ 3 ] = 1.0f;
//
//	matDiffuse[ 0 ] = 0.6f;
//	matDiffuse[ 1 ] = 0.6f;
//	matDiffuse[ 2 ] = 0.6f;
//	matDiffuse[ 3 ] = 1.0f;
	
	float lightAmbient[]	= { 0.8f, 0.8f, 0.8f, 1.0f };
	float lightDiffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
	float position[]		= { 1.0f, 1.0f, 0.0f, 0.0f };
	
//	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient );
//	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse );
	
	glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );
	glLightfv( GL_LIGHT0, GL_POSITION, position );
}

///////////////////////////////////////////////////////////
// TRACKBALL.
///////////////////////////////////////////////////////////

void testApp :: initTrackball ()
{
	gTrackBallRotation[ 0 ] = 0.0f;
	gTrackBallRotation[ 1 ] = 0.0f;
	gTrackBallRotation[ 2 ] = 0.0f;
	gTrackBallRotation[ 3 ] = 0.0f;

	gWorldRotation[ 0 ]	= 0.0f;
	gWorldRotation[ 1 ]	= 0.0f;
	gWorldRotation[ 2 ]	= 0.0f;
	gWorldRotation[ 3 ]	= 0.0f;
	
	center.set( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f );
	
	gTrackBall = false;
}

void testApp :: mouseTrackballDown( int x, int y )
{
	startTrackball( x, y, 0, 0, ofGetWidth(), ofGetHeight() );
	gTrackBall = true;
}

void testApp :: mouseTrackballUp( int x, int y )
{
	gTrackBall = false;
	rollToTrackball( x, y, gTrackBallRotation );
	if( gTrackBallRotation[0] != 0.0 )
	{
		addToRotationTrackball( gTrackBallRotation, gWorldRotation );
	}
	gTrackBallRotation[ 0 ] = gTrackBallRotation[ 1 ] = gTrackBallRotation[ 2 ] = gTrackBallRotation[ 3 ] = 0.0f;
}

void testApp :: mouseTrackballMove( int x, int y )
{
	if (gTrackBall)
	{
		rollToTrackball( x, y, gTrackBallRotation );
	}
}

///////////////////////////////////////////////////////////
// MOUSE SPHERE.
///////////////////////////////////////////////////////////

void testApp :: mouseSphereInit()
{
	isMouseDown = false;
}

void testApp :: mouseSphereDown( int x, int y )
{
	isMouseDown = true;
	
	mouseCurr.x = x;
	mouseCurr.y = y;
}

void testApp :: mouseSphereUp( int x, int y )
{
//	mousePrev.x = mouseCurr.x;
//	mousePrev.y = mouseCurr.y;
//	
//	mouseCurr.x = x;
//	mouseCurr.y = y;
	
	isMouseDown = false;
}

void testApp :: mouseSphereMove( int x, int y )
{
	mousePrev.x = mouseCurr.x;
	mousePrev.y = mouseCurr.y;
	
	mouseCurr.x = x;
	mouseCurr.y = y;
	
	float mp[ 2 ];
	float mc[ 2 ];
	float cp[ 2 ];
	
	cp[ 0 ] = center.x;
	cp[ 1 ] = center.y;
	
	if( isMouseDown )
	{
		mp[ 0 ] = mousePrev.x;
		mp[ 1 ] = mousePrev.y;
		
		mc[ 0 ] = mouseCurr.x;
		mc[ 1 ] = mouseCurr.y;
		
		mouseVel = mouseCurr - mousePrev;
		
		rollToTrackballSingle( cp, mc, mp, gWorldRotation );
	}
}

void testApp :: mouseSphereMoveResidual()
{
	float mp[ 2 ];
	float mc[ 2 ];
	float cp[ 2 ];
	
	if( isMouseDown )
		return;
	
	if( mouseVel.length() == 0 )
		return;
	
	mouseVel *= 0.95;
	
	if( mouseVel.length() < 0.01 )
	{
		mouseVel.set( 0, 0 );
		return;
	}
	
	cp[ 0 ] = center.x;
	cp[ 1 ] = center.y;
	
	mp[ 0 ] = mouseCurr.x;
	mp[ 1 ] = mouseCurr.y;

	mc[ 0 ] = mouseCurr.x + mouseVel.x;
	mc[ 1 ] = mouseCurr.y + mouseVel.y;
	
	rollToTrackballSingle( cp, mc, mp, gWorldRotation );
}

///////////////////////////////////////////////////////////
// UPDATE.
///////////////////////////////////////////////////////////

void testApp::update()
{
	mouseSphereMoveResidual();
	
	updateRibbons();
}

void testApp :: updateRibbons()
{
	for( int i=0; i<ribbonsTotal; i++ )
	{
		if( !ribbons[ i ].ribbon->isPlayedIn() )
		{
			ribbons[ i ].ribbon->playInStep();
		}
		else
		{
			if( !ribbons[ i ].ribbon->isPlayedOut() )
			{
				ribbons[ i ].ribbon->playOutStep();
			}
			else
			{
				stepRibbonLocation( &ribbons[ i ] );
				updateRibbonLocation( &ribbons[ i ] );
			}
		}
	}
}

void testApp :: stepRibbonLocation( RibbonInfo *ribbon )
{
	if( ++ribbon->locationIndex > locationsTotal - 1 )
		ribbon->locationIndex = 0;
}

void testApp :: updateRibbonLocation( RibbonInfo *ribbon )
{
	int i = ribbon->locationIndex;
	int j = ribbon->locationIndex + 1;
	
	if( j > locationsTotal - 1 )
		j = 0;
	
	float *points	= new float[ 4 ];
	points[ 0 ]		= locations[ i * 2 + 0 ];
	points[ 1 ]		= locations[ i * 2 + 1 ];
	points[ 2 ]		= locations[ j * 2 + 0 ];
	points[ 3 ]		= locations[ j * 2 + 1 ];
	
	ribbon->ribbon->reset();
	ribbon->ribbon->set( points );
	
	delete[] points;
}

///////////////////////////////////////////////////////////
// DRAW.
///////////////////////////////////////////////////////////

void testApp::draw()
{
	drawBg();
	
	float s = 290;
	
	glPushMatrix();
	glTranslatef( ofGetWidth() * 0.5, ofGetHeight() * 0.5, -ofGetHeight() );
	glScalef( s, s, s );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glRotatef( gTrackBallRotation[ 0 ], gTrackBallRotation[ 1 ], gTrackBallRotation[ 2 ], gTrackBallRotation[ 3 ] );
	glRotatef( gWorldRotation[ 0 ], gWorldRotation[ 1 ], gWorldRotation[ 2 ], gWorldRotation[ 3 ] );
	
	drawSphere();
	drawRibbons();
	drawRays();

//	drawLines();
//	drawTriangle();
//	drawCube();
	
	glPopMatrix();
}

void testApp :: drawBg()
{
	glDisable( GL_DEPTH_TEST );
	
	glPushMatrix();
	
	int screenW = 320;
	int screenH = 480;
	
	glTranslatef( (float)( screenW * 0.5f ), (float)( screenH * 0.5f ), 0 );
	glScalef( 512, 512, 0 );
	
	earthBg.getTextureReference().bind();
	
	glVertexPointer( 3, GL_FLOAT, 0, planeCenter );
	glTexCoordPointer( 2, GL_FLOAT, 0, planeTexture );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	
	earthBg.getTextureReference().unbind();
	
	glPopMatrix();
	
	glEnable( GL_DEPTH_TEST );
}

void testApp :: drawSphere ()
{
	glPushMatrix();
	glMultMatrixf( &sphereOrientation[ 0 ] );
	
	earthMap.getTextureReference().bind();
	sphere.setSphereDrawStyle( 100012 );
//	sphere.setSphereDrawStyle( 100013 );
	sphere.drawSphere( 1, 20, 20 );
	earthMap.getTextureReference().unbind();
	
	glPopMatrix();
}

void testApp :: drawRibbons()
{
	for( int i=0; i<ribbonsTotal; i++ )
	{
		ribbons[ i ].ribbon->drawLine();
//		ribbons[ i ].ribbon->drawFill();
	}
}

void testApp :: drawRays()
{
	glDepthMask( GL_FALSE );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE );	// additive blending.
		
	ofxMatrix4x4 mvMat;						// model view matrix, to work out the eye position.
	float modelview[ 16 ];
	glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
	
	mvMat._mat[ 0 ][ 0 ] = modelview[ 0 ];
	mvMat._mat[ 0 ][ 1 ] = modelview[ 1 ];
	mvMat._mat[ 0 ][ 2 ] = modelview[ 2 ];
	mvMat._mat[ 0 ][ 3 ] = modelview[ 3 ];
	mvMat._mat[ 1 ][ 0 ] = modelview[ 4 ];
	mvMat._mat[ 1 ][ 1 ] = modelview[ 5 ];
	mvMat._mat[ 1 ][ 2 ] = modelview[ 6 ];
	mvMat._mat[ 1 ][ 3 ] = modelview[ 7 ];
	mvMat._mat[ 2 ][ 0 ] = modelview[ 8 ];
	mvMat._mat[ 2 ][ 1 ] = modelview[ 9 ];
	mvMat._mat[ 2 ][ 2 ] = modelview[ 10 ];
	mvMat._mat[ 2 ][ 3 ] = modelview[ 11 ];
	mvMat._mat[ 3 ][ 0 ] = modelview[ 12 ];
	mvMat._mat[ 3 ][ 1 ] = modelview[ 13 ];
	mvMat._mat[ 3 ][ 2 ] = modelview[ 14 ];
	mvMat._mat[ 3 ][ 3 ] = modelview[ 15 ];
	
	ofxMatrix4x4 mvMatInv;
    mvMatInv.invert( mvMat );
	ofxVec3f eye = ofxVec3f( 0.0, 0.0, 0.0 ) * mvMatInv;
	
	float e[] =  { eye.x, eye.y, eye.z };
	float rs[] = { 0.03, 0.50, 0.05 };
	float rc[] = { 0.94, 0.40, 0.18, 0.70 };
	float cs[] = { 0.06, 0.60, 0.05 };
	float cc[] = { 1.00, 1.00, 1.00, 0.70 };
	
	rayImage.getTextureReference().bind();
	for( int i=0; i<raysTotal; i++ )
	{
		float rp[] = { rays[ i ].p.x, rays[ i ].p.y, rays[ i ].p.z };
		drawRay( &rp[ 0 ], &e[ 0 ], &rs[ 0 ], &rc[ 0 ] );
	}
	drawRay( &copenhagen[ 0 ], &e[ 0 ], &cs[ 0 ], &cc[ 0 ] );
	rayImage.getTextureReference().unbind();
	
	float bs[] = { 0.05, 0.05, 0.05 };
	float bc[] = { 0.94, 0.40, 0.18, 0.70 };
	float cbs[] = { 0.10, 0.10, 0.00 };
	float cbc[] = { 1.00, 1.00, 1.00, 0.70 };
	
	rayBaseImage.getTextureReference().bind();
	for( int i=0; i<raysTotal; i++ )
	{
		float bp[] = { rays[ i ].p.x, rays[ i ].p.y, rays[ i ].p.z };
		drawRayBase( &bp[ 0 ], &bs[ 0 ], &bc[ 0 ] );
	}
	drawRayBase( &copenhagen[ 0 ], &cbs[ 0 ], &cbc[ 0 ] );
	rayBaseImage.getTextureReference().unbind();
	
	glDepthMask( GL_TRUE );
//	glBlendFunc( GL_ONE, GL_ZERO );
}

void testApp :: drawRay( float *p, float *e, float *s, float *c )
{
	ofxVec3f eye;	// camera eye vector.
	ofxVec3f uv;	// up vector.
	ofxVec3f rv;	// right vector.
	ofxVec3f ov;	// out vector.

	eye.set( e[ 0 ], e[ 1 ], e[ 2 ] );
	
	uv.set( p[ 0 ], p[ 1 ], p[ 2 ] );
	rv = eye - uv;
	rv.normalize();
	ov = rv.getCrossed( uv );
	ov.normalize();
	rv = ov.getCrossed( uv );
	rv.normalize();
	
	float *mat = new float[ 16 ];
	mat[0]	= rv.x;
	mat[1]	= rv.y;
	mat[2]	= rv.z;
	mat[3]	= 0;
	mat[4]	= uv.x;
	mat[5]	= uv.y;
	mat[6]	= uv.z;
	mat[7]	= 0;
	mat[8]	= ov.x;
	mat[9]	= ov.y;
	mat[10]	= ov.z;
	mat[11]	= 0;
	mat[12]	= 0;
	mat[13]	= 0;
	mat[14]	= 0;
	mat[15]	= 1;
	
	glPushMatrix();
	glTranslatef( p[ 0 ], p[ 1 ], p[ 2 ] );
	
	glPushMatrix();	// draw ray.
	
	glMultMatrixf( mat );
	delete[] mat;
	glRotatef( 90, 0, 1, 0 );
	
	glScalef( s[ 0 ], s[ 1 ], s[ 2 ] );
	
	glVertexPointer( 3, GL_FLOAT, 0, planeBottomCenter );
	glTexCoordPointer( 2, GL_FLOAT, 0, planeTextureInv );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glColor4f( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ] );		// blood orange.
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	
	glPopMatrix();
	
	glPopMatrix();
}

void testApp :: drawRayBase( float *p, float *s, float *c )
{
	glPushMatrix();
	glTranslatef( p[ 0 ], p[ 1 ], p[ 2 ] );
	
	float *rbm = new float[ 16 ];		// ray base matrix.
	rotateToSphereSurface( rbm, p[ 0 ],  p[ 1 ],  p[ 2 ] );
	glMultMatrixf( rbm );
	delete[] rbm;
	
	glScalef( s[ 0 ], s[ 1 ], s[ 2 ] );
	
	glVertexPointer( 3, GL_FLOAT, 0, planeCenter );
	glTexCoordPointer( 2, GL_FLOAT, 0, planeTexture );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glColor4f( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ] );
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	
	glPopMatrix();
}

void testApp :: drawLines ()
{
	float lat		= locations[ locationIndex * 2 + 0 ];
	float lon		= locations[ locationIndex * 2 + 1 ];
	float radius	= 2;

	ofxVec3f p = sphericalToCartesian( lat, lon, radius );
	
	GLfloat lines[] = {
		0.0f, 0.0f, 0.0f,
		p.x, p.y, p.z
	};
	
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f
	};
	
	glVertexPointer( 3, GL_FLOAT, 0, lines );
	glColorPointer( 4, GL_FLOAT, 0, colors );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glDrawArrays( GL_LINES, 0, 2 );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}

void testApp :: drawTriangle ()
{
	GLfloat triangle[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	glPushMatrix();
	glTranslatef( -0.5f, -0.5f, -1.0f );
	
	glTranslatef( 0, 0, -1 );
	
	glVertexPointer(3, GL_FLOAT, 0, triangle);
	glColorPointer(4, GL_FLOAT, 0, colors);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	
	glPopMatrix();
}

void testApp :: drawCube ()
{
	GLfloat box[] = 
	{
		// FRONT
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		// BACK
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		// LEFT
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		// RIGHT
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		// TOP
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		// BOTTOM
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
	};
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	
	glVertexPointer(3, GL_FLOAT, 0, box);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// FRONT AND BACK
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	
	// LEFT AND RIGHT
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	
	// TOP AND BOTTOM
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	
	glPopMatrix();
}

///////////////////////////////////////////////////////////
// EXIT.
///////////////////////////////////////////////////////////

void testApp::exit()
{
	printf("exit()\n");
}

///////////////////////////////////////////////////////////
// HANDLERS.
///////////////////////////////////////////////////////////

void testApp::mouseMoved(int x, int y )
{
	printf("mouseMoved: %i, %i\n", x, y);	// this will never get called 
}

// mouse functions are there for backwards compatibility
// they are simply the first finger to touch the screen
// you can omit the mouse functions and just use touch functions
// or omit touch functions and just use mouse functions if you don't need multitouch
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	printf("mouseDragged: %i, %i %i\n", x, y, button);
}

void testApp::mousePressed(int x, int y, int button)
{
	printf("mousePressed: %i, %i %i\n", x, y, button);
}

void testApp::mouseReleased()
{
	printf("frameRate: %.3f, frameNum: %i\n", ofGetFrameRate(), ofGetFrameNum());
}

void testApp::mouseReleased(int x, int y, int button)
{
	printf("mouseReleased: %i, %i %i\n", x, y, button);
}

//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchDown: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
//		mouseTrackballDown( x, y );
	}
	
	if( touchId == 0 )
	{
		mouseSphereDown( x, y );
	}
}

void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchMoved: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
//		mouseTrackballMove( x, y );
	}
	
	if( touchId == 0 )
	{
		mouseSphereMove( x, y );
	}
}


void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchUp: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
//		mouseTrackballUp( x, y );
	}
	
	if( touchId == 0 )
	{
		mouseSphereUp( x, y );
	}
}

void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchDoubleTap: %i, %i %i\n", x, y, touchId);
	ofToggleFullscreen();
}

