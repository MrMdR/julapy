#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	srt.parse( "forbidden_planet.srt" );
	
	video.loadMovie( "FORBIDDEN_PLANET.mov" );
	video.play();
	video.setPosition( ofRandom( 0.0, 1.0 ) );
	
	fontCooperBlack.loadFont( "fonts/cooperBlack.ttf", 28 );
	fontBubble.loadFont( "fonts/cooperBlack.ttf", 12 );
}

//--------------------------------------------------------------
void testApp::update()
{
	video.update();
	
	int ms = (int)( ( video.getDuration() * 1000 ) * video.getPosition() );
	
	string newSubtitleStr = srt.getSubtitleText( ms );
	
	if( newSubtitleStr != subtitleStr )
	{
		subtitleStr = newSubtitleStr;
		
		if( subtitleStr == "" )
		{
			playOutSpeachBubbles();
		}
		else 
		{
			addSpeachBubble( subtitleStr );
		}
	}
	
	updateSpeachBubbles();
}

void testApp :: addSpeachBubble ( string bubbleText )
{
	ofPoint p;
	p.set( ofRandom( 0, video.width ), ofRandom( 0, video.height ), 0 );
	
	ofRectangle r;
	r.width		= 200;
	r.height	= 100;
	
	bubbles.push_back( SpeachBubble() );
	SpeachBubble& bubble = bubbles.back();
	bubble.init( bubbleText, &fontBubble );
	bubble.setPosition( p );
//	bubble.setSize( r );
	bubble.playIn();
}



void testApp :: playOutSpeachBubbles ()
{
	for( int i=0; i<bubbles.size(); i++ )
	{
		SpeachBubble& bubble = bubbles[ i ];
		bubble.playOut();
	}
}

void testApp :: updateSpeachBubbles ()
{
	int i = 0;
	int t = bubbles.size();
	
	for( i=0; i<t; i++ )
	{
		SpeachBubble& bubble = bubbles[ i ];
		bubble.update();
		
		if( bubble.bFinished )
		{
			bubbles.erase( bubbles.begin() + i );
			
			--i;
			--t;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();
	
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	ofSetColor( 0xFFFFFF );
	video.draw( 0, 0 );
	
	ofSetColor( 0xFF00FF );
	fontCooperBlack.drawString( subtitleStr, 50, 550 );
	
	drawSpeachBubbles();
}

void testApp :: drawSpeachBubbles ()
{
	for( int i=0; i<bubbles.size(); i++ )
	{
		SpeachBubble& bubble = bubbles[ i ];
		
		ofSetColor( 0xFFFFFF );
		
		ofFill();
		bubble.drawBubble();
		
		ofNoFill();
		ofEnableSmoothing();
		bubble.drawBubble();
		ofDisableSmoothing();

		ofSetColor( 0xFF0000 );
		
		ofFill();
		bubble.drawText();
	}
}

void testApp :: drawSpeachBubble ( SpeachBubble& bubble )
{
	ofSetColor( 0xFFFFFF );
	bubble.drawBubble();
	
	ofSetColor( 0xFF0000 );
	bubble.drawText();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == ' ' )
	{
		float p;
		p = mouseX / (float)ofGetWidth();
		
		video.setPosition( p );
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

