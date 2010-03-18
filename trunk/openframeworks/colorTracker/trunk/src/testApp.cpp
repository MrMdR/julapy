#include "testApp.h"

/////////////////////////////////////////////
//	SETUP.
/////////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	
	videoRect.x			= 290;
	videoRect.y			= 110;
	videoRect.width		= 320;
	videoRect.height	= 240;
	
	video.setVerbose(true);
	video.initGrabber( videoRect.width, videoRect.height );

	colImg.allocate( videoRect.width, videoRect.height );				// Live Cam as input source
    hsvImg.allocate( videoRect.width, videoRect.height );				// HSV Image
    hueImg.allocate( videoRect.width, videoRect.height );				// Hue Image
    satImg.allocate( videoRect.width, videoRect.height );				// Saturation Image
    valImg.allocate( videoRect.width, videoRect.height );				// value Image

	hsvTotal	= TOTAL_OBJECTS_TRACKED;
	hsv			= new HSVData[ hsvTotal ];
	hsvIndex	= 0;
	
	for( int i=0; i<hsvTotal; i++ )
	{
		hsv[ i ].hue		= 0.0;
		hsv[ i ].hueRange	= 0.06;
		hsv[ i ].sat		= 0.62;
		hsv[ i ].satRange	= 0.94;
		hsv[ i ].val		= 0.8;
		hsv[ i ].valRange	= 0.62;
		
		hsv[ i ].img.allocate( videoRect.width, videoRect.height );									// tracker image.
		hsv[ i ].imgPixels = new unsigned char [ (int)videoRect.width * (int)videoRect.height ];	// tracker image pixels.
		
		hsv[ i ].bShowImg		= true;
		hsv[ i ].bShowContour	= true;
	}
	
	blur		= 0;
	threshold	= 20;
	
	initContourAnalysis();
	initGui();
}

void testApp :: initContourAnalysis ()
{
	cdataTotal	= TOTAL_OBJECTS_TRACKED;
	cdata		= new ContourData[ cdataTotal ];
	
	for( int i=0; i<cdataTotal; i++ )
	{
		cdata[ i ].nBlobs			= 0;
		cdata[ i ].box				= new CvBox2D32f[ MAX_NUM_CONTOURS_TO_FIND ];
		cdata[ i ].blobAngle		= new float[ MAX_NUM_CONTOURS_TO_FIND ];
		cdata[ i ].contourReg		= new vector<ofxPoint2f>[ MAX_NUM_CONTOURS_TO_FIND ];
		cdata[ i ].contourSmooth	= new vector<ofxPoint2f>[ MAX_NUM_CONTOURS_TO_FIND ];
		cdata[ i ].contourSimple	= new vector<ofxPoint2f>[ MAX_NUM_CONTOURS_TO_FIND ];
		cdata[ i ].contourHull		= new vector<ofxPoint2f>[ MAX_NUM_CONTOURS_TO_FIND ];
	}
	
	smoothPct		= 0.13f;
	tolerance		= 20.0f;
	
	cfMinArea		= 0.001;	// do not go to zero, shit will get weird.
	cfDetail		= 0;
	bShowBox		= false;
	bShowEllipse	= false;
	bShowAngle		= false;
	bShowLines		= false;
}

void testApp :: initGui ()
{
	for( int i=0; i<hsvTotal; i++ )
	{
		if( i > 0 )
			gui.addPage();
		
		gui.addTitle( "hsv " + ofToString( i, 0 ) );
		gui.addSlider( "hue          ", hsv[ i ].hue,		0.0, 1.0 );
		gui.addSlider( "hueWidth     ", hsv[ i ].hueRange,	0.0, 1.0 );
		gui.addSlider( "sat          ", hsv[ i ].sat,		0.0, 1.0 );
		gui.addSlider( "satWidth     ", hsv[ i ].satRange,	0.0, 1.0 );
		gui.addSlider( "val          ", hsv[ i ].val,		0.0, 1.0 );
		gui.addSlider( "valWidth     ", hsv[ i ].valRange,	0.0, 1.0 );
		gui.addToggle( "bShowImg     ", hsv[ i ].bShowImg			 );
		gui.addToggle( "bShowContour ", hsv[ i ].bShowContour		 );
	}

	gui.addPage();
	gui.addTitle( "image processing" );
	gui.addSlider( "blur      ", blur,		0.0, 30.0  );
	gui.addSlider( "threshold ", threshold,	0.0, 255.0 );

	gui.addPage();
	gui.addTitle( "contour analysis" );
	gui.addSlider( "contour detail   ", cfDetail,	0,   3   );
	gui.addSlider( "contour min area ", cfMinArea,	0.001, 0.2 );
	gui.addToggle( "contour ellipse  ", bShowEllipse	);
	gui.addToggle( "contour box      ", bShowBox		);
	gui.addToggle( "contour angle    ", bShowAngle		);
	gui.addToggle( "contour lines    ", bShowLines		);

//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void testApp::update()
{
	ofBackground( 100,100,100 );

	video.grabFrame();
	
	if( video.isFrameNew() )
	{
		colImg.setFromPixels( video.getPixels(), videoRect.width, videoRect.height );	
		colImg.mirror( false, true );
		
		hsvImg = colImg;
		hsvImg.convertRgbToHsv();
		hsvImg.convertToGrayscalePlanarImages( hueImg, satImg, valImg );
	
		unsigned char * huePixels = hueImg.getPixels();
		unsigned char * satPixels = satImg.getPixels();
		unsigned char * valPixels = valImg.getPixels();
		int nPixels = videoRect.width * videoRect.height;
	
		for( int i=0; i<hsvTotal; i++ )
		{
			hsv[ i ].minHue = max( ( hsv[ i ].hue - hsv[ i ].hueRange * 0.5) * 255, 000.0 );
			hsv[ i ].maxHue = min( ( hsv[ i ].hue + hsv[ i ].hueRange * 0.5) * 255, 255.0 );
			hsv[ i ].minSat = max( ( hsv[ i ].sat - hsv[ i ].satRange * 0.5) * 255, 000.0 );
			hsv[ i ].maxSat = min( ( hsv[ i ].sat + hsv[ i ].satRange * 0.5) * 255, 255.0 );
			hsv[ i ].minVal = max( ( hsv[ i ].val - hsv[ i ].valRange * 0.5) * 255, 000.0 );
			hsv[ i ].maxVal = min( ( hsv[ i ].val + hsv[ i ].valRange * 0.5) * 255, 255.0 );
			
			for( int j=0; j<nPixels; j++ )
			{
				if
				(
					( huePixels[ j ] >= hsv[ i ].minHue && huePixels[ j ] <= hsv[ i ].maxHue ) &&
					( satPixels[ j ] >= hsv[ i ].minSat && satPixels[ j ] <= hsv[ i ].maxSat ) &&
					( valPixels[ j ] >= hsv[ i ].minVal && valPixels[ j ] <= hsv[ i ].maxVal )
				)
				{
					hsv[ i ].imgPixels[ j ] = 255;
				}
				else
				{
					hsv[ i ].imgPixels[ j ] = 0;
				}
			}
			
			hsv[ i ].img.setFromPixels( hsv[ i ].imgPixels, videoRect.width, videoRect.height );
			
			hsv[ i ].img.blur( blur );
			hsv[ i ].img.threshold( threshold );
			
			computeContourAnalysis( i );
		}
		
		LT.calcColorRange
		(
			hsv[ hsvIndex ].hue,
			hsv[ hsvIndex ].hueRange,
			hsv[ hsvIndex ].sat,
			hsv[ hsvIndex ].satRange,
			hsv[ hsvIndex ].val
		);
	}
}

void testApp :: computeContourAnalysis ( int i )
{
	int maxArea;
	maxArea = videoRect.width * videoRect.height;
	
	int runningBlobs;
	runningBlobs = contourFinder.findContours
	(
		hsv[ i ].img,					// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		MAX_NUM_CONTOURS_TO_FIND,		// max number of contours to find.
		false,							// find holes.
		false							// use approximation.
	);
	
	if( runningBlobs == 0 )
	{
		cdata[ i ].nBlobs = 0;
		
		return;
	}
	
	// TODO :: work out the biggest blob, drop the rest.
	
	int nBlobs;
	nBlobs = contourFinder.nBlobs;
	
	cdata[ i ].nBlobs = nBlobs;
	
	cdata[ i ].blobArea.clear();
	cdata[ i ].blobArea.assign( nBlobs, 1.0f );
	cdata[ i ].blobBoundingRect.clear();
	cdata[ i ].blobBoundingRect.assign( nBlobs, ofRectangle() );
	cdata[ i ].blobCentroid.clear();
	cdata[ i ].blobCentroid.assign( nBlobs, ofPoint() );
	
	for( int j=0; j<nBlobs; j++ )
	{
		cdata[ i ].blobArea.at( j )					= contourFinder.blobs[ j ].area;
		
		cdata[ i ].blobBoundingRect.at( j ).x		= contourFinder.blobs[ j ].boundingRect.x;
		cdata[ i ].blobBoundingRect.at( j ).y		= contourFinder.blobs[ j ].boundingRect.y;
		cdata[ i ].blobBoundingRect.at( j ).width	= contourFinder.blobs[ j ].boundingRect.width;
		cdata[ i ].blobBoundingRect.at( j ).height	= contourFinder.blobs[ j ].boundingRect.height;
		
		cdata[ i ].blobCentroid.at( j ).x			= contourFinder.blobs[ j ].centroid.x;
		cdata[ i ].blobCentroid.at( j ).y			= contourFinder.blobs[ j ].centroid.y;
		
		int contourLength;
		contourLength = contourFinder.blobs[ j ].pts.size();
		
		fitEllipse
		(
			contourFinder.blobs[ j ].pts,
			cdata[ i ].box[ j ]
		);
		
		cdata[ i ].blobAngle[ j ] = getOrientation( contourFinder.blobs[ j ].pts );
		
		findLines
		(
			contourFinder.blobs[ j ].pts,	// blob points.
			cdata[ i ].geomLines,			// lines vector.
			30,								// angle threshold.
			40,								// minimum length.
			30								// resolution.
		);
		
		cdata[ i ].contourReg[ j ].clear();
		cdata[ i ].contourReg[ j ].assign( contourLength, ofxPoint2f() );
		cdata[ i ].contourSmooth[ j ].clear();
		cdata[ i ].contourSmooth[ j ].assign( contourLength, ofxPoint2f() );
		cdata[ i ].contourSimple[ j ].clear();
		cdata[ i ].contourHull[ j ].clear();
		
		for( int k=0; k<contourLength; k++ )
		{
			cdata[ i ].contourReg[ j ].at( k ) = contourFinder.blobs[ j ].pts[ k ];
		}
		
		contourS.smooth( cdata[ i ].contourReg[ j ], cdata[ i ].contourSmooth[ j ], smoothPct );
		contourS.simplify( cdata[ i ].contourSmooth[ j ], cdata[ i ].contourSimple[ j ], tolerance );
		contourS.convexHull( cdata[ i ].contourSmooth[ j ], cdata[ i ].contourHull[ j ] );
	}
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void testApp::draw()
{
	ofSetColor(0xffffff);
	
	glPushMatrix();
	glTranslatef( videoRect.x, videoRect.y, 0 );
	
	int pad;
	pad = 10;
	
	int w;
	w = videoRect.width + pad;
	
	int h;
	h = videoRect.height + pad;
	
	colImg.draw( w * 0, h * 0 );
	hsvImg.draw( w * 1, h * 0 );
	
	for( int i=0; i<hsvTotal; i++ )
	{
		if( i == hsvIndex )
		{
			int b;	// border.
			b = 4;
			
			ofFill();
			ofSetColor( 0xFF0000 );		// red border.
			ofRect
			(
				w * i - b,
				h * 1 - b,
				videoRect.width  + b * 2,
				videoRect.height + b * 2
			);
			ofSetColor( 0xFFFFFF );
		}
		
		ofFill();
		ofSetColor( 0x000000 );			// black bg.
		ofRect
		(
			w * i,
			h * 1,
			videoRect.width,
			videoRect.height
		);
		ofSetColor( 0xFFFFFF );
		
		if( hsv[ i ].bShowImg )
		{
			hsv[ i ].img.draw( w * i, h * 1 );
		}
	}
	
	glPushMatrix();
	glTranslatef( w * 2, h * 0, 0 );

	int x;
	x = 140;
	
	int y;
	y = 10;
	
	int dy;
	dy = 15;
	
	LT.drawColorRange( 0, 0, 120, 44 );
	
	ofSetColor( 0xffffff );
	ofDrawBitmapString( "minHue: " + ofToString( hsv[ hsvIndex ].minHue ), x, y       );
	ofDrawBitmapString( "maxHue: " + ofToString( hsv[ hsvIndex ].maxHue ), x, y += dy );
	ofDrawBitmapString( "minSat: " + ofToString( hsv[ hsvIndex ].minSat ), x, y += dy );
	ofDrawBitmapString( "maxSat: " + ofToString( hsv[ hsvIndex ].maxSat ), x, y += dy );
	ofDrawBitmapString( "minVal: " + ofToString( hsv[ hsvIndex ].minVal ), x, y += dy );
	ofDrawBitmapString( "maxVal: " + ofToString( hsv[ hsvIndex ].maxVal ), x, y += dy );
	
	glPopMatrix();
	
	for( int i=0; i<cdataTotal; i++ )
	{
		glPushMatrix();
		glTranslatef( w * i, h * 1, 0 );
			drawContourAnalysis( i );
		glPopMatrix();
	}
	
	glPopMatrix();
	
	gui.draw();
}


void testApp :: drawContourAnalysis( int i )
{
	if( !hsv[ i ].bShowContour )
		return;
	
	for( int j=0; j<cdata[ i ].nBlobs; j++ )
	{
		if( cfDetail == 0 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata[ i ].contourReg[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata[ i ].contourReg[ j ].at( k ).x,
					cdata[ i ].contourReg[ j ].at( k ).y
				);
			}
			ofEndShape( true );
		}
		else if( cfDetail == 1 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata[ i ].contourSmooth[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata[ i ].contourSmooth[ j ].at( k ).x,
					cdata[ i ].contourSmooth[ j ].at( k ).y
				);
			}
			ofEndShape(true);
		}
		else if( cfDetail == 2 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata[ i ].contourSimple[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata[ i ].contourSimple[ j ].at( k ).x,
					cdata[ i ].contourSimple[ j ].at( k ).y
				);
			}
			ofEndShape(true);
		}
		else if( cfDetail == 3 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata[ i ].contourHull[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata[ i ].contourHull[ j ].at( k ).x,
					cdata[ i ].contourHull[ j ].at( k ).y
				);
			}
			ofEndShape(true);
			
			for( int k=0; k<cdata[ i ].contourHull[ j ].size(); k++ )
			{
				ofCircle
				(
					cdata[ i ].contourHull[ j ].at( k ).x,
					cdata[ i ].contourHull[ j ].at( k ).y,
					2
				);
			}
		}
		
		if( bShowBox )
		{
			ofSetRectMode( OF_RECTMODE_CENTER );
			
			ofNoFill();
			ofSetColor( 0xFFFFFF );
			
			glPushMatrix();
			glTranslatef
			(
				cdata[ i ].box[ j ].center.x,
				cdata[ i ].box[ j ].center.y,
				0
			);
			glRotatef( cdata[ i ].box[ j ].angle, 0, 0, 1 );
			ofRect
			(
				0,
				0,
				cdata[ i ].box[ j ].size.width,
				cdata[ i ].box[ j ].size.height
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
				cdata[ i ].box[ j ].center.x,
				cdata[ i ].box[ j ].center.y,
				0
			);
			glRotatef( cdata[ i ].box[ j ].angle, 0, 0, 1 );
			ofEllipse
			(
				0,
				0,
				cdata[ i ].box[ j ].size.width  * 0.5,
				cdata[ i ].box[ j ].size.height * 0.5
			);
			glPopMatrix();
		}
		
		if( bShowAngle )
		{
			ofSetColor( 0xFFFFFF );
			
			float x1,y1,x2,y2;
			
			x1 = cdata[ i ].blobCentroid[ j ].x + 25 * cos( cdata[ i ].blobAngle[ j ] );
			y1 = cdata[ i ].blobCentroid[ j ].y + 25 * sin( cdata[ i ].blobAngle[ j ] );
			x2 = cdata[ i ].blobCentroid[ j ].x - 25 * cos( cdata[ i ].blobAngle[ j ] );
			y2 = cdata[ i ].blobCentroid[ j ].y - 25 * sin( cdata[ i ].blobAngle[ j ] );
			
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
			
			x1 = cdata[ i ].blobCentroid[ j ].x + 10 * cos( cdata[ i ].blobAngle[ j ] + HALF_PI );
			y1 = cdata[ i ].blobCentroid[ j ].y + 10 * sin( cdata[ i ].blobAngle[ j ] + HALF_PI );
			x2 = cdata[ i ].blobCentroid[ j ].x - 10 * cos( cdata[ i ].blobAngle[ j ] + HALF_PI );
			y2 = cdata[ i ].blobCentroid[ j ].y - 10 * sin( cdata[ i ].blobAngle[ j ] + HALF_PI );
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
		}
		
		if( bShowLines )
		{
			ofSetColor( 0x8aff00 );
			ofNoFill();
			for( int k=0; k<cdata[ i ].geomLines.size(); k++ )
			{
				ofLine
				(
					cdata[ i ].geomLines[ k ].x,
					cdata[ i ].geomLines[ k ].y,
					cdata[ i ].geomLines[ k ].z,
					cdata[ i ].geomLines[ k ].w
				);
				ofCircle
				(
					cdata[ i ].geomLines[ k ].x,
					cdata[ i ].geomLines[ k ].y,
					3
				);
				ofCircle
				(
					cdata[ i ].geomLines[ k ].z,
					cdata[ i ].geomLines[ k ].w,
					3
				);
			}
		}
	}
}

/////////////////////////////////////////////
//	HANDLERS.
/////////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == 's' )
		video.videoSettings();
	
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
	
	if( key >= '1' && key <= '3' )		// need to work out how to make the limit hsvTotal.
	{
		hsvIndex = key - '0' - 1;
	}
	
	if( key == 'i' )
	{
		hsv[ hsvIndex ].bShowImg = !hsv[ hsvIndex ].bShowImg;
	}
	
	if( key == 'c' )
	{
		hsv[ hsvIndex ].bShowContour = !hsv[ hsvIndex ].bShowContour;
	}
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button)
{
	if
	(
		x >= videoRect.x					&&
		x <  videoRect.x + videoRect.width	&&
		y >= videoRect.y					&&
		y <  videoRect.y + videoRect.height
	)
	{
		int p;		// pixel no.
		p = ( y - videoRect.y ) * 320 + ( x - videoRect.x );
		
		hsv[ hsvIndex ].hue = hueImg.getPixels()[ p ] / 255.0;
		hsv[ hsvIndex ].sat = satImg.getPixels()[ p ] / 255.0;
		hsv[ hsvIndex ].val = valImg.getPixels()[ p ] / 255.0;
	}
}

void testApp::mouseReleased()
{
}
