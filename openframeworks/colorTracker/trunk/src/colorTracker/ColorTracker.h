/*
 *  ColorTracker.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 6/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COLOR_TRACKER
#define COLOR_TRACKER

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxNetwork.h"
#include "ofxOpenCv.h"
#include "laserTracking.h"
#include "ofContourAnalysis.h"
#include "contourSimplify.h"

#include <fstream>

#define		MAX_NUM_CONTOURS_TO_FIND	20

struct HSVData
{
	float	hue, sat, val;
	float	hueRange, satRange, valRange;
	
	int		minHue, maxHue;
	int		minSat, maxSat;
	int		minVal, maxVal;
	
	ofxCvGrayscaleImage		img;
	unsigned char *         imgPixels;
	
	bool	bShowImg;
	bool	bShowContour;
};

struct ContourData
{
	int						nBlobs;
	
	vector<float>			blobArea;
	vector<ofRectangle>		blobBoundingRect;
	vector<ofPoint>			blobCentroid;
	
	CvBox2D32f				*box;
	float					*blobAngle;
	
	vector<ofxPoint4f>      geomLines;
	
	vector<ofxPoint2f>		*contourReg;
	vector<ofxPoint2f>		*contourSmooth;
	vector<ofxPoint2f>		*contourSimple;
	vector<ofxPoint2f>		*contourHull;
};

struct ColorTrackerData
{
	bool  active;
	float cx;
	float cy;
	float width;
	float height;
	float angle;
};


class ColorTracker
{

public :
	
	void init	( int noColorsTracked );
	void update	();
	void draw	();
	
	void initContourAnalysis	();
	void initTCP				();
	void initGui				();
	
	void updateTrackerData		();
	void sendTrackerData		();
	
	const ColorTrackerData*	getTrackerData		();
	int						getNoColorsTracked	();
	
	void computeContourAnalysis ( int i );
	void drawContourAnalysis	( int i );
	
	void writeToFile	( string filename = "config" );
	void loadFromFile	( string filename = "config" );
	
	void keyPressed		( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	();
	
	int						noColorsTracked;
	
	ofVideoGrabber			video;
	ofRectangle				videoRect;
	
	ofxCvColorImage		    colImg;
	ofxCvColorImage		    hsvImg;
	ofxCvGrayscaleImage		hueImg;
	ofxCvGrayscaleImage		satImg;
	ofxCvGrayscaleImage		valImg;
	
	ofxTCPServer			TCP;
	
	int	blur;
	int	threshold;
	
	HSVData				*hsv;
	int					hsvTotal;
	int					hsvIndex;
	
	ContourData			*cdata;
	int					cdataTotal;
	
	ColorTrackerData	*data;
	
	ofxCvContourFinder		contourFinder;
	contourSimplify			contourS;
	
	float					smoothPct;
	float					tolerance;
	
	int						cfDetail;
	float					cfMinArea;
	float					cfMaxArea;
	
	bool					bShowRect;
	bool					bShowBox;
	bool					bShowEllipse;
	bool					bShowAngle;
	bool					bShowLines;
	
protected:
	
	laserTracking LT;	
};

#endif