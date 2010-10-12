#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	screenGrab.setup( "movies/" );
	screenGrab.setPause( true );
	
	srt.parse( "frankenstein.srt" );

	//-- video.
	
	bIsPlaying = !screenGrab.isRecording();
	
	videoProgress = 0.4102;
	
	video.loadMovie( "frankenstein.mov" );
	if( bIsPlaying )
		video.play();
	video.setPaused( !bIsPlaying );
	video.setPosition( videoProgress );
	video.setVolume( 0 );
		
	//-- sound.
	
	sound.loadSound( "frankenstein.mp3", true );
	sound.play();
	sound.setPosition( videoProgress );
	sound.setVolume( 1 );
	sound.setLoop( true );
	
	float audioLengthInvFreq = sound.length / (float)sound.internalFreq;
	soundTotalFrames = floor( audioLengthInvFreq * 25 );
	
	//-- video images.
	
	videoRect.width		= video.width;
	videoRect.height	= video.height;
	
	videoSmlScale = 0.25;
	
	videoCol.allocate( videoRect.width, videoRect.height );
	videoImg.allocate( videoRect.width, videoRect.height );
	videoSml.allocate( (int)( videoRect.width * videoSmlScale ), (int)( videoRect.height * videoSmlScale ) );
	
	fontCooperBlack.loadFont( "fonts/cooperBlack.ttf", 28 );
	fontBubble.loadFont( "fonts/smackattack.ttf", 12 );
	
	audio.init();
	audio.setMirrorData( true );
	audio.setMaxDecay( 0.98 );
	audioPeak = 0;
	audioData = new float[ SPEACH_BUBBLE_RESOLUTION ];
	
	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
	haarTracker.setup( &haarFinder );
}

//--------------------------------------------------------------
void testApp::update()
{
	if( screenGrab.isRecording() )
	{
		//-- video position.
		
		float t;
		t = ofGetFrameNum() / 25.0;
		
		float d;
		d = video.getDuration();
		
		float p;
		p = t / d;
		
		p += videoProgress;
		
		video.setPosition( p );
		sound.setPosition( p );
		
		//-- sound position.
		
//		float soundPosition = 0;
//		soundPosition  = ofGetFrameNum() / (float)soundTotalFrames;
//		soundPosition += videoProgress;
//		
//		sound.setPosition( soundPosition );
	}
	
	videoCol.setFromPixels( video.getPixels(), videoRect.width, videoRect.height );
	videoImg.setFromColorImage( videoCol );
	videoSml.scaleIntoMe( videoImg );
	
	updateHaarFinder();
	
	audio.update();
	audio.getFftPeakData( audioData, SPEACH_BUBBLE_RESOLUTION );
	audioPeak += ( audio.getAveragePeak() - audioPeak ) * 0.2;
	
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

void testApp :: updateHaarFinder ()
{
	haarTracker.findHaarObjects( videoSml );
	
	bHaarFaceFound = ( haarTracker.haarItems.size() > 0 );
}

void testApp :: addSpeachBubble ( string bubbleText )
{
	bubbles.push_back( SpeachBubble() );
	SpeachBubble& bubble = bubbles.back();
	bubble.init( bubbleText, &fontBubble );
	
	ofRectangle r;
	r = bubble.getRect();
	
	float a		= ofRandom( 0, TWO_PI );
	float rx	= ( videoRect.width  - r.width  ) * 0.5;
	float ry	= ( videoRect.height - r.height ) * 0.5;
	
	float ang	= ofRandom( 0, TWO_PI );
	float px	= rx * cos( a ) + videoRect.x + videoRect.width  * 0.5;
	float py	= ry * sin( a ) + videoRect.y + videoRect.height * 0.5;
	
	ofPoint p;
	p.set( px, py, 0 );
	
	ofPoint t;
	t.set
	(
		videoRect.x + videoRect.width  * 0.5,
		videoRect.y + videoRect.height * 0.5
	);
	
	bubble.setPosition( p );
	bubble.setTarget( t );
	
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
		bubble.setVolume( audioPeak );
		bubble.setVolumeData( audioData );
		
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
	//-- bg.
	
	ofFill();
	
	int gray = 30;
	
	ofSetColor( gray, gray, gray );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	//-- video image large.
	
	int gap = 10;
	
	glPushMatrix();
	glTranslatef( gap, gap, 0 );
	
	drawBorder( videoImg.width, videoImg.height );
	
	ofSetColor( 0xFFFFFF );
	videoImg.draw( 0, 0 );

	//-- video image small.
	
	glPushMatrix();
	glTranslatef( videoRect.width + gap, 0, 0 );
	
	drawBorder( videoSml.width, videoSml.height );
	
	videoSml.draw( 0, 0 );
	
	drawHaarTracker();
	
	glPushMatrix();
	glTranslatef( 0, videoSml.height + gap, 0 );
	
	audio.draw( 512, 256 );
	
	glPopMatrix();
	glPopMatrix();

	//-- subtitles.
	
	ofFill();
	ofSetColor( 0xFF00FF );
	fontCooperBlack.drawString( subtitleStr, 0, 620 );
	
	drawSpeachBubbles();
	
	glPopMatrix();
	
	drawProgressBar();
	
	screenGrab.save();
}

void testApp :: drawSpeachBubbles ()
{
	for( int i=0; i<bubbles.size(); i++ )
	{
		SpeachBubble& bubble = bubbles[ i ];

		//-- black outline.
		
		ofSetColor( 0x000000 );
		ofSetLineWidth( 3 );
		
		ofNoFill();
		ofEnableSmoothing();
		
		bubble.drawBubble();
		bubble.drawPoint();
		
		ofDisableSmoothing();
		
		//-- white fill.
		
		ofFill();
		ofSetColor( 0xFFFFFF );
		
		bubble.drawBubble();
		bubble.drawPoint();

		//-- white smaller border.
		
		ofSetColor( 0xFFFFFF );
		ofSetLineWidth( 1 );
		
		ofNoFill();
		ofEnableSmoothing();
		
		bubble.drawBubble();
		bubble.drawPoint();
		
		ofDisableSmoothing();
		
		//-- text.
		
		ofSetColor( 0x000000 );
		ofSetLineWidth( 1 );
		
		ofFill();
		bubble.drawText();
	}
}

void testApp :: drawSpeachBubble ( SpeachBubble& bubble )
{
	ofSetColor( 0xFFFFFF );
	bubble.drawBubble();
	
	ofSetColor( 0x000000 );
	bubble.drawText();
}

void testApp :: drawBorder ( int w, int h )
{
	int b = 1;
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	ofRect( -b, -b, w + b * 2, h + b * 2 );
}

void testApp :: drawHaarTracker ()
{
	ofNoFill();
	ofSetColor( 0xFF00FF );

	for( int i=0; i<haarTracker.haarItems.size(); i++ )
	{
		ofxCvHaarTrackerItem& haarItem = haarTracker.haarItems[ i ];
		
		const ofRectangle& r = haarItem.rectEase;
		
		ofRect( r.x, r.y, r.width, r.height );
	}
}

void testApp :: drawProgressBar ()
{
	float p = video.getPosition();
	
	int h = 10;
	
	ofFill();
	ofSetColor( 0xFFFF00 );
	ofRect( 0, ofGetHeight() - h, (int)( p * ofGetWidth() ), h );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == ' ' )
	{
		float p;
		p = mouseX / (float)ofGetWidth();
		
		video.setPosition( p );
		sound.setPosition( p );
	}
	
	if( key == 'p' )
	{
		video.setPaused( bVideoPaused = !bVideoPaused );
	}
	
	if( key == 's' )
	{
		screenGrab.togglePause();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
	ofPoint p;
	p.set( x, y, 0 );
	
	float v = mouseX / (float)ofGetWidth();
	
	for( int i=0; i<bubbles.size(); i++ )
	{
		SpeachBubble& bubble = bubbles[ i ];
//		bubble.setTarget( p );
//		bubble.setVolume( v );
	}
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

