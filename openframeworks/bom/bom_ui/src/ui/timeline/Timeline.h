/*
 *  Timeline.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 21/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "Model.h"
#include "EventDataItem.h"

#include "Btn.h"
#include "TimelineMarker.h"

class Timeline : public ofxSprite
{

public:
	
	Timeline()
	{
		setup();
	}
	
	~Timeline() {}
	
	//==================================================
	
	Model*					model;
	vector<EventDataItem>*	data;
	
	ofImage*		bar;
	ofImage*		scrubber;
	
	ofRectangle		rect;
	ofPoint			scrubberCenter;
	
	Btn				btn;
	float			position;
	float			positionNew;
	float			positionEase;
	
	int				selectedEventID;
	bool			bMarkerPressed;
	
	bool			bTimelinePlay;
	
	vector<TimelineMarker*> markers;
	
	//==================================================
	
	
	void setup ()
	{
		x = 104;
		y = 624;

		position		= 0;
		positionNew		= 0;
		positionEase	= 0.2;
		
		bar			= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar" );
		scrubber	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_scrubber" );

		scrubberCenter.x = 68;
		scrubberCenter.y = 46;
		
		selectedEventID		= -1;
		bMarkerPressed		= false;
		
		rect.x		= 166;
		rect.y		= 625;
		rect.width	= 852;
		rect.height	= 45;
		
		btn.setPos( rect.x, rect.y );
		btn.setSize( rect.width, rect.height );
		
		ofAddListener( btn.btnOverEvent,	this, &Timeline :: btnOver );
		ofAddListener( btn.btnOutEvent,		this, &Timeline :: btnOut );
		ofAddListener( btn.btnPressEvent,	this, &Timeline :: btnPressed );
		
		//-- model.
		
		model = Model :: getInstance();
		
		data			= model->getEventData();
		bTimelinePlay	= model->getTimelinePlay();
		
		ofAddListener( model->timelinePlayChangeEvent, this, &Timeline :: timelinePlayChanged );
		
		//-- markers.
		
		int t = data->size();
		for( int i=0; i<t; i++ )
		{
			EventDataItem& dataItem = data->at( i );
			
			TimelineMarker* marker;
			marker = new TimelineMarker( i );
			marker->populate( dataItem );
			
			float p;
			p = model->getYearAsProgress( dataItem.year );
			
			int mx, my;
			mx = rect.x + ( rect.width - 1 ) * p;
			my = rect.y;
			
			marker->setPos( mx, my );
			
			ofAddListener( marker->markerPressEvent, this, &Timeline :: markerPressed );
			
			markers.push_back( marker );
		}
	}
	
	void timelinePlayChanged ( bool& bTimelinePlay )
	{
		this->bTimelinePlay = bTimelinePlay;
	}
	
	void update ()
	{
		if( bMarkerPressed )
		{
			EventDataItem& dataItem = data->at( selectedEventID );
			positionNew = model->getYearAsProgress( dataItem.year );
			
			float dx;
			dx = ABS( positionNew - position );
			
			if( dx < 0.01 )	// scrubber has arrived at marker position.
			{
				bMarkerPressed = false;
			}
		}
		else if( btn.isMouseDown() )
		{
			positionNew = ( btn.getMouseX() - rect.x ) / (float)rect.width;
			positionNew = ofClamp( positionNew, 0, 1 );
		}
		else
		{
			if( bTimelinePlay )
			{
				positionNew += TIMELINE_PLAY_SPEED;
				
				if( positionNew > 1.0 )
					positionNew = 0;
				
				position = positionNew;
				model->setProgress( position );
				
				checkMarkersAgainstProgress();
				
				return;
			}
		}
		
		position += ( positionNew - position ) * positionEase;
		model->setProgress( position );
		
		if( bMarkerPressed )	// do not check markers against progress if one is selected.
			return;
			
		checkMarkersAgainstProgress();
	}
	
	void checkMarkersAgainstProgress ()
	{
		float range = 0.05;
		bool found	= false;
		
		int t = data->size();
		for( int i=0; i<t; i++ )
		{
			EventDataItem& dataItem = data->at( i );
			float p = model->getYearAsProgress( dataItem.year );
			
			if( p > ( position - range * 0.5 ) && p < ( position + range * 0.5 ) )
			{
				model->setTimelineMarkerPress( dataItem.id );
				
				found = true;
			}
		}
		
		if( !found )
		{
			model->setTimelineMarkerPress( -1 );
		}
	}
	
	void draw ()
	{
		drawBar();
//		drawRect();
//		drawPlayhead();
		drawMarkers();
		drawScrubber();
	}
	
	void drawBar ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		bar->draw( x, y );
		
		ofDisableAlphaBlending();
	}
	
	void drawRect ()
	{
		ofFill();
		ofSetColor( 0xFF0000 );
		ofRect( rect.x, rect.y, rect.width, rect.height );
	}
	
	void drawPlayhead ()
	{
		float w;
		w = 2;
		
		float px;
		px = (int)( rect.x + ( ( rect.width - w ) * position ) );
		
		ofFill();
		ofSetColor( 0x00FF00 );
		ofRect( px, rect.y, w, rect.height );
	}
	
	void drawMarkers ()
	{
		for( int i=0; i<markers.size(); i++ )
		{
			TimelineMarker* marker;
			marker = markers[ i ];
			
			marker->draw();
		}
	}
	
	void drawScrubber ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		float px;
		px = (int)( rect.x + ( rect.width * position ) );
		
		float py;
		py = (int)( rect.y + ( rect.height * 0.5 ) );
		
		scrubber->draw( px - scrubberCenter.x, py - scrubberCenter.y );
		
		ofDisableAlphaBlending();
	}
	
	//==================================================
	
	void btnOver ( int & btnId )
	{
		//
	}
	
	void btnOut ( int & btnId )
	{
		//
	}
	
	void btnPressed ( int & btnId )
	{
		//
	}
	
	//==================================================
	
	void markerPressed ( int & eventID )
	{
		if( selectedEventID == eventID )
			return;
		
		selectedEventID = eventID;
		
		model->setTimelineMarkerPress( eventID );
		
		bMarkerPressed = true;
		
		if( model->getTimelinePlay() )				// stop play when a marker is clicked.
			model->setTimelinePlay( false );
	}
};