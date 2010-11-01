/*
 *  ofxBomParser.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 21/09/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include <fstream>

class ofxBomParser_v2
{
	
public:
	
	ofxBomParser_v2()
	{
		data	= NULL;
		pixels	= NULL;
		
		clear();
	};
	
	~ofxBomParser_v2()
	{
		clear();
	};
	
	//--------------------------------

	int		lonSteps;
	int		latSteps;
	float	lonStart;
	float	latStart;
	float	lonInc;
	float	latInc;
	float	NODATA;
	
	float*	data;
	int		dataSize;
	float	dataMin;
	float	dataMax;
	float	dataRange;
	unsigned char* pixels;
	int		width;
	int		height;
	
//------------------------------------------------------------- SAMPLE	
//
//	ncols         41
//	nrows         36
//	xllcorner     144.45
//	yllcorner     -44.05
//	cellsize      0.1
//	NODATA_value  -9999
//	81 81 81 81 81 80 80 80 79 79 79 78 78 78 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 
//	81 81 82 81 81 81 80 80 80 79 79 78 78 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 77 78 78 78 
//	
//------------------------------------------------------------- SAMPLE
	
	void loadData ( string filename, float capMin = 0, float capMax = 0 )
	{
		ifstream	dataFile;
		string		dataStr;
		
		bool cap;
		cap = ( capMin != 0 && capMax != 0 );
		
		int i;
		i = 0;
		
		dataFile.open( ofToDataPath( filename ).c_str() );
		
		vector<string> split;
		
		if( dataFile.is_open() )
		{
			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			lonSteps	= atoi( split[ split.size() - 1 ].c_str() );
			width		= lonSteps;

			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			latSteps	= atoi( split[ split.size() - 1 ].c_str() );
			height		= latSteps;

			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			lonStart	= atof( split[ split.size() - 1 ].c_str() );

			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			latStart	= atof( split[ split.size() - 1 ].c_str() );

			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			lonInc		= atof( split[ split.size() - 1 ].c_str() );
			latInc		= lonInc;

			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			NODATA		= atof( split[ split.size() - 1 ].c_str() );
			
			dataSize = width * height;
			
			if( data != NULL )
			{
				delete[] data;
				data = NULL;
			}
			data = new float[ dataSize ];
			
			for( int i=0; i<height; i++ )
			{
				getline( dataFile, dataStr );
				split	= ofSplitString( dataStr, " " );
				
				for( int j=0; j<width; j++ )
				{
					float v = atof( split[ j ].c_str() );
					
					if( cap )
					{
						v = ofClamp( v, capMin, capMax );
					}
					
//					if( v == NODATA )
//					{
//						v = 0;
//					}
					int p	= i * width + j;
					
					data[ p ] = v;
				}
			}
		}
		
		dataFile.close();
		
		//--

		if( cap )
		{
			dataMin = capMin;
			dataMax = capMax;
		}
		else
		{
			dataMin = data[ 0 ];
			dataMax = data[ 0 ];
			
			for( int i=0; i<dataSize; i++ )
			{
				if( dataMin > data[ i ] )
					dataMin = data[ i ];
				
				if( dataMax < data[ i ] )
					dataMax = data[ i ];
			}
		}
		
		dataRange = dataMax - dataMin;
	}
	
	void createPixelsFromData ()
	{
		if( pixels != NULL )
		{
			delete[] pixels;
			pixels = NULL;
		}
		pixels = new unsigned char[ dataSize ];
		
		for( int i=0; i<dataSize; i++ )
		{
			float v	= data[ i ];
			float p	= ( v - dataMin ) / dataRange;
			
			pixels[ i ] = p * 255;
		}
	}
	
	void clear ()
	{
		if( data != NULL )
		{
			delete[] data;
			data = NULL;
		}
		
		if( pixels != NULL )
		{
			delete[] pixels;
			pixels = NULL;
		}
		
		lonSteps	= 0;
		latSteps	= 0;
		lonStart	= 0;
		latStart	= 0;
		lonInc		= 0;
		latInc		= 0;
		NODATA		= -9999;
		
		dataSize	= 0;
		dataMin		= 0;
		dataMax		= 0;
		dataRange	= 0;
		width		= 0;
		height		= 0;
	}
	
};