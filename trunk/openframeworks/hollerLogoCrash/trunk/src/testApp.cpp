#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 25 );
	
	logo.loadImage( "holler_logo.png" );
	logo.setImageType( OF_IMAGE_GRAYSCALE );
	
	logoPixles = logo.getPixels();
	
	int logoPixelsTotal;
	logoPixelsTotal = logo.width * logo.height;
	
	float pixelThreshold;
	pixelThreshold = 0.5;
	
	logoBWPixles = new unsigned char[ logoPixelsTotal ];
	
	for( int i=0; i<logoPixelsTotal; i++ )		// clean up image.
	{
		if( logoPixles[ i ] < (int)( pixelThreshold * 255 ) )
		{
			logoBWPixles[ i ] = 255;
		}
		else 
		{
			logoBWPixles[ i ] = 0;
		}
	}
	
	logoBW.allocate( logo.width, logo.height );
	logoBW.setFromPixels( logoBWPixles, logo.width, logo.height );
	
	bShowImage = true;
	
	initContourAnalysis();
	computeContourAnalysis();
	parseLogoShapes();
}

///////////////////////////////////////////
//	contour analysis.
///////////////////////////////////////////

void testApp :: initContourAnalysis ()
{
	maxContoursToFind		= 20;
	
	cdata.nBlobs			= 0;
	cdata.box				= new CvBox2D32f[ maxContoursToFind ];
	cdata.blobAngle			= new float[ maxContoursToFind ];
	cdata.contourReg		= new vector<ofxPoint2f>[ maxContoursToFind ];
	cdata.contourSmooth		= new vector<ofxPoint2f>[ maxContoursToFind ];
	cdata.contourSimple		= new vector<ofxPoint2f>[ maxContoursToFind ];
	cdata.contourHull		= new vector<ofxPoint2f>[ maxContoursToFind ];
	
	smoothPct		= 0.13f;
	tolerance		= 20.0f;
	
	cfMinArea		= 0.001;	// do not go to zero, shit will get weird.
	cfDetail		= 0;
	
	bShowRect		= true;
	bShowBox		= true;
	bShowEllipse	= false;
	bShowAngle		= false;
	bShowLines		= false;
}

void testApp :: computeContourAnalysis ()
{
	int maxArea;
	maxArea = logoBW.width * logoBW.height;
	
	int runningBlobs;
	runningBlobs = contourFinder.findContours
	(
		logoBW,							// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	if( runningBlobs == 0 )
	{
		cdata.nBlobs = 0;
		
		return;
	}
	
	int nBlobs;
	nBlobs = contourFinder.nBlobs;
	
	cdata.nBlobs = nBlobs;
	
	cdata.blobArea.clear();
	cdata.blobArea.assign( nBlobs, 1.0f );
	cdata.blobBoundingRect.clear();
	cdata.blobBoundingRect.assign( nBlobs, ofRectangle() );
	cdata.blobCentroid.clear();
	cdata.blobCentroid.assign( nBlobs, ofPoint() );
	
	for( int j=0; j<nBlobs; j++ )
	{
		cdata.blobArea.at( j )					= contourFinder.blobs[ j ].area;
		
		cdata.blobBoundingRect.at( j ).x		= contourFinder.blobs[ j ].boundingRect.x;
		cdata.blobBoundingRect.at( j ).y		= contourFinder.blobs[ j ].boundingRect.y;
		cdata.blobBoundingRect.at( j ).width	= contourFinder.blobs[ j ].boundingRect.width;
		cdata.blobBoundingRect.at( j ).height	= contourFinder.blobs[ j ].boundingRect.height;
		
		cdata.blobCentroid.at( j ).x			= contourFinder.blobs[ j ].centroid.x;
		cdata.blobCentroid.at( j ).y			= contourFinder.blobs[ j ].centroid.y;
		
		int contourLength;
		contourLength = contourFinder.blobs[ j ].pts.size();
		
//		fitEllipse
//		(
//			contourFinder.blobs[ j ].pts,
//			cdata.box[ j ]
//		);
		
//		cdata.blobAngle[ j ] = getOrientation( contourFinder.blobs[ j ].pts );
		
//		findLines
//		(
//			contourFinder.blobs[ j ].pts,	// blob points.
//			cdata.geomLines,			// lines vector.
//			30,								// angle threshold.
//			40,								// minimum length.
//			30								// resolution.
//		);
		
		cdata.contourReg[ j ].clear();
		cdata.contourReg[ j ].assign( contourLength, ofxPoint2f() );
		cdata.contourSmooth[ j ].clear();
		cdata.contourSmooth[ j ].assign( contourLength, ofxPoint2f() );
		cdata.contourSimple[ j ].clear();
		cdata.contourHull[ j ].clear();
		
		for( int k=0; k<contourLength; k++ )
		{
			cdata.contourReg[ j ].at( k ) = contourFinder.blobs[ j ].pts[ k ];
		}
		
		contourS.smooth( cdata.contourReg[ j ], cdata.contourSmooth[ j ], smoothPct );
		contourS.simplify( cdata.contourSmooth[ j ], cdata.contourSimple[ j ], tolerance );
		contourS.convexHull( cdata.contourSmooth[ j ], cdata.contourHull[ j ] );
	}
}

void testApp :: drawContourAnalysis()
{
	for( int j=0; j<cdata.nBlobs; j++ )
	{
		if( cfDetail == 0 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourReg[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourReg[ j ].at( k ).x,
					cdata.contourReg[ j ].at( k ).y
				);
			}
			ofEndShape( true );
		}
		else if( cfDetail == 1 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourSmooth[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourSmooth[ j ].at( k ).x,
					cdata.contourSmooth[ j ].at( k ).y
				);
			}
			ofEndShape(true);
		}
		else if( cfDetail == 2 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourSimple[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourSimple[ j ].at( k ).x,
					cdata.contourSimple[ j ].at( k ).y
				);
			}
			ofEndShape(true);
		}
		else if( cfDetail == 3 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourHull[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourHull[ j ].at( k ).x,
					cdata.contourHull[ j ].at( k ).y
				);
			}
			ofEndShape(true);
			
			for( int k=0; k<cdata.contourHull[ j ].size(); k++ )
			{
				ofCircle
				(
					cdata.contourHull[ j ].at( k ).x,
					cdata.contourHull[ j ].at( k ).y,
					2
				);
			}
		}
		
		if( bShowRect )
		{
			ofNoFill();
			ofSetColor( 0xFF00FF );
			
			ofRect
			(
				cdata.blobBoundingRect[ j ].x,
				cdata.blobBoundingRect[ j ].y,
				cdata.blobBoundingRect[ j ].width,
				cdata.blobBoundingRect[ j ].height
			);
		}
		
		continue;
		
		if( bShowBox )
		{
			ofSetRectMode( OF_RECTMODE_CENTER );
			
			ofNoFill();
			ofSetColor( 0xFFFFFF );
			
			glPushMatrix();
			glTranslatef
			(
				cdata.box[ j ].center.x,
				cdata.box[ j ].center.y,
				0
			);
			glRotatef( cdata.box[ j ].angle, 0, 0, 1 );
			ofRect
			(
				0,
				0,
				cdata.box[ j ].size.width,
				cdata.box[ j ].size.height
			);
			glPopMatrix();
			
			ofSetRectMode( OF_RECTMODE_CORNER );
		}
		
		if( bShowEllipse )
		{
			ofNoFill();
			ofSetColor( 0xFFFFFF );
			
			glPushMatrix();
			glTranslatef
			(
				cdata.box[ j ].center.x,
				cdata.box[ j ].center.y,
				0
			);
			glRotatef( cdata.box[ j ].angle, 0, 0, 1 );
			ofEllipse
			(
				0,
				0,
				cdata.box[ j ].size.width  * 0.5,
				cdata.box[ j ].size.height * 0.5
			);
			glPopMatrix();
		}
		
		if( bShowAngle )
		{
			ofSetColor( 0xFFFFFF );
			
			float x1,y1,x2,y2;
			
			x1 = cdata.blobCentroid[ j ].x + 25 * cos( cdata.blobAngle[ j ] );
			y1 = cdata.blobCentroid[ j ].y + 25 * sin( cdata.blobAngle[ j ] );
			x2 = cdata.blobCentroid[ j ].x - 25 * cos( cdata.blobAngle[ j ] );
			y2 = cdata.blobCentroid[ j ].y - 25 * sin( cdata.blobAngle[ j ] );
			
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
			
			x1 = cdata.blobCentroid[ j ].x + 10 * cos( cdata.blobAngle[ j ] + HALF_PI );
			y1 = cdata.blobCentroid[ j ].y + 10 * sin( cdata.blobAngle[ j ] + HALF_PI );
			x2 = cdata.blobCentroid[ j ].x - 10 * cos( cdata.blobAngle[ j ] + HALF_PI );
			y2 = cdata.blobCentroid[ j ].y - 10 * sin( cdata.blobAngle[ j ] + HALF_PI );
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
		}
		
		if( bShowLines )
		{
			ofSetColor( 0x8aff00 );
			ofNoFill();
			for( int k=0; k<cdata.geomLines.size(); k++ )
			{
				ofLine
				(
					cdata.geomLines[ k ].x,
					cdata.geomLines[ k ].y,
					cdata.geomLines[ k ].z,
					cdata.geomLines[ k ].w
				);
				ofCircle
				(
					cdata.geomLines[ k ].x,
					cdata.geomLines[ k ].y,
					3
				);
				ofCircle
				(
					cdata.geomLines[ k ].z,
					cdata.geomLines[ k ].w,
					3
				);
			}
		}
	}
}

void testApp :: parseLogoShapes ()
{
	int t;
	t = cdata.nBlobs;
	
	for( int i=0; i<t; i++ )				// initialise shapes.
	{
		shapes.push_back( LogoShape() );
		
		LogoShape& s	= shapes.back();
		s.parent		= -1;
		s.child			= -1;
		s.noPolys		= 1;
	}
	
	for( int i=0; i<t; i++ )				// check if shapes are embedded - left to right.
	{
		ofRectangle r1;
		r1.x		= cdata.blobBoundingRect[ i ].x;
		r1.y		= cdata.blobBoundingRect[ i ].y;
		r1.width	= cdata.blobBoundingRect[ i ].width;
		r1.height	= cdata.blobBoundingRect[ i ].height;
		
		LogoShape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i + j ) % t;
			
			ofRectangle r2;
			r2.x		= cdata.blobBoundingRect[ k ].x;
			r2.y		= cdata.blobBoundingRect[ k ].y;
			r2.width	= cdata.blobBoundingRect[ k ].width;
			r2.height	= cdata.blobBoundingRect[ k ].height;
			
			LogoShape& s2 = shapes.at( k );
			
			bool embedded = false;
			embedded = checkEmbeddedRectangles( r1, r2 );
			
			if( embedded )
			{
				s1.child	= k;
				s2.parent	= i;
			}
		}
	}

	for( int i=t-1; i>=0; i-- )				// check if shapes are embedded - right to left.
	{
		ofRectangle r1;
		r1.x		= cdata.blobBoundingRect[ i ].x;
		r1.y		= cdata.blobBoundingRect[ i ].y;
		r1.width	= cdata.blobBoundingRect[ i ].width;
		r1.height	= cdata.blobBoundingRect[ i ].height;
		
		LogoShape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i - j );
			
			if( k < 0 )
				k = t + k;
			
			ofRectangle r2;
			r2.x		= cdata.blobBoundingRect[ k ].x;
			r2.y		= cdata.blobBoundingRect[ k ].y;
			r2.width	= cdata.blobBoundingRect[ k ].width;
			r2.height	= cdata.blobBoundingRect[ k ].height;
			
			LogoShape& s2 = shapes.at( k );
			
			bool embedded;
			embedded = checkEmbeddedRectangles( r1, r2 );
			
			if( embedded )
			{
				s1.child	= k;
				s2.parent	= i;
			}
		}
	}
	
	for( int i=0; i<t; i++ )			// copy child data into parent.
	{
		LogoShape& s1 = shapes.at( i );
		
		bool hasChild;
		hasChild = ( s1.child > -1 );
		
		if( hasChild )
		{
			s1.noPolys += 1;
		}
		
		s1.polyPoints = new vector<ofxPoint2f>[ s1.noPolys ];
		
		copyPolygonData( cdata.contourReg[ i ], s1.polyPoints[ 0 ] );
		
		if( hasChild )
		{
			copyPolygonData( cdata.contourReg[ s1.child ], s1.polyPoints[ 1 ] );
		}
	}
	
//	ofSetColor( 0xFF0000 );
//	ofNoFill();
//	ofBeginShape();
//	for( int k=0; k<cdata.contourReg[ j ].size(); k++ )
//	{
//		ofVertex
//		(
//		 cdata.contourReg[ j ].at( k ).x,
//		 cdata.contourReg[ j ].at( k ).y
//		 );
//	}
//	ofEndShape( true );
//	
//	ofRect
//	(
//	 cdata.blobBoundingRect[ j ].x,
//	 cdata.blobBoundingRect[ j ].y,
//	 cdata.blobBoundingRect[ j ].width,
//	 cdata.blobBoundingRect[ j ].height
//	 );
}

bool testApp :: checkEmbeddedRectangles( const ofRectangle& r1, const ofRectangle& r2 )
{
	bool p1, p2, p3, p4;
	
	p1 = ( ( r2.x >= r1.x ) && ( r2.y >= r1.y ) );
	p2 = ( ( r2.x + r2.width <= r1.x + r1.width ) && ( r2.y >= r1.y ) );
	p3 = ( ( r2.x + r2.width <= r1.x + r1.width ) && ( r2.y + r2.height <= r1.y + r1.height ) );
	p4 = ( ( r2.x >= r1.x ) && ( r2.y + r2.height <= r1.y + r1.height ) );
	
	return ( p1 && p2 && p3 && p4 );
}

void testApp :: copyPolygonData ( const vector<ofxPoint2f>& p1, vector<ofxPoint2f>& p2 )
{
	for( int i=0; i<p1.size(); i++ )
	{
		p2.push_back( ofxPoint2f() );
		
		ofxPoint2f& p = p2.back();
		p.x = p1.at( i ).x;
		p.y = p1.at( i ).y;
	}
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{

}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	if( bShowImage )
	{
		ofSetColor( 0xFFFFFF );
		logoBW.draw( 0, 0 );
	}
	
	drawContourAnalysis();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == 'i' )
	{
		bShowImage = !bShowImage;
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

