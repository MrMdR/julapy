#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(255,255,255);
	ofSetVerticalSync( true );
	ofEnableSmoothing();

	int i;
	float x = 100;
	float y = (int)( ofGetHeight() * 0.5 );
	float w = ofGetWidth() - 200;
	float h = 200;
	float d = 140;
	for( int i=0; i<RIBBON_MAX_LENGTH; i++ )
	{
		float p = i / (float)( RIBBON_MAX_LENGTH - 1 );
		float z = d * sin( p * 2 * PI );
		
		rps[ i * 3 + 0 ] = x + w * p;
		rps[ i * 3 + 1 ] = y;
		rps[ i * 3 + 2 ] = z;
		
		float cl = 0.7;
		
		rcl[ i * 4 + 0 ] = cl;
		rcl[ i * 4 + 1 ] = cl;
		rcl[ i * 4 + 2 ] = cl;
		rcl[ i * 4 + 3 ] = cl;
		rcl[ i * 4 + 4 ] = cl;
		rcl[ i * 4 + 5 ] = cl;
		rcl[ i * 4 + 6 ] = cl;
		rcl[ i * 4 + 7 ] = cl;
	}
	
	ofxVec3f upAxis = ofxVec3f( 0, 1, 0 );
	
	for( int i=0; i<RIBBON_MAX_LENGTH; i++ )
	{
		if( i < RIBBON_MAX_LENGTH - 1 )
		{
			float t0x = rps[ i * 3 + 0 ];	// xyz position of 1st trail point.
			float t0y = rps[ i * 3 + 1 ];
			float t0z = rps[ i * 3 + 2 ];
			
			float t1x = rps[ i * 3 + 3 ];	// xyz position of 2nd trail point.
			float t1y = rps[ i * 3 + 4 ];
			float t1z = rps[ i * 3 + 5 ];
			
			ofxVec3f t0 = ofxVec3f( t0x, t0y, t0z );	// position vector of 1st trail point.
			ofxVec3f t1 = ofxVec3f( t1x, t1y, t1z );	// position vector of 2nd trail point.
			
			ofxVec3f v1 = t0 - t1;
			v1.normalize();
			ofxVec3f ya = ofxVec3f( upAxis );
			ofxVec3f v2 = ya.cross( v1 );
			ofxVec3f v3 = v1.cross( v2 ).normalize();

			rvd[ i * 3 + 0 ] = v3.x;
			rvd[ i * 3 + 1 ] = v3.y;
			rvd[ i * 3 + 2 ] = v3.z;
			
			float w		= h * 0.5;
			float xOff	= v3.x * w;
			float yOff	= v3.y * w;
			float zOff	= v3.z * w;
			
			rvt[ i * 6 + 0 ] = t0x - xOff;
			rvt[ i * 6 + 1 ] = t0y - yOff;
			rvt[ i * 6 + 2 ] = t0z - zOff;
			rvt[ i * 6 + 3 ] = t0x + xOff;
			rvt[ i * 6 + 4 ] = t0y + yOff;
			rvt[ i * 6 + 5 ] = t0z + zOff;
		}
		else
		{
			rvd[ i * 3 + 0 ] = 0;
			rvd[ i * 3 + 1 ] = 0;
			rvd[ i * 3 + 2 ] = 0;
			
			rvt[ i * 6 + 0 ] = rps[ i * 3 + 0 ];
			rvt[ i * 6 + 1 ] = rps[ i * 3 + 1 ];
			rvt[ i * 6 + 2 ] = rps[ i * 3 + 2 ];
			rvt[ i * 6 + 3 ] = rps[ i * 3 + 0 ];
			rvt[ i * 6 + 4 ] = rps[ i * 3 + 1 ];
			rvt[ i * 6 + 5 ] = rps[ i * 3 + 2 ];
		}
		
		upAxis.rotate( 1, ofxVec3f( 1, 0, 0 ) );
	}
	
	ribbonType.loadTrueTypeFont( "ChartITCbyBTBla.ttf", 120 );
}

//--------------------------------------------------------------
void testApp::update()
{
	//
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor( 0, 0, 0 );

	ofDrawBitmapString( "fps: " + ofToString( ofGetFrameRate() ), 10, 20 );

//	drawRibbonFill();
	drawRibbonMesh();
	
	ofFill();
	ofSetColor( 0, 0, 0 );
	ribbonType.drawTypeOnRibbon( "hello ribbonsss", rps, rvd, RIBBON_MAX_LENGTH );

	ofNoFill();
	ofSetColor( 0, 0, 0 );
	ribbonType.drawTypeOnRibbon( "hello ribbonsss", rps, rvd, RIBBON_MAX_LENGTH );
}

void testApp :: drawRibbonFill()
{
	glBegin( GL_QUAD_STRIP );
	
	for( int i=0; i<RIBBON_MAX_LENGTH; i++ )
	{
		float r = rcl[ i * 8 + 0 ];
		float g = rcl[ i * 8 + 1 ];
		float b = rcl[ i * 8 + 2 ];
		float a = rcl[ i * 8 + 3 ];
		
		glColor4f( r, g, b, a );
		
		float v0x = rvt[ i * 6 + 0 ];
		float v0y = rvt[ i * 6 + 1 ];
		float v0z = rvt[ i * 6 + 2 ];
		float v1x = rvt[ i * 6 + 3 ];
		float v1y = rvt[ i * 6 + 4 ];
		float v1z = rvt[ i * 6 + 5 ];
		
		glVertex3f( v0x, v0y, v0z );
		glVertex3f( v1x, v1y, v1z );
	}
	
	glEnd();
}

void testApp :: drawRibbonMesh()
{
	bool drawDiagonal = true;

	ofEnableSmoothing();
	
	glBegin( GL_LINES );
	
	for( int i=0; i<RIBBON_MAX_LENGTH; i++ )
	{
		float r = rcl[ i * 4 + 0 ];
		float g = rcl[ i * 4 + 1 ];
		float b = rcl[ i * 4 + 2 ];
		float a = rcl[ i * 4 + 3 ];
		
		glColor4f( r, g, b, a );
		
		float v0x = rvt[ i * 6 + 0 ];
		float v0y = rvt[ i * 6 + 1 ];
		float v0z = rvt[ i * 6 + 2 ];
		float v1x = rvt[ i * 6 + 3 ];
		float v1y = rvt[ i * 6 + 4 ];
		float v1z = rvt[ i * 6 + 5 ];
		
		if( i < RIBBON_MAX_LENGTH - 1 )
		{
			float v2x = rvt[ i * 6 + 6 ];
			float v2y = rvt[ i * 6 + 7 ];
			float v2z = rvt[ i * 6 + 8 ];
			float v3x = rvt[ i * 6 + 9 ];
			float v3y = rvt[ i * 6 + 10 ];
			float v3z = rvt[ i * 6 + 11 ];
			
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

//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
	if( key == 'f')ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
