/*
 *  EventPanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 22/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "BasePanel.h"
#include "ofxVec2f.h"

#include "Model.h"
#include "EventDataItem.h"
#include "Btn.h"

class EventPanel : public BasePanel
{

public:
	
	EventPanel()
	{
		posIndex	= -1;
		thumbImage	= NULL;
		bg			= NULL;
		iconRain	= NULL;
		iconTemp	= NULL;
		play		= NULL;
		shadow		= NULL;
		target		= NULL;
	}
	
	~EventPanel()
	{
		btn.disableAllEvents();
		
		ofRemoveListener( btn.btnPressEvent, this, &EventPanel :: btnPressEventHandler );
	};
	
	//==================================================

	Model*			model;
	int				tabType;
	
	EventDataItem	data;
	int				posIndex;
	
	ofImage*		thumbImage;
	
	ofImage*		bg;
	ofImage*		iconRain;
	ofImage*		iconTemp;
	ofImage*		play;
	ofImage*		shadow;
	ofImage*		target;
	
	ofPoint			startPoint;
	ofPoint			mapPoint;
	ofPoint			centerPoint;
	ofPoint			bgPoint;
	ofPoint			iconPoint;
	ofPoint			playPoint;
	ofPoint			shadowPoint;
	ofRectangle		whiteBox;
	ofPoint			titlePoint;
	ofPoint			targetPoint;
	
	ofTrueTypeFont	title;
	
	Btn				btn;
	ofEvent<int>	eventPanelPressedEvent;
	
	//==================================================
	
	void populate ( const EventDataItem& data )
	{
		this->data = data;
	}
	
	void setPanelPositionIndex ( int i )
	{
		posIndex = i;
	}
	
	int getPanelPositionIndex ()
	{
		return posIndex;
	}
	
	void setup ()
	{
		model = Model :: getInstance();
		tabType = model->getTabType();
		
		ofPoint latLon;
		latLon.x = data.lon;
		latLon.y = ABS( data.lat );		// made values positive for simplicity.	
		
		mapPoint = model->convertEarthPointToPixelPoint( latLon );
//		x = (int)p.x;
//		y = (int)p.y;
		
		startPoint.x = x;
		startPoint.y = y;
		
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_bg" );
		iconRain	= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_icon_rain" );
		iconTemp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_icon_temp" );
		play		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_play" );
//		shadow		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_shadow" );
		target		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_target" );
		
		thumbImage	= (ofImage*)ofxAssets :: getInstance()->getAssetByFileName( data.thumb );
		
		centerPoint.set( 0, -90 );
		bgPoint.set( -119, -216, 0 );
		iconPoint.set( -81, -50, 0 );
		playPoint.set( -67, -180, 0 );
		shadowPoint.set( -81, -3, 0 );
		titlePoint.set( -45, -46, 0 );
		
		if( target != NULL )
			targetPoint.set( (int)( mapPoint.x - target->width * 0.5 ), (int)( mapPoint.y - target->height * 0.5 ) );
		
		whiteBox.x		= -80;
		whiteBox.y		= -175;
		whiteBox.width	= 155;
		whiteBox.height	= 112;
		
		title.loadFont( "assets/LIBRARY/fonts/Rockwell.ttf", 11 );
		
		btn.setPosAndSize( -91 + x, -186 + y, 177, 182 );
		
		ofAddListener( btn.btnPressEvent,			this, &EventPanel :: btnPressEventHandler );
		ofAddListener( model->tabTypeChangeEvent,	this, &EventPanel :: tabTypeChanged );
	}
	
	void btnPressEventHandler ( int& btnID )
	{
		ofNotifyEvent( eventPanelPressedEvent, this->data.id, this );
	}
	
	void tabTypeChanged ( int& tabType )
	{
		this->tabType = tabType;
	}
	
	void update ()
	{
		BasePanel :: update();
		
		y = startPoint.y + 20 - 20 * alpha;
	}
	
	void draw ()
	{
		ofPoint p1;
		p1.x = x + centerPoint.x;
		p1.y = y + centerPoint.y;
		
		ofPoint p2;
		p2.x = mapPoint.x;
		p2.y = mapPoint.y;
		
		ofxVec2f v;
		v.set( p2.x - p1.x, p2.y - p1.y );
		
		float d;
		d = ofDist( p1.x, p1.y, p2.x, p2.y );
		
		float ang;
		ang = (float)( atan2( p2.y - p1.y, p2.x - p1.x ) * RAD_TO_DEG );
		ang += 90;
		
		glPushMatrix();
		glTranslatef( p1.x, p1.y, 0 );
		glRotatef( ang, 0, 0, 1 );
		
		ofEnableAlphaBlending();
		
		ofFill();
		ofSetColor( 25, 25, 25, 200 * alpha );
		ofBeginShape();
		ofVertex( -8,  0 );
		ofVertex( -1, -d );
		ofVertex(  1, -d );
		ofVertex(  8,  0 );
		ofEndShape( true );
		
		glPopMatrix();
		
		//--
		
		ofSetColor( 255, 255, 255, 255 * alpha );
		
		if( target != NULL )
			target->draw( targetPoint.x, targetPoint.y );
		
		if( bg != NULL )
			bg->draw( x + bgPoint.x, y + bgPoint.y );
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			if( iconRain != NULL )
				iconRain->draw( x + iconPoint.x, y + iconPoint.y );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			if( iconTemp != NULL )
				iconTemp->draw( x + iconPoint.x, y + iconPoint.y );
		}
		
		if( shadow != NULL )
			shadow->draw( x + shadowPoint.x, y + shadowPoint.y );
		
//		ofFill();
//		ofSetColor( 255, 255, 255, 255 * alpha );
//		ofRect( x + whiteBox.x, y + whiteBox.y, whiteBox.width, whiteBox.height );
		
		if( thumbImage != NULL )
			thumbImage->draw( x + whiteBox.x, y + whiteBox.y );
		
		if( play != NULL )
			play->draw( x + playPoint.x, y + playPoint.y );

		drawTitle();
		
//		ofFill();
//		ofSetColor( 0xFF0000 );
//		ofRect( x - 1, y - 1, 3, 3 );
		
		ofDisableAlphaBlending();
	}
	
	void drawTitle ()
	{
		string titleStr;
		titleStr = data.title;
		
		vector<string> lines;
		lines = ofSplitString( titleStr, "/" );
		
		bool bTwoLines;
		bTwoLines = lines.size() == 2;
		
		int tx, ty;
		
		if( bTwoLines )
		{
			tx = x + titlePoint.x + 3;
			ty = y + titlePoint.y + 5;
		}
		else
		{
			tx = x + titlePoint.x + 3;
			ty = y + titlePoint.y + 13;
		}
		
		title.drawString( lines[ 0 ], tx, ty );
		
		if( !bTwoLines )
			return;
			
		title.drawString( lines[ 1 ], tx, ty + 16 );
	}
	
};