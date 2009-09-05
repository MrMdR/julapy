/*
 *  TriangleFieldConfig.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleFieldConfig.h"


TriangleFieldConfig :: TriangleFieldConfig()
{
	setAudioNorm( 0 );
	setSize( 600, 600 );
}

TriangleFieldConfig :: ~TriangleFieldConfig()
{

}

void TriangleFieldConfig :: init()
{
	//
}

void TriangleFieldConfig :: update()
{
	//
}

void TriangleFieldConfig :: setSize( int value0, int value1 )
{
	width	= value0;
	height	= value1;
}

void TriangleFieldConfig :: setAudioNorm( float value )
{
	audioNorm = value;
}

void TriangleFieldConfig :: copyTo( TriangleField *tfs, int tfTotal, bool copyColor )
{
	int total;
	
	if( fieldsTotal <= tfTotal )
		total = fieldsTotal;
	else
		total = tfTotal;
	
	for( int i=0; i<total; i++ )
	{
		fields[ i ].copyTo( &tfs[ i ], copyColor );
	}
}

void TriangleFieldConfig :: copyChangesTo( TriangleField *tfs, int tfTotal )
{
	//
}