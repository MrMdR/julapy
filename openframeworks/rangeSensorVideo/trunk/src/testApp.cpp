#include "testApp.h"


//--------------------------------------------------------------
// INIT.
//--------------------------------------------------------------

void testApp :: setup()
{ 
	ofBackground(255,255,255);
	
	initSerial();
	initVideo();
}

void testApp :: initSerial ()
{
	ofSetVerticalSync(true);
	
	serial.enumerateDevices();
	
	//----------------------------------- note:
	serial.setup("/dev/tty.usbserial-A70064qu", 9600);		// < this should be set
	// to whatever com port
	// your serial device is 
	// connected to.  
	// (ie, COM4 on a pc, dev/tty.... on a mac)
	// arduino users check in arduino app....
	//----------------------------------- 
	
	font.loadFont("DIN.otf", 20);
	
	full = 0;
	bMoviePlay = false;
	posVal = 0;
	memset(values, 0, 640 * sizeof(float));
	state = 0;
}

void testApp :: initVideo ()
{
//	videoPlayer.loadMovie( "movies/onthebalcony_30fps_450x300_jpg.mov" );
	videoPlayer.loadMovie( "movies/decompose_jpg.mov" );
	videoPlayer.setPaused( true );
}

//--------------------------------------------------------------
// UPDATE.
//--------------------------------------------------------------

void testApp :: update()
{
	updateSerial();
	updateVideo();
}

void testApp :: updateSerial ()
{
	// (1) write the letter "!" to serial - 
	//this will tell the arduino we ready to get data:
	serial.writeByte('!');
	
	// (2) read
	// we try to read 2 bytes
	
	//clear our variables
	int nTimesRead  = 0;
	int nBytesRead  = 0;
	int nRead		= 0;  // a temp variable to keep count per read
	
	//we are going to read a 0 - 1024 number as two bytes
	//we need a buffer to store the two bytes and a second 
	//buffer with space for the terminating zero byte 
	unsigned char bytesReturned[2];
	unsigned char bytesReadString[3];
	
	
	//clear our buffers
	memset(bytesReadString, 0, 3);
	memset(bytesReturned, 0, 2);
	
	//we read as much as possible so we make sure we get the newest data
	while( (nRead = serial.readBytes( bytesReturned, 2)) > 0){
		nTimesRead++;	
		nBytesRead = nRead;
	}
	
	//if we have got both bytes
	if( nBytesRead == 2){
		
		//lets update our buffer
		memcpy(bytesReadString, bytesReturned, 2);
		
		//we need to put the bytes back into an int
		int num = ( (unsigned char)bytesReadString[1] << 8 | (unsigned char)bytesReadString[0] );
		
		//the number as a string
		message = "1st byte:"+ ofToString( (unsigned char)bytesReadString[0] ) + "  2nd byte:" + ofToString( (unsigned char)bytesReadString[1] ) +" = "+ofToString(num);
		
		valueIn = num;
		
		videoPosition += ( ( (float)valueIn/ (float)200.0 ) - videoPosition ) * 0.2;
		
//		//smooth the value a little - take 25% of the incomming value
//		pct *= 0.75;
//		pct += 0.25 * ((float)num/1023.0);
//		
//		//this is hack so the movie doesn't loop around
//		if(pct < 0.001) pct = 0.001;
//		if(pct > 0.999) pct = 0.999;
//		
//		normalizedValue = pct;
//		smoothedValue	= pct * 1023;
//		
//		//set the 1st video position based on 
//		//the value coming in
//		VID.setPosition(pct);
//		
//		//for our value graph
//		if(posVal >= 640) posVal -= 640;
//		values[posVal] = normalizedValue;
//		posVal++;
//		
//		
//		//for our trigger video
//		
//		//our high trigger value for the video
//		if(smoothedValue > 900){
//			//only trigger the video once
//			if( state == 0 ){
//				state = 1;
//				VID2.setPosition(0);
//				VID2.setSpeed(1);
//			}
//		}else{
//			state = 0;
//		}
//		
//		//stop our video when it gets to the end
//		if(VID2.getPosition() > 0.95)
//		{
//			VID2.setSpeed(0);
//		}
	}
}

void testApp :: updateVideo ()
{
	videoPlayer.setPosition( videoPosition );
}


//--------------------------------------------------------------
// DRAW.
//--------------------------------------------------------------

void testApp :: draw()
{
	drawSerial();
	drawVideo();
}

void testApp :: drawSerial ()
{
	string str2 = message+"\n";
	
	string str = "";
	
	str += "number is "+ofToString(valueIn)+"\n";
//	str += "smoothed is "+ofToString(smoothedValue, 2)+"\n";
//	str += "normalized is "+ofToString(normalizedValue, 7)+"\n";
	
	ofSetColor(0x000000);
	
	for(int i = 0; i < 640; i++){
		ofLine(i, 480, i, 480 - values[i] * 100);
	}	
	
	
//	if(smoothedValue > 900){
//		ofSetColor(0x00DD22);
//		ofRect(380,230, 32, 32);
//		ofSetColor(0xFFFF00);		
//		ofRect(382,232, 28, 28);
//		
//		ofSetColor(0x00DD22);		
//		str += "state is high\n";
//	}
//	else if(smoothedValue < 100){
//		ofSetColor(0x00DD22);
//		ofRect(380,230, 32, 32);
//		ofSetColor(0x005508);		
//		ofRect(382,232, 28, 28);
//		
//		ofSetColor(0x00DD22);		
//		str += "state is low\n";
//	}
//	else{
//		ofSetColor(0x00DD22);
//		ofRect(382,230, 32, 32);
//		
//		ofSetColor(0x00DD22);
//		str += "state is middle\n";
//	}
	
	ofSetColor(0x00DD22);
	
	font.drawString(str2, 50, 535);	
	font.drawString(str, 50, 580);
}

void testApp :: drawVideo ()
{
	ofSetColor(0xFFFFFF);
	
    videoPlayer.draw( 0, 0 );
}

//--------------------------------------------------------------
// HANDLERS.
//--------------------------------------------------------------

void testApp::keyPressed  (int key)
{ 
	//
}

void testApp::keyReleased(int key)
{ 
	//
}

void testApp::mouseMoved(int x, int y )
{
//	int width = ofGetWidth();
//	videoPosition = (float)x / (float)width;
}

void testApp::mouseDragged(int x, int y, int button)
{
	//
}

void testApp::mousePressed(int x, int y, int button)
{
	//
}

void testApp::mouseReleased(int x, int y, int button)
{
	//
}

void testApp::windowResized(int w, int h)
{
	//
}

