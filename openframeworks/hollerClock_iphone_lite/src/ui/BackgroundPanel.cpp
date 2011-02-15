/*
 *  BackgroundPanel.cpp
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 11/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BackgroundPanel.h"

BackgroundPanel :: BackgroundPanel ( ofxFlashMovieClip* asset )
{
	this->asset = asset;
	
	ix = asset->x();
	iy = asset->y();
	
	bOnScreen = true;
}

BackgroundPanel :: ~BackgroundPanel ()
{

}

void BackgroundPanel :: checkTileIsOnScreen( ofRectangle& screenRect )
{
	bool b1 = screenRect.inside( ofPoint( rect.x, rect.y ) );
	bool b2 = screenRect.inside( ofPoint( rect.x + rect.width, rect.y ) );
	bool b3 = screenRect.inside( ofPoint( rect.x + rect.width, rect.y + rect.height ) );
	bool b4 = screenRect.inside( ofPoint( rect.x, rect.y + rect.height ) );
	
	bOnScreen = b1 || b2 || b3 || b4;
	
	asset->visible( bOnScreen );
}