/*
 *  ofxSRT.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 2/06/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_SRT
#define OFX_SRT

#include "ofMain.h"
#include <fstream>

struct ofxSrtItem
{
	int		id;
	
	string	startTimeStr;
	int		startTimeMs;
	float	startTimeNorm;
	
	string	endTimeStr;
	int		endTimeMs;
	float	endTimeNorm;
	
	float	durationFlt;
	
	vector<string> lines;
};

class ofxSrt
{

public :
	
	 ofxSrt	();
	~ofxSrt	();
	
	void parse				( string srtFilePath );
	string getSubtitleText	( int milliseconds );

	vector<ofxSrtItem>	srtItems;	
	
private :
	
	void cleanLine						( string& lineStr );
	void parseItem						( vector<string>& dataChunk );
	void parseItemTime					( ofxSrtItem& item, string& timeStr );
	void parseItemTimeToMilliSeconds	( string& timeStr, int& ms );
};


#endif