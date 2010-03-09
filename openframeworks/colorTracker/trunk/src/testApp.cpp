#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	
	videoRect.width		= 320;
	videoRect.height	= 240;
	
	video.setVerbose(true);
	video.initGrabber( videoRect.width, videoRect.height );

	colImg.allocate( videoRect.width, videoRect.height );				// Live Cam as input source
    hsvImg.allocate( videoRect.width, videoRect.height );				// HSV Image
    hueImg.allocate( videoRect.width, videoRect.height );				// Hue Image
    satImg.allocate( videoRect.width, videoRect.height );				// Saturation Image
    valImg.allocate( videoRect.width, videoRect.height );				// value Image
	trkImg.allocate( videoRect.width, videoRect.height );				// tracker image.
	blrImg.allocate( videoRect.width, videoRect.height );				// blured image.

    trkImgPixels = new unsigned char [ (int)videoRect.width * (int)videoRect.height ];	// tracker image pixels.

	hue			= 0.0;
	hueWidth	= 0.06;
	sat			= 0.62;
	satWidth	= 0.94;
	val			= 0.8;
	valWidth	= 0.62;
	
	blur		= 0;
	threshold	= 20;
	
	initContourAnalysis();
	initGui();
}

void testApp :: initGui ()
{
	gui.addTitle( "HSV properties" );
	gui.addSlider( "hue       ", hue,		0.0, 1.0 );
	gui.addSlider( "hueWidth  ", hueWidth,  0.0, 1.0 );
	gui.addSlider( "sat       ", sat,		0.0, 1.0 );
	gui.addSlider( "satWidth  ", satWidth,	0.0, 1.0 );
	gui.addSlider( "val       ", val,		0.0, 1.0 );
	gui.addSlider( "valWidth  ", valWidth,	0.0, 1.0 );

	gui.addPage();
	gui.addTitle( "image processing" );
	gui.addSlider( "blur      ", blur,		0.0, 30.0  );
	gui.addSlider( "threshold ", threshold,	0.0, 255.0 );

	gui.addPage();
	gui.addTitle( "contour analysis" );
	gui.addSlider( "contour detail  ", cfDetail, 0, 3 );
	gui.addToggle( "contour ellipse ", bShowEllipse   );
	gui.addToggle( "contour angle   ", bShowAngle     );
	gui.addToggle( "contour lines   ", bShowLines     );

	gui.loadFromXML();
	
	gui.show();
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground( 100,100,100 );

	video.grabFrame();
	
	if( video.isFrameNew() )
	{
		minHue = max( ( hue - hueWidth * 0.5) * 255, 000.0);
		maxHue = min( ( hue + hueWidth * 0.5) * 255, 255.0);
		minSat = max( ( sat - satWidth * 0.5) * 255, 000.0);
		maxSat = min( ( sat + satWidth * 0.5) * 255, 255.0);
		minVal = max( ( val - valWidth * 0.5) * 255, 000.0);
		maxVal = min( ( val + valWidth * 0.5) * 255, 255.0);
	
		colImg.setFromPixels( video.getPixels(), videoRect.width, videoRect.height );	
		
		hsvImg = colImg;
		hsvImg.convertRgbToHsv();
		hsvImg.convertToGrayscalePlanarImages( hueImg, satImg, valImg );
	
		unsigned char * huePixels = hueImg.getPixels();
		unsigned char * satPixels = satImg.getPixels();
		unsigned char * valPixels = valImg.getPixels();
		int nPixels = videoRect.width * videoRect.height;
	
		for( int i=0; i<nPixels; i++ )
		{
			if
			(
				( huePixels[ i ] >= minHue && huePixels[ i ] <= maxHue ) &&
				( satPixels[ i ] >= minSat && satPixels[ i ] <= maxSat ) &&
				( valPixels[ i ] >= minVal && valPixels[ i ] <= maxVal )
			)
			{
				trkImgPixels[ i ] = 255;
			}
			else
			{
				trkImgPixels[ i ] = 0;
			}
		}
		
		trkImg.setFromPixels( trkImgPixels, videoRect.width, videoRect.height );
		
		blrImg = trkImg;
		blrImg.blur( blur );
		blrImg.threshold( threshold );
		
		computeContourAnalysis();
		
		LT.calcColorRange( hue, hueWidth, sat, satWidth, val );
	}
}

/////////////////////////////////////////////
//	CONTOUR ANALYSIS
/////////////////////////////////////////////

void testApp :: initContourAnalysis ()
{
	contourReg		= new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourSmooth	= new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourSimple	= new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourHull		= new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	box				= new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
	blobAngle		= new float[MAX_NUM_CONTOURS_TO_FIND];
	smoothPct		= 0.13f;
	tolerance		= 20.0f;
	
	cfDetail		= 0;
	bShowEllipse	= false;
	bShowAngle		= false;
	bShowLines		= false;
}

void testApp :: computeContourAnalysis ()
{
	int runningBlobs;
	runningBlobs = contourFinder.findContours( blrImg, 100, 9999999, MAX_NUM_CONTOURS_TO_FIND, false, false );

	if( runningBlobs == 0 )
		return;
	
	for( int i=0; i<contourFinder.nBlobs; i++ )
	{
		int length_of_contour = contourFinder.blobs[ i ].pts.size();
		
		fitEllipse( contourFinder.blobs[i].pts, box[i] );
		
		blobAngle[i] = getOrientation(contourFinder.blobs[i].pts);
		
		findLines( contourFinder.blobs[i].pts,geomLines,30,40,30 );
		
		contourReg[i].clear();
		contourReg[i].assign(length_of_contour, ofxPoint2f());
		contourSmooth[i].clear();
		contourSmooth[i].assign(length_of_contour, ofxPoint2f());
		
		for(int j = 0; j < length_of_contour; j++){
			contourReg[i].at(j) = contourFinder.blobs[i].pts[j];
		}
		
		contourS.smooth(contourReg[i], contourSmooth[i], smoothPct);
		contourSimple[i].clear();
		
		contourS.simplify(contourSmooth[i], contourSimple[i], tolerance);
		contourS.convexHull(contourSimple[i], contourHull[i]);
	}
}

void testApp :: drawContourAnalysis()
{
	ofFill();
	ofSetColor(0x333333);
	ofRect(0,0,320,240);
	ofSetColor(0xffffff);
	
	for( int i=0; i<contourFinder.nBlobs; i++ )
	{
		//-------------------  draw the contours
		if(cfDetail == 0){
			ofSetColor(0xff0000);
			ofNoFill();
			drawBlob(0,0,contourFinder.blobs[i]);
		}else if(cfDetail == 1){
			ofSetColor(0xff0000);
			ofNoFill();
			ofBeginShape();
			for(unsigned int j = 0; j < contourSmooth[i].size(); j++){
				ofVertex(contourSmooth[i].at(j).x, contourSmooth[i].at(j).y);
			}
			ofEndShape(true);
		}else if(cfDetail == 2){
			ofSetColor(0xff0000);
			ofNoFill();
			ofBeginShape();
			for(unsigned int k = 0; k < contourSimple[i].size(); k++){
				ofVertex(contourSimple[i].at(k).x, contourSimple[i].at(k).y);
			}
			ofEndShape(true);
		}else if(cfDetail == 3){
			ofSetColor(0x220000);
			ofNoFill();
			ofBeginShape();
			for(unsigned int z = 0; z < contourHull[i].size(); z++){
				ofVertex(contourHull[i].at(z).x, contourHull[i].at(z).y);
			}
			ofEndShape(true);
			for(unsigned int w = 0; w < contourHull[i].size(); w++){
				ofCircle(contourHull[i].at(w).x, contourHull[i].at(w).y, 2);
			}
		}
		//------------------- fit ellipse to that blob and draw it
		if(bShowEllipse){
			
			ofNoFill();
			ofSetColor(0xffffff);
			
			glPushMatrix();
			glTranslatef(box[i].center.x, box[i].center.y,0);
			glRotatef(box[i].angle,0,0,1);
			ofEllipse( 0,0, box[i].size.width*.5,box[i].size.height*.5);
			glPopMatrix();
		}
		//------------------- fit angle of orientation
		if(bShowAngle){
			ofSetColor(0xffffff);
			
			float x1,y1,x2,y2;
			
			x1 = contourFinder.blobs[i].centroid.x + 25 * cos(blobAngle[i]);
			y1 = contourFinder.blobs[i].centroid.y + 25 * sin(blobAngle[i]);
			x2 = contourFinder.blobs[i].centroid.x - 25 * cos(blobAngle[i]);
			y2 = contourFinder.blobs[i].centroid.y - 25 * sin(blobAngle[i]);
			
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
			
			x1 = contourFinder.blobs[i].centroid.x + 10 * cos(blobAngle[i]+HALF_PI);
			y1 = contourFinder.blobs[i].centroid.y + 10 * sin(blobAngle[i]+HALF_PI);
			x2 = contourFinder.blobs[i].centroid.x - 10 * cos(blobAngle[i]+HALF_PI);
			y2 = contourFinder.blobs[i].centroid.y - 10 * sin(blobAngle[i]+HALF_PI);
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
		}
		//------------------- fit geometry lines on countour
		if(bShowLines){
			ofSetColor(0x8aff00);
			ofNoFill();
			for(unsigned int j = 0; j < geomLines.size(); j++){
				ofLine(geomLines[j].x,geomLines[j].y,geomLines[j].z,geomLines[j].w);
				ofCircle(geomLines[j].x,geomLines[j].y,3);
				ofCircle(geomLines[j].z,geomLines[j].w,3);
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(0xffffff);
	
	glPushMatrix();
	glTranslatef( 290, 110, 0 );
	
	int pad;
	pad = 10;
	
	int w;
	w = videoRect.width + pad;
	
	int h;
	h = videoRect.height + pad;
	
	colImg.draw( w * 0, h * 0 );
	hsvImg.draw( w * 1, h * 0 );
	trkImg.draw( w * 2, h * 0 );
	blrImg.draw( w * 2, h * 1 );
	
	glPushMatrix();
	glTranslatef( w * 1, h, 0 );

	int x;
	x = 140;
	
	int y;
	y = 10;
	
	int dy;
	dy = 15;
	
	LT.drawColorRange( 0, 0, 120, 44 );
	
	ofSetColor( 0xffffff );
	ofDrawBitmapString( "minHue: " + ofToString( minHue ), x, y       );
	ofDrawBitmapString( "maxHue: " + ofToString( maxHue ), x, y += dy );
	ofDrawBitmapString( "minSat: " + ofToString( minSat ), x, y += dy );
	ofDrawBitmapString( "maxSat: " + ofToString( maxSat ), x, y += dy );
	ofDrawBitmapString( "minVal: " + ofToString( minVal ), x, y += dy );
	ofDrawBitmapString( "maxVal: " + ofToString( maxVal ), x, y += dy );
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( w * 0, h * 1, 0 );
		drawContourAnalysis();
	glPopMatrix();
	
	glPopMatrix();
	
	gui.draw();
}


//--------------------------------------------------------------
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
void testApp::mouseReleased(){
}
