#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofEvents.h"

#include "ofxFlash.h"

#include "EventData.h"
#include "EventItem.h"
#include "EventImageItem.h"
#include "EventVideoItem.h"
#include "EventTextItem.h"

#include "TestBtn.h"

class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();

	void btnPressed			( int & btnId );
	void itemCloseHandler	( int & itemId );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	EventData			eventData;
	vector<EventItem*>	eventItems;
	vector<TestBtn*>	btns;
	
	ofxStage			stage;
};

#endif
