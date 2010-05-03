/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#include "ofxCvHaarTracker.h"

ofxCvHaarTracker ::  ofxCvHaarTracker()	{ }
ofxCvHaarTracker :: ~ofxCvHaarTracker()	{ }

ofxCvHaarFinder* ofxCvHaarTracker :: setup( string haarFile )
{
	haarFinder = new ofxCvHaarFinder();
	haarFinder->setup( haarFile );
	
	init();
	
	return haarFinder;
}

ofxCvHaarFinder* ofxCvHaarTracker :: setup( ofxCvHaarFinder* hf )
{
	haarFinder = hf;
	
	init();
	
	return haarFinder;
}

void ofxCvHaarTracker :: init ()
{
	haarItemIndex			= 0;
	haarItemIteratorIndex	= 0;
	
	haarItemIDCounter		= 10;
	haarItemIDCounterLimit	= 99;
}

void ofxCvHaarTracker :: findHaarObjects( ofxCvGrayscaleImage &image )
{
	haarFinder->findHaarObjects( image );
	
	for( int i=0; i<haarFinder->blobs.size(); i++ )
	{
		ofRectangle haarRect;
		
		haarRect.x		= haarFinder->blobs[ i ].boundingRect.x;
		haarRect.y		= haarFinder->blobs[ i ].boundingRect.y;
		haarRect.width	= haarFinder->blobs[ i ].boundingRect.width;
		haarRect.height = haarFinder->blobs[ i ].boundingRect.height;
		
		bool isMatch = false;
		
		for( int j=0; j<haarItems.size(); j++ )
		{
			if( isMatch )
			{
				break;
			}
				
			if( !haarItems[ j ].hasBeenMatched() )
			{
				isMatch = haarItems[ j ].checkItem( haarRect );
				
				if( isMatch )
				{
					haarItems[ j ].add( haarRect );
				}
			}
		}
		
		if( !isMatch )
		{
			ofxCvHaarTrackerItem haarItem = ofxCvHaarTrackerItem();
			haarItem.set( haarRect );
			haarItem.setID( haarItemIDCounter );
			
			haarItems.push_back( haarItem );
		}
		
		++haarItemIDCounter;
		if( haarItemIDCounter > haarItemIDCounterLimit )
		{
			haarItemIDCounter = 10;
		}
	}
	
	for( int j=0; j<haarItems.size(); j++ )
	{
		haarItems[ j ].update();
		haarItems[ j ].easeItemPosition( 0.7 );
		haarItems[ j ].easeItemSize( 0.3 );
		
		if( haarItems[ j ].isItemIdle() )
		{
			haarItems.erase( haarItems.begin() + j );
		}
	}
}

bool ofxCvHaarTracker :: hasNextHaarItem ()
{
	bool b;
	
	b = true;
	
	while( haarItemIteratorIndex < haarItems.size() && !haarItems[ haarItemIteratorIndex ].isValid() )
	{
		++haarItemIteratorIndex;
	}
	
	b = ( haarItemIteratorIndex < haarItems.size() );
	
	if( b )
	{
		haarItemIndex = haarItemIteratorIndex;
		++haarItemIteratorIndex;
	}
	else
	{
		haarItemIndex			= 0;
		haarItemIteratorIndex	= 0;
	}
	
	return b;
}

int ofxCvHaarTracker :: getHaarItemID ()
{
	return haarItems[ haarItemIndex ].getID();
}

void ofxCvHaarTracker :: getHaarItemRect ( ofRectangle& haarRect )
{
	haarRect.x		= haarItems[ haarItemIndex ].rect.x;
	haarRect.y		= haarItems[ haarItemIndex ].rect.y;
	haarRect.width	= haarItems[ haarItemIndex ].rect.width;
	haarRect.height	= haarItems[ haarItemIndex ].rect.height;
}

void ofxCvHaarTracker :: getHaarItemRectEased ( ofRectangle& haarRect )
{
	haarRect.x		= haarItems[ haarItemIndex ].rectEase.x;
	haarRect.y		= haarItems[ haarItemIndex ].rectEase.y;
	haarRect.width	= haarItems[ haarItemIndex ].rectEase.width;
	haarRect.height	= haarItems[ haarItemIndex ].rectEase.height;
}

void ofxCvHaarTracker :: clearHaarItems ()
{
	haarItems.clear();
}
