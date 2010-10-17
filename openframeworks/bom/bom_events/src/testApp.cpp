#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	stage.addListeners();
	
	eventData.loadFile( "events.xml" );
	
	for( int i=0; i<eventData.items.size(); i++ )
	{
		const EventDataItem& data = eventData.items[ i ];
		
		EventItem* item;
		if( data.type == EVENT_TYPE_IMAGE )
		{
			item = new EventImageItem();
		}
		else if( data.type == EVENT_TYPE_VIDEO )
		{
			item = new EventVideoItem();
		}
		else if( data.type == EVENT_TYPE_TEXT )
		{
			item = new EventTextItem();
		}
		
		item->populate( data, i );
		item->setup();
		item->hide();
		
		ofAddListener( item->closeEvent, this, &testApp :: itemCloseHandler );
		
		eventItems.push_back( item );
		
		//---
		
		TestBtn* btn;
		btn = new TestBtn();
		btn->setPos( 20 + i * 30, ofGetHeight() - 40 );
		btn->setSize( 20, 20 );
		btn->enableMouseEvents();
		btn->populate( i );
		
		ofAddListener( btn->btnPressEvent, this, &testApp :: btnPressed );
		
		btns.push_back( btn );
		
		//---
		
		stage.addChild( item );
	}
	
	eventItems[ 0 ]->show();
}

void testApp :: btnPressed ( int & btnId )
{
	EventItem* item;
	
	for( int i=0; i<eventItems.size(); i++ )
	{
		item = eventItems[ i ];
		
		if( i == btnId )
		{
			item->show();
		}
		else
		{
			item->hide();
		}
	}
}

void testApp :: itemCloseHandler ( int & itemId )
{
	eventItems[ itemId ]->hide();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{

}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{

}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{

}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

