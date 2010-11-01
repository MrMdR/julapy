#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
#include "ofxResizeUtil.h"
#include "ofxContourUtil.h"
#include "ofxCvContourSimplify.h"
#include "ofxColorPicker.h"
#include "ofxScreenGrabUtil.h"
#include "ofxTileSaver.h"
#include "ofxFlash.h"

#include "BomAnimation.h"
#include "Slider.h"
#include "PlayBtn.h"
#include "TempRainBtn.h"

class Blob : public ofxCvBlob
{
public:
	ofColor	color;
	int		layer;
};

class testApp : public ofBaseApp
{

public:
	
	void setup	();
	void update	();
	void draw	();

	void initUI			();
	void initBomAnim	();
	void initAusMap		();
	void initOpenCv		();
	void initContours	();
	void initGui		();
	void initColor		();
	
	void updateBomAnim			();
	void updateOpenCv			();
	void updateBlobs			();
	int  updateContours			( ofxCvGrayscaleImage& image );
	void copyBlob				( ofxCvBlob& blob, ofxCvBlob& blobCopy, float xoff = 0, float yoff = 0, float scale = 1.0 );
	void updateColor			();
	ofColor interpolateColor	( const ofColor& c1, const ofColor& c2, float p );
	
	void drawBorder					( const ofRectangle& rect );
	void drawBomAnim				();
	void drawBomImage				();
	void drawMapImage				( ofImage& image, bool bUseAlpha );
	void drawUI						();
	void drawBand					();
	void drawBandSum				();
	void drawContoursSmall			();
	void drawContoursLarge			();
	void drawContourBoundingBoxes	( vector<Blob>& blobs );
	void drawContourPoints			( vector<Blob>& blobs );
	void drawContourLines			( vector<Blob>& blobs, bool useBlobColor = false );
	void drawContourCurveLines		( vector<Blob>& blobs, bool useBlobColor = false );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	bool					bDebug;
	bool					bSmoothing;
	bool					bPause;
	bool					bDrawPoints;
	bool					bDrawLines;
	bool					bDrawCurves;
	bool					bDrawSimplified;
	bool					bDrawColor;
	bool					bRotateColor;

	ofRectangle				screenRect;
	ofRectangle				sourceRect;
	ofRectangle				largeRect;
	ofRectangle				smallRect;
	ofRectangle				debugRect;
	
	BomAnimation			bomTempAnim;
	BomAnimation			bomRainAnim;
	BomAnimation*			bomAnim;
	ofxCvGrayscaleImage		bomImage;
	float					bomAnimProgress;
	float					bomAninSpeed;
	
	ofImage					ausStroke;
	ofImage					ausMask;
	ofImage					ausCover;
	
	ofxCvGrayscaleImage		bandSum;
	ofxCvGrayscaleImage*	bands;
	float*					bandCutoffs;
	int						bandsTotal;
	int						bandIndex;
	
	ofxCvContourFinder		contourFinder;
	ofxContourUtil			contourUtil;
	ofxCvContourSimplify	contourSimplify;
	vector<Blob>			contourBlobs;
	vector<Blob>			contourBlobsScaled;
	vector<Blob>			contourBlobsSimplified;
	float					contourSmoothScale;
	float					contourSimplifyScale;
	float					contourSimplifyTolerance;
	
	ofxColorPicker			colorPicker0;
	ofxColorPicker			colorPicker1;
	
	ofxStage				stage;
	Slider					slider;
	PlayBtn					playBtn;
	TempRainBtn				tempRainBtn;
	
	ofxScreenGrabUtil		screenGrabUtil;
	ofxTileSaver			tileSaver;
};

#endif
