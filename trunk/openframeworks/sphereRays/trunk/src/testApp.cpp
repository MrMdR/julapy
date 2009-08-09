#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp::setup()
{
	ofBackground( 0, 0, 0 );
	ofSetBackgroundAuto( true );
	ofSetFrameRate( 60 );
	ofDisableArbTex();
	ofEnableAlphaBlending();
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	initAxis();
	initEarth();
	initLocations();
	initRibbons();
	initRays();
	initDebug();
	
	mouseSphereInit();
//	initLighting();
}

void testApp :: initAxis()
{
	axisSize = 800;
}

void testApp :: initEarth()
{
	sphereSize = 500;
	
	quadratic = gluNewQuadric();					// Create A Pointer To The Quadric Object
	gluQuadricTexture( quadratic, GL_TRUE );		// Create Texture Coords
	gluQuadricNormals( quadratic, GLU_SMOOTH );		// Create Smooth Normals
	
	ofxVec3f right;
	ofxVec3f up;
	ofxVec3f out;
	
	right = ofxVec3f(  1,  0,  0 );
	up    = ofxVec3f(  0,  0, -1 );
	out   = ofxVec3f(  0,  1,  0 );
	
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
	
	earthMap.loadImage( "maps/EarthMap_night.jpg" );
//	earthMap.loadImage( "maps/EarthMap_gebco_bathy_2048x1024.jpg" );
}

void testApp :: initLocations()
{
	locationIndex	= 0;
	locationsTotal	= 0;
	locationsMax	= 200;
	locations		= new float[ locationsMax * 2 ];		// latitude and longitude pairs.
	
//	addLocation( -33.87784462833714, 151.2179660797119 );	// Holler Sydney
//	addLocation( 35.689488, 139.691706 );					// Tokyo
//	addLocation( 9.009697, -79.603243 );					// Panama City
//	addLocation( 40.756054, -73.986951 );					// New York
//	addLocation( 48.856667, 2.350987 );						// Paris
//	addLocation( 25.271139, 55.307485 );					// Dubai
//	addLocation( 51.500152, -0.126236 );					// London
	
	for( int i=0; i<locationsMax; i++ )
	{
		float rndLat = ofRandom( -90, 90 );
		float rndLon = ofRandom( -180, 180 );
		
		addLocation( rndLat, rndLon );
	}
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
	raysTotal = 4000;
	rays = new Ray[ raysTotal ];
	
	for( int i=0; i<raysTotal; i++ )
	{
		float randLat = ofRandom( -90, 90 );
		float randLon = ofRandom( -180, 180 );
		
		rays[ i ].p.set( sphericalToCartesian( randLat, randLon, 1 ) );
		rays[ i ].p.normalize();
	}
	
	rayTarget		= new float[ 2 ];
	rayTarget[ 0 ]	= 0;
	rayTarget[ 1 ]	= 0;
	
	rayImage.loadImage( "rays/white01.png" );
	rayBaseImage.loadImage( "rays/white_base01.png" );
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

void testApp :: initDebug()
{
	dbInc = 1;
	dbLat = 0;
	dbLon = 0;
}

///////////////////////////////////////////////////////////
// MOUSE SPHERE.
///////////////////////////////////////////////////////////

void testApp :: mouseSphereInit()
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
	glPushMatrix();
	glTranslatef( ofGetWidth() * 0.5, ofGetHeight() * 0.5, -ofGetHeight() );

	glRotatef( gTrackBallRotation[ 0 ], gTrackBallRotation[ 1 ], gTrackBallRotation[ 2 ], gTrackBallRotation[ 3 ] );
	glRotatef( gWorldRotation[ 0 ], gWorldRotation[ 1 ], gWorldRotation[ 2 ], gWorldRotation[ 3 ] );

//	drawAxis();
	drawSphere();
	drawRays();
//	drawRibbons();
//	drawLatLonPlane();
	
	glPopMatrix();
	
	drawDebug();
}

void testApp :: drawAxis()
{
	glPushMatrix();
	glScalef( axisSize, axisSize, axisSize );
	
	glBegin( GL_LINES );
	
	glColor3f(  0.7, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 1.0, 0.0, 0.0 );
	
	glColor3f(  0.0,  0.7, 0.0 );
	glVertex3f( 0.0,  0.0, 0.0 );
	glVertex3f( 0.0, -1.0, 0.0 );
	
	glColor3f(  0.0, 0.0, 0.7 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, 1.0 );
	
	glEnd();
	
	glPopMatrix();
}

void testApp :: drawSphere ()
{
	glPushMatrix();
	glScalef( sphereSize, sphereSize, sphereSize );
	glMultMatrixf( &sphereOrientation[ 0 ] );
	glColor4f( 1, 1, 1, 1 );
	
	earthMap.getTextureReference().bind();
	gluSphere( quadratic, 1, 50, 50 );
	earthMap.getTextureReference().unbind();
	
	glPopMatrix();
}

void testApp :: drawRibbons()
{
	glPushMatrix();
	glScalef( 300, 300, 300 );
	
	for( int i=0; i<ribbonsTotal; i++ )
	{
		ribbons[ i ].ribbon->drawLine();
//		ribbons[ i ].ribbon->drawFill();
	}
	
	glPopMatrix();
}

void testApp :: drawRays()
{
	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	
	glPushMatrix();
	glScalef( sphereSize, sphereSize, sphereSize );
	
	GLfloat plane[] = 
	{
		-0.5f, 0.0f, 0.0f,
		 0.5f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		 0.5f, 1.0f, 0.0f,
	};
	
	GLfloat planeCenter[] = 
	{
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f
	};
	
	GLfloat planeTex[] = 
	{
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	
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
	
	rayImage.getTextureReference().bind();
	for( int i=0; i<raysTotal; i++ )
	{
		ofxVec3f uv;	// up vector.
		ofxVec3f rv;	// right vector.
		ofxVec3f ov;	// out vector.
		
		glPushMatrix();
		glTranslatef( rays[ i ].p.x, rays[ i ].p.y, rays[ i ].p.z );
		
		uv.set( rays[ i ].p );
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
		
		glPushMatrix();	// draw ray.
		
		glMultMatrixf( mat );
		delete[] mat;
		glRotatef( 90, 0, 1, 0 );
		
		glScalef( 0.03, 0.5, 0.05 );
		
		glVertexPointer( 3, GL_FLOAT, 0, plane );
		glTexCoordPointer( 2, GL_FLOAT, 0, planeTex );
		
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		
		glColor4f( 0.94f, 0.40f, 0.18f, 0.7f );		// blood orange.
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		
		glPopMatrix();
		
		glPopMatrix();
	}
	rayImage.getTextureReference().unbind();
	
	rayBaseImage.getTextureReference().bind();
	for( int i=0; i<raysTotal; i++ )
	{
		glPushMatrix();
		glTranslatef( rays[ i ].p.x, rays[ i ].p.y, rays[ i ].p.z );
		
		float *rbm = new float[ 16 ];		// ray base matrix.
		rotateToSphereSurface( rbm, rays[ i ].p.x, rays[ i ].p.y, rays[ i ].p.z );
		glMultMatrixf( rbm );
		delete[] rbm;
		
		glScalef( 0.05, 0.05, 0.05 );
		
		glVertexPointer( 3, GL_FLOAT, 0, planeCenter );
		glTexCoordPointer( 2, GL_FLOAT, 0, planeTex );
		
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		
		glColor4f( 0.94f, 0.40f, 0.18f, 0.7f );		// blood orange.
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		
		glPopMatrix();
	}
	rayBaseImage.getTextureReference().unbind();
	
	glPopMatrix();
	
	glDepthMask( GL_TRUE );
	glBlendFunc( GL_ONE, GL_ZERO );
}

void testApp :: drawLatLonPlane ()
{
	GLfloat planeCenter[] = 
	{
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f
	};
	
	float *rtm = new float[ 16 ];
	
	ofxVec3f rtp = sphericalToCartesian( dbLat, dbLon, sphereSize );
	rtp *= 1.3;
	rotateToSphereSurface( rtm, dbLat, dbLon );
	
	glPushMatrix();
		
		glTranslatef( rtp.x, rtp.y, rtp.z );
	
		glMultMatrixf( rtm );
		delete[] rtm;
	
		glScalef( 100, 100, 0 );
	
		glVertexPointer( 3, GL_FLOAT, 0, planeCenter );
		glEnableClientState( GL_VERTEX_ARRAY );

		glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	
	glPopMatrix();
}

void testApp :: drawDebug()
{
	ofSetColor( 0xFF0000 );
	ofDrawBitmapString
	(
		"fps   :: " + ofToString(ofGetFrameRate(), 2) + "\n\n" +
		"dbInc :: press +/- to change :: " + ofToString( dbInc, 2 ) + "\n\n" +
		"dbLat :: press 1 to change   :: " + ofToString( dbLat, 2 ) + "\n\n" +
		"dbLon :: press 2 to change   :: " + ofToString( dbLon, 2 ) + "\n\n",
		20,
		20
	);
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	float inc = 3;
	
	if( key == '-' )
		dbInc = -1;
	
	if( key == '=' )
		dbInc = 1;
		
	if( key == '1' )
		dbLat += dbInc * inc;
	
	if( key == '2' )
		dbLon += dbInc * inc;
	
	if( key == ' ' )
	{
		dbLat = 0;
		dbLon = 0;
	}
}

void testApp :: keyReleased( int key )
{

}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{
	mouseSphereMove( x, y );
}

void testApp :: mousePressed( int x, int y, int button )
{
	mouseSphereDown( x, y );
}

void testApp :: mouseReleased( int x, int y, int button )
{
	mouseSphereUp( x, y );
}

void testApp :: windowResized( int w, int h )
{

}

