#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{	 
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber( WIDTH, HEIGHT );
        
	colorLargeImage.allocate( WIDTH, HEIGHT );
	colorSmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
    grayLargeImage.allocate( WIDTH, HEIGHT );
	graySmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
	
	sourceToSampleScale = WIDTH / SAMPLE_WIDTH;
	sampleToSourceScale	= SAMPLE_WIDTH / (float)WIDTH;

	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
	haarTracker.setup( &haarFinder );
	
	blurImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
	
	cross.loadImage( "cross02.png" );
	wantedSketch.loadImage( "wanted_sketch.png" );
	interlace.loadImage( "interlace.png" );
	
	blurPixels	= new unsigned char[ WIDTH * HEIGHT * 4 ];
	blurTexture.allocate( WIDTH, HEIGHT, GL_RGBA );
	
	squarePixels = new unsigned char[ WIDTH * HEIGHT * 4 ];
	squareTexture.allocate( WIDTH, HEIGHT, GL_RGBA );
	
	crimModeIndex	= 3;
	crimModeTotal	= 4;
	
	krungthepSmall.loadFont( "Krungthep.ttf", 14 );
	krungthepLarge.loadFont( "Krungthep.ttf", 24 );
	
	milliSecondCounter	= 0;
	interlaceCounter	= 0;
	interlaceY			= 0;
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground( 10, 10, 10 );
    
    bool bNewFrame = false;
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if( bNewFrame )
	{
		colorLargeImage.setFromPixels( vidGrabber.getPixels(), WIDTH, HEIGHT );

		colorLargeImage.mirror( false, true );
		colorSmallImage.scaleIntoMe( colorLargeImage );
		
		grayLargeImage	= colorLargeImage;
		graySmallImage	= colorSmallImage;
		
		haarTracker.findHaarObjects( graySmallImage );
	}
	
	milliSecondCounter += (int)( 1000.0f / (float)ofGetFrameRate() );
	if( milliSecondCounter > 999 )
	{
		milliSecondCounter = 0;
	}
	
	if( ++interlaceCounter % 2 == 0 )
	{
		if( ++interlaceY > 4 )
		{
			interlaceY = 0;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	int i;
	float x, y, w, h;
	float wRatio, hRatio, scale;
	int faceID;
	float faceMode;
	
	#ifdef DEBUG_MODE
	#else
	
		wRatio = ofGetWidth() / (float)WIDTH;
		hRatio = ofGetHeight() / (float)HEIGHT;
	
		scale = MIN( wRatio, hRatio );
		
		glPushMatrix();
		glTranslatef
		( 
			(int)( ( ofGetWidth() - ( WIDTH * scale ) ) * 0.5f ),
			(int)( ( ofGetHeight() - ( HEIGHT * scale ) ) * 0.5f ),
			0
		);
		glScalef( scale, scale, 0 );
	
	#endif
	
	ofSetColor( 0xffffff );
	grayLargeImage.draw( 0, 0 );
	
	#ifdef DEBUG_MODE
		graySmallImage.draw( WIDTH, 0 );
	#endif
	
	drawCameraText();
	
	ofSetColor( 255, 255, 255 );
	ofEnableAlphaBlending();
	interlace.draw( 0, -interlaceY );
	ofDisableAlphaBlending();
	
	while( haarTracker.hasNextHaarItem() )
	{
		faceID		= haarTracker.getHaarItemID();
		faceMode	= ( faceID % 10 ) / 10.0f;
		
		haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );
		
		x	*= sourceToSampleScale;
		y	*= sourceToSampleScale;
		w	*= sourceToSampleScale;
		h	*= sourceToSampleScale;

		if( faceMode >= 0.72f )
		{
			drawBlackBar( x, y, w, h );
		}
		else if( faceMode >= 0.42f )
		{
			drawCross( x, y, w, h );
		}
		else if( faceMode >= 0.16f )
		{
			drawSquares( x, y, w, h );
		}
		else
		{
			drawBlur( x, y, w, h );
		}
	}
	
	#ifdef DEBUG_MODE
	#else
		glPopMatrix();
	#endif
}

void testApp :: drawFaceRect ( float x, float y, float w, float h )
{
	ofSetColor( 0xFF0000 );
	ofRect( x, y, w, h );
}

void testApp :: drawBlackBar ( float x, float y, float w, float h )
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect
	(
	 x,
	 y + h * 0.25f,
	 w,
	 h * 0.24f
	 );
}

void testApp :: drawCross ( float x, float y, float w, float h )
{
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	cross.draw
	(
	 x - w * 0.15f,
	 y - h * 0.15f,
	 w * 1.3f,
	 h * 1.3f
	 );
	ofDisableAlphaBlending();
}

void testApp :: drawWantedSketch ( float x, float y, float w, float h )
{
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	wantedSketch.draw
	(
	 x - w * 0.15f,
	 y - h * 0.15f,
	 w * 1.3f,
	 h * 1.3f
	 );
	ofDisableAlphaBlending();
}

void testApp :: drawBlur ( float x, float y, float w, float h )
{
	blurImage = graySmallImage;
	blurImage.blur( 14 );
	
	unsigned char *pixels = blurImage.getPixels();
	int px, py;
	int rx, ry, ri;
	int rx2, ry2;
	int i, j;
	int r, g, b, a;
	
	r = g = b = a = 255;
	
	float ang;
	float lenMax, len;
	float lx, ly;
	float mx, my;
	
	x	*= sampleToSourceScale;
	y	*= sampleToSourceScale;
	w	*= sampleToSourceScale;
	h	*= sampleToSourceScale;
	
	y -= h * 0.25f;
	h *= 1.5f;
	x -= w * 0.1f;
	w *= 1.2f;
	
	for( py=0; py<SAMPLE_HEIGHT; py++ )
	{
		for( px=0; px<SAMPLE_WIDTH; px++ )
		{
			i = ( px * sourceToSampleScale * 4 ) + ( py * sourceToSampleScale * WIDTH * 4 );
			j = ( px * 1 ) + ( py * SAMPLE_WIDTH * 1 );
			
			if( px >= (int)x && px < (int)x + w && py >= (int)y && py < (int)y + h )
			{	
				mx		= px - ( x + w * 0.5f );
				my		= py - ( y + h * 0.5f );
				ang		= atan2( mx, my );
				lx		= w * 0.5f * sin( ang );
				ly		= h * 0.5f * cos( ang );
				lenMax	= sqrt( lx * lx + ly * ly );
				len		= sqrt( mx * mx + my * my );
				
				r = pixels[ j + 0 ];
				g = pixels[ j + 0 ];
				b = pixels[ j + 0 ];
				
				if( len < lenMax )
				{
					rx2 = px * sourceToSampleScale;
					ry2	= py * sourceToSampleScale;
					
					for( rx=rx2; rx<rx2+sourceToSampleScale; rx++ )
					{
						for( ry=ry2; ry<ry2+sourceToSampleScale; ry++ )
						{
							ri = ( rx * 4 ) + ( ry * WIDTH * 4 );
							
							blurPixels[ ri + 0 ] = r;
							blurPixels[ ri + 1 ] = g;
							blurPixels[ ri + 2 ] = b;
							
							if( len > lenMax * 0.7f )
							{
								blurPixels[ ri + 3 ] = (int)( ( lenMax - len ) / ( lenMax * 0.3f ) * 255 );
							}
							else
							{
								blurPixels[ ri + 3 ] = a;
							}
						}
					}
				}
				else
				{
					rx2 = px * sourceToSampleScale;
					ry2	= py * sourceToSampleScale;
					
					for( rx=rx2; rx<rx2+sourceToSampleScale; rx++ )
					{
						for( ry=ry2; ry<ry2+sourceToSampleScale; ry++ )
						{
							ri = ( rx * 4 ) + ( ry * WIDTH * 4 );
							
							blurPixels[ ri + 0 ] = 0;
							blurPixels[ ri + 1 ] = 0;
							blurPixels[ ri + 2 ] = 0;
							blurPixels[ ri + 3 ] = 0;
						}
					}
				}
			}
			else
			{
				rx2 = px * sourceToSampleScale;
				ry2	= py * sourceToSampleScale;
				
				for( rx=rx2; rx<rx2+sourceToSampleScale; rx++ )
				{
					for( ry=ry2; ry<ry2+sourceToSampleScale; ry++ )
					{
						ri = ( rx * 4 ) + ( ry * WIDTH * 4 );
						
						blurPixels[ ri + 0 ] = 0;
						blurPixels[ ri + 1 ] = 0;
						blurPixels[ ri + 2 ] = 0;
						blurPixels[ ri + 3 ] = 0;
					}
				}
			}
		}
	}
	
	blurTexture.loadData( blurPixels, WIDTH, HEIGHT, GL_RGBA );
	
	ofNoFill();
	ofSetColor( 255, 255, 255 );
	ofEnableAlphaBlending();
	blurTexture.draw( 0, 0 );
	ofDisableAlphaBlending();
}

void testApp :: drawSquares ( float x, float y, float w, float h )
{
	unsigned char *pixels = grayLargeImage.getPixels();
	int px, py;
	int rx, ry, ri;
	int i, j;
	int r, g, b, a;
	int res;
	
	float ang;
	float lenMax, len;
	float lx, ly;
	float mx, my;
	
	r = g = b = a = 255;
	res = 8;
	
	y -= h * 0.4f;
	h *= 1.6f;
	x -= w * 0.1f;
	w *= 1.2f;
	
	for( py=0; py<HEIGHT; py++ )
	{
		for( px=0; px<WIDTH; px++ )
		{
			i = ( px * 4 ) + ( py * WIDTH * 4 );
			j = ( px * 1 ) + ( py * WIDTH * 1 );
			
			if( px >= (int)x && px < (int)( x + w ) && py >= (int)y && py < (int)( y + h ) )
			{	
				if
					( 
					 ( ( px - (int)x ) % res == 0 ) &&
					 ( ( py - (int)y ) % res == 0 ) 
					 )
				{
					mx		= px - ( x + w * 0.5f );
					my		= py - ( y + h * 0.5f );
					ang		= atan2( mx, my );
					lx		= w * 0.5f * sin( ang );
					ly		= h * 0.5f * cos( ang );
					lenMax	= sqrt( lx * lx + ly * ly );
					len		= sqrt( mx * mx + my * my );
					
					r = pixels[ j + 0 ];
					g = pixels[ j + 0 ];
					b = pixels[ j + 0 ];
					
					if( len < lenMax )
					{
						for( rx=px; rx<px+res; rx++ )
						{
							for( ry=py; ry<py+res; ry++ )
							{
								ri = ( rx * 4 ) + ( ry * WIDTH * 4 );
								
								squarePixels[ ri + 0 ] = r;
								squarePixels[ ri + 1 ] = g;
								squarePixels[ ri + 2 ] = b;
								//								squarePixels[ ri + 3 ] = a;
								
								//								squarePixels[ ri + 0 ] = 255;
								//								squarePixels[ ri + 1 ] = 0;
								//								squarePixels[ ri + 2 ] = 0;
								//								squarePixels[ ri + 3 ] = 255;
								
								if( len > lenMax * 0.8f )
								{
									squarePixels[ ri + 3 ] = (int)( ( lenMax - len ) / ( lenMax * 0.2f ) * 255 );
								}
								else
								{
									squarePixels[ ri + 3 ] = a;
								}
							}
						}
					}
					else
					{
						for( rx=px; rx<px+res; rx++ )
						{
							for( ry=py; ry<py+res; ry++ )
							{
								ri = ( rx * 4 ) + ( ry * WIDTH * 4 );
								
								squarePixels[ ri + 0 ] = 0;
								squarePixels[ ri + 1 ] = 0;
								squarePixels[ ri + 2 ] = 0;
								squarePixels[ ri + 3 ] = 0;
							}
						}
					}
				}
			}
			else
			{
				squarePixels[ i + 0 ] = 0;
				squarePixels[ i + 1 ] = 0;
				squarePixels[ i + 2 ] = 0;
				squarePixels[ i + 3 ] = 0;
			}
		}
	}
	
	squareTexture.loadData( squarePixels, WIDTH, HEIGHT, GL_RGBA );
	
	ofNoFill();
	ofSetColor( 255, 255, 255 );
	ofEnableAlphaBlending();
	squareTexture.draw( 0, 0 );
	ofDisableAlphaBlending();
}

void testApp :: drawCameraText ()
{
	ofSetColor( 0, 0, 0 );
	ofFill();
	ofRect( 8, 8, 175, 24 );
	ofRect( 447, 8, 188, 24 );
	ofRect( 216, 430, 266, 40 );
	
	ofNoFill();
	
	char dateString[ 255 ];
	sprintf( dateString, "%02d / %02d / %02d" , ofGetDay(), ofGetMonth(), ofGetYear() );
	
	char timeString[ 255 ];
	sprintf( timeString, "%02d : %02d : %02d.%03d", ofGetHours(), ofGetMinutes(), ofGetSeconds(), milliSecondCounter );
	
	ofSetColor( 0xFFFFFF );
	krungthepSmall.drawString( dateString, 10, 25 );
	krungthepSmall.drawString( timeString, 450, 25 );
	krungthepLarge.drawString( "CRIMCAM 033", 220, 460 );
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
		case ' ':
			vidGrabber.videoSettings();
		break;
	}
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
void testApp::mouseReleased()
{
	//
}
