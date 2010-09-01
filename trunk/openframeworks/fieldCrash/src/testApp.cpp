#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	
	bDebug		= true;
	bSmoothing	= false;
	bPause		= false;
	
	screenRect.width	= ofGetWidth();
	screenRect.height	= ofGetHeight();
	
	noiseRect.width		= 320;
	noiseRect.height	= 240;
	
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
}

void testApp :: initNoise ()
{
	noiseVel.x = 0.0;
	noiseVel.y = 0.0;
	noiseVel.z = 0.0;
	
	noiseScl.x = 0.01;
	noiseScl.y = 0.01;
	noiseScl.z = 0.01;
	
	noiseSclMaster = 0.5;
	
	noiseImage.allocate( noiseRect.width, noiseRect.height );
}

void testApp :: initOpenCv ()
{
	noiseBandsTotal = 10;
	noiseBandIndex	= 0;
	
	noiseBands			= new ofxCvGrayscaleImage[ noiseBandsTotal ];
	noiseBandCutoffs	= new float[ noiseBandsTotal ];
	noiseBandSize		= new float[ noiseBandsTotal ];
	
	for( int i=0; i<noiseBandsTotal; i++ )
	{
		ofxCvGrayscaleImage& noiseBand = noiseBands[ i ];
		noiseBand.allocate( noiseRect.width, noiseRect.height );
		
		float& noiseBandCutoff = noiseBandCutoffs[ i ];
		noiseBandCutoff = 0.1 + ( i / (float)( noiseBandsTotal - 1 ) ) * 0.8;
		
		noiseBandSize[ i ] = 0.01;
	}
	
	noiseBandSum.allocate( noiseRect.width, noiseRect.height );
}

void testApp :: initContours ()
{
	contourSmooth	= 1.0;
	contourSimplify	= 5.5f;
}

void testApp :: initGui ()
{
	gui.setAutoSave( false );
	
	gui.addTitle( "general" );
	
	gui.addToggle( "bDebug  ",			bDebug );
	gui.addToggle( "bSmoothing  ",		bSmoothing );
	
	gui.addPage( "noise" );
	
	gui.addSlider( "noiseVel.x  ",		noiseVel.x, -1.0, 1.0 );
	gui.addSlider( "noiseVel.y  ",		noiseVel.y, -1.0, 1.0 );
	gui.addSlider( "noiseVel.z  ",		noiseVel.z, -1.0, 1.0 );

	gui.addSlider( "noiseScl.x  ",		noiseScl.x, 0, 1.0 );
	gui.addSlider( "noiseScl.y  ",		noiseScl.y, 0, 1.0 );
	gui.addSlider( "noiseScl.z  ",		noiseScl.z, 0, 1.0 );
	
	gui.addSlider( "noiseSclMaster  ",	noiseSclMaster, 0, 1.0 );
	
	gui.addPage( "bands" );
	for( int i=0; i<noiseBandsTotal; i++ )
	{
		gui.addSlider( "band " + ofToString( i, 0 ) + " cutoff  ",	noiseBandCutoffs[ i ],	0, 1.0 );
		gui.addSlider( "band " + ofToString( i, 0 ) + " size  ",	noiseBandSize[ i ],		0, 0.1 );
	}
	gui.addSlider( "noiseBandIndex  ",	noiseBandIndex, 0, noiseBandsTotal - 1 );
	
	gui.addPage( "contour" );
	gui.addSlider( "contourSmooth  ",	contourSmooth,		0, 1.0 );
	gui.addSlider( "contourSimplify  ",	contourSimplify,	0, 50.0 );
	
	gui.show();
	gui.setPage( 3 );

	gui.loadFromXML();
}

//--------------------------------------------------------------
void testApp::update()
{
	if( bPause )
		return;
	
	noiseLoc += noiseVel;
	
	updateNoiseImage();
	updateOpenCv();

	int blobs;
	blobs = updateContours();
	
	if( blobs > 0 )
	{
		copyAndScaleBlobs();
	}
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
	
	ofxCvGrayscaleImage noiseBandTop;
	ofxCvGrayscaleImage noiseBandBtm;
	
	noiseBandTop.allocate( noiseRect.width, noiseRect.height );
	noiseBandBtm.allocate( noiseRect.width, noiseRect.height );
	
	for( int i=0; i<noiseBandsTotal; i++ )
	{
		int bandCutoff;
		bandCutoff = noiseBandCutoffs[ i ] * 255;
		
		int bandSize;
		bandSize = noiseBandSize[ i ] * 255;
		
		noiseBandTop = noiseImage;
		noiseBandBtm = noiseImage;
		
		noiseBandTop.threshold( bandCutoff + bandSize, true );
		noiseBandBtm.threshold( bandCutoff - bandSize, true );
		
		noiseBandTop -= noiseBandBtm;
		
		ofxCvGrayscaleImage& noiseBand = noiseBands[ i ];
		noiseBand = noiseBandTop;

		noiseBandSum += noiseBand;
	}
	
	noiseBandTop.clear();
	noiseBandBtm.clear();
}

int testApp :: updateContours ()
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
		noiseBandSum,					// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void testApp :: copyAndScaleBlobs ()
{
	//-- remove old blobs.
	
	for( int i=0; i<contourBlobsScaled.size(); i++ )
	{
		ofxCvBlob& blob = contourBlobsScaled[ i ];
		blob.pts.clear();
	}
	contourBlobsScaled.clear();
	
	//-- copy blobs and scale.
	
	float scale;
	scale = largeRect.width / (float)noiseRect.width;
	
	float xoff;
	xoff = largeRect.x;
	
	float yoff;
	yoff = largeRect.y;
	
	vector<ofxCvBlob>& blobs = contourFinder.blobs;
	
	for( int i=0; i<blobs.size(); i++ )
	{
		ofxCvBlob& blob = blobs[ i ];
		
		contourBlobsScaled.push_back( ofxCvBlob() );
		ofxCvBlob& blobCopy = contourBlobsScaled.back();

		for( int j=0; j<blob.pts.size(); j++ )
		{
			ofPoint& p1 = blob.pts[ j ];
			
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
		
		contourUtil.smooth( blobCopy.pts,	contourSmooth );
		contourUtil.simplify( blobCopy.pts, contourSimplify );
		
		blobCopy.nPts					= blobCopy.pts.size();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	drawContoursLarge();

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
	drawContourBoundingBoxes( contourFinder.blobs );
	
	ofNoFill();
	ofSetColor( 0x00FFFF );
	drawContourLines( contourFinder.blobs );
}

void testApp :: drawContoursLarge ()
{
//	ofNoFill();
//	ofSetColor( 0xDD00CC );
//	drawContourBoundingBoxes( contourBlobsScaled );
	
	ofNoFill();
	ofSetColor( 0xFFFFFF );
//	drawContourLines( contourBlobsScaled );
	drawContourCurveLines( contourBlobsScaled );
	drawContourPoints( contourBlobsScaled );
}

void testApp :: drawContourBoundingBoxes ( vector<ofxCvBlob>& blobs )
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

void testApp :: drawContourPoints ( vector<ofxCvBlob>& blobs )
{
	ofFill();
	ofSetColor( 0xFF0000 );
	
	for( int i=0; i<blobs.size(); i++ )
	{
		int t = blobs[ i ].pts.size();
		
		for( int j=0; j<t+1; j++ )		// extra points to close each polygon
		{
			int k = j % t;
			
			ofCircle( blobs[ i ].pts[ k ].x, blobs[ i ].pts[ k ].y, 1 );
		}
	}
}

void testApp :: drawContourLines ( vector<ofxCvBlob>& blobs )
{
	for( int i=0; i<blobs.size(); i++ )
	{
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

void testApp :: drawContourCurveLines ( vector<ofxCvBlob>& blobs )
{
	for( int i=0; i<blobs.size(); i++ )
	{
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

