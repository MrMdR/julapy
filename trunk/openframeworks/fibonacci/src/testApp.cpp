#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	sequenceLength	= 20;
	sequence		= new float[ sequenceLength ];
}

//--------------------------------------------------------------
void testApp::update()
{
//	updateFibonacci();
	updatePadovan();
}

void testApp :: updateFibonacci ()
{
	sequenceTotal = 0;
	
	float j;
	j = ( mouseX / (float)ofGetWidth() - 0.5 ) * 2;
	j = MIN( j,  1 );
	j = MAX( j, -1 );
	
	for( int i=0; i<sequenceLength; i++ )
	{
		if( i == 0 )
		{
			sequence[ i ]	 = 0;
			sequenceTotal	+= sequence[ i ];
			
			continue;
		}
		
		if( i == 1 )
		{
			sequence[ i ]	 = 1;
			sequenceTotal	+= sequence[ i ];
			
			continue;
		}
		
		sequence[ i ]	 = sequence[ i - 1 ] + sequence[ i - 2 ];
		sequenceTotal	+= sequence[ i ];
	}
}

void testApp :: updatePadovan ()
{
	sequenceTotal = 0;
	
	float j;
	j = ( mouseX / (float)ofGetWidth() - 0.5 ) * 2;
	j = MIN( j,  1 );
	j = MAX( j, -1 );
	
	for( int i=0; i<sequenceLength; i++ )
	{
		if( i == 0 )
		{
			sequence[ i ]	 = 1;
			sequenceTotal	+= sequence[ i ];
			
			continue;
		}
		
		if( i == 1 )
		{
			sequence[ i ]	 = 1;
			sequenceTotal	+= sequence[ i ];
			
			continue;
		}

		if( i == 2 )
		{
			sequence[ i ]	 = 1;
			sequenceTotal	+= sequence[ i ];
			
			continue;
		}
		
		sequence[ i ]	 = sequence[ i - 2 ] + sequence[ i - 3 ];
		sequenceTotal	+= sequence[ i ];
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();
	
	float x;
	x = 0;
	
	int c;
	c = 200;
	
	int cd;
	cd = 200 / sequenceLength;
	
	for( int i=sequenceLength-1; i>=0; i-- )
	{
		float v;
		v = sequence[ i ] / sequenceTotal;
		
		float w;
		w = v * ofGetWidth();
	
		ofSetColor( c, c, c );
		ofRect( x, 0, w, ofGetHeight() );
		
		x += w;
		c -= cd;
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == 's' )
	{
		ofImage img;
		img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
		img.saveImage( "image.png" );
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

