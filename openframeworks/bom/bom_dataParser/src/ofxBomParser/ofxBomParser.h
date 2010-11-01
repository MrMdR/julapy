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

class ofxBomParser
{
	
public:
	
	ofxBomParser()
	{
		data	= NULL;
		pixels	= NULL;
		
		clear();
	};
	
	~ofxBomParser()
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
//	BUREAU OF METEOROLOGY                                                           
//	[HEADER]                                                                        
//	meanT_anom  ANALYSIS:       1911010119191231                                    
//	LATITUDE - LONGITUDE GRID                                                       
//	ROW NUMBER AT SOUTHWEST CORNER                                                  
//	COLUMN NUMBER AT SOUTHWEST CORNER                                               
//	ROW CHANGES WITH LONGITUDE                                                      
//	NUMBER OF LONGITUDES IN GRID             886                                    
//	NUMBER OF LATITUDES  IN GRID             691                                    
//	SOUTHWEST LONGITUDE                    112.000                                  
//	SOUTHWEST LATITUDE                      44.500                                  
//	GRID INCREMENT OF LONGITUDE             0.050                                   
//	GRID INCREMENT OF LATITUDE              0.050                                   
//	[ANALYSIS ARRAY]                                                                
//	[$]                                                                             
//	-0.28   -0.28   -0.28   -0.28   -0.28   -0.28   -0.28   -0.28   -0.28   -0.28
//	-0.28   -0.28   -0.28   -0.28   -0.28   -0.28   -0.28   -0.27   -0.27   -0.27
//
//------------------------------------------------------------- SAMPLE
	
	void loadData ( string filename )
	{
		ifstream	dataFile;
		string		dataStr;
		
		int i;
		i = 0;
		
		dataFile.open( ofToDataPath( filename ).c_str() );
		
		vector<string> split;
		
		if( dataFile.is_open() )
		{
			getline( dataFile, dataStr );		// skip 7 lines.
			getline( dataFile, dataStr );
			getline( dataFile, dataStr );
			getline( dataFile, dataStr );
			getline( dataFile, dataStr );
			getline( dataFile, dataStr );
			getline( dataFile, dataStr );
			
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

			getline( dataFile, dataStr );
			split		= ofSplitString( dataStr, " " );
			latInc		= atof( split[ split.size() - 1 ].c_str() );

			getline( dataFile, dataStr );		// skip 2 lines.
			getline( dataFile, dataStr );
			
			dataSize = width * height;
			
			if( data != NULL )
			{
				delete[] data;
				data = NULL;
			}
			data = new float[ dataSize ];
			
			for( int i=0; i<height; i++ )
			{
				int j = 0;
				
				while( j < width )
				{
					getline( dataFile, dataStr );
					split	= ofSplitString( dataStr, " " );
					
					for( int k=0; k<split.size(); k++ )
					{
						float v = atof( split[ k ].c_str() );
						int p	= i * width + j + k;
						
						data[ p ] = v;
					}
					
					j += split.size();
				}
			}
		}
		
		dataFile.close();
		
		//--

		dataMin = data[ 0 ];
		dataMax = data[ 0 ];
		
		for( int i=0; i<dataSize; i++ )
		{
			if( dataMin > data[ i ] )
				dataMin = data[ i ];
			
			if( dataMax < data[ i ] )
				dataMax = data[ i ];
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
		
		dataSize	= 0;
		dataMin		= 0;
		dataMax		= 0;
		dataRange	= 0;
		width		= 0;
		height		= 0;
	}
	
};