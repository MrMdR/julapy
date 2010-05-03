/*
 *  Pong.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HOLLER_PONG
#define HOLLER_PONG

#include "Paddle.h"
#include "Ball.h"
#include "Score.h"
#include "Backdrop.h"

class Pong
{

public :
	
	Pong();
	
	void init		();
	void update		();
	void draw		();
	
	void setPlayerOnePosition	( float p );
	void setPlayerTwoPosition	( float p );
	void flipSides				( bool b );
	
	void setPaused		( bool b );
	bool getPaused		();
	void togglePause	();
	void reset			();
	bool checkReset		();
	
	void setBallPosition	( const ofPoint& p );
	ofPoint getBallPosition	();
	
	void drawBackdrop			();
	void drawBackdropDivider	();
	void drawBackdropStars		();
	void drawPaddles			();
	void drawBall				();
	void drawScore				();
	void drawHollerLogo			();
	void drawBlackAlphaLayer	();
	void drawPaused				();
	void drawReset				();
	
private :
	
	void checkBounds			();
	void checkPaddleHit			();
	void checkLeftPaddleHit		();
	void checkRightPaddleHit	();
	
	void resetPoint				();
	
	Paddle		paddles[ 2 ];
	Paddle*		paddleOne;
	Paddle*		paddleTwo;
	
	Ball		ball;
	Score		score;
	Backdrop	backdrop;
	
	ofTrueTypeFont	font;
	ofTrueTypeFont	font2;

	int			paddleColors[ 2 ];
	bool		paddleFlip;
	float		pongVelGain;
	float		pongVelLimit;
	
	bool		bPaused;
	bool		bReset;
	
	int			resetCount;
	int			resetCountLimit;
	
};

#endif