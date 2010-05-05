#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 25 );
	
	screenGrab.setup( "movies/" );
//	screenGrab.setPause( false );
	
	bUseCamera = true;
	
	initCamera();
	initLogo();
	initCv();
	initContours();
	initBox2d();
}

void testApp :: initCamera ()
{
	cameraRect.width	= 640;
	cameraRect.height	= 480;
	
	if( bUseCamera )
	{
		camera.setVerbose( true );
		camera.initGrabber( cameraRect.width, cameraRect.height );
	}
	else
	{
		video.loadMovie( "input_video.mov" );
		
		if( !screenGrab.isRecording() )
			video.play();
	}

}

void testApp :: initLogo ()
{
	logo.loadImage( "holler_logo_med.png" );
	logo.setImageType( OF_IMAGE_GRAYSCALE );
	
	logoPixles = logo.getPixels();
	
	int logoPixelsTotal;
	logoPixelsTotal = logo.width * logo.height;
	
	float pixelThreshold;
	pixelThreshold = 0.5;
	
	logoBWPixles = new unsigned char[ logoPixelsTotal ];
	
	for( int i=0; i<logoPixelsTotal; i++ )		// clean up image.
	{
		if( logoPixles[ i ] > (int)( pixelThreshold * 255 ) )
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
	
	logo.loadImage( "holler_logo_med_alpha.png" );
	
	logoCropRect.width	= (int)( logo.width * ( cameraRect.height / (float)cameraRect.width ) );
	logoCropRect.height	= (int)( logo.height );
	logoCropRect.x		= (int)( ( ofGetWidth()  - logoCropRect.width  ) * 0.5 );
	logoCropRect.y		= (int)( ( ofGetHeight() - logoCropRect.height ) * 0.5 );
	
	ofxCvGrayscaleImage logoCopy;
	logoCopy.allocate( logoBW.width, logoBW.height );
	logoCopy = logoBW;
	logoCopy.setROI( logoCropRect );
	
	ofxCvGrayscaleImage logoCrop;
	logoCrop.allocate( logoCropRect.width, logoCropRect.height );
	logoCrop.setFromPixels( logoCopy.getRoiPixels(), logoCropRect.width, logoCropRect.height );
	
	logoSmall.allocate( cameraRect.width, cameraRect.height );
	logoSmall.scaleIntoMe( logoCrop );
	
	logoSmallIntersect.allocate( cameraRect.width, cameraRect.height );
	logoLargeIntersect.allocate( logo.width, logo.height );
	logoLargeIntersectCrop.allocate( logoCropRect.width, logoCropRect.height );
}

void testApp :: initCv ()
{
	cameraColorImage.allocate( cameraRect.width, cameraRect.height );
	cameraGrayImage.allocate( cameraRect.width, cameraRect.height );
	cameraGrayPrevImage.allocate( cameraRect.width, cameraRect.height );
	cameraGrayDiffImage.allocate( cameraRect.width, cameraRect.height );
}

void testApp :: initContours ()
{
	//
}

void testApp :: initBox2d ()
{
	box2d.init();
	box2d.setGravity( 0, 10 );
	box2d.setFPS( 30.0 );
	
	int thick;
	thick = 1;
	
	float wallMass		= 1.0;
	float wallBounce	= 0.5;
	float wallFriction	= 0.3;
	
	float fw;
	fw = 0.5;
	
	int sw, sh;
	sw = ofGetWidth() * 0.5;
	sh = ofGetHeight();
	
	int t = 10;
	
	floor.setPhysics( 0, 0, 0 );			// fixed.
	floor.addVertex( -sw * 0.5, t );
	floor.addVertex(  0, -t );
	floor.addVertex(  sw * 0.5, t );
	floor.createShape( box2d.getWorld(), ofGetWidth() * 0.5, ofGetHeight() + t );
	
//	ofxBox2dRect floor;
//	floor.setPhysics( wallMass, wallBounce, wallFriction );
//	floor.setup
//	(
//		box2d.getWorld(),
//		ofGetWidth() * ( 1.0 - fw ) * 0.5,
//		ofGetHeight(),
//		ofGetWidth() * fw,
//		thick,
//		true
//	);
	
//	ceil.setPhysics( wallMass, wallBounce, wallFriction );
//	ceil.setup( box2d.getWorld(), 0, 0, ofGetWidth(), thick, true );
	
//	left.setPhysics( wallMass, wallBounce, wallFriction );
//	left.setup( box2d.getWorld(), 0, 0, thick, ofGetHeight(), true );
//	
//	right.setPhysics( wallMass, wallBounce, wallFriction );
//	right.setup( box2d.getWorld(), ofGetWidth(), 0, thick, ofGetHeight(), true );
}


//--------------------------------------------------------------
void testApp::update()
{
	updateCamera();
	
	if( cameraNewFrame )
	{
		if( bUseCamera )
		{
			updateCv( camera.getPixels() );
		}
		else
		{
			updateCv( video.getPixels() );
		}

		
		if( ofGetFrameNum() % 3 == 0 )
		{
			updateContours();
		
			clearShapes();
			parseShapes();
		
			updateTriangles();
			addTrianglesToBox2d();
		}
	}
	
	box2d.update();
}

void testApp :: updateCamera ()
{
	if( bUseCamera )
	{
		camera.grabFrame();
		cameraNewFrame = camera.isFrameNew();
	}
	else 
	{
		if( screenGrab.isRecording() )
		{
			float t;
			t = ofGetFrameNum() / 25.0;
			
			float p;
			p = t / video.getDuration();
			
			video.setPosition( p );
		}
		
		cameraNewFrame = true;
	}
}

void testApp :: updateCv ( unsigned char *pixels )
{
	cameraColorImage.setFromPixels( pixels, cameraRect.width, cameraRect.height );
	cameraColorImage.mirror( false, true );
	
	cameraGrayPrevImage	= cameraGrayImage;
	cameraGrayImage		= cameraColorImage;
	
	cameraGrayDiffImage.absDiff( cameraGrayImage, cameraGrayPrevImage );
	cameraGrayDiffImage.threshold( 30 );
	
	logoSmallIntersect  = cameraGrayDiffImage;
	logoSmallIntersect &= logoSmall;
	
	logoLargeIntersectCrop.scaleIntoMe( logoSmallIntersect, CV_INTER_CUBIC );
	
	logoLargeIntersect.setAnchorPoint( -logoCropRect.x, -logoCropRect.y );
	logoLargeIntersect.setRoiFromPixels( logoLargeIntersectCrop.getPixels(), logoCropRect.width, logoCropRect.height );
}

void testApp :: updateContours ()
{
	int maxArea;
	maxArea = logoBW.width * logoBW.height;
	
	float cfMinArea;
	cfMinArea = 0.003;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		logoLargeIntersect,				// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		false,							// find holes.
		false							// use approximation.
	);
}

void testApp :: parseShapes ()
{
	int t;
	t = contourFinder.nBlobs;
	
	if( t == 0 )
		return;
	
	for( int i=0; i<t; i++ )				// initialise shapes.
	{
		Shape s = Shape();
		
		s.parent	= -1;
		s.child		= -1;
		s.noPolys	= 1;
		
		shapes.push_back( s );
	}
	
	for( int i=0; i<t; i++ )				// check if shapes are embedded - left to right.
	{
		ofRectangle r1;
		r1.x		= contourFinder.blobs[ i ].boundingRect.x;
		r1.y		= contourFinder.blobs[ i ].boundingRect.y;
		r1.width	= contourFinder.blobs[ i ].boundingRect.width;
		r1.height	= contourFinder.blobs[ i ].boundingRect.height;
		
		Shape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i + j ) % t;
			
			ofRectangle r2;
			r2.x		= contourFinder.blobs[ k ].boundingRect.x;
			r2.y		= contourFinder.blobs[ k ].boundingRect.y;
			r2.width	= contourFinder.blobs[ k ].boundingRect.width;
			r2.height	= contourFinder.blobs[ k ].boundingRect.height;
			
			Shape& s2 = shapes.at( k );
			
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
		r1.x		= contourFinder.blobs[ i ].boundingRect.x;
		r1.y		= contourFinder.blobs[ i ].boundingRect.y;
		r1.width	= contourFinder.blobs[ i ].boundingRect.width;
		r1.height	= contourFinder.blobs[ i ].boundingRect.height;
		
		Shape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i - j );
			
			if( k < 0 )
				k = t + k;
			
			ofRectangle r2;
			r2.x		= contourFinder.blobs[ k ].boundingRect.x;
			r2.y		= contourFinder.blobs[ k ].boundingRect.y;
			r2.width	= contourFinder.blobs[ k ].boundingRect.width;
			r2.height	= contourFinder.blobs[ k ].boundingRect.height;
			
			Shape& s2 = shapes.at( k );
			
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
		Shape& s1 = shapes.at( i );
		
		bool hasChild;
		hasChild = ( s1.child > -1 );
		
		if( hasChild )
		{
			s1.noPolys += 1;
		}
		
		copyPolygonData( contourFinder.blobs[ i ].pts, s1.polyPoints );
		
		if( hasChild )
		{
			copyPolygonData( contourFinder.blobs[ s1.child ].pts, s1.polyPoints );
		}
	}
	
	for( int i=0; i<t; i++ )						// remove child contours with a parent, as they have already been copied to parent.
	{
		Shape& s1 = shapes.at( i );
		
		if( s1.parent != -1 )						// has parent. remove it.
		{
			shapes.erase( shapes.begin() + i );		// remove from array, step back counter after removed.
			
			--i;
			--t;
		}
	}
}

void testApp :: clearShapes ()
{
	for( int i=0; i<shapes.size(); i++ )
	{
		Shape& s = shapes.at( i );
		s.polyPoints.clear();
	}
	
	shapes.clear();
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

void testApp :: copyPolygonData ( const vector<ofPoint>& p1, vector<ofPoint>& p2 )
{
	for( int i=0; i<p1.size(); i++ )
	{
		p2.push_back( ofPoint() );
		
		ofPoint& p = p2.back();
		p.x = p1.at( i ).x;
		p.y = p1.at( i ).y;
	}
}

void testApp :: copyPolygonData ( const vector<ofxPoint2f>& p1, vector<ofPoint>& p2 )
{
	for( int i=0; i<p1.size(); i++ )
	{
		p2.push_back( ofPoint() );
		
		ofPoint& p = p2.back();
		p.x = p1.at( i ).x;
		p.y = p1.at( i ).y;
	}
}

void testApp :: updateTriangles ()
{
	triangle.clear();
	
	for( int i=0; i<shapes.size(); i++ )
	{
		int resolution;
		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.03 );
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.2 );
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.3 );
		
		triangle.triangulate( shapes.at( i ).polyPoints, resolution );
	}
	
	int t;
	t = triangles.size();
	
	for( int i=0; i<t; i++ )
	{
		ofxBox2dPolygonCustom& tri = triangles[ i ];
		
		ofPoint p = tri.getPosition();
		
		if			// check if off screen.
		(
			p.x < -100					||
			p.x > ofGetWidth()  + 100	||
			p.y > ofGetHeight() + 100
		)
		{
			tri.destroyShape();
			
			triangles.erase( triangles.begin() + i );		// remove from array, step back counter after removed.
			
			--i;
			--t;
		}
	}
}

void testApp :: addTrianglesToBox2d ()
{
	for( int i=0; i<triangle.getTriangles().size(); i++ )
	{
		ofxTriangleData& triData = triangle.getTriangles().at( i );
		
		float triScale = 1.0;
		
		ofPoint tp;
		tp = triangle.getTriangleCenter( triData );
		
		ofPoint ta, tb, tc;
		ta = triData.a - tp;
		tb = triData.b - tp;
		tc = triData.c - tp;
		
		ofPoint tsa, tsb, tsc, tsp;
		tsa	= ta * triScale;
		tsb	= tb * triScale;
		tsc	= tc * triScale;
		tsp	= tp * triScale;
		
		tsa.x += logoCropRect.x * 0.5;
		tsa.y += logoCropRect.y * 0.5;
		tsb.x += logoCropRect.x * 0.5;
		tsb.y += logoCropRect.y * 0.5;
		tsc.x += logoCropRect.x * 0.5;
		tsc.y += logoCropRect.y * 0.5;
		tsp.x += logoCropRect.x * 0.5;
		tsp.y += logoCropRect.y * 0.5;
		
		ofxBox2dPolygonCustom tri;
		
//		tri.setPhysics( 0, 0, 0 );			// fixed.
		tri.setPhysics
		(
			1.0,							// mass.
			ofRandom( 0.5, 0.8 ),			// bouce.
			ofRandom( 0.1, 0.7 )			// friction.
		);
		
		tri.addVertex( tsa );
		tri.addVertex( tsb );
		tri.addVertex( tsc );
		tri.createShape( box2d.getWorld(), tsp.x, tsp.y );
		
		float velMax;
		velMax = 1;
		
		ofxVec2f vel;
		vel.normalize();
		vel.x	= ofRandom( -velMax, velMax );
		vel.y	= ofRandom( -velMax, velMax );
		tri.setVelocity( vel.x, vel.y );
		
		triangles.push_back( tri );
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	ofSetColor( 0xFFFFFF );

	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, 40 );
	cameraGrayDiffImage.draw( logoCropRect.x, logoCropRect.y, logoCropRect.width, logoCropRect.height );
	ofDisableAlphaBlending();
	ofSetColor( 0xFFFFFF );
	
	ofEnableAlphaBlending();
	logo.draw( 0, 0 );
	ofDisableAlphaBlending();
	
//	logoLargeIntersect.draw( 0, 0 );
	
	ofRectangle smlRect;
	smlRect.width	= 160;
	smlRect.height	= 120;
	
	int pad;
	pad = 10;
	
	glPushMatrix();
	glTranslatef( ofGetWidth() - smlRect.width - pad, pad, 0 );
	
	drawBorder( smlRect );
	cameraColorImage.draw( 0, 0, smlRect.width, smlRect.height );

	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	cameraGrayDiffImage.draw( 0, 0, smlRect.width, smlRect.height );
	
	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	logoSmall.draw( 0, 0, smlRect.width, smlRect.height );
	
	glTranslatef( 0, smlRect.height + pad, 0 );

	drawBorder( smlRect );
	logoSmallIntersect.draw( 0, 0, smlRect.width, smlRect.height );
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( logoCropRect.x, logoCropRect.y, 0 );
	
//	contourFinder.draw();
//	drawShapes();
	
	glPopMatrix();

	drawTriangles();
//	floor.draw();
//	box2d.draw();
	
	screenGrab.save();
}

void testApp :: drawBorder ( const ofRectangle& rect )
{
	int b = 2;
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	ofRect( -b, -b, rect.width + b * 2, rect.height + b * 2 );
}

void testApp :: drawShapes ()
{
	ofEnableSmoothing();
	
	for( int i=0; i<shapes.size(); i++ )
	{
		ofFill();
		ofSetColor( 0xFFFFFF );
		
		drawShape( shapes[ i ] );
		
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		
		drawShape( shapes[ i ] );
	}
	
	ofDisableSmoothing();
}

void testApp :: drawShape ( Shape& shape )
{
	ofBeginShape();
	
	for( int i=0; i<shape.polyPoints.size(); i++ )
	{
		ofPoint& p1 = shape.polyPoints.at( i );
		
		ofVertex( p1.x, p1.y );
	}
	
	ofEndShape( true );
}

void testApp :: drawTriangles ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		triangles[ i ].draw();
	}
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

