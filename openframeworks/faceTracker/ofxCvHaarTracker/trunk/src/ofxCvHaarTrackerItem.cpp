/*
 *  FaceItem.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#include "ofMain.h"
#include "ofxCvHaarTrackerItem.h"

ofxCvHaarTrackerItem :: ofxCvHaarTrackerItem()
{
	itemID			= 0;
	
	isMatch			= true;
	
	isIdle			= false;
	idleCount		= 0;
	idleCountLimit	= 10;
	
	rectBufferCount	= 0;
	rectBufferMin	= 7;
	rectBufferLimit	= 20;
	
	centreDistanceLimit	= 50;
}

ofxCvHaarTrackerItem :: ~ofxCvHaarTrackerItem()
{
	clearRectBuffer();
}

void ofxCvHaarTrackerItem :: setID( int hid )
{
	itemID = hid;
}

int ofxCvHaarTrackerItem :: getID ( )
{
	return itemID;
}

void ofxCvHaarTrackerItem :: set( const ofRectangle& haarRect )
{
	int i;
	
	for( i=0; i<rectBufferLimit; i++ )
	{
		rectBuffer.push_back( ofRectangle() );
		rectBuffer.back().x			= haarRect.x;
		rectBuffer.back().y			= haarRect.y;
		rectBuffer.back().width		= haarRect.width;
		rectBuffer.back().height	= haarRect.height;
	}
	
	rect.x			= haarRect.x;
	rect.y			= haarRect.y;
	rect.width		= haarRect.width;
	rect.height		= haarRect.height;

	rectEase.x		= haarRect.x;
	rectEase.y		= haarRect.y;
	rectEase.width	= haarRect.width;
	rectEase.height = haarRect.height;
}

void ofxCvHaarTrackerItem :: add( const ofRectangle& haarRect )
{
	rectBuffer.erase( rectBuffer.begin() );
	rectBuffer.push_back( ofRectangle() );
	rectBuffer.back().x			= haarRect.x;
	rectBuffer.back().y			= haarRect.y;
	rectBuffer.back().width		= haarRect.width;
	rectBuffer.back().height	= haarRect.height;
	
	rect.x		= haarRect.x;
	rect.y		= haarRect.y;
	rect.width	= haarRect.width;
	rect.height = haarRect.height;
}

bool ofxCvHaarTrackerItem :: checkItem( const ofRectangle& haarRect )
{
	isMatch = checkAgainstCentreDistance
	(
		haarRect.x + haarRect.width  * 0.5,
		haarRect.y + haarRect.height * 0.5
	);
	
	return isMatch;
}

bool ofxCvHaarTrackerItem :: checkAgainstCentreDistance ( float cx, float cy )
{
	float dx, dy;
	float len;
	
	dx = ( rect.x + rect.width  * 0.5 ) - cx;
	dy = ( rect.y + rect.height * 0.5 ) - cy;

	len = sqrt( dx * dx + dy * dy );
	
	return ( len < centreDistanceLimit );
}

bool ofxCvHaarTrackerItem :: hasBeenMatched ()
{
	return isMatch;
}

void ofxCvHaarTrackerItem :: update()
{
	if( isMatch )
	{
		if( rectBufferCount < rectBufferLimit )
		{
			++rectBufferCount;
		}
	}
	else
	{
		++idleCount;
		
		if( idleCount >= idleCountLimit )
		{
			isIdle = true;
		}
	}
	
	isMatch = false;
}

void ofxCvHaarTrackerItem :: easeItemPosition ( float easeScale )
{
	rectEase.x += ( rect.x - rectEase.x ) * easeScale;
	rectEase.y += ( rect.y - rectEase.y ) * easeScale;
}

void ofxCvHaarTrackerItem :: easeItemSize ( float easeScale )
{
	rectEase.width  += ( rect.width  - rectEase.width )  * easeScale;
	rectEase.height += ( rect.height - rectEase.height ) * easeScale;
}

bool ofxCvHaarTrackerItem :: isItemIdle ()
{
	return isIdle;
}

bool ofxCvHaarTrackerItem :: isValid()
{
	return ( rectBufferCount > rectBufferMin );
}

void ofxCvHaarTrackerItem :: clearRectBuffer ()
{
	int i;
	
	for( i=0; i<rectBuffer.size(); i++ )
	{
		rectBuffer.erase( rectBuffer.begin() + i );
	}
}