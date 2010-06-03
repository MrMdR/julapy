/*
 *  ofxSRT.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 2/06/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSRT.h"


ofxSrt :: ofxSrt()
{
	
}

ofxSrt :: ~ofxSrt ()
{

}

string ofxSrt :: getSubtitleText ( int ms )
{
	for( int i=0; i<srtItems.size(); i++ )
	{
		ofxSrtItem& item = srtItems[ i ];
		
		int m1 = item.startTimeMs;
		int m2 = item.endTimeMs;
		
		if( ms >= m1 && ms <= m2 )
		{
			string str;
			
			for( int j=0; j<item.lines.size(); j++ )
			{
				str += item.lines[ j ];
				str += "\n";
			}
			
			return str;
		}
	}
	
	return "";
}

void ofxSrt :: parse( string filename )
{
	ifstream	dataFile;
	string		dataStr;
	
	dataFile.open( ofToDataPath( filename ).c_str() );

	vector<string> dataChunk;
	
	if( dataFile.is_open() )
	{
		while( !dataFile.eof() )
		{
			getline( dataFile, dataStr );
			
			cleanLine( dataStr );

			if( dataStr == "" )
			{
				if( dataChunk.size() == 0 )
					return;
				
				parseItem( dataChunk );
				
				dataChunk.clear();
			}
			else 
			{
				dataChunk.push_back( dataStr );
			}
		}
	}
	
	dataFile.close();
}

void ofxSrt :: cleanLine ( string& lineStr )
{
	vector<string> data = ofSplitString( lineStr, "\r" );		// remove the french line break.
	
	if( data.size() == 0 )
	{
		lineStr = "";
	}
	else 
	{
		lineStr = data[ 0 ];
	}
	
	data.clear();
}

void ofxSrt :: parseItem ( vector<string>& dataChunk )
{
	srtItems.push_back( ofxSrtItem() );
	ofxSrtItem& item = srtItems.back();

	item.id = atoi( dataChunk[ 0 ].c_str() );
	
	parseItemTime( item, dataChunk[ 1 ] );
	
	if( dataChunk.size() < 2 )
		return;
	
	for( int i=2; i<dataChunk.size(); i++ )
	{
		item.lines.push_back( dataChunk[ i ] );
	}
}

void ofxSrt :: parseItemTime ( ofxSrtItem& item, string& timeStr )
{
	vector<string> times = ofSplitString( timeStr, " --> " );

	item.startTimeStr	= times[ 0 ];
	item.endTimeStr		= times[ 1 ];
	
	parseItemTimeToMilliSeconds( item.startTimeStr, item.startTimeMs	);
	parseItemTimeToMilliSeconds( item.endTimeStr,	item.endTimeMs		);
}

void ofxSrt :: parseItemTimeToMilliSeconds ( string& timeStr, int& ms )
{
	vector<string> times	= ofSplitString( timeStr, "," );
	vector<string> hms		= ofSplitString( timeStr, ":" );
	
	int hours	= atoi( hms[ 0 ].c_str() );
	int minutes	= atoi( hms[ 1 ].c_str() );
	int seconds	= atoi( hms[ 2 ].c_str() );
	int milli	= atoi( times[ 1 ].c_str() );
	
	ms = 0;
	ms += hours * 60 * 60 * 1000;
	ms += minutes * 60 * 1000;
	ms += seconds * 1000;
	ms += milli;
}
