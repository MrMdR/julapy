/*
 *  OscPad.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#define ORIENTATE_LANDSCAPE		1
#define ORIENTATE_PORTRAIT		2

#define ROWS_DEFAULT		3
#define ROWS_MIN			1
#define ROWS_MAX			10
#define COLUMNS_DEFAULT		5
#define COLUMNS_MIN			1
#define COLUMNS_MAX			10

#define PORT 12345

#include "ofxOsc.h"

class OscPad
{

public :
	
	OscPad();
	~OscPad();

	void init();
	void setOffOnRelease( bool offOnRelease = false );
	void setOrientation( int orientation = ORIENTATE_LANDSCAPE );
	void setNumberOfRows( int rows = ROWS_DEFAULT );
	void setNumberOfColumns( int columns = COLUMNS_DEFAULT );
	void update();
	bool getButtonState( int x=0, int y=0 );
	
private :	
	
	ofxOscReceiver	receiver;
	int				orientation;
	int				rows;
	int				columns;
	bool			offOnRelease;
	string			addresses[ COLUMNS_MAX ][ ROWS_MAX ];
	bool			states[ COLUMNS_MAX ][ ROWS_MAX ];
};