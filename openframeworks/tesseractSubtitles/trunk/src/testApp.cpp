	#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	
	ofRectangle screenRect;
	screenRect.width	= ofGetWidth();
	screenRect.height	= ofGetHeight();
	
	screenGrab.setup( "movies/image", &screenRect );
//	screenGrab.start();
	
	bImageChanged		= false;
	
	fontVerdana.loadFont( "fonts/verdana.ttf", 22 );
//	fontCooperBlack.loadFont( "fonts/cooperBlack.ttf", 22 );
	fontCooperBlack.loadFont( "fonts/cooperBlack.ttf", 28 );
	
	screenScale			= 0.46;
	
	bCheckImage			= true;
	hsvDiffThreshold	= 0.003;
	hsvBlur				= 2;
	hsvBlurThreshold	= 79;
	
	minBlobSize.width	= 1;
	minBlobSize.height	= 1;
	
	maxBlobSize.width	= 20;
	maxBlobSize.height	= 20;
	
	tessStr				= "TEST";

	initImage();
	initHSV();
	initMotionSub();
	initBlobs();
	initGui();
	
	loadFromFile();
}

void testApp :: initImage ()
{
	image.loadMovie( "videos/ghost_in_the_shell_chap_12.mov" );
//	image.play();
//	image.setPaused( bIsPlaying = true );
	image.setPosition( videoPosition = 0 );
	
	imageRect.x			= 300;
	imageRect.y			= 100;
	imageRect.width		= image.width;
	imageRect.height	= image.height;
	
	imageColor.allocate( image.width, image.height );
	
	float px, py;
	px = 1.0;
	py = 0.3;
	
	roi.x		= 0;
	roi.y		= (int)( image.height * ( 1 - py ) );
	roi.width	= (int)( image.width  * px );
	roi.height	= (int)( image.height * py );
	
	finalImg.allocate( roi.width, roi.height );
	finalImgCopy.allocate( roi.width, roi.height );
	finalImgDiff.allocate( roi.width, roi.height );
}

void testApp :: initHSV ()
{
	hsv.hue			= 0.0;
	hsv.hueRange	= 0.06;
	hsv.sat			= 0.62;
	hsv.satRange	= 0.94;
	hsv.val			= 0.8;
	hsv.valRange	= 0.62;
	
	hsvSourceImg.allocate( roi.width, roi.height );				// HSV Image
	hsvProcessedImg.allocate( roi.width, roi.height );			// HSV Processed Image
	
	hsvHueImg.allocate( roi.width, roi.height );				// Hue Image
	hsvSatImg.allocate( roi.width, roi.height );				// Saturation Image
	hsvValImg.allocate( roi.width, roi.height );				// value Image
	
	hsvProcessedImgPixels = new unsigned char [ (int)roi.width * (int)roi.height ];		// HSV Processed Image pixels.
}

void testApp :: initMotionSub ()
{
	motionImg.allocate( roi.width, roi.height );
}

void testApp :: initBlobs ()
{
	blobImg.allocate( roi.width, roi.height );
}

void testApp :: initGui ()
{
	gui.addTitle( "general" );
	gui.addSlider( "screenScale",		screenScale,		0, 1.0 );
	gui.addToggle( "bIsPlaying",		bIsPlaying );
	gui.addSlider( "videoPosition",		videoPosition,		0, 1.0 );
	
	gui.addPage( "openCV" );
	gui.addToggle( "bCheckImage",			bCheckImage );
	gui.addSlider( "hsvDiffThreshold",		hsvDiffThreshold,	0, 0.1 );
	gui.addSlider( "hsvBlur",				hsvBlur,			0, 30  );
	gui.addSlider( "hsvBlurThreshold",		hsvBlurThreshold,	0, 255 );
	gui.addSlider( "minBlobSize.width",		minBlobSize.width,	0, 10.0 );
	gui.addSlider( "minBlobSize.height",	minBlobSize.height,	0, 10.0 );
	gui.addSlider( "maxBlobSize.width",		maxBlobSize.width,	0, 20.0 );
	gui.addSlider( "maxBlobSize.height",	maxBlobSize.height,	0, 20.0 );
	
	gui.addPage( "HSV" );
	gui.addSlider( "hsv.hue",		hsv.hue,		0, 1.0 );
	gui.addSlider( "hsv.hueRange",	hsv.hueRange,	0, 1.0 );
	gui.addSlider( "hsv.sat",		hsv.sat,		0, 1.0 );
	gui.addSlider( "hsv.satRange",	hsv.satRange,	0, 1.0 );
	gui.addSlider( "hsv.val",		hsv.val,		0, 1.0 );
	gui.addSlider( "hsv.valRange",	hsv.valRange,	0, 1.0 );
	
	gui.show();
	gui.setPage( 3 );
}

//--------------------------------------------------------------
void testApp::update()
{
	if( screenGrab.isRecording() )
	{
		float t;
		t = ofGetFrameNum() / (float)ofGetFrameRate();
		
		float p;
		p = t / image.getDuration();
		
		image.setPosition( p );
	}
	
	imageColor.setFromPixels( image.getPixels(), image.width, image.height );
		
	checkHSV();
	checkMotionSub();
	checkBlobs();
	
	checkForCommonFill( finalImg, hsvProcessedImg, blobImg );
	
	checkImageChange();
	
	if( !bCheckImage )
		return;
		
	if( bImageChanged )
	{
		tessStr = tess.findTextInImage( finalImg.getPixels(), finalImg.width, finalImg.height, OF_IMAGE_GRAYSCALE );
		parseTesseractString( tessStr );
		
		cout << tessStr << endl;
	}
}

void testApp :: checkHSV ()
{
	ofxCvColorImage imageColorTemp;										// have to copy image due to bug with roi.
	imageColorTemp.allocate( imageColor.width, imageColor.height );
	imageColorTemp = imageColor;
	imageColorTemp.setROI( roi );
	
	hsvSourceImg.setFromPixels( imageColorTemp.getRoiPixels(), roi.width, roi.height );
	
	imageColorTemp.clear();
	
	hsvSourceImg.convertRgbToHsv();
	hsvSourceImg.convertToGrayscalePlanarImages( hsvHueImg, hsvSatImg, hsvValImg );
	
	unsigned char * huePixels = hsvHueImg.getPixels();
	unsigned char * satPixels = hsvSatImg.getPixels();
	unsigned char * valPixels = hsvValImg.getPixels();
	int nPixels = roi.width * roi.height;
	
	hsv.minHue = max( ( hsv.hue - hsv.hueRange * 0.5) * 255, 000.0 );
	hsv.maxHue = min( ( hsv.hue + hsv.hueRange * 0.5) * 255, 255.0 );
	hsv.minSat = max( ( hsv.sat - hsv.satRange * 0.5) * 255, 000.0 );
	hsv.maxSat = min( ( hsv.sat + hsv.satRange * 0.5) * 255, 255.0 );
	hsv.minVal = max( ( hsv.val - hsv.valRange * 0.5) * 255, 000.0 );
	hsv.maxVal = min( ( hsv.val + hsv.valRange * 0.5) * 255, 255.0 );
		
	for( int j=0; j<nPixels; j++ )
	{
		if
		(
			( huePixels[ j ] >= hsv.minHue && huePixels[ j ] <= hsv.maxHue ) &&
			( satPixels[ j ] >= hsv.minSat && satPixels[ j ] <= hsv.maxSat ) &&
			( valPixels[ j ] >= hsv.minVal && valPixels[ j ] <= hsv.maxVal )
		)
		{
			hsvProcessedImgPixels[ j ] = 255;
		}
		else
		{
			hsvProcessedImgPixels[ j ] = 0;
		}
	}
		
	hsvProcessedImg.setFromPixels( hsvProcessedImgPixels, hsvProcessedImg.width, hsvProcessedImg.height );
	hsvProcessedImg.blurGaussian( hsvBlur );
	hsvProcessedImg.threshold( hsvBlurThreshold );
}

void testApp :: checkMotionSub ()
{
//	motionImg = hsvProcessedImg;
//	motionImg.absDiff( hsvProcessedImg );
}

void testApp :: checkBlobs ()
{
	contourFinder.findContours
	(
		hsvProcessedImg,									// image to be used.
		(int)( minBlobSize.width * minBlobSize.height ),	// min area.
		(int)( maxBlobSize.width * maxBlobSize.height ),	// max area.
		100,												// max number of contours to find.
		false,												// find holes.
		false												// use approximation.
	);

	blobImg.set( 0 );
	
	for( int i=0; i<contourFinder.nBlobs; i++ )
	{
		const ofxCvBlob& blob = contourFinder.blobs.at( i );
		
		blobImg.setROI( blob.boundingRect );
		blobImg.set( 255 );
		blobImg.resetROI();
		
		blobImg.drawBlobIntoMe( contourFinder.blobs[ i ], 255 );
	}
}

void testApp :: checkForCommonFill ( ofxCvGrayscaleImage& imageOut, ofxCvGrayscaleImage& image1, ofxCvGrayscaleImage& image2 )
{
	int noPixels;
	noPixels = imageOut.width * imageOut.height;

	unsigned char* imageOutPixels;
	unsigned char* image1Pixels;
	unsigned char* image2Pixels;
	
	imageOutPixels	= new unsigned char[ noPixels ];
	image1Pixels	= image1.getPixels();
	image2Pixels	= image2.getPixels();
	
	for( int i=0; i<noPixels; i++ )
	{
		if
		( 
			image1Pixels[ i ] == 255 &&
			image2Pixels[ i ] == 255
		)
		{
			imageOutPixels[ i ] = 255;
		}
		else
		{
			imageOutPixels[ i ] = 0;
		}
	}
	
	imageOut.setFromPixels( imageOutPixels, imageOut.width, imageOut.height );
	
	delete[] imageOutPixels;
}

void testApp :: checkImageChange ()
{
	finalImgDiff	 = finalImg;
	finalImgDiff	-= finalImgCopy;
	finalImgCopy	 = finalImg;
	
	int pTotal;
	int pCount;
	
	pTotal	= finalImgDiff.width * finalImgDiff.height;
	pCount	= finalImgDiff.countNonZeroInRegion( 0, 0, finalImgDiff.width, finalImgDiff.height );
	
	float pPercent;
	pPercent = pCount / (float)pTotal;
	
	bImageChanged = ( pPercent > hsvDiffThreshold );
}

void testApp :: parseTesseractString ( string &str )
{
	// remove unwanted bits.
}

//--------------------------------------------------------------
void testApp::draw()
{
//	ofBackground( 20, 20, 20 );
	ofBackground( 40, 40, 40 );
	
	glPushMatrix();
	glTranslatef( imageRect.x, imageRect.y, 0 );
	
	glPushMatrix();
	glScalef( screenScale, screenScale, 0 );

	drawSourceImage();

	glPushMatrix();
//	glTranslatef( 0, imageColor.height + 30, 0 );
	glTranslatef( imageColor.width + 30, 0, 0 );

	drawHSV();
	
	glPushMatrix();
	glTranslatef( 0, hsvProcessedImg.height + 30, 0 );
	
	drawMotionImage();
	
	glPushMatrix();
	glTranslatef( 0, motionImg.height + 30, 0 );
	
	drawBlobs();

	glPushMatrix();
	glTranslatef( 0, blobImg.height + 30, 0 );
	
	drawFinal();
	
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	drawTesseractText();
	
	glPopMatrix();
	
	gui.draw();
	
	screenGrab.save();
}

void testApp :: drawSourceImage ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	int b = 2;
	ofRect
	(
		-b,
		-b,
		imageColor.width  + b * 2,
		imageColor.height + b * 2
	);
	
	imageColor.draw( 0, 0 );
	
	drawROI();
}

void testApp :: drawHSV ()
{
	drawBorder( hsvProcessedImg, 0xFFFFFF, 2 );	

	ofFill();
	ofSetColor( 0xFFFFFF );
	hsvProcessedImg.draw( 0, 0 );
}

void testApp :: drawMotionImage ()
{
	drawBorder( motionImg, 0xFFFFFF, 2 );	
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	motionImg.draw( 0, 0 );
}

void testApp :: drawBlobs ()
{
	drawBorder( blobImg, 0xFFFFFF, 2 );
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	blobImg.draw( 0, 0 );
	
//	ofNoFill();
//	ofSetLineWidth( 0.1 );
//	contourFinder.draw();
}

void testApp :: drawFinal ()
{
	drawBorder( finalImg, 0xFFFFFF, 2 );
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	finalImg.draw( 0, 0 );
}

void testApp :: drawBorder ( ofxCvImage& img, int color, int thickness )
{
	ofFill();
	ofSetColor( color );
	
	int b = thickness;
	ofRect
	(
		-b,
		-b,
		img.width  + b * 2,
		img.height + b * 2
	);
}

void testApp :: drawROI ()
{
	ofNoFill();
	ofSetLineWidth( 3 );
	ofSetColor( 0XFF0000 );
	ofRect( roi.x, roi.y, roi.width, roi.height );
}

void testApp :: drawTesseractText ()
{
	ofSetColor( 255, 0, 255 );
	fontCooperBlack.drawString( tessStr, 0, 450 );
}

/////////////////////////////////////////////
//	CONFIG.
/////////////////////////////////////////////

void testApp :: writeToFile	( string filename )
{
	ofstream fout;
	fout.open( ofToDataPath( filename ).c_str() );
	
	string dataStr;
		
	dataStr = 
	ofToString( hsv.hue,		6 ) + " " +
	ofToString( hsv.hueRange,	6 ) + " " +
	ofToString( hsv.sat,		6 ) + " " +
	ofToString( hsv.satRange,	6 ) + " " +
	ofToString( hsv.val,		6 ) + " " +
	ofToString( hsv.valRange,	6 );
		
	fout << dataStr << endl;
	
	fout.close();
}

void testApp :: loadFromFile ( string filename )
{
	ifstream	dataFile;
	string		dataStr;
	
	dataFile.open( ofToDataPath( filename ).c_str() );
	
	if( dataFile.is_open() )
	{
		while( !dataFile.eof() )
		{
			getline( dataFile, dataStr );
			
			if( dataStr == "" )
				continue;
			
			vector<string> data;
			data = ofSplitString( dataStr, " " );
			
			hsv.hue			= atof( data[ 0 ].c_str() );
			hsv.hueRange	= atof( data[ 1 ].c_str() );
			hsv.sat			= atof( data[ 2 ].c_str() );
			hsv.satRange	= atof( data[ 3 ].c_str() );
			hsv.val			= atof( data[ 4 ].c_str() );
			hsv.valRange	= atof( data[ 5 ].c_str() );
		}
	}
	
	dataFile.close();
}

/////////////////////////////////////////////
//	HANDLERS.
/////////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key >= '0' && key <= '9' )
	{
		gui.setPage( key - '0' );
		gui.show();
	}
	else
	{
		switch(key)
		{
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if( key == ' ' )
	{
		image.setPaused( bIsPlaying = !bIsPlaying );
	}

	if( key == 'r' )
	{
		image.setPosition( 0 );
	}
	
	if( key == 's' )
	{
		if( screenGrab.isRecording() )
		{
			screenGrab.stop();
		}
		else
		{
			screenGrab.start();
		}
	}
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
	int ix, iy, iw, ih;
	
	ix = imageRect.x + (int)( roi.x		 * screenScale );
	iw = ix			 + (int)( roi.width	 * screenScale );
	iy = imageRect.y + (int)( roi.y		 * screenScale );
	ih = iy			 + (int)( roi.height * screenScale );
	
	if
	(
		x >= ix &&
		x <  iw &&
		y >= iy	&&
		y <  ih
	)	
	{
		int p;														// pixel no.
		p = ( ( y - iy ) * roi.width + ( x - ix ) ) / screenScale;
		
		hsv.hue = hsvHueImg.getPixels()[ p ] / 255.0;
		hsv.sat = hsvSatImg.getPixels()[ p ] / 255.0;
		hsv.val = hsvValImg.getPixels()[ p ] / 255.0;
		
		writeToFile();
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

