
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
	ofxLocation.addListener( this );
	
	renderArea.x      = 0;
	renderArea.y	  = 0;
	renderArea.width  = ofGetScreenWidth();
	renderArea.height = ofGetScreenHeight();

	verdana.loadFont(ofToDataPath("verdana.ttf"),8, false, true);
	verdana.setLineHeight(20.0f);
	
	coreLocation	= new ofxiPhoneCoreLocation();
	hasGPS			= coreLocation->startLocation();
	
	sender.setup( HOST, PORT );
	
	resX = 20;
	bandSize = 0.1;
	bandMult = (int)( 1 / bandSize );
	
	oscTime			= 0;
	oscTimeout		= 1000;
	
	speedValue		= 0;
	speedValueMax	= 0;
	speedValueNorm	= 0;
	
	locationTime	= 0;
	
	drawOffset		= 0;
	drawVertical	= false;
	
	int dataFillNum;									// fill the screen.
	dataFillNum = ceil( renderArea.height / resX );
	for( int i=0; i<dataFillNum; i++ )
	{
		tiltY.push_back( 0 );
		tiltX.push_back( 0 );
		speed.push_back( 0 );
		audio.push_back( 0 );
	}
}


///////////////////////////////////////////////////////////
// UPDATE.
///////////////////////////////////////////////////////////

void testApp::update()
{
	elapsedTime = ofGetElapsedTimeMillis();
	
	if( checkOscTimer() )
	{
		ofPoint orientation;
		orientation  = ofxAccelerometer.getOrientation();
		orientation /= 360.0;	// -0.5 to 0.5
		orientation *= 4;
		orientation += 0.5;		// 0 to 1
		
		orientation.x = MIN( MAX( orientation.x, 0 ), 1 );
		orientation.y = MIN( MAX( orientation.y, 0 ), 1 );
		orientation.z = MIN( MAX( orientation.z, 0 ), 1 );
		
		tiltY.push_back( orientation.y );
		tiltX.push_back( orientation.x );
		
		float audioNorm;
		audioNorm = 0.0;
//		audioNorm = iPhoneGetMicAverageLevel();
		
		audio.push_back( audioNorm );
		
		speed.push_back( speedValueNorm );
		
		sendOscMessage();
	}
}

bool testApp :: checkOscTimer ()
{
	if( elapsedTime > oscTime )
	{
		int count;
		count = (int)( elapsedTime / (float)oscTimeout );
		count += 1;
		
		oscTime = count * oscTimeout;
		
		drawOffset = 1;
		
		return true;
	}
	
	drawOffset = ( oscTime - elapsedTime ) / (float)oscTimeout;
	
	return false;
}

void testApp :: sendOscMessage ()
{
	ofxOscMessage m;
	m.setAddress( "/acceleration/xyz" );
	m.addFloatArg( tiltX.back() );
	m.addFloatArg( tiltY.back() );
	m.addFloatArg( speed.back() );
	m.addFloatArg( audio.back() );
	sender.sendMessage( m );
}

///////////////////////////////////////////////////////////
// DRAW.
///////////////////////////////////////////////////////////

void testApp :: draw()
{
	int renderPad = 10;
	int renderHeight;
	int renderWidth;
	int barsTotal = 4;
	
	if( drawVertical )
	{
		renderHeight = (int)( ( renderArea.height - renderPad * ( barsTotal + 1 ) ) * 1/(float)barsTotal );
	}
	else
	{
		renderHeight = (int)( ( renderArea.width  - renderPad * ( barsTotal + 1 ) ) * 1/(float)barsTotal );
	}
	
	if( drawVertical )
	{
		renderWidth = renderArea.width;
	}
	else
	{
		renderWidth = renderArea.height;
	}
	
	if( !drawVertical )
	{
		glPushMatrix();
		glRotatef( 90, 0, 0, 1 );
		glTranslatef( 0, -renderArea.width, 0 );
	}
	
	glPushMatrix();
	glTranslatef( -resX + drawOffset * resX, 0, 0 );
	
	glPushMatrix();
	glTranslatef( 0, renderHeight * 0 + renderPad * 1, 0 );
		drawData( &tiltY, renderWidth, renderHeight );
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 0, renderHeight * 1 + renderPad * 2, 0 );
		drawData( &tiltX, renderWidth, renderHeight );
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 0, renderHeight * 2 + renderPad * 3, 0 );
		drawData( &speed, renderWidth, renderHeight );
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 0, renderHeight * 3 + renderPad * 4, 0 );
		drawData( &audio, renderWidth, renderHeight );
	glPopMatrix();

	glPopMatrix();

	if( drawVertical )
	{
		ofSetColor( 0xFF00FF );
		string str;
		str =	"tiltY      :: " + ofToString( tiltY.back(), 2 ) + "\n" +
				"tiltX      :: " + ofToString( tiltX.back(), 2 ) + "\n" +
				"speedValue :: " + ofToString( speedValue, 4 ) + "\n" +
				"latitude   :: " + ofToString( coreLocation->getLatitude(), 4 ) + "\n" +
				"longitude  :: " + ofToString( coreLocation->getLongitude(), 4 );
		verdana.drawString( str, 10, 10);
	}
	
	if( !drawVertical )
	{
		glPopMatrix();
	}
}

void testApp :: drawData( vector<float> *data, int w, int h )
{
	int dataMax;
	dataMax = ceil( w / resX ) + 1;		// add one for smooth transition.
	
	int dataTotal;
	dataTotal = data->size();
	
	int dataIndex;
	dataIndex = 0;
	if( dataTotal > dataMax )
		dataIndex = dataTotal - dataMax;
	
	for( int i=dataIndex; i<dataTotal; i++ )
	{
		int j;
		j = i - dataIndex;
		
		float dataValue;
		dataValue = data->at( i );
		
		float dataBanded;
		dataBanded = (int)( dataValue * bandMult ) / (float)bandMult;
		
		int dataColor;
		dataColor = (int)( dataBanded * 255 );
		
		ofSetColor( dataColor, dataColor, dataColor );
		ofRect( j * resX, 0, resX, h );
	}
}

///////////////////////////////////////////////////////////
// EXIT.
///////////////////////////////////////////////////////////

void testApp::exit()
{
	//
}

///////////////////////////////////////////////////////////
// MULTI TOUCH HANDLERS.
///////////////////////////////////////////////////////////

void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	//
}

void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	//
}


void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	drawVertical = !drawVertical;
}

void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	ofToggleFullscreen();
}

///////////////////////////////////////////////////////////
// ALERT HANDLERS.
///////////////////////////////////////////////////////////

void testApp::lostFocus()
{
	//
}

void testApp::gotFocus()
{
	//
}

void testApp::gotMemoryWarning()
{
	//
}

///////////////////////////////////////////////////////////
// LOCATION HANDLERS.
///////////////////////////////////////////////////////////

void testApp :: locationChange ( ofxLocationCustomLocationData *data )
{
	int elapsedMs;
	elapsedMs = ofGetElapsedTimeMillis();
	
	int timeMs;
	timeMs = elapsedMs - locationTime;

	locationTime	= elapsedMs;
	
	float kms		= data->distMoved * 1/1000.0;
	float hr		= timeMs		  * 1/1000.0 * 1/60.0 * 1/60.0;
	
	speedValue		= kms / hr;
	
	//-- new approach.
	
	speedValue		= data->speed;		// meters per second.
	speedValue		*= 1/1000.0;		// kilometers per second.
	speedValue		*= 60 * 60;			// kilmeters per hour.
	
	if( speedValueMax < speedValue )
		speedValueMax = speedValue;
	
	if( speedValueMax == 0 )
		return;
	
	speedValueNorm = speedValue / speedValueMax;
}

void testApp :: headingChange ( ofxLocationCustomHeadingData *data )
{
	//
}
