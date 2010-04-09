/*
 *  Pong.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Pong.h"

Pong :: Pong()
{

}

////////////////////////////////////////////////
//	INIT.
////////////////////////////////////////////////

void Pong :: init ()
{
	backdrop.init();
	
	paddleColors[ 0 ] = 0xFF00FF;
	paddleColors[ 1 ] = 0x00FF00;
	
	paddleFlip	= false;
	
	paddleOne	= &paddles[ 0 ];
	paddleTwo	= &paddles[ 1 ];
	
	paddles[ 0 ].setColor( paddleColors[ 0 ] );
	paddles[ 0 ].setSide( 0 );
	
	paddles[ 1 ].setColor( paddleColors[ 1 ] );
	paddles[ 1 ].setSide( 1 );
	
	int dirX, dirY;
	dirX = ( ofRandom( 0, 1) < 0.5 ) ? -1 : 1;
	dirY = ( ofRandom( 0, 1) < 0.5 ) ? -1 : 1;
	
	ball.setPositon( (int)( ofGetWidth() * 0.5 ), (int)( ofGetHeight() * 0.5 ) );
	ball.setVelocity( 5 * dirX, 5 * dirY );
	
	score.init();
	
	font.loadFont( "fonts/mono0765.ttf", 100 );
	font2.loadFont( "fonts/mono0755.ttf", 56 );
	
	pongVelGain		= 1.2;
	pongVelLimit	= 30;
	
	bPaused = false;
	bReset	= false;
	
	resetCount		= 0;
	resetCountLimit	= 15;
}

////////////////////////////////////////////////
//	PLAYER POSITIONS.
////////////////////////////////////////////////

void Pong :: setPlayerOnePosition ( float p )
{
	if( bPaused )
		return;
	
	paddleOne->setPosition( p );
}

void Pong :: setPlayerTwoPosition ( float p )
{
	if( bPaused )
		return;
	
	paddleTwo->setPosition( p );
}

void Pong :: flipSides ( bool b )
{
	if( bPaused )
		return;
	
	if( paddleFlip == b )
	{
		return;
	}
	
	paddleFlip = b;
	
	if( paddleFlip )
	{
		paddleOne	= &paddles[ 1 ];
		paddleTwo	= &paddles[ 0 ];
	}
	else
	{
		paddleOne	= &paddles[ 0 ];
		paddleTwo	= &paddles[ 1 ];
	}

	paddleOne->setColor( paddleColors[ 0 ] );
	paddleTwo->setColor( paddleColors[ 1 ] );
}

void Pong :: setPaused ( bool b )
{
	if( bReset )
		return;
	
	if( bPaused != b )
	{
		bPaused = b;
	}
}

bool Pong :: getPaused ()
{
	return bPaused;
}

void Pong :: togglePause ()
{
	bPaused = !bPaused;
}

void Pong :: reset ()
{
	bReset		= true;
	bPaused		= false;
	resetCount	= 0;
		
	score.reset();
		
	resetPoint();
}

////////////////////////////////////////////////
//	UPDATE.
////////////////////////////////////////////////

void Pong :: update ()
{
	if( bPaused )
		return;
	
	if( checkReset() )
		return;
	
	backdrop.update();
	
	paddles[ 0 ].update();
	paddles[ 1 ].update();

	ball.vel.limit( pongVelLimit );
	ball.update();
	
	checkBounds();
	checkPaddleHit();
	
	backdrop.setBallPos( ball.loc.x, ball.loc.y );
}

bool Pong :: checkReset ()
{
	if( bReset )
	{
		if( ++resetCount >= resetCountLimit )
		{
			bReset = false;
		}
		
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////
//	CHECKS.
////////////////////////////////////////////////

void Pong :: checkBounds ()
{
	float br;
	br = ball.radius;
	
	bool out;
	out = false;
	
	bool winP1;
	winP1 = false;
	
	bool winP2;
	winP2 = false;
	
	if( ball.loc.x < br )
	{
		ball.vel.x *= -1;
		out		= true;
		winP2	= true;
	}
	
	if( ball.loc.x > ofGetWidth() - br )
	{
		ball.vel.x *= -1;
		out		= true;
		winP1	= true;
	}
	
	if( ball.loc.y < br )
	{
		ball.vel.y *= -1;
		out = true;
	}
	
	if( ball.loc.y > ofGetHeight() - br )
	{
		ball.vel.y *= -1;
		out = true;
	}
	
	if( winP1 || winP2 )
	{
		if( winP1 )
		{
			score.addToPlayerOne( 1 );
		}
		
		if( winP2 )
		{
			score.addToPlayerTwo( 1 );
		}
		
		resetPoint();
		
		return;
	}
	
	if( out )
	{
		ball.update();
	}
}

void Pong :: checkPaddleHit ()
{
	checkLeftPaddleHit();
	checkRightPaddleHit();
}

void Pong :: checkLeftPaddleHit ()
{
	float br;
	br = ball.radius;
	
	Paddle& paddle = paddles[ 0 ];
	
	bool bPaddleV;		// in vertical range.
	bool bPaddleD;		// heading in the right direction.
	bool bPaddleH;		// in horizontal range, makes contact with paddle.
	
	bPaddleV = ( ball.loc.y > paddle.rect.y ) && ( ball.loc.y < paddle.rect.y + paddle.rect.height );
	
	bPaddleD = ( ball.vel.x < 0 );
	
	bPaddleH = ( ball.loc.x < paddle.rect.x + paddle.rect.width + br );
	
	if( bPaddleV && bPaddleD && bPaddleH )
	{
		ball.vel.x	*= -1;
		ball.vel	*= pongVelGain;
		ball.update();
	}
}

void Pong :: checkRightPaddleHit ()
{
	float br;
	br = ball.radius;
	
	Paddle& paddle = paddles[ 1 ];
	
	bool bPaddleV;		// in vertical range.
	bool bPaddleD;		// heading in the right direction.
	bool bPaddleH;		// in horizontal range, makes contact with paddle.
	
	bPaddleV = ( ball.loc.y > paddle.rect.y ) && ( ball.loc.y < paddle.rect.y + paddle.rect.height );
	
	bPaddleD = ( ball.vel.x > 0 );
	
	bPaddleH = ( ball.loc.x > paddle.rect.x - br );
	
	if( bPaddleV && bPaddleD && bPaddleH )
	{
		ball.vel.x	*= -1;
		ball.vel	*= pongVelGain;
		ball.update();
	}
}

////////////////////////////////////////////////
//	RESET.
////////////////////////////////////////////////

void Pong :: resetPoint ()
{
	int dirX, dirY;
	dirX = ( ofRandom( 0, 1) < 0.5 ) ? -1 : 1;
	dirY = ( ofRandom( 0, 1) < 0.5 ) ? -1 : 1;
	
	ball.setPositon( (int)( ofGetWidth() * 0.5 ), (int)( ofGetHeight() * 0.5 ) );
	ball.setVelocity( 5 * dirX, 5 * dirY );
}

////////////////////////////////////////////////
//	DRAW.
////////////////////////////////////////////////

void Pong :: draw ()
{
	drawBackdrop();
	drawPaddles();
	drawBall();
	drawScore();
//	drawHollerLogo();
	drawPaused();
	drawReset();
}

void Pong :: drawBackdrop ()
{
	drawBackdropDivider();
	drawBackdropStars();
}

void Pong :: drawBackdropDivider ()
{
	backdrop.drawDivider();
}

void Pong :: drawBackdropStars ()
{
	backdrop.drawStarLayers();
}

void Pong :: drawPaddles ()
{
	paddles[ 0 ].draw();
	paddles[ 1 ].draw();
}

void Pong :: drawBall ()
{
	ball.draw();
}

void Pong :: drawScore ()
{
	score.draw();
}

void Pong :: drawHollerLogo ()
{
	string str;
	str = "holler";
	
	ofRectangle rect;
	rect = font2.getStringBoundingBox( str, 0, 0 );
	
	int sx, sy;
	
	sx = (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
	sy = ofGetHeight() - 20;
	
	ofSetColor( 0xFFFFFF );
	font2.drawString( str, sx, sy );
}

void Pong :: drawBlackAlphaLayer ()
{
	ofFill();
	ofEnableAlphaBlending();
	ofSetColor( 0, 0, 0, 180 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	ofDisableAlphaBlending();
}

void Pong :: drawPaused ()
{
	if( !bPaused )
		return;
	
	drawBlackAlphaLayer();
	
	string str;
	str = "PAUSED";
	
	ofRectangle rect;
	rect = font.getStringBoundingBox( str, 0, 0 );
	
	int sx, sy;
	
	sx = (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
	sy = (int)( ( ofGetHeight() + rect.height ) * 0.5 );

	ofSetColor( 0x00FFFF );
	font.drawString( str, sx - 4, sy - 4 );
	
	ofSetColor( 0xFF00FF );
	font.drawString( str, sx + 4, sy + 4 );
	
	ofSetColor( 0xFFFFFF );
	font.drawString( str, sx, sy );
}

void Pong :: drawReset ()
{
	if( !bReset )
		return;
	
	drawBlackAlphaLayer();
	
	string str;
	str = "RESET";
	
	ofRectangle rect;
	rect = font.getStringBoundingBox( str, 0, 0 );
	
	int sx, sy;
	
	sx = (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
	sy = (int)( ( ofGetHeight() + rect.height ) * 0.5 );

	ofSetColor( 0x00FFFF );
	font.drawString( str, sx - 4, sy - 4 );
	
	ofSetColor( 0xFF00FF );
	font.drawString( str, sx + 4, sy + 4 );
	
	ofSetColor( 0xFFFFFF );
	font.drawString( str, sx, sy );
}