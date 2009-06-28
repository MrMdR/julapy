#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(255,255,255);
	ofSetVerticalSync( true );
//	ofEnableSmoothing();

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
		
		rvt[ i * 6 + 0 ] = rps[ i * 3 + 0 ];
		rvt[ i * 6 + 1 ] = rps[ i * 3 + 1 ] + h * 0.5;
		rvt[ i * 6 + 2 ] = rps[ i * 3 + 2 ];
		rvt[ i * 6 + 3 ] = rps[ i * 3 + 0 ];
		rvt[ i * 6 + 4 ] = rps[ i * 3 + 1 ] - h * 0.5;
		rvt[ i * 6 + 5 ] = rps[ i * 3 + 2 ];

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
	
	fontSize = 120;
	letter = 'Q';

	font.loadFont( "ChartITCbyBTBla.ttf", fontSize, true, true, true );
	
	clearLetterVertices();
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
	
	drawRibbonType();
	drawLetter( letter );
	drawLetterOF();
}

void testApp :: drawRibbonType ()
{
	int strLength = 13;
	
	char hello[ strLength ];
	hello[ 0 ] = 'T';
	hello[ 1 ] = 'y';
	hello[ 2 ] = 'p';
	hello[ 3 ] = 'e';
	hello[ 4 ] = ' ';
	hello[ 5 ] = 'r';
	hello[ 6 ] = 'i';
	hello[ 7 ] = 'b';
	hello[ 8 ] = 'b';
	hello[ 9 ] = 'o';
	hello[ 10 ] = 'n';
	hello[ 11 ] = 's';
	hello[ 12 ] = '!';
	
	int x = 0;
	int pad = 10;
	
	for( int i=0; i<strLength; i++ )
	{
		string str = "";
		str += hello[ i ];
		
		drawLetterOnRibbon( hello[ i ], x, fontSize * 0.5 );

		if( hello[ i ] == ' ' )
		{
			x += 50;
		}
		else
		{
			ofRectangle rect = font.getStringBoundingBox( str, 0, 0 );
			x += rect.width + pad;
		}
	}
}

void testApp :: drawLetterOnRibbon ( int letter, float xOffset, float yOffset )
{
	ofTTFCharacter ttfChar;
	ttfChar = font.getCharacterAsPoints( letter );

	for( int k=0; k<ttfChar.contours.size(); k++ )
	{
		if( k != 0 )
		{
			drawLetterVertices();
		}
		
		for( int i=0; i<ttfChar.contours[ k ].pts.size(); i++ )
		{
			float px = ttfChar.contours[ k ].pts[ i ].x + xOffset;
			float py = ttfChar.contours[ k ].pts[ i ].y + yOffset;
			
			float rw = 0;	// ribbon width.
			float lx = 0;	// lower x bounds.
			float ux = 0;	// upper x bounds.

			ofxVec3f poi;	// point of intersect with the center line of the ribbon.
			
			int j;
			for( j=0; j<( RIBBON_MAX_LENGTH - 1 ) * 3; j+=3 )
			{
				ofxVec3f p1 = ofxVec3f( rps[ j + 0 ], rps[ j + 1 ], rps[ j + 2 ] );
				ofxVec3f p2 = ofxVec3f( rps[ j + 3 ], rps[ j + 4 ], rps[ j + 5 ] );
				ofxVec3f p21 = p2 - p1;
				
				lx = rw;
				rw += p21.length();
				ux = rw;
				
				if( px >= lx && px < ux )
				{
					float p = ( px - lx ) / ( ux - lx );
					
					poi = p1 + p21 * p;
				}
			}
			
			poi += ofxVec3f( 0, py, 0 );
			
//			ofxVec3f v0 = ofxVec3f( rps[ j + 0 ], rps[ j + 1 ], rps[ j + 2 ] );
//			ofxVec3f v1 = ofxVec3f( rvt[ j * 6 + 0 ], rvt[ j * 6 + 1 ], rvt[ j * 6 + 2 ] );
//			ofxVec3f v2 = ofxVec3f( rvt[ j * 6 + 3 ], rvt[ j * 6 + 4 ], rvt[ j * 6 + 5 ] );
//			ofxVec3f v10 = v1 - v0;
//			ofxVec3f v20 = v2 - v0;
//			v10.normalize();
//			v20.normalize();
			
			float *point = new float[ 3 ];
			point[ 0 ] = poi.x;
			point[ 1 ] = poi.y;
			point[ 2 ] = poi.z;
			
			polyVertices.push_back( point );
		}
	}
	drawLetterVertices();
	clearLetterVertices();
}

void testApp :: drawLetter ( int letter )
{
	ofTTFCharacter ttfChar;
	ttfChar = font.getCharacterAsPoints( letter );
	
	ofPushMatrix();
	ofTranslate( 400, 180, 0 );
	
	for( int k=0; k<ttfChar.contours.size(); k++ )
	{
		if( k != 0 ) 
		{
			drawLetterVertices();
		}
		
		for( int i=0; i<ttfChar.contours[ k ].pts.size(); i++ )
		{
			float *point = new float[ 3 ];
			point[ 0 ] = ttfChar.contours[ k ].pts[ i ].x;
			point[ 1 ] = ttfChar.contours[ k ].pts[ i ].y;
			point[ 2 ] = 0;
			polyVertices.push_back( point );
		}
	}
	drawLetterVertices();
	clearLetterVertices();
	
	ofPopMatrix();	
}

void testApp :: clearLetterVertices ()
{
	currentStartVertex = 0;
	for( vector<float*>::iterator itr=polyVertices.begin(); itr!=polyVertices.end(); ++itr )
	{
		delete [] (*itr);
	}
	polyVertices.clear();
}

void testApp :: drawLetterVertices ()
{
	if( (int)polyVertices.size() > currentStartVertex )
	{
		float *point = new float[ 3 ];
		point[ 0 ] = polyVertices[ currentStartVertex ][ 0 ];
		point[ 1 ] = polyVertices[ currentStartVertex ][ 1 ];
		point[ 2 ] = polyVertices[ currentStartVertex ][ 2 ];
		polyVertices.push_back( point );
	}
	
	int numToDraw = polyVertices.size() - currentStartVertex;
	if( numToDraw > 0)
	{
		GLfloat *points = new GLfloat[ polyVertices.size() * 3 ];
		
		int k = 0;
		
		for( int i=currentStartVertex; i<(int)polyVertices.size(); i++ )
		{
			points[ k + 0 ]	= polyVertices[ i ][ 0 ];
			points[ k + 1 ] = polyVertices[ i ][ 1 ];
			points[ k + 2 ] = polyVertices[ i ][ 2 ];
			
			k += 3;
		}
		
		glColor4f( 0, 0, 0, 1 );
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, &points[0] );
		glDrawArrays( GL_LINE_STRIP, 0, numToDraw );
		
		delete [] points;
		
		currentStartVertex = (int)polyVertices.size();
	}
}

void testApp :: drawLetterOF ()
{
	string str = "";
	str += char(letter);
	
	font.drawString( str, 550, 180 );
	
	ofTTFCharacter testChar;
	testChar = font.getCharacterAsPoints(letter);
	
	ofNoFill();
	ofPushMatrix();
		ofTranslate( 700, 180, 0);
		ofBeginShape();
			for(int k = 0; k <testChar.contours.size(); k++)
			{
				if( k!= 0)ofNextContour(true);
				for(int i = 0; i < testChar.contours[k].pts.size(); i++)
				{
					ofVertex(testChar.contours[k].pts[i].x, testChar.contours[k].pts[i].y);
				}
			}
		ofEndShape( true );
	ofPopMatrix();
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
void testApp::keyPressed  (int key){
	letter = key;
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
