#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
//	frameRate = 25;
	frameRate = 60;
	
	ofSetVerticalSync( true );
	ofSetFrameRate( frameRate );
	ofSetCircleResolution( 100 );
	
	screenGrab.setup( "movies/" );
//	screenGrab.setPause( false );
	
	bDebug		= false;
	bFullScreen	= false;
	bSmooth		= true;
	bUseCamera	= true;
	
	initColors();
	initCamera();
	initLogo();
	initCv();
	initContours();
	initBox2d();
	initOpticalField();
	initCirclePacker();
}

void testApp :: initColors ()
{
	int colorSize;
	
//	int colorArray[] =			// image 1
//	{
//		0xFEFEFD,0xF9F634,0xEBDD45,0xCFDB9C,0xF8DECE,0xEB98AF,0xF7F5C8,0xD40D24,0xB1A09B,0xE54B85,
//		0xD4D2CF,0x433035,0x8FC9EE,0x9C9474,0xB72220,0x9EC18F,0x92A52D,0xCB9A2F,0x9D6C75,0x95C930,
//		0x79A6B4,0x8EB9D2,0xD2BD42,0xCD4474,0x4E8C3E,0xE3E8EB,0xBB5F27,0x9A314D,0xCEBBB5,0xF9D2DC,
//		0xC9CB5C,0x6A5571
//	};
//	colorSize = 32;
	
//	int colorArray[] =			// image 2
//	{
//		0xE9589F,0x15105E,0x571661,0x981E63,0x16238E,0x5F2A8E,0xA749A5,0x972E8D,0x3DA1F9,0x1A5EE4,
//		0x56052B,0x6148AE,0x6362DF,0xB29DFA,0x9D62D9,0x100020,0xE46ACF,0x1C47B2,0x8032F0,0xD9467F,
//		0xC32E6A,0x8A1036,0x2A0535,0x132DC7,0x30000B,0xC53893,0xB03C7A,0x5A35C8,0x963CC8,0xC2253F,
//		0x653879,0x193778
//	};
//	colorSize = 32;
	
//	int colorArray[] =			// image 3
//	{
//		0xC5490E,0xE8EAD6,0xDFA161,0xAED0B3,0xD8DCB5,0x96B69C,0xA89B63,0xE4B78B,0xE48E19,0xF6D9B4,
//		0x6A9F92,0x645A23,0x6F8D5F,0xF0CF66,0xD8CF99,0xBFDDC9,0xF2D294,0x180B07,0x284725,0x6A1A06,
//		0xFDF3C4,0x67714A,0x145351,0xF5D013,0xA9822E,0x1E7483,0xF2A4F,0xD2EFC2,0xFAF08B,0xE2756B,
//		0xB9C174,0xB5C61C
//	};
//	colorSize = 32;				
	
//	int colorArray[] =			// image 4
//	{
//		0xFFD44C,0xDC6517,0xFFB73A,0x5C2908,0xFF941F,0xFFAB25,0x50100,0xFFF3A9,0x270B03,0xF88022,
//		0x873F0C,0x3F1905,0xF48D3F,0xFF9D36,0xFFD30F,0xC48613,0xE4AA1A,0xF1AC41,0xFF9100,0xEB8300,
//		0xC98E3D,0xFFBE5E,0xFFAF00,0xE3B76A,0x422613,0xD7AB00,0x835E2A,0xE49864,0xFFA164,0x2A1011,
//		0xCEC500,0xBEF950
//	};
//	colorSize = 32;

//	int colorArray[] =			// image 6
//	{
//		0x9F6255,0xE3A991,0x975E26,0x521D25,0xFFFFFE,0xF9C99E,0xCE9271,0xB58255,0xCA904E,0xCC9728,
//		0xAC6681,0xB18220,0xDCA872,0xEDB784,0xBC7562,0x844056,0xD6A64C,0x803C20,0xFFF7D4,0xF5CA7C,
//		0xE6AF53,0xFDD7B7,0xC48386,0xB47436,0x612269,0xDD9A7A,0xD8CBC1,0xDD994F,0xEBACAD,0xEEBD2D,
//		0xF9DADF,0xD691A5
//	};
//	colorSize = 32;

//	int colorArray[] =			// image 7
//	{
//		0xF4FAF2,0xFD7E5,0xC6597,0xCE2C43,0x6B5D3,0x242855,0x5ADEE6,0x290B6,0xA2E6EB,0xEFE7AA,
//		0xD04F90,0x54ABDF,0xE89A5E,0xF1E75B,0xF1EC19,0xEF9E98,0xF29318,0xA9D9A2,0x97A9E2,0xA59BA8,
//		0x5BABAE,0xB1F467,0xA99B66,0x69E7AD,0x6256A0,0x6D9B55,0x96EC2,0x1F3A89,0xE7C0CA,0xAA8A2F,
//		0x597FD3,0xB2D62F
//	};
//	colorSize = 32;
	
//	int colorArray[] =			// custom.
//	{
//		0xFF00FF,0x00FFFF,0xFFFF00
//	};
//	colorSize = 3;

//	int colorArray[] =			// custom.
//	{
//		0xFF0000,0x0000FF,0xFF00FF
//	};
//	colorSize = 3;
	
	int colorArray[] =			// custom 1.
	{
		0x4C526B,0xB00347,0x306C73,0xC70146,0xEC2E2B,0x8A0B42,0xF98D10,0x188379,0xF77616,0xDA083C,
		0x6C1444,0xF14823,0x692552,0x9E0644,0xE31635,0x5D2D55,0x760D3E,0x693661,0x4977F,0xF35520,
		0x8A1E52,0xE02331,0x5F1843,0xF5631C,0x825053,0xAF1E44,0x5B4164,0xD04131,0x7B2A5B,0xAD4641,
		0xEF3C27,0xD86426
	};
	colorSize = 32;

//	int colorArray[] =			// custom 2.
//	{
//		0xF5CA11,0xECD693,0xE9071B,0xE0AD18,0x469089,0x17289,0xEDCE5B,0xAAAD8D,0x115265,0x9C9866,
//		0x5F5052,0xE59B6B,0x5C8F68,0xB49C22,0xF8E397,0xD8CB8E,0xD1B687,0xE9561E,0xA31727,0xE4624F,
//		0x624A22,0x8A7057,0x917019,0xF9EFA3,0xB3C292,0xE30E20,0x97488,0xEDC319,0xE8D392
//	};
//	colorSize = 29;

//	int colorArray[] =			// custom 2.
//	{
//		0x2B4098,0xDE4F9C,0x8A59,0xECE018,0xF4D2D8,0xB3DFE4,0xFAE85E,0xFFDC96,0xED89AD,0xEE6221,
//		0x192570,0xF49822,0x7A1D1,0xECB3D0,0x2AD91,0xF45363,0x74CBDB,0xA5A6D5,0xE51B29,0xDB0C66,
//		0xFA9D66,0x647ABE,0x73C797,0x40BA79,0xB3D452,0xB4DCAA,0xC0D832,0xA386C0,0x97298E,0x3BC0BD,
//		0x98C536,0x66BF48
//	};
//	colorSize = 32;
	
	colors.resize( colorSize );
	copy( colorArray, colorArray + colorSize, colors.begin() );
}

void testApp :: initCamera ()
{
	cameraRect.width	= 640;
	cameraRect.height	= 480;
	
	cameraRect.width	*= 0.5;
	cameraRect.height	*= 0.5;
	
	if( bUseCamera )
	{
		camera.setVerbose( true );
		camera.initGrabber( cameraRect.width, cameraRect.height );
	}
	else
	{
		video.loadMovie( "input_video_320x240.mov" );
		
		if( !screenGrab.isRecording() )
			video.play();
	}

}

void testApp :: initLogo ()
{
	logo.loadImage( "holler_logo_lrg.png" );
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
	
	logoJpg.loadImage( "holler_logo_lrg.jpg" );
	logoAlpha.loadImage( "holler_logo_med_alpha.png" );
	
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
	
	logoCopy.clear();
	logoCrop.clear();
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
//	box2d.setGravity( 0, 50 );
//	box2d.setFPS( 40 );
	box2d.setGravity( 0, 10 );
	box2d.setFPS( 30 );
	
	int thick;
	thick = 1;
	
	float wallMass		= 1.0;
	float wallBounce	= 0.5;
	float wallFriction	= 0.3;
	
	float fw;
	fw = 0.5;
	
	ofxBox2dRect floor;
	floor.setPhysics( wallMass, wallBounce, wallFriction );
	floor.setup( box2d.getWorld(), 0, ofGetHeight(), ofGetWidth(), thick, true );
	
	ofxBox2dRect ceil;
	ceil.setPhysics( wallMass, wallBounce, wallFriction );
	ceil.setup( box2d.getWorld(), 0, 0, ofGetWidth(), thick, true );
	
	int bottomGap;
	bottomGap = 100;
	
	ofxBox2dRect left;
	left.setPhysics( wallMass, wallBounce, wallFriction );
	left.setup( box2d.getWorld(), 0, 0, thick, ofGetHeight() - bottomGap, true );
	
	ofxBox2dRect right;
	right.setPhysics( wallMass, wallBounce, wallFriction );
	right.setup( box2d.getWorld(), ofGetWidth(), 0, thick, ofGetHeight() - bottomGap, true );
}

void testApp :: initOpticalField ()
{
	opticalFieldRect.width	= 160;
	opticalFieldRect.height	= 120;
	
	opticalField.init( cameraRect, opticalFieldRect );
	opticalField.setImageType( GL_LUMINANCE );
	opticalField.setMirror( false, false );
}

void testApp :: initCirclePacker ()
{
	ofRectangle logoRect;
	logoRect.width	= logoJpg.width;
	logoRect.height	= logoJpg.height;
	
	circlePacker.setColorBoundsImage( &logoJpg, logoRect );
	circlePacker.setCircleRadiusMin( 2 );
	circlePacker.setCircleRadiusMax( 20 );
	circlePacker.setCircleGap( 1.0 );
	circlePacker.setCircleDeathGap( 2.0 );
//	circlePacker.setCircleColorBounds( true );
//	circlePacker.setCircleDeathColor( 0x000000 );
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
	
		opticalField.update( cameraGrayImage.getPixels() );
		
		if( ofGetFrameNum() % 1 == 0 )
		{
			int blobs;
			blobs = updateContours();
			
			if( blobs > 0 )
			{
				clearShapes();
				parseShapes();
				scaleShapes();
		
				addCirclesToBox2d();
				
//				updateTriangles();
//				addTrianglesToBox2d();
			}
		}
	}
	
//	updateTriangleShapes();
	updateCirclePacker();
	updateCircles();
	
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
			t = ofGetFrameNum() / (float)frameRate;
			
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
}

int testApp :: updateContours ()
{
	int maxArea;
	maxArea = logoSmallIntersect.width * logoSmallIntersect.height;
	
	float cfMinArea;
	cfMinArea = 0.001;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		logoSmallIntersect,				// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void testApp :: parseShapes ()
{
	int t;
	t = contourFinder.nBlobs;
	
	if( t == 0 )
		return;
	
	for( int i=0; i<t; i++ )				// initialise shapes.
	{
		shapes.push_back( Shape() );
		Shape& s = shapes.back();
		
		s.parent	= -1;
		s.child		= -1;
		s.noPolys	= 1;
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

void testApp :: scaleShapes ()
{
	float scale;
	scale = logoCropRect.width / (float)cameraRect.width;
	
	for( int i=0; i<shapes.size(); i++ )
	{
		Shape& shape = shapes[ i ];
		
		for( int j=0; j<shape.polyPoints.size(); j++ )
		{
			ofPoint& p = shape.polyPoints[ j ];
			p	*= scale;
			p.x	+= logoCropRect.x;
			p.y	+= logoCropRect.y;
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

void testApp :: addCirclesToBox2d ()
{
	vector<Circle>& circles	= circlePacker.getCircles();
	
	int i = 0;
	int t = circles.size();
	
	for( i; i<t; i++ )
	{
		Circle& circle	= circles[ i ];
		
		if( circle.bUnderMinRadius )
			continue;
		
		ofPoint p = circle.loc;
		
		for( int j=0; j<shapes.size(); j++ )
		{
			Shape& shape = shapes[ j ];
			
			if( triangle.isPointInsidePolygon( shape.polyPoints, p ) )
			{
				//-- add to box2d.
				
				
				b2dCircles.push_back( ofxBox2dCircleCustom() );
				ofxBox2dCircleCustom& circ = b2dCircles.back();
				
				circ.setPhysics
				(
					1.0,							// mass.
					ofRandom( 0.5, 0.8 ),			// bouce.
					ofRandom( 0.1, 0.7 )			// friction.
				);
				circ.setup( box2d.getWorld(), p.x, p.y, circle.radius, false );
				
				int color;
				color = colors[ (int)( colors.size() * ofRandom( 0.0, 1.0 ) ) ];
				
				circ.color = color;
				
				ofPoint vn;
				vn = p;
				vn.x -= logoCropRect.x;
				vn.y -= logoCropRect.y;
				vn.x /= (float)logoCropRect.width;
				vn.y /= (float)logoCropRect.height;
				
				ofPoint vo;
				opticalField.getVelAtNorm( vn.x, vn.y, &vo.x, &vo.y );
				vo *= 2;
				
				circ.setVelocity( vo.x, vo.y );
				
				//-- remove from packer.
				
				circle.neighbours.clear();
				circles.erase( circles.begin() + i );		// remove circle from packer.
				
				--i;
				--t;
				
				break;
			}
		}
	}
}

void testApp :: updateCirclePacker ()
{
	circlePacker.addCircles( 40, 0xFFFFFF );
	circlePacker.update();
}

void testApp :: updateCircles ()
{
	int t;
	t = b2dCircles.size();
	
	for( int i=0; i<t; i++ )
	{
		ofxBox2dCircleCustom& circle = b2dCircles[ i ];
		
		ofPoint p = circle.getPosition();
		
		if			// check if off screen.
		(
			p.x < -circle.getRadius() - 10							||
			p.x > ofGetWidth()  + circle.getRadius() + 10			||
			p.y > ofGetHeight() + circle.getRadius() + 10
		)
		{
			circle.destroyShape();
			
			b2dCircles.erase( b2dCircles.begin() + i );		// remove from array, step back counter after removed.
			
			--i;
			--t;
		}
	}
}

void testApp :: killCircles ()
{
	for( int i=0; i<b2dCircles.size(); i++ )
	{
		b2dCircles[ i ].destroyShape();
	}
	
	b2dCircles.clear();
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
	
//	ofEnableAlphaBlending();
//	logoAlpha.draw( 0, 0 );
//	ofDisableAlphaBlending();
	
	if( bDebug )
	{
		drawDebug();
	}

	drawCirclePacker();
	drawCircles();
	
//	floor.draw();
//	box2d.draw();
	
	screenGrab.save();
	
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 10, ofGetHeight() - 10 );
}

void testApp :: drawDebug ()
{
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
	
	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	opticalField.drawOpticalFlow( 10 );
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( logoCropRect.x, logoCropRect.y, 0 );
	
	glPopMatrix();
	
//	contourFinder.draw();
//	drawShapes();
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
//		ofFill();
//		ofSetColor( 0xFFFFFF );
//		
//		drawShape( shapes[ i ] );
		
		ofNoFill();
		ofSetColor( 0xFF0000 );
		
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

void testApp :: drawCirclePacker ()
{
	ofFill();
	circlePacker.draw();

	if( !bSmooth )
		return;

	ofNoFill();
	ofEnableSmoothing();
	
	circlePacker.draw();
	
	ofDisableSmoothing();
}

void testApp :: drawCircles ()
{
	ofFill();
	
	for( int i=0; i<b2dCircles.size(); i++ )
	{
		b2dCircles[ i ].draw();
	}

	if( !bSmooth )
		return;
	
	ofNoFill();
	ofEnableSmoothing();
	
	for( int i=0; i<b2dCircles.size(); i++ )
	{
		b2dCircles[ i ].draw();
	}
	
	ofDisableSmoothing();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'f' )
	{
		bFullScreen = !bFullScreen;
		
		ofToggleFullscreen();
	}
	
	if( key == 'k' )
	{
		killCircles();
	}
	
	if( key == 's' )
	{
		bSmooth = !bSmooth;
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
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

