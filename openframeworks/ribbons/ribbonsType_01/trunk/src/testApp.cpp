#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( 100 );
	ofSetVerticalSync( true );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofBackground( 150, 150, 150 );
//	ofBackground( 0, 0, 0 );

	pTotal		= MAX_PARTICLES;
	trailIndex	= 0;
	trailCount	= 0;
	
	stageWidth		= ofGetWidth();
	stageHeight		= ofGetHeight();
	stageCenterX	= stageWidth * 0.5;
	stageCenterY	= stageHeight * 0.5;
	
	posSpread = 600;
	velSpread = 2;
	
	for(int i=0; i<pTotal; i++)
	{
		pos[ i ][ 0 ] = ofRandom( -1, 1 ) * posSpread;
		pos[ i ][ 1 ] = ofRandom( -1, 1 ) * posSpread;
		pos[ i ][ 2 ] = ofRandom( -1, 1 ) * posSpread;
		
		vel[ i ][ 0 ] = ofRandom( -1, 1 ) * velSpread; 
		vel[ i ][ 1 ] = ofRandom( -1, 1 ) * velSpread; 
		vel[ i ][ 2 ] = ofRandom( -1, 1 ) * velSpread; 
		
		col[ i ][ 0 ] = 1;
		col[ i ][ 1 ] = 1;
		col[ i ][ 2 ] = 1;
		col[ i ][ 3 ] = 1;
	}
	
	sphericalField.setCenter( 0, 0, 0 );
	sphericalField.setRadius( posSpread );
	
#ifdef USE_VBO
	
	initVBO();
	
#endif
	
//	colourMapImage.loadImage( "bw_map.jpg" );
	colourMapImage.loadImage( "bw_red_map.jpg" );
//	colourMapImage.loadImage( "3426927909_2c8066cec6.jpg" );
//	colourMapImage.loadImage( "3276739319_f3b90fef70.jpg" );
//	colourMapImage.loadImage( "3277282859_41d0df132b.jpg" );

	upAxis.set( 0, 1, 0 );
	upAxisRot = 5;
	
	tileSaver.init( 10, 0, true );
	
	useAdditiveBlending = false;
	showRibbonType		= true;
	
	initRibbonType();
}

void testApp :: initVBO ()
{
#ifdef USE_VBO
	
	glGenBuffersARB( MAX_PARTICLES * 2, &vbo[ 0 ] );
	
	for( int i=0; i<MAX_PARTICLES; i++ )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ i * 2 + 0 ] );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAX_TRAIL_LENGTH * 3 * 2 * sizeof( float ), tvr[ i ], GL_STREAM_DRAW_ARB );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ i * 2 + 1 ] );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAX_TRAIL_LENGTH * 4 * 2 * sizeof( float ), tcl[ i ], GL_STREAM_DRAW_ARB );
	}
	
#endif
}

void testApp :: initRibbonType ()
{
	ribbonType.loadTrueTypeFont( "ChartITCbyBTBla.ttf", 20 );
	ribbonType.wrapRibbonSurface( false );
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( tileSaver.bGoTiling )
		return;
	
	noiseField.update();
	
	for( int i=0; i<pTotal; i++ )
	{
		ofxVec3f vel;
		ofxVec3f sph = sphericalField.getNormalisedForce( pos[ i ][ 0 ], pos[ i ][ 1 ], pos[ i ][ 2 ] );
		ofxVec3f noi = noiseField.getNormalisedForce( pos[ i ][ 0 ], pos[ i ][ 1 ], pos[ i ][ 2 ] );
		
		sph *= 1.2;
		
		vel = sph + noi;
		vel.normalize();
		vel *= 3;
		
		pos[ i ][ 0 ] += vel.x;
		pos[ i ][ 1 ] += vel.y;
		pos[ i ][ 2 ] += vel.z;

		int j = 0;
		if( trailIndex < MAX_TRAIL_LENGTH )
		{
			j = trailIndex;
		}
		else
		{
			j = MAX_TRAIL_LENGTH - 1;
		}
		
		// TRAIL POSITIONS.
		if( trailIndex > 0 )
		{
			memmove( trl[ i ] + 3, trl[ i ], 3 * j * sizeof(float) );
		}

		trl[ i ][ 0 ] = pos[ i ][ 0 ];
		trl[ i ][ 1 ] = pos[ i ][ 1 ];
		trl[ i ][ 2 ] = pos[ i ][ 2 ];

		// TRAIL VERTEX.
		if( trailIndex > 0 )
		{
			memmove( tvd[ i ] + 3, tvd[ i ], 3 * j * sizeof(float) );
			memmove( tvr[ i ] + 6, tvr[ i ], 6 * j * sizeof(float) );
		}
			
		if( trailIndex == 0 )
		{
			tvd[ i ][ 0 ] = 0;
			tvd[ i ][ 1 ] = 0;
			tvd[ i ][ 2 ] = 0;
			
			tvr[ i ][ 0 ] = pos[ i ][ 0 ];
			tvr[ i ][ 1 ] = pos[ i ][ 1 ];
			tvr[ i ][ 2 ] = pos[ i ][ 2 ];
			tvr[ i ][ 3 ] = pos[ i ][ 0 ];
			tvr[ i ][ 4 ] = pos[ i ][ 1 ];
			tvr[ i ][ 5 ] = pos[ i ][ 2 ];
		}
		else
		{
			int m = 0;
			int n = 1;
			
			float t0x = trl[ i ][ m * 3 + 0 ];	// xyz position of 1st trail point.
			float t0y = trl[ i ][ m * 3 + 1 ];
			float t0z = trl[ i ][ m * 3 + 2 ];
			
			float t1x = trl[ i ][ n * 3 + 0 ];	// xyz position of 2nd trail point.
			float t1y = trl[ i ][ n * 3 + 1 ];
			float t1z = trl[ i ][ n * 3 + 2 ];
			
			ofxVec3f t0 = ofxVec3f( t0x, t0y, t0z );	// position vector of 1st trail point.
			ofxVec3f t1 = ofxVec3f( t1x, t1y, t1z );	// position vector of 2nd trail point.
			
			ofxVec3f v1 = t0 - t1;
			v1.normalize();
			ofxVec3f ya = ofxVec3f( upAxis );
			ofxVec3f v2 = ya.cross( v1 );
			ofxVec3f v3 = v1.cross( v2 ).normalize();
			
			tvd[ i ][ 0 ] = v3.x;
			tvd[ i ][ 1 ] = v3.y;
			tvd[ i ][ 2 ] = v3.z;
			
			float w		= 0.2;
			float xOff	= v3.x * w;
			float yOff	= v3.y * w;
			float zOff	= v3.z * w;
			
			tvr[ i ][ 0 ] = t0x - xOff;
			tvr[ i ][ 1 ] = t0y - yOff;
			tvr[ i ][ 2 ] = t0z - zOff;
			tvr[ i ][ 3 ] = t0x + xOff;
			tvr[ i ][ 4 ] = t0y + yOff;
			tvr[ i ][ 5 ] = t0z + zOff;
		}
		
		// TRAIL COLOUR.
		int r, g, b;
//		mapColour( pos[ i ][ 0 ], pos[ i ][ 1 ], &r, &g, &b );
		mapColour( pos[ i ][ 0 ], pos[ i ][ 2 ], &r, &g, &b );

		if( trailIndex > 0 )
		{
			memmove( tcl[ i ] + 4 * 2, tcl[ i ], 4 * 2 * j * sizeof(float) );
		}

		tcl[ i ][ 0 ] = tcl[ i ][ 4 ] = r / 255.0f;
		tcl[ i ][ 1 ] = tcl[ i ][ 5 ] = g / 255.0f;
		tcl[ i ][ 2 ] = tcl[ i ][ 6 ] = b / 255.0f;
		tcl[ i ][ 3 ] = tcl[ i ][ 7 ] = 0.7f;
	}
	
	if( trailIndex < MAX_TRAIL_LENGTH )
	{
		trailIndex += 1;
		trailCount = trailIndex;
	}
	
	upAxis.rotate( upAxisRot, ofxVec3f( 1, 0, 0 ) );
	
	rotateY += 0.1;
}

void testApp :: mapColour ( float x, float y, int *r, int *g, int *b )
{
	float cx, cy;
	
	cx = ( x + posSpread * 0.5 ) / posSpread;
	cy = ( y + posSpread * 0.5 ) / posSpread;
	
	cx = MAX( MIN( cx, 1 ), 0 );
	cy = MAX( MIN( cy, 1 ), 0 );
	
	unsigned char *pixels = colourMapImage.getPixels();
	
	int w = (int)( ( colourMapImage.width - 1 ) * cx ) * 3;
	int h = (int)( ( colourMapImage.height - 1 ) * cy );
	int i = h * colourMapImage.width * 3 + w;
	
	*r = pixels[ i + 0 ];
	*g = pixels[ i + 1 ];
	*b = pixels[ i + 2 ];
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	ofSetColor( 0xFFFFFF );

	tileSaver.begin();
	
	glPushMatrix();
	glTranslatef( stageCenterX, stageCenterY, 0 );
	glRotatef( rotateY, 0, 1, 0 );

	ofEnableAlphaBlending();
	if( showRibbonType )
	{
		drawRibbonType();
	}
	
#ifdef USE_VBO
	
	drawRibbonFillVBO();
	
#else
	
//	drawPoint();
//	drawRibbonFill();
	
#endif
	
//	drawRibbonMesh();
	
	glPopMatrix();
	
	tileSaver.end();

	ofSetColor( 0x000000 );
	ofDrawBitmapString
	(
		"fps :: " + ofToString(ofGetFrameRate(), 2) + "\n\n"
		"total particles :: " + ofToString( pTotal ),
		20,
		20
	);
}

void testApp :: drawPoint()
{
	for( int i=0; i<pTotal; i++ )
	{
		glPushMatrix();
		glTranslatef( pos[ i ][ 0 ], pos[ i ][ 1 ], pos[ i ][ 2 ] );
		ofCircle( 0, 0, 4 );
		glPopMatrix();
	}
}

void testApp :: drawRibbonFill()
{
	for( int i=0; i<pTotal; i++ )
	{
		glBegin( GL_QUAD_STRIP );
		
		for( int j=0; j<trailCount; j++ )
		{
			float r = tcl[ i ][ j * 4 * 2 + 0 ];
			float g = tcl[ i ][ j * 4 * 2 + 1 ];
			float b = tcl[ i ][ j * 4 * 2 + 2 ];
			float a = tcl[ i ][ j * 4 * 2 + 3 ];
				
			glColor4f( r, g, b, a );
			
			float v0x = tvr[ i ][ j * 3 * 2 + 0 ];
			float v0y = tvr[ i ][ j * 3 * 2 + 1 ];
			float v0z = tvr[ i ][ j * 3 * 2 + 2 ];
			float v1x = tvr[ i ][ j * 3 * 2 + 3 ];
			float v1y = tvr[ i ][ j * 3 * 2 + 4 ];
			float v1z = tvr[ i ][ j * 3 * 2 + 5 ];
			
			glVertex3f( v0x, v0y, v0z );
			glVertex3f( v1x, v1y, v1z );
		}
		
		glEnd();
	}
}

void testApp :: drawRibbonMesh()
{
	bool drawDiagonal = true;
	
	for( int i=0; i<pTotal; i++ )
	{
		glBegin( GL_LINES );
		
		for( int j=0; j<trailCount; j++ )
		{
			float r = tcl[ i ][ j * 4 * 2 + 0 ];
			float g = tcl[ i ][ j * 4 * 2 + 1 ];
			float b = tcl[ i ][ j * 4 * 2 + 2 ];
			float a = tcl[ i ][ j * 4 * 2 + 3 ];
			
			glColor4f( r, g, b, a );
			
			float v0x = tvr[ i ][ j * 6 + 0 ];
			float v0y = tvr[ i ][ j * 6 + 1 ];
			float v0z = tvr[ i ][ j * 6 + 2 ];
			float v1x = tvr[ i ][ j * 6 + 3 ];
			float v1y = tvr[ i ][ j * 6 + 4 ];
			float v1z = tvr[ i ][ j * 6 + 5 ];
			
			if( j < trailCount - 1 )
			{
				float v2x = tvr[ i ][ j * 6 + 6 ];
				float v2y = tvr[ i ][ j * 6 + 7 ];
				float v2z = tvr[ i ][ j * 6 + 8 ];
				float v3x = tvr[ i ][ j * 6 + 9 ];
				float v3y = tvr[ i ][ j * 6 + 10 ];
				float v3z = tvr[ i ][ j * 6 + 11 ];
				
				glVertex3f( v2x, v2y, v2z );
				glVertex3f( v0x, v0y, v0z );
				
				glVertex3f( v0x, v0y, v0z );
				glVertex3f( v1x, v1y, v1z );
				
				glVertex3f( v1x, v1y, v1z );
				glVertex3f( v3x, v3y, v3z );
				
				if( drawDiagonal )
				{
					glVertex3f( v3x, v3y, v3z );
					glVertex3f( v0x, v0y, v0z );
				}
			}
			else
			{
				glVertex3f( v0x, v0y, v0z );
				glVertex3f( v1x, v1y, v1z );
			}
		}
		
		glEnd();
	}
}

void testApp :: drawRibbonFillVBO()
{
#ifdef USE_VBO
	
	for( int i=0; i<MAX_PARTICLES; i++ )
	{
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ i * 2 + 0 ] );
		glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, 0, MAX_TRAIL_LENGTH * 3 * 2 * sizeof( float ), tvr[ i ] );
		glVertexPointer( 3, GL_FLOAT, 0, 0 );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ i * 2 + 1 ] );
		glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, 0, MAX_TRAIL_LENGTH * 4 * 2 * sizeof( float ), tcl[ i ] );
		glColorPointer( 4, GL_FLOAT, 0, 0 );
		
		glDrawArrays( GL_QUAD_STRIP, 0, MAX_TRAIL_LENGTH * 2 );
		
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	}
	
#endif
}

void testApp :: drawRibbonType()
{
	ofSetColor( 0, 0, 0, 80 );
	
	for( int i=0; i<pTotal; i++ )
	{
		if( trailCount > 0 )
		{
//			ofFill();
//			ribbonType.setRibbinColorArray( tcl[ i ] );
//			ribbonType.drawTypeOnRibbon( "hey bee do your thing with that sting", trl[ i ], tvd[ i ], trailCount );
			
			ofNoFill();
			ribbonType.setRibbinColorArray( tcl[ i ] );
			ribbonType.drawTypeOnRibbon( "hey bee do your thing with that sting", trl[ i ], tvd[ i ], trailCount );
		}
	}
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{

}

void testApp :: keyReleased( int key )
{
	if( key == 's' )
	{
		char str[255];
		sprintf( str, "ribbons_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		tileSaver.finish( str, false );
	}
	
	if( key == 'a' )
	{
		useAdditiveBlending = !useAdditiveBlending;
		
		if( useAdditiveBlending )
		{
			glDisable( GL_DEPTH_TEST );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		else
		{
			glEnable( GL_DEPTH_TEST );
			glBlendFunc( GL_ONE, GL_ZERO );
		}
	}
	
	if( key == 'r' )
	{
		showRibbonType = !showRibbonType;
	}
}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{

}

void testApp :: mousePressed( int x, int y, int button )
{

}

void testApp :: mouseReleased( int x, int y, int button )
{

}

void testApp :: windowResized( int w, int h )
{
	//
}

