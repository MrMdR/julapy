/*
 *  ColorTracker.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 6/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorTracker.h"

/////////////////////////////////////////////
//	SETUP.
/////////////////////////////////////////////

void ColorTracker :: init ( int v )
{
	noColorsTracked		= v;
	
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
	
	hsvTotal	= noColorsTracked;
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
	initTCP();
	initGui();
}

void ColorTracker :: initContourAnalysis ()
{
	cdataTotal	= noColorsTracked;
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
	
	bShowRect		= false;
	bShowBox		= true;
	bShowEllipse	= false;
	bShowAngle		= false;
	bShowLines		= false;
}

void ColorTracker :: initTCP ()
{
	TCP.setup( 11999 );
}

void ColorTracker :: initGui ()
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
	
	gui.addToggle( "bounding rect    ", bShowRect		);
	gui.addToggle( "contour box      ", bShowBox		);
	gui.addToggle( "contour ellipse  ", bShowEllipse	);
	gui.addToggle( "contour angle    ", bShowAngle		);
	gui.addToggle( "contour lines    ", bShowLines		);
	
	//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void ColorTracker :: update ()
{
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
		
		sendTrackerData();
	}
}

void ColorTracker :: computeContourAnalysis ( int i )
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
	
	// TODO :: work out the biggest blob, drop the rest. - think its already arrange by largest.
	
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

void ColorTracker :: sendTrackerData ()
{
	string str;
	for( int i=0; i<cdataTotal; i++ )
	{
		int b;
		b = ( cdata[ i ].nBlobs > 0 ) ? 1 : 0;
		
		float x, y;
		x = cdata[ i ].box[ 0 ].center.x / (float)videoRect.width;
		y = cdata[ i ].box[ 0 ].center.y / (float)videoRect.height;
		
		float w, h;
		w = cdata[ i ].box[ 0 ].size.width  / (float)videoRect.width;
		h = cdata[ i ].box[ 0 ].size.height / (float)videoRect.height;
		
		float a;
		a = cdata[ i ].box[ 0 ].angle / 360.0;
		
		str += "[";
		str += ofToString( b, 0 ) + ",";
		str += ofToString( x, 4 ) + ",";
		str += ofToString( y, 4 ) + ",";
		str += ofToString( w, 4 ) + ",";
		str += ofToString( h, 4 ) + ",";
		str += ofToString( a, 4 ) + ",";
		str += "]";
		
		if( i < cdataTotal - 1 )
			str += "\n";
	}
	
	for( int i=0; i<TCP.getNumClients(); i++ )
	{
		TCP.send( i, str );
	}	
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void ColorTracker :: draw()
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
		int vx;
		int vy;
		int pr;
		
		pr = 3;		// 3 per row.
		
		vx = w * ( i % pr );
		vy = h * ( (int)( i / pr ) + 1 );
		
		if( i == hsvIndex )
		{
			int b;	// border.
			b = 4;
			
			ofFill();
			ofSetColor( 0xFF0000 );		// red border.
			ofRect
			(
				vx - b,
				vy - b,
				videoRect.width  + b * 2,
				videoRect.height + b * 2
			);
			ofSetColor( 0xFFFFFF );
		}
		
		ofFill();
		ofSetColor( 0x000000 );			// black bg.
		ofRect
		(
			vx,
			vy,
			videoRect.width,
			videoRect.height
		);
		ofSetColor( 0xFFFFFF );
		
		if( hsv[ i ].bShowImg )
		{
			hsv[ i ].img.draw( vx, vy );
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
	
	ofDrawBitmapString( "no. of TCP clients connected :: " + ofToString( TCP.getNumClients() ), 0, y+= 60 );
	
	glPopMatrix();
	
	for( int i=0; i<cdataTotal; i++ )
	{
		int vx;
		int vy;
		int pr;
		
		pr = 3;		// 3 per row.
		
		vx = w * ( i % pr );
		vy = h * ( (int)( i / pr ) + 1 );
		
		glPushMatrix();
		glTranslatef( vx, vy, 0 );
		drawContourAnalysis( i );
		glPopMatrix();
	}
	
	glPopMatrix();
	
	gui.draw();
}


void ColorTracker :: drawContourAnalysis( int i )
{
	if( !hsv[ i ].bShowContour )
		return;
	
	for( int j=0; j<cdata[ i ].nBlobs; j++ )
	{
		if( j > 0 )		// only interested in the largest blob.
			return;
		
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
		
		if( bShowRect )
		{
			ofNoFill();
			ofSetColor( 0xFFFFFF );
			
			ofRect
			(
				cdata[ i ].blobBoundingRect[ j ].x,
				cdata[ i ].blobBoundingRect[ j ].y,
				cdata[ i ].blobBoundingRect[ j ].width,
				cdata[ i ].blobBoundingRect[ j ].height
			);
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
//	CONFIG.
/////////////////////////////////////////////

void ColorTracker :: writeToFile	( string filename )
{
	ofstream fout;
	fout.open( ofToDataPath( filename ).c_str() );
	
	for( int i=0; i<hsvTotal; i++ )
	{
		string dataStr;
		
		dataStr = 
		ofToString( hsv[ i ].hue,		6 ) + " " +
		ofToString( hsv[ i ].hueRange,	6 ) + " " +
		ofToString( hsv[ i ].sat,		6 ) + " " +
		ofToString( hsv[ i ].satRange,	6 ) + " " +
		ofToString( hsv[ i ].val,		6 ) + " " +
		ofToString( hsv[ i ].valRange,	6 );
		
		fout << dataStr << endl;
	}
	
	fout.close();
}

void ColorTracker :: loadFromFile ( string filename )
{
	ifstream	dataFile;
	string		dataStr;
	
	int i;
	i = 0;
	
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
			
			hsv[ i ].hue		= atof( data[ 0 ].c_str() );
			hsv[ i ].hueRange	= atof( data[ 1 ].c_str() );
			hsv[ i ].sat		= atof( data[ 2 ].c_str() );
			hsv[ i ].satRange	= atof( data[ 3 ].c_str() );
			hsv[ i ].val		= atof( data[ 4 ].c_str() );
			hsv[ i ].valRange	= atof( data[ 5 ].c_str() );
			
			++i;
		}
	}
	
	dataFile.close();
}

/////////////////////////////////////////////
//	HANDLERS.
/////////////////////////////////////////////

void ColorTracker :: keyPressed( int key )
{
	if( key == 'v' )
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
	
	if( key >= '1' && key <= '9' )		// need to work out how to make the limit hsvTotal.
	{
		hsvIndex	= key - '0' - 1;
		hsvIndex	= MIN( hsvIndex, noColorsTracked - 1 );
		hsvIndex	= MAX( hsvIndex, 0 );
	}
	
	if( key == 'i' )
	{
		hsv[ hsvIndex ].bShowImg = !hsv[ hsvIndex ].bShowImg;
	}
	
	if( key == 'c' )
	{
		hsv[ hsvIndex ].bShowContour = !hsv[ hsvIndex ].bShowContour;
	}
	
	if( key == 'w' )
	{
		writeToFile();
	}
	
	if( key == 'l' )
	{
		loadFromFile();
	}
}

void ColorTracker :: mouseMoved( int x, int y )
{
	//
}

void ColorTracker :: mouseDragged( int x, int y, int button )
{
	//
}

void ColorTracker :: mousePressed( int x, int y, int button )
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

void ColorTracker :: mouseReleased ()
{
	//
}
