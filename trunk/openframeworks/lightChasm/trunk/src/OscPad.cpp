/*
 *  OscPad.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscPad.h"

OscPad :: OscPad()
{

}

OscPad :: ~OscPad()
{

}


void OscPad :: init( ofxOscReceiver	*recvr )
{
	receiver = recvr;
	
	addresses[ 0 ][ 0 ] = "/button/A1";
	addresses[ 0 ][ 1 ] = "/button/A2";
	addresses[ 0 ][ 2 ] = "/button/A3";
	addresses[ 1 ][ 0 ] = "/button/B1";
	addresses[ 1 ][ 1 ] = "/button/B2";
	addresses[ 1 ][ 2 ] = "/button/B3";
	addresses[ 2 ][ 0 ] = "/button/C1";
	addresses[ 2 ][ 1 ] = "/button/C2";
	addresses[ 2 ][ 2 ] = "/button/C3";
	addresses[ 3 ][ 0 ] = "/button/D1";
	addresses[ 3 ][ 1 ] = "/button/D2";
	addresses[ 3 ][ 2 ] = "/button/D3";
	addresses[ 4 ][ 0 ] = "/button/E1";
	addresses[ 4 ][ 1 ] = "/button/E2";
	addresses[ 4 ][ 2 ] = "/button/E3";

	states[ 0 ][ 0 ] = false;
	states[ 0 ][ 1 ] = false;
	states[ 0 ][ 2 ] = false;
	states[ 1 ][ 0 ] = false;
	states[ 1 ][ 1 ] = false;
	states[ 1 ][ 2 ] = false;
	states[ 2 ][ 0 ] = false;
	states[ 2 ][ 1 ] = false;
	states[ 2 ][ 2 ] = false;
	states[ 3 ][ 0 ] = false;
	states[ 3 ][ 1 ] = false;
	states[ 3 ][ 2 ] = false;
	states[ 4 ][ 0 ] = false;
	states[ 4 ][ 1 ] = false;
	states[ 4 ][ 2 ] = false;
	
	sliderAddresses[ 0 ] = "/slider/1";
	sliderAddresses[ 1 ] = "/slider/2";
	sliderAddresses[ 2 ] = "/slider/3";
	sliderAddresses[ 3 ] = "/slider/4";
	sliderAddresses[ 4 ] = "/slider/5";
	sliderAddresses[ 5 ] = "/slider/6";
	
	sliderValues[ 0 ] = 0;
	sliderValues[ 1 ] = 0;
	sliderValues[ 2 ] = 0;
	sliderValues[ 3 ] = 0;
	sliderValues[ 4 ] = 0;
	sliderValues[ 5 ] = 0;
	
	setOffOnRelease( true );
	setExclusiveMode( false );
	setOrientation( ORIENTATE_LANDSCAPE );
	setNumberOfRows( ROWS_DEFAULT );
	setNumberOfColumns( COLUMNS_DEFAULT );
}

void OscPad :: setOffOnRelease( bool value )
{
	offOnRelease = value;
}

void OscPad :: setExclusiveMode( bool useExclusiveMode )
{
	exclusiveMode = useExclusiveMode;
}

void OscPad :: setOrientation( int value )
{
	if( value == ORIENTATE_PORTRAIT || value == ORIENTATE_LANDSCAPE )
	{
		orientation = value;
	}
}

void OscPad :: setNumberOfRows( int value )
{
	if( value > ROWS_MAX )
	{
		value = ROWS_MAX;
	}
	
	if( value < ROWS_MIN )
	{
		value = ROWS_MIN;
	}
	
	rows = value;
}

void OscPad :: setNumberOfColumns( int value )
{
	if( value > COLUMNS_MAX )
	{
		value = COLUMNS_MAX;
	}
	
	if( value < COLUMNS_MIN )
	{
		value = COLUMNS_MIN;
	}
	
	columns = value;
}

void OscPad :: update()
{
	while( receiver->hasWaitingMessages() )
	{
		ofxOscMessage m;
		
		receiver->getNextMessage( &m );
		
		string address = m.getAddress();
		
		for ( int i=0; i<m.getNumArgs(); i++ )
		{
			if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
			{
				bool state;
				bool found = false;
				
				if( m.getArgAsInt32( i ) == 1 )
				{
					state = true;
				}
				else
				{
					state = false;
				}
				
				int j = 0;
				int k = 0;
				
				for( j=0; j<columns; j++ )
				{
					for( k=0; k<rows; k++ )
					{
						string tempAddress = addresses[ j ][ k ];
						
						if( exclusiveMode )	// if any other buttons are turned on, turn them off.
						{
							if( orientation == ORIENTATE_PORTRAIT )
							{
								states[ j ][ k ] = false;
							}
							
							if( orientation == ORIENTATE_LANDSCAPE )
							{
								states[ k ][ j ] = false;
							}
						}
						
						if( tempAddress == address )
						{
							if( orientation == ORIENTATE_PORTRAIT )
							{
								if( offOnRelease )
								{
									states[ j ][ k ] = state;
								}
								else
								{
									if( state )
									{
										states[ j ][ k ] = !states[ j ][ k ];
									}
								}
							}
							
							if( orientation == ORIENTATE_LANDSCAPE )
							{
								if( offOnRelease )
								{
									states[ k ][ j ] = state;
								}
								else
								{
									if( state )
									{
										states[ k ][ j ] = !states[ k ][ j ];
									}
								}
							}

							found = true;
						}
					}
				}
				
				if( found )
				{
					ofNotifyEvent( oscPadChangeEvent, j, this );
				}
			}
			
			// SLIDERS.
			if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			{
				for( int j=0; j<SLIDERS_TOTAL; j++ )
				{
					string sliderAddress = sliderAddresses[ j ];
					
					if( address == sliderAddress )
					{
						sliderValues[ j ] = m.getArgAsFloat( i );
						
						break;
					}
				}
			}
		}
	}
}

bool OscPad :: getButtonState( int x, int y )
{
	x -= 1;
	y -= 1;
	
	if( x < 0 )
		x = 0;
	
	if( y < 0 )
		y = 0;
	
	if( orientation == ORIENTATE_PORTRAIT )
	{
		return states[ y ][ x ];
	}
	
	if( orientation == ORIENTATE_LANDSCAPE )
	{
		return states[ x ][ y ];
	}
}
	
float OscPad :: getSliderValue( int sliderNumber )
{
	int sliderIndex = sliderNumber - 1;
	
	if( sliderIndex < 0 )
	{
		sliderIndex = 0;
	}
	
	if( sliderIndex > SLIDERS_TOTAL - 1 )
	{
		sliderIndex = SLIDERS_TOTAL - 1;
	}
	
	return sliderValues[ sliderIndex ];
}
