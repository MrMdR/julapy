#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	
	bDebug			= true;
	bSmoothing		= false;
	bPause			= false;
	bDrawPoints		= true;
	bDrawLines		= false;
	bDrawCurves		= false;
	bDrawSimplified	= true;
	bDrawColor		= true;
	
	screenRect.width	= ofGetWidth();
	screenRect.height	= ofGetHeight();

	switch ( 1 )
	{
		case 1 :
			noiseRect.width		= 160;
			noiseRect.height	= 120;
		break;

		case 2 :
			noiseRect.width		= 320;
			noiseRect.height	= 240;
		break;
			
		case 3 :
			noiseRect.width		= 640;
			noiseRect.height	= 480;
		break;
	}
	
	debugRect.width		= 160;
	debugRect.height	= 120;
	
	largeRect			= ofxResizeUtil :: cropToSize( noiseRect, screenRect );
	largeRect.x			= (int)largeRect.x;
	largeRect.y			= (int)largeRect.y;
	largeRect.width		= (int)largeRect.width;
	largeRect.height	= (int)largeRect.height;
	
	initNoise();
	initOpenCv();
	initContours();
	initGui();
	initColor();
}

void testApp :: initNoise ()
{
	noiseVel.x = 0.1;
	noiseVel.y = 0.0;
	noiseVel.z = 0.0;
	
	noiseScl.x = 0.03;
	noiseScl.y = 0.03;
	noiseScl.z = 0.03;
	
	noiseSclMaster = 0.5;
	
	noiseImage.allocate( noiseRect.width, noiseRect.height );
}

void testApp :: initOpenCv ()
{
	noiseBandsTotal = 10;
	noiseBandIndex	= 0;
	
	noiseBands			= new ofxCvGrayscaleImage[ noiseBandsTotal ];
	noiseBandCutoffs	= new float[ noiseBandsTotal ];
	
	for( int i=0; i<noiseBandsTotal; i++ )
	{
		ofxCvGrayscaleImage& noiseBand = noiseBands[ i ];
		noiseBand.allocate( noiseRect.width, noiseRect.height );
		
		float& noiseBandCutoff = noiseBandCutoffs[ i ];
		noiseBandCutoff = 0.1 + ( i / (float)( noiseBandsTotal - 1 ) ) * 0.8;
	}
	
	noiseBandSum.allocate( noiseRect.width, noiseRect.height );
}

void testApp :: initContours ()
{
	contourSmoothScale			= 0.0;
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
	
	gui.addPage( "noise" );
	
	gui.addSlider( "noiseVel.x  ",		noiseVel.x, -1.0, 1.0, true );
	gui.addSlider( "noiseVel.y  ",		noiseVel.y, -1.0, 1.0, true );
	gui.addSlider( "noiseVel.z  ",		noiseVel.z, -1.0, 1.0, true );

	gui.addSlider( "noiseScl.x  ",		noiseScl.x, 0, 1.0, true );
	gui.addSlider( "noiseScl.y  ",		noiseScl.y, 0, 1.0, true );
	gui.addSlider( "noiseScl.z  ",		noiseScl.z, 0, 1.0, true );
	
	gui.addSlider( "noiseSclMaster  ",	noiseSclMaster, 0, 1.0, true );
	
	gui.addPage( "bands" );
	gui.addTitle( "bands index", 22 );
	gui.addSlider( "noiseBandIndex  ",	noiseBandIndex, 0, noiseBandsTotal - 1 );
	for( int i=0; i<noiseBandsTotal; i++ )
	{
		gui.addTitle( "band " + ofToString( i, 0 ), 22 );
		gui.addSlider( "band " + ofToString( i, 0 ) + " cutoff  ",	noiseBandCutoffs[ i ],	0, 1.0 );
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
	colorPicker0.setColorRadius( 1.0 );
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
	
	noiseLoc += noiseVel;
	
	updateNoiseImage();
	updateOpenCv();
	updateBlobs();
	updateColor();
}

ofPoint testApp :: getNoiseAtPoint( const ofPoint& point )
{
	float p = getNoiseAtPoint( point.x, point.y );
	
	ofxVec2f v = ofxVec2f( 0, 1 );
	v.rotate( p * 720 );
	
	return ofPoint( v.x, v.y );
}

float testApp :: getNoiseAtPoint( float x, float y )
{
	float p = noise.noiseuf
	(
		( x + noiseLoc.x ) * noiseScl.x * noiseSclMaster,
		( y + noiseLoc.y ) * noiseScl.y * noiseSclMaster,
		noiseLoc.z * noiseScl.z
	);
	
	float m;
	m = 2;
	
	p *= m;
	p -= ( m - 1 ) * 0.5;
	p = MAX( MIN( p, 1.0 ), 0.0 );
	
	return p;
}

void testApp :: updateNoiseImage ()
{
	unsigned char * pixels;
	pixels = new unsigned char[ (int)( noiseRect.width * noiseRect.height ) ];

	for( int y=0; y<noiseRect.height; y++ )
	{
		for( int x=0; x<noiseRect.width; x++ )
		{
			int p;
			p = ( y * noiseRect.width ) + x;
			
			float n;
			n = getNoiseAtPoint( x, y );
			n *= 255;
			
			pixels[ p ] = (int)n;
		}
	}
	
	noiseImage.setFromPixels( pixels, noiseRect.width, noiseRect.height );
	
	delete [] pixels;
}

void testApp :: updateOpenCv ()
{
	noiseBandSum.set( 0 );
	
	for( int i=0; i<noiseBandsTotal; i++ )
	{
		int bandCutoff;
		bandCutoff	= noiseBandCutoffs[ i ] * 255;
		
		ofxCvGrayscaleImage& noiseBand = noiseBands[ i ];
		noiseBand		= noiseImage;
		noiseBand.threshold( bandCutoff, false );
		
		if( !bDebug )
			continue;
		
		int t = noiseRect.width * noiseRect.height;
		unsigned char* pixels0 = noiseBand.getPixels();
		unsigned char* pixels1 = noiseBandSum.getPixels();
		
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
	scale = largeRect.width / (float)noiseRect.width;
	
	float xoff;
	xoff = largeRect.x;
	
	float yoff;
	yoff = largeRect.y;

	for( int i=0; i<noiseBandsTotal; i++ )
	{
		ofxCvGrayscaleImage& image = noiseBands[ i ];
		
		int numOfBlobs;
		numOfBlobs = updateContours( image );
		
		if( numOfBlobs == 0 )
			continue;
		
		ofColor c0 = colorPicker0.getColor();
		ofColor c1 = colorPicker1.getColor();
		ofColor c2 = interpolateColor( c0, c1, noiseBandCutoffs[ i ] );
		
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
	maxArea = noiseRect.width * noiseRect.height;
	
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
	angle = 0.001;

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

//--------------------------------------------------------------
void testApp::draw()
{
	//-- background.
	
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	//-- large.
	
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
	
	if( bDrawColor )
	{
		ofFill();
		drawContourLines( contourBlobsSimplified, true );
	}
	
	//-- debug.
	
	if( !bDebug )
		return;
	
	int pad;
	pad = 10;
	
	glPushMatrix();
	glTranslatef( ofGetWidth() - debugRect.width - pad, pad, 0 );
	
	drawBorder( debugRect );
	drawNoiseImage();
	
	glTranslatef( 0, debugRect.height + pad, 0 );
	
	drawBorder( debugRect );
	drawNoiseBand();

	glTranslatef( 0, debugRect.height + pad, 0 );
	
	drawBorder( debugRect );
	drawNoiseBandSum();

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

void testApp :: drawNoiseImage ()
{
	ofSetColor( 0xFFFFFF );
	noiseImage.draw( 0, 0, debugRect.width, debugRect.height );
}

void testApp :: drawNoiseBand ()
{
	ofSetColor( 0xFFFFFF );
	ofxCvGrayscaleImage& noiseBand = noiseBands[ noiseBandIndex ];
	noiseBand.draw( 0, 0, debugRect.width, debugRect.height );
}

void testApp :: drawNoiseBandSum ()
{
	ofSetColor( 0xFFFFFF );
	noiseBandSum.draw( 0, 0, debugRect.width, debugRect.height );
}

void testApp :: drawContoursSmall ()
{
	float scale;
	scale = debugRect.width / (float)noiseRect.width;
	
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
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'p' )
	{
		bPause = !bPause;
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

