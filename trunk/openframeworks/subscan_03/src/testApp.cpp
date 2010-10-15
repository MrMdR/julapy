#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	
	font.loadFont( "fonts/cooperBlack.ttf", 30 );
	
	searchStr		= "";
	bSearchFound	= false;
	
//	srt.parse( "rocky/rocky.srt" );
	srt.parse( "frank/frankenstein.srt" );
	
//	video.loadMovie( "rocky/rocky.mov" );
	video.loadMovie( "frank/frankenstein.mov" );
	video.play();
	video.setPaused( false );
	video.setPosition( videoProgress = 0.0 );
	
	videoRect.width		= video.width;
	videoRect.height	= video.height;
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	int ms = (int)( ( video.getDuration() * 1000 ) * video.getPosition() );
	
	string newSubtitleStr = srt.getSubtitleText( ms );
	
	if( newSubtitleStr != subtitleStr )
	{
		subtitleStr = newSubtitleStr;
		
		if( subtitleStr == "" )
		{
			// remove.
		}
		else 
		{
			// add.
		}
	}
}

void testApp :: addKey ( int key )
{
	keys.push_back( key );
	if( keys.size() > 20 )
		keys.erase( keys.begin() + 0 );
}

void testApp :: createString ()
{
	keyString = "";
	for( int i=0; i<keys.size(); i++ )
	{
		keyString += keys[ i ];
	}
}

void testApp :: updateVideoProgress ( float p )
{
	videoProgress = ofClamp( p, 0, 1 );
	video.setPosition( videoProgress );
}

bool testApp :: searchWord ( string word )
{
	vector<ofxSrtItem*> items;
	ofxSrtItem* item;
	
	for( int i=0; i<srt.srtItems.size(); i++ )
	{
		item = &srt.srtItems[ i ];
		
		for( int j=0; j<item->lines.size(); j++ )
		{
			string& line = item->lines[ j ];
			
			vector<string> words = ofSplitString( line, " " );
			
			for( int k=0; k<words.size(); k++ )
			{
				string wordLowerCase	= toLowerCase( words[ k ] );
				string wordAlphaClean	= alphaClean( wordLowerCase );
				
				if( wordAlphaClean == word )
				{
					items.push_back( item );
					
					break;
				}
			}
		}
	}
	
	if( items.size() == 0 )
		return false;
	
	item = items[ (int)ofRandom( 0, items.size() ) ];
	items.clear();
	
	int ms	= item->startTimeMs;
	float p	= ms / ( video.getDuration() * 1000 );
	
	updateVideoProgress( p );
	
	return true;
}

string testApp :: toUpperCase ( string str )
{
	string strUpper = "";
	
	for( int i=0; i<str.length(); i++ )
	{
		strUpper += toupper( str[ i ] );
	}
	
	return strUpper;
}

string testApp :: toLowerCase ( string str )
{
	string strLower = "";
	
	for( int i=0; i<str.length(); i++ )
	{
		strLower += tolower( str[ i ] );
	}
	
	return strLower;
}

string testApp :: alphaClean ( string str )
{
	string strClean = "";
	
	for( int i=0; i<str.length(); i++ )
	{
		if( isalpha( str[ i ] ) )
		{
			strClean += str[ i ];
		}
	}
	
	return strClean;
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	ofSetColor( 0xFFFFFF );
	video.draw( 0, 0 );

	ofSetColor( 0x00FFFF );
	font.drawString( subtitleStr, 20, ofGetHeight() - 200 );
	
	ofSetColor( 0xFF00FF );
	font.drawString( keyString, 20, ofGetHeight() - 50 );
	
	drawProgressBar();
}

void testApp :: drawProgressBar ()
{
	float p = video.getPosition();
	
	int h = 10;
	
	ofFill();
	ofSetColor( 0xFFFF00 );
	ofRect( 0, ofGetHeight() - h, (int)( p * ofGetWidth() ), h );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	cout << key << endl;

	bool bAlphaNumeric = isalpha( key ) || isnumber( key );
	
	if( bAlphaNumeric || key == ' ' || key == 13 )
	{
		string keyStr;
		keyStr = "";
		keyStr += key;					
		keyStr = toLowerCase( keyStr );		// making key lower case.
		key = keyStr[ 0 ];

		if( key == ' ' || key == 13 )
		{
			vector<string> words = ofSplitString( keyString, " " );
			if( words.size() > 0 )
			{
				searchStr		= words[ words.size() - 1 ];
				bSearchFound	= searchWord( searchStr );
			}
		}
		
		if( key != 13 )
			addKey( key );
	}
	else if( key == OF_KEY_BACKSPACE )
	{
		if( keys.size() > 0 )
			keys.erase( keys.begin() + keys.size() - 1 );
	}
	
	createString();
}

void testApp::keyReleased(int key)
{
	//
}

void testApp::mouseMoved(int x, int y )
{
	//
}

void testApp::mouseDragged(int x, int y, int button)
{
	if( y > ofGetHeight() - 10 )
	{
		updateVideoProgress( mouseX / (float)ofGetWidth() );
	}
}

void testApp::mousePressed(int x, int y, int button)
{
	if( y > ofGetHeight() - 10 )
	{
		updateVideoProgress( mouseX / (float)ofGetWidth() );
	}
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

