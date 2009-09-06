/*
 *  ofxScreenGrabUtil.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

class ofxScreenGrabUtil
{
public :
	ofxScreenGrabUtil();
	~ofxScreenGrabUtil();
	
	void setup( string name = "", ofRectangle *rect = NULL );
	void start();
	void save();
	void stop();
	bool isRecording();
	
	void setArea( ofRectangle *rect );
	
private :
	
	ofImage image;
	
	string fileName;
	bool recording;
	int counter;

	ofRectangle saveArea;
};