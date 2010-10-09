#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	
	bDebug			= true;
	bSmoothing		= false;
	bPause			= false;
	bDrawPoints		= false;
	bDrawLines		= false;
	bDrawCurves		= false;
	bDrawSimplified	= true;
	bDrawColor		= false;
	bRotateColor	= true;
	
	screenRect.width	= ofGetWidth();
	screenRect.height	= ofGetHeight();
	
	sourceRect.width	= 886;
	sourceRect.height	= 691;
	
	float smallRectScale;
	smallRectScale = 0.25;
	
	smallRect.width		= sourceRect.width  * smallRectScale;
	smallRect.height	= sourceRect.height * smallRectScale;

	float largeRectExtra;
	largeRectExtra		= 0.04;			// the extra is to cover the whole screen. set to 0 to see what happens without it.
//	largeRectExtra		= 0;
	
	largeRect			= ofxResizeUtil :: cropToSize( smallRect, screenRect );
	largeRect.x			-= largeRect.width  * largeRectExtra * 0.5;
	largeRect.y			-= largeRect.height * largeRectExtra * 0.5;
	largeRect.width		*= 1 + largeRectExtra;
	largeRect.height	*= 1 + largeRectExtra;
	
	debugRect.height	= 150;			// default height.
	debugRect.width		= (int)( ( debugRect.height / sourceRect.height ) * sourceRect.width );
	
	screenGrabUtil.setup( "movie/frame" );
	screenGrabUtil.setPause( true );
	
	tileSaver.init( 10, 0, true );
	
	initBomAnim();
	initAusMap();
	initOpenCv();
	initContours();
	initGui();
	initColor();
}

void testApp :: initBomAnim ()
{
	bomAnim.loadAnimationImages( "temperature_images" );
	
	bomImage.allocate( smallRect.width, smallRect.height );
}

void testApp :: initAusMap ()
{
	ausStroke.loadImage( "aus/aus_886x691_stroke.png" );
	ausMask.loadImage( "aus/aus_886x691_mask.png" );
}

void testApp :: initOpenCv ()
{
	bandsTotal = 10;
	bandIndex	= 0;
	
	bands		= new ofxCvGrayscaleImage[ bandsTotal ];
	bandCutoffs	= new float[ bandsTotal ];
	
	for( int i=0; i<bandsTotal; i++ )
	{
		ofxCvGrayscaleImage& band = bands[ i ];
		band.allocate( smallRect.width, smallRect.height );
		
		float& bandCutoff = bandCutoffs[ i ];
		bandCutoff = 0.1 + ( i / (float)( bandsTotal - 1 ) ) * 0.8;
	}
	
	bandSum.allocate( smallRect.width, smallRect.height );
}

void testApp :: initContours ()
{
	contourSmoothScale			= 0.5;
	contourSimplifyScale		= 0.0;
	contourSimplifyTolerance	= 0.0;
}

void testApp :: initGui ()
{
	gui.setAutoSave( false );
	
	gui.addTitle( "general" );
	
	gui.addToggle( "bDebug  ",			bDebug );
	gui.addToggle( "bSmoothing  ",		bSmoothing );
	gui.addToggle( "bDrawPoints  ",		bDrawPoints );
	gui.addToggle( "bDrawLines  ",		bDrawLines );
	gui.addToggle( "bDrawCurves  ",		bDrawCurves );
	gui.addToggle( "bDrawSimplified  ",	bDrawSimplified );
	gui.addToggle( "bDrawColor  ",		bDrawColor );
	
	gui.addPage( "bands" );
	gui.addTitle( "bands index", 22 );
	gui.addSlider( "bandIndex  ",	bandIndex, 0, bandsTotal - 1 );
	for( int i=0; i<bandsTotal; i++ )
	{
		gui.addTitle( "band " + ofToString( i, 0 ), 22 );
		gui.addSlider( "band " + ofToString( i, 0 ) + " cutoff  ",	bandCutoffs[ i ],	0, 1.0 );
	}
	
	gui.addPage( "contour" );
	gui.addSlider( "contourSmoothScale ",		contourSmoothScale,			0, 1.0 );
	gui.addSlider( "contourSimplifyScale ",		contourSimplifyScale,		0, 50.0 );
	gui.addSlider( "contourSimplifyTolerance ",	contourSimplifyTolerance,	0, 0.1 );
	
	gui.show();
	gui.setPage( 4 );

//	gui.loadFromXML();
}

void testApp :: initColor ()
{
	colorPicker0.setColorRadius( 0.65 );
	colorPicker1.setColorRadius( 1.0 );
	
	colorPicker0.setColorAngle( 3 / 6.0 );
	colorPicker1.setColorAngle( 5 / 6.0 );
	
	colorPicker0.enable();
	colorPicker1.enable();
	
	colorPicker0.setSize( 300, 100, 150, 300 );
	colorPicker1.setSize( 480, 100, 150, 300 );
	
	colorPicker0.addListeners();
	colorPicker1.addListeners();
}

//--------------------------------------------------------------
void testApp::update()
{
	if( bPause )
		return;
	
	if( tileSaver.bGoTiling )
		return;
	
	updateBomAnim();
	updateOpenCv();
	updateBlobs();
	updateColor();
}

void testApp :: updateBomAnim ()
{
	if( ofGetFrameNum() % 10 == 0 )
		bomAnim.nextFrame();
	
	ofxCvGrayscaleImage* image;
	image = bomAnim.getBomImage();
	
	bomImage.scaleIntoMe( *image );
}

void testApp :: updateOpenCv ()
{
	bandSum.set( 0 );
	
	for( int i=0; i<bandsTotal; i++ )
	{
		int bandCutoff;
		bandCutoff	= bandCutoffs[ i ] * 255;
		
		ofxCvGrayscaleImage& band = bands[ i ];
		band		= bomImage;
		band.threshold( bandCutoff, false );
		
		if( !bDebug )
			continue;
		
		int t = smallRect.width * smallRect.height;
		unsigned char* pixels0 = band.getPixels();
		unsigned char* pixels1 = bandSum.getPixels();
		
		for( int j=0; j<t; j++ )
		{
			if( pixels0[ j ] == 255 )
				pixels1[ j ] = bandCutoff;
		}
	}
}

void testApp :: updateBlobs ()
{
	//-- remove old blobs.

	contourBlobs.clear();
	contourBlobsScaled.clear();
	contourBlobsSimplified.clear();
	
	//-- copy blobs and scale.
	
	float scale;
	scale = largeRect.width / (float)smallRect.width;
	
	float xoff;
	xoff = largeRect.x;
	
	float yoff;
	yoff = largeRect.y;

	for( int i=0; i<bandsTotal; i++ )
	{
		ofxCvGrayscaleImage& image = bands[ i ];
		
		int numOfBlobs;
		numOfBlobs = updateContours( image );
		
		if( numOfBlobs == 0 )
			continue;
		
		ofColor c0 = colorPicker0.getColor();
		ofColor c1 = colorPicker1.getColor();
		ofColor c2 = interpolateColor( c0, c1, bandCutoffs[ i ] );
		
		for( int j=0; j<numOfBlobs; j++ )
		{
			ofxCvBlob& blob = contourFinder.blobs[ j ];
			
			contourBlobs.push_back( Blob() );
			contourBlobsScaled.push_back( Blob() );
			contourBlobsSimplified.push_back( Blob() );
			
			copyBlob( blob, contourBlobs.back() );
			copyBlob( blob, contourBlobsScaled.back(), xoff, yoff, scale );
			copyBlob( blob, contourBlobsSimplified.back(), xoff, yoff, scale );
			
			contourBlobs.back().color			= c2;
			contourBlobsScaled.back().color		= c2;
			contourBlobsSimplified.back().color	= c2;

			if( contourSmoothScale > 0 )
			{
				contourUtil.smooth( contourBlobsSimplified.back().pts, contourSmoothScale, 1.0 );
			}

			if( contourSimplifyScale > 0 )
			{
				contourUtil.simplify( contourBlobsSimplified.back().pts, contourSimplifyScale );
			}
			
			if( contourSimplifyTolerance > 0 )
			{
				float tolerance = contourSimplifyTolerance;
				tolerance *= ( 1 / (float)contourBlobsSimplified.back().pts.size() );
				tolerance *= 100;
				
				vector<ofPoint> ptsOut;
				contourSimplify.simplify( contourBlobsSimplified.back().pts, ptsOut, tolerance );
				contourBlobsSimplified.back().pts = ptsOut;
			}
		}
	}
}

int testApp :: updateContours ( ofxCvGrayscaleImage& image )
{
	int maxArea;
	maxArea = smallRect.width * smallRect.height;
	
	float cfMinArea;
	cfMinArea = 0.001;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		image,							// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void testApp :: copyBlob ( ofxCvBlob& blob, ofxCvBlob& blobCopy, float xoff, float yoff, float scale )
{
	for( int k=0; k<blob.pts.size(); k++ )
	{
		ofPoint& p1 = blob.pts[ k ];
		
		blobCopy.pts.push_back( ofPoint() );
		ofPoint& p2 = blobCopy.pts.back();
		
		p2.x = p1.x * scale + xoff;
		p2.y = p1.y * scale + yoff;
	}
	
	blobCopy.area					= blob.area;
	blobCopy.boundingRect.x			= blob.boundingRect.x * scale + xoff;
	blobCopy.boundingRect.y			= blob.boundingRect.y * scale + yoff;
	blobCopy.boundingRect.width		= blob.boundingRect.width  * scale + xoff;
	blobCopy.boundingRect.height	= blob.boundingRect.height * scale + yoff;
	blobCopy.centroid.x				= blob.centroid.x * scale + xoff;
	blobCopy.centroid.y				= blob.centroid.y * scale + yoff;
	blobCopy.hole					= blob.hole;
	blobCopy.length					= blob.length;
	blobCopy.nPts					= blob.nPts;
	
	blobCopy.nPts = blobCopy.pts.size();
}

void testApp :: updateColor ()
{
	if( bDebug )
	{
		colorPicker0.show();
		colorPicker1.show();
	}
	else 
	{
		colorPicker0.hide();
		colorPicker1.hide();
	}

	float angle;
	angle = ( bRotateColor ) ? 0.00025 : 0;

	colorPicker0.setColorAngle( colorPicker0.getColorAngle() + angle );
	colorPicker1.setColorAngle( colorPicker1.getColorAngle() + angle );
}

ofColor testApp :: interpolateColor ( const ofColor& c1, const ofColor& c2, float p )
{
	ofColor c;
	c.r = ( c2.r - c1.r ) * p + c1.r;
	c.g = ( c2.g - c1.g ) * p + c1.g;
	c.b = ( c2.b - c1.b ) * p + c1.b;
	
	return c;
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp::draw()
{
	tileSaver.begin();
	
	//-- background.
	
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	drawBomAnim();
	
	//-- large.
	
	if( bDrawColor )
	{
		ofColor c0 = colorPicker0.getColor();
		
		ofFill();
		ofSetColor( c0.r, c0.g, c0.b );
		ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
		
		drawContourLines( contourBlobsSimplified, true );
	}
	
	if( bDrawLines )
	{
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		drawContourLines( contourBlobsScaled );
	}
	
	if( bDrawSimplified )
	{
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		drawContourLines( contourBlobsSimplified );
	}
	
	if( bDrawCurves )
	{
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		drawContourCurveLines( contourBlobsScaled );
	}
	
	if( bDrawPoints )
	{
		ofFill();
		ofSetColor( 0xFF0000 );
		drawContourPoints( contourBlobsScaled );
	}
	
	tileSaver.end();
	
	if( screenGrabUtil.isRecording() )
		screenGrabUtil.save();
	
	//-- debug.
	
	if( !bDebug )
		return;
	
	int pad;
	pad = 10;
	
	glPushMatrix();
	glTranslatef( ofGetWidth() - debugRect.width - pad, pad, 0 );
	
	drawBorder( debugRect );
	drawBomImage();
	
	glTranslatef( 0, debugRect.height + pad, 0 );
	
	drawBorder( debugRect );
	drawBand();

	glTranslatef( 0, debugRect.height + pad, 0 );
	
	drawBorder( debugRect );
	drawBandSum();

	glTranslatef( 0, debugRect.height + pad, 0 );
	
	drawBorder( debugRect );
	drawContoursSmall();
	
	glPopMatrix();
	
	gui.draw();
}

void testApp :: drawBorder ( const ofRectangle& rect )
{
	int b = 2;
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	ofRect( -b, -b, rect.width + b * 2, rect.height + b * 2 );
}

void testApp :: drawBomAnim ()
{
	ofSetColor( 0xFFFFFF );
	
	float s;
	s = largeRect.width / sourceRect.width;
	
	glPushMatrix();
	glTranslatef( largeRect.x, largeRect.y, 0 );
	glScalef( s, s, 0 );
	bomAnim.draw( 0, 0 );
	glPopMatrix();
}

void testApp :: drawBomImage ()
{
	ofSetColor( 0xFFFFFF );
	bomImage.draw( 0, 0, debugRect.width, debugRect.height );
}

void testApp :: drawBand ()
{
	ofSetColor( 0xFFFFFF );
	ofxCvGrayscaleImage& band = bands[ bandIndex ];
	band.draw( 0, 0, debugRect.width, debugRect.height );
}

void testApp :: drawBandSum ()
{
	ofSetColor( 0xFFFFFF );
	bandSum.draw( 0, 0, debugRect.width, debugRect.height );
}

void testApp :: drawContoursSmall ()
{
	float scale;
	scale = debugRect.width / (float)smallRect.width;
	
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, debugRect.width, debugRect.height );
	
	glScalef( scale, scale, 0 );
	
	ofNoFill();
	ofSetColor( 0xDD00CC );
	drawContourBoundingBoxes( contourBlobs );
	
	ofNoFill();
	ofSetColor( 0x00FFFF );
	drawContourLines( contourBlobs );
}

void testApp :: drawContourBoundingBoxes ( vector<Blob>& blobs )
{
	for( int i=0; i<blobs.size(); i++ )
	{
		ofRect
		(
			blobs[ i ].boundingRect.x,
			blobs[ i ].boundingRect.y,
			blobs[ i ].boundingRect.width,
			blobs[ i ].boundingRect.height
		);
	}
}

void testApp :: drawContourPoints ( vector<Blob>& blobs )
{
	for( int i=0; i<blobs.size(); i++ )
	{
		int t = blobs[ i ].pts.size();
		
		for( int j=0; j<t+1; j++ )		// extra points to close each polygon
		{
			int k = j % t;
			int r = 1;
			
			ofRect( blobs[ i ].pts[ k ].x - r, blobs[ i ].pts[ k ].y - r, r * 2, r * 2 );
		}
	}
}

void testApp :: drawContourLines ( vector<Blob>& blobs, bool useBlobColor )
{
	for( int i=0; i<blobs.size(); i++ )
	{
		if( useBlobColor )
		{
			ofColor& c = blobs[ i ].color;
			ofSetColor( c.r, c.g, c.b );
		}
		
		int t = blobs[ i ].pts.size();
		
		ofBeginShape();
		for( int j=0; j<t+1; j++ )		// extra points to close each polygon
		{
			int k = j % t;
			
			ofVertex( blobs[ i ].pts[ k ].x, blobs[ i ].pts[ k ].y );
		}
		ofEndShape();
	}
}

void testApp :: drawContourCurveLines ( vector<Blob>& blobs, bool useBlobColor )
{
	for( int i=0; i<blobs.size(); i++ )
	{
		if( useBlobColor )
		{
			ofColor& c = blobs[ i ].color;
			ofSetColor( c.r, c.g, c.b );
		}
		
		int t = blobs[ i ].pts.size();
		
		ofBeginShape();
		for( int j=0; j<t+3; j++ )		// extra points to close each polygon
		{
			int k = j % t;
			
			ofCurveVertex( blobs[ i ].pts[ k ].x, blobs[ i ].pts[ k ].y );
		}
		ofEndShape();
	}
}

//--------------------------------------------------------------
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
	
	if( key == 's' )
	{
		char str[255];
		sprintf( str, "image_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		tileSaver.finish( str, false );
	}
	
	if( key == 'm' )
	{
		screenGrabUtil.togglePause();
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'p' )
	{
		bPause = !bPause;
	}
	
	if( key == ' ' )
	{
		bRotateColor = !bRotateColor;
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

