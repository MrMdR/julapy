/*
 *  DataNormaliser.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/06/09.
 *
 */

#include "ofxDataNormaliser.h"

ofxDataNormaliser :: ofxDataNormaliser()
{
	init();
}

ofxDataNormaliser :: ~ofxDataNormaliser()
{
	
}


void ofxDataNormaliser :: init()
{
	valueCurr	= 0;
	valuesIndex	= 0;
	values		= new float[ DEFAULT_BUFFER_SIZE ];
	valueMin	= 0;
	valueMax	= 0;
	
	isFull		= false;

	setUseBuffer( true );
	setBufferSize( DEFAULT_BUFFER_SIZE );
}

void ofxDataNormaliser :: addValue( float value )
{
	valueCurr = value;
	
	if( useBuffer )
	{
		values[ valuesIndex ] = value;
		
		if( ++valuesIndex > valuesSize - 1 )
		{
			valuesIndex	= 0;
			isFull		= true;
		}
	}
}

float ofxDataNormaliser :: getNormalisedValue()
{
	if( useBuffer )
	{
		checkValueBoundsFromBuffer();
	}
	else
	{
		checkValueBounds();
	}
	
	float range			= valueMax - valueMin;
	float scaledValue	= 0;
	if( range != 0 )
	{
		scaledValue = ( valueCurr - valueMin ) / range;
	}
	
	return scaledValue;
}

void ofxDataNormaliser :: checkValueBounds()
{
	if( valueMin > valueCurr )
		valueMin = valueCurr;
	
	if( valueMax < valueCurr )
		valueMax = valueCurr;
}

void ofxDataNormaliser :: checkValueBoundsFromBuffer ()
{
	float valueMinTemp = 0;
	float valueMaxTemp = 0;
	
	if( isFull )
	{
		for( int i=0; i<valuesSize; i++ )
		{
			int j = valuesIndex - i;
			if( j < 0 )
			{
				j = valuesSize + j;
			}
			
			float valueCurr = values[ j ];
			
			if( i == 0 )
			{
				valueMinTemp = valueCurr;
				valueMaxTemp = valueCurr;
			}
			else
			{
				if( valueCurr < valueMinTemp )
				{
					valueMinTemp = valueCurr;
				}
				
				if( valueCurr > valueMaxTemp )
				{
					valueMaxTemp = valueCurr;
				}
			}
		}
	}
	else
	{
		for( int i=0; i<=valuesIndex; i++ )
		{
			int j = valuesIndex - i;
			
			float valueCurr = values[ j ];
			
			if( i == 0 )
			{
				valueMinTemp = valueCurr;
				valueMaxTemp = valueCurr;
			}
			else
			{
				if( valueCurr < valueMinTemp )
				{
					valueMinTemp = valueCurr;
				}
				
				if( valueCurr > valueMaxTemp )
				{
					valueMaxTemp = valueCurr;
				}
			}
		}
	}
	
	valueMin = valueMinTemp;
	valueMax = valueMaxTemp;
}

////////////////////////////////////////////////////
//	SETTERS.
////////////////////////////////////////////////////

void ofxDataNormaliser :: setUseBuffer( bool b )
{
	useBuffer = b;
}

void ofxDataNormaliser :: setBufferSize( int size )
{
	valuesSize = size;
}
