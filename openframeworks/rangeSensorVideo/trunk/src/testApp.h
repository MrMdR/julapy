#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void initSerial();
		void initVideo();

		void update();
		void updateSerial();
		void updateVideo();

		void draw();
		void drawSerial();
		void drawVideo();
		
		void keyPressed(int key);
		void keyReleased(int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		ofVideoPlayer 		videoPlayer;
		float				videoPosition;
		bool                frameByframe;
		
		ofSerial			serial;
		ofTrueTypeFont		font;

		string				message;				//displays how the data comes in
	
		bool				bMoviePlay;			//for the trigger movie
		int					valueIn;				//the latest number we have recieved 
		float				smoothedValue;		//smooths the incomming value
		float				normalizedValue;		//the value as 0.0 to 1.0 range
		int					state;
		float				pct;				//for bluring over time
	
		int					full;					//whether we are fullscreen video or not 
		bool				isSetup;				//whether we are connected to the device
		float				values[640];
		int					posVal;
};

#endif	

