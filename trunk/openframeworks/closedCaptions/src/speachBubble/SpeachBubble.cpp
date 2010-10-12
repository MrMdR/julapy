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
	volume		= 0;
	volumePush	= 0;
	volumeDir	= ofRandom( 0, 1.0 ) > 0.5 ? 1 : -1;
	volumeData	= NULL;
}

SpeachBubble :: ~SpeachBubble() {}

void SpeachBubble :: init ( string str, ofTrueTypeFont* f )
{
	bubbleStr	= "";
	font		= f;
	
	vector<string> sentences = ofSplitString( str, "\n" );
	if( sentences.size() == 0 )
		sentences.push_back( str );
	
	int w = 1;
	
	for( int i=0; i<sentences.size(); i++ )
	{
		vector<string> words = ofSplitString( sentences[ i ], " " );
		
		for( int j=0; j<words.size(); j++ )
		{
			bubbleStr += words[ j ];
			
			if( i == sentences.size() - 1 && j == words.size() - 1 )		// do not add anything to the end of caption.
				break;
			
			if( w++ % 3 == 0 )
			{
				bubbleStr += "\n";
			}
			else 
			{
				bubbleStr += " ";
			}
		}
	}
	
	rectText = font->getStringBoundingBox( bubbleStr, 0, 0 );
	
	rect.width  = rectText.width  + 50;
	rect.height = rectText.height + 60;
}

void SpeachBubble :: setPosition ( ofPoint& p )
{
	position.x = p.x;
	position.y = p.y;
}

void SpeachBubble :: setTarget ( ofPoint& p )
{
	target.x = p.x;
	target.y = p.y;
}

void SpeachBubble :: setVolume ( float v )
{
	volume		= v;
	volumePush += v;
}

void SpeachBubble :: setVolumeData ( float *v )
{
	volumeData = v;
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

ofRectangle SpeachBubble :: getRect ()
{
	ofRectangle r;
	r.width		= rect.width;
	r.height	= rect.height;
	
	return r;
}

void SpeachBubble :: drawBubble ()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glScalef( scale, scale, 0 );
	
	ofBeginShape();
	
	int resolution	= SPEACH_BUBBLE_RESOLUTION;
	int resHalf		= (int)( resolution * 0.5 );
	
	int i = ofRandom( 0, resolution - 1 );
	int t = resolution + i;
	
	for( i; i<t; i++ )
	{
		int j = i % resolution;
		
		float a = j / (float)( resolution - 1 );
		a *= TWO_PI;
		
		float wobble = 0;
		float wa = 0;
		
		if( j < resHalf )		// mirroring the wobble.
		{
			wa = a;
		}
		else 
		{
			wa = ( resolution - 1 - j ) / (float)( resolution - 1 );
			wa *= TWO_PI;
			wa *= -1;
		}
		
		wobble = 10 * volume * sin( wa * ( 20 * volume ) + volumePush * 0.25 );
		
		if( volumeData != NULL )
		{
			wobble *= 2 * volumeData[ j ];
		}
		
		float x = ( rect.width  + wobble ) * 0.5 * cos( a );
		float y = ( rect.height + wobble ) * 0.5 * sin( a );
		
		ofVertex( x, y );
	}
	
	ofEndShape( true );
	
	glPopMatrix();
}

void SpeachBubble :: drawPoint ()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glScalef( scale, scale, 0 );
	
	float aRad = atan2( target.x - position.x, target.y - position.y );
	aRad *= -1;
	aRad += PI * 0.5;
 	
	float aDeg = aRad;
	aDeg *= RAD_TO_DEG;
	
	float px = ( rect.width  * 0.5 ) * cos( aRad );
	float py = ( rect.height * 0.5 ) * sin( aRad );
	
//	float length = (float)sqrt( px * px + py * py );
	
	float length = MAX( rect.width * 0.5, rect.height * 0.5 );
	length += 20;
	
//	ofSetColor( 0xFF0000 );		// debug.
//	ofCircle( px, py, 4 );
	
	glPushMatrix();
	glRotatef( aDeg, 0, 0, 1 );
	
	ofTriangle
	(
		0,		-10,
		length,  0,
		0,		 10
	);
	
	glPopMatrix();
	
	glPopMatrix();
}

void SpeachBubble :: drawText ()
{
	ofTrueTypeFont& ttf = *font;
	
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glScalef( scale, scale, 0 );
	
	float ttfX = 0;
	float ttfY = 0;
	
	ttfX = -rectText.width  * 0.5;
	ttfY = -rectText.height * 0.5;
	
	ttfX += 2;
	ttfY += 10;

	ttf.drawString( bubbleStr, ttfX, ttfY );
	
	glPopMatrix();
}

