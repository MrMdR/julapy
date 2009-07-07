
#include "testApp.h"

///////////////////////////////////////////////////////////
// SETUP.
///////////////////////////////////////////////////////////

void testApp::setup()
{
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(true);
	ofSetFrameRate(60);
	
	ofxAccelerometer.setup();

	ofxMultiTouch.addListener(this);
	
	createSphere(16,16);

//    glGenBuffers( 1, &vertexBuffer );
//    glGenBuffers( 1, &indexBuffer );
//	
//    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
//    glBufferData( GL_ARRAY_BUFFER, sizeof( Vertices ), Vertices, GL_STATIC_DRAW );
//    
//	glEnableClientState( GL_VERTEX_ARRAY );
//	glVertexPointer( 3, GL_FLOAT, sizeof( SVertex ), (void*)offsetof( SVertex, position ) );
//
//	glEnableClientState( GL_COLOR_ARRAY );
//	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( SVertex ), (void*)offsetof( SVertex, color ) );	

	
//	glEnableClientState( GL_VERTEX_ARRAY );
//	glEnableClientState( GL_COLOR_ARRAY );
//	glVertexPointer( 3, GL_FLOAT, sizeof(SVertex), Vertices );
//	glColorPointer(	3, GL_FLOAT, sizeof(SVertex), &Vertices[0].r);
//	glPointSize( 2.0 );
//	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	
	ChangeY = 0.025;	//Indicates how much the y values of the highest and deepest vertex
	PointsOnly = false;	//Indicated, if the triangles or only points are drawn
	
	initTrackball();
}

void testApp :: createSphere(int PointRows, int PointsPerRow)
{
	NumVertices = (PointRows-2)*PointsPerRow + 2;
	Vertices = new SVertex[NumVertices];
	IndexVect.clear();  //to be sure it is empty
	float x,y,z;
	int i,j;
	double r;
	for (i = 1; i < (PointRows-1); i++)
	{
		for (j = 0; j < PointsPerRow; j++)
		{
			y = 1.0 - float(i) / float(PointRows-1)*2.0;
			r = sin (acos(y));  //radius of the row
			x = r * sin(float(j) / float(PointsPerRow)*PI*2.0);
			
			z = r * cos(float(j) / float(PointsPerRow)*PI*2.0);
			Vertices[(i-1)*PointsPerRow+j].position[ 0 ] = x;
			Vertices[(i-1)*PointsPerRow+j].position[ 1 ] = y;
			Vertices[(i-1)*PointsPerRow+j].position[ 2 ] = z;
			Vertices[(i-1)*PointsPerRow+j].color[ 0 ] = (float)(i) / float(PointRows);
			Vertices[(i-1)*PointsPerRow+j].color[ 1 ] = 0.7;
			Vertices[(i-1)*PointsPerRow+j].color[ 2 ] = (float)(j) / float(PointsPerRow);
			Vertices[(i-1)*PointsPerRow+j].color[ 3 ] = 1;
		}
		
	}
	//The highest and deepest vertices:
	Vertices[(PointRows-2)*PointsPerRow].position[ 0 ] = 0.0;
	Vertices[(PointRows-2)*PointsPerRow].position[ 1 ] = 1.0;
	Vertices[(PointRows-2)*PointsPerRow].position[ 2 ] = 0.0;
	Vertices[(PointRows-2)*PointsPerRow].color[ 0 ] = 1.0;
	Vertices[(PointRows-2)*PointsPerRow].color[ 1 ] = 0.7;
	Vertices[(PointRows-2)*PointsPerRow].color[ 2 ] = 1.0;
	Vertices[(PointRows-2)*PointsPerRow].color[ 3 ] = 1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].position[ 0 ] = 0.0;
	Vertices[(PointRows-2)*PointsPerRow+1].position[ 1 ] = -1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].position[ 2 ] = 0.0;
	Vertices[(PointRows-2)*PointsPerRow+1].color[ 0 ] = 1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].color[ 1 ] = 0.7;
	Vertices[(PointRows-2)*PointsPerRow+1].color[ 2 ] = 1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].color[ 3 ] = 1.0;
	
	for (i = 1; i < (PointRows-2); i++)
	{
		for (j = 0; j < (PointsPerRow-1); j++)
		{
			IndexVect.push_back((i-1)*PointsPerRow+j);
			IndexVect.push_back((i-1)*PointsPerRow+j+1);
			IndexVect.push_back((i)*PointsPerRow+j);
			
			IndexVect.push_back((i-1)*PointsPerRow+j+1);
			IndexVect.push_back((i)*PointsPerRow+j+1);
			IndexVect.push_back((i)*PointsPerRow+j);
		}
		
		IndexVect.push_back((i-1)*PointsPerRow+PointsPerRow-1);
		IndexVect.push_back((i-1)*PointsPerRow);
		IndexVect.push_back((i)*PointsPerRow+j);
		
		IndexVect.push_back((i)*PointsPerRow);
		IndexVect.push_back((i-1)*PointsPerRow);
		IndexVect.push_back((i)*PointsPerRow+j);
	}		
	
	//The triangles to the highest and deepest vertices:
	for (j = 0; j< (PointsPerRow-1); j++)
	{
		IndexVect.push_back(j);
		IndexVect.push_back(j+1);
		IndexVect.push_back((PointRows-2)*PointsPerRow);
	}
	IndexVect.push_back(j);
	IndexVect.push_back(0);
	IndexVect.push_back((PointRows-2)*PointsPerRow);
	
	for (j = 0; j< (PointsPerRow-1); j++)
	{
		IndexVect.push_back((PointRows-3)*PointsPerRow+j);
		IndexVect.push_back((PointRows-3)*PointsPerRow+j+1);
		IndexVect.push_back((PointRows-2)*PointsPerRow+1);
	}
	IndexVect.push_back((PointRows-3)*PointsPerRow+j);
	IndexVect.push_back((PointRows-3)*PointsPerRow);
	IndexVect.push_back((PointRows-2)*PointsPerRow+1);
	Indices = new GLubyte[IndexVect.size()];  //allocate the required memory
	for (i = 0; i < IndexVect.size(); i++)
	{
		Indices[i] = IndexVect[i];
	}
	NumIndices = IndexVect.size();
	IndexVect.clear();  //no longer needed, takes only memory
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
	
	gWorldRotation[ 0 ]	= 155.0f;
	gWorldRotation[ 1 ]	= 0.0f;
	gWorldRotation[ 2 ]	= -1.0f;
	gWorldRotation[ 3 ]	= 0.0f;
	
	gTrackBall = false;
}

void testApp :: mouseTrackballDown( int x, int y )
{
	startTrackball( x, y, 0, 0, ofGetWidth(), ofGetHeight() );
//	glutMotionFunc( mouseTrackball );
	gTrackBall = true;
}

void testApp :: mouseTrackballUp( int x, int y )
{
	gTrackBall = false;
//	glutMotionFunc (NULL);
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
// UPDATE.
///////////////////////////////////////////////////////////

void testApp::update()
{
//	Vertices[NumVertices-2].y += ChangeY;
//	Vertices[NumVertices-1].y -= ChangeY;
//	
//	if (Vertices[NumVertices-2].y> 1.5 || Vertices[NumVertices-2].y< 0.5)
//		ChangeY *= -1;
}

///////////////////////////////////////////////////////////
// DRAW.
///////////////////////////////////////////////////////////

void testApp::draw()
{
	glPushMatrix();
	glTranslatef( ofGetWidth() * 0.5, ofGetHeight() * 0.5, -ofGetHeight() );
	glScalef( 300, 300, 300 );

	glRotatef( gTrackBallRotation[ 0 ], gTrackBallRotation[ 1 ], gTrackBallRotation[ 2 ], gTrackBallRotation[ 3 ] );
	glRotatef( gWorldRotation[ 0 ], gWorldRotation[ 1 ], gWorldRotation[ 2 ], gWorldRotation[ 3 ] );
	
//	drawTriangle();
	drawCube();
//	drawSphere();

	glPopMatrix();
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
	
	glPopMatrix();
}

void testApp :: drawCube ()
{
	GLfloat box[] = {
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
	
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	
	glPopMatrix();
}

void testApp :: drawSphere()
{
	// simple method.
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Indices ), Indices, GL_STATIC_DRAW );
	glDrawElements( GL_TRIANGLES, sizeof(Indices)/sizeof(GLubyte), GL_UNSIGNED_BYTE, (void*)0 );	
	
	// static draw vertex.
	//	glEnableClientState( GL_VERTEX_ARRAY );
	//	glVertexPointer( 3, GL_FLOAT, sizeof( SVertex ), &Vertices[0].position );
	//	glEnableClientState( GL_COLOR_ARRAY );
	//	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( SVertex ), &Vertices[0].color );
	//	glDrawElements( GL_TRIANGLES, sizeof( Indices )/sizeof(GLubyte), GL_UNSIGNED_BYTE, Indices );	
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
		mouseTrackballDown( x, y );
	}
}

void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchMoved: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
		mouseTrackballMove( x, y );
	}
}


void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchUp: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
		mouseTrackballUp( x, y );
	}
}

void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchDoubleTap: %i, %i %i\n", x, y, touchId);
	ofToggleFullscreen();
}

