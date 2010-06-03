/*
 *  
 *  emptyExample
 *
 *  Created by lukasz karluk on 2/06/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SpeachBubble.h"

SpeachBubble ::  SpeachBubble() 
{
	bPlayingIn	= false;
	bPlayingOut	= false;
	bFinished	= false;
	
	scale		= 0;
}

SpeachBubble :: ~SpeachBubble() {}

void SpeachBubble :: init ( string str, ofTrueTypeFont* f )
{
	bubbleStr	= str;
	font		= f;
	
	rect = font->getStringBoundingBox( bubbleStr, 0, 0 );
	
	rectPad.width	= 40;
	rectPad.height	= 50;
}

void SpeachBubble :: setPosition ( ofPoint& p )
{
	pos.x = p.x;
	pos.y = p.y;
}

void SpeachBubble :: setSize ( ofRectangle& r )
{
	rect.x		= r.x;
	rect.y		= r.y;
	rect.width	= r.width;
	rect.height	= r.height;
}

void SpeachBubble :: playIn ()
{
	bPlayingIn	= true;
}

void SpeachBubble :: playOut ()
{
	bPlayingIn	= false;
	bPlayingOut	= true;
}

void SpeachBubble :: update	()
{
	float ease;
	ease = 0.5;
	
	if( bPlayingIn )
	{
		scale += ( 1.0  - scale  ) * ease;
	}
	
	if( bPlayingOut )
	{
		scale += ( 0.0  - scale  ) * ease;
		
		if( scale < 0.1 )
		{
			bFinished = true;
		}
	}
}

void SpeachBubble :: drawBubble ()
{
	glPushMatrix();
	glTranslatef( pos.x, pos.y, 0 );
	glScalef( scale, scale, 0 );
	
	ofEllipse( 0, 0, rect.width + rectPad.width, rect.height + rectPad.height );
	
	glPopMatrix();
}


void SpeachBubble :: drawText ()
{
	ofTrueTypeFont& ttf = *font;
	
	glPushMatrix();
	glTranslatef( pos.x, pos.y, 0 );
	glScalef( scale, scale, 0 );
	
	float ttfX = 0;
	float ttfY = 0;
	
	ttfX = -rect.width  * 0.5;
	ttfY = -rect.height * 0.5;
	
	ttfX += 2;
	ttfY += 10;

	ttf.drawString( bubbleStr, ttfX, ttfY );
	
	glPopMatrix();
}

