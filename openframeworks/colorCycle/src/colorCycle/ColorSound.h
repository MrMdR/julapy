/*
 *  ColorSound.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 31/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#ifdef TARGET_OF_IPHONE
#include "ofxALSoundPlayer.h"
#else
#include "ofxDirList.h"
#endif

class ColorSound
{

public:
	
	 ColorSound()	{};
	~ColorSound()	{};

	//--------------------------------
	
#ifdef TARGET_OF_IPHONE
	vector<ofxALSoundPlayer*>	spBackground;
	vector<ofxALSoundPlayer*>	spMeshDrag;
	vector<ofxALSoundPlayer*>	spPointAdd;
	vector<ofxALSoundPlayer*>	spPointRemove;
	vector<ofxALSoundPlayer*>	spPointCollide;
	vector<ofxALSoundPlayer*>	spPointShuffle;
#else
	ofxDirList					DIR;
	vector<ofSoundPlayer*>		spBackground;
	vector<ofSoundPlayer*>		spMeshDrag;
	vector<ofSoundPlayer*>		spPointAdd;
	vector<ofSoundPlayer*>		spPointRemove;
	vector<ofSoundPlayer*>		spPointCollide;
	vector<ofSoundPlayer*>		spPointShuffle;
#endif
	
	float						collisionSoundTimeout;
	float						collisionSoundTimeoutLimit;
	bool						bCollisionSoundTimeout;
	
	int							backgroundSoundIndex;

	//--------------------------------
	
	void setup ()
	{
		
#ifdef TARGET_OF_IPHONE
		
		vector<string> soundNames;
		soundNames.push_back( "sounds/background/BackgroundLoop1.caf" );
		soundNames.push_back( "sounds/background/BackgroundLoop2.caf" );
		soundNames.push_back( "sounds/background/BackgroundLoop3.caf" );
		soundNames.push_back( "sounds/background/BackgroundLoop4.caf" );
		soundNames.push_back( "sounds/background/BackgroundLoop5.caf" );
		soundNames.push_back( "sounds/background/BackgroundLoop6.caf" );
		loadSounds( soundNames, spBackground, true );
		
		soundNames.clear();
		soundNames.push_back( "sounds/point_add/PountAdd2.caf" );
		loadSounds( soundNames, spPointAdd );
		
		soundNames.clear();
		soundNames.push_back( "sounds/point_collide/PointAdd1.caf" );
		soundNames.push_back( "sounds/point_collide/PointRemove2.caf" );
		loadSounds( soundNames, spPointCollide );
		
		soundNames.clear();
		soundNames.push_back( "sounds/point_remove/PointRemove1.caf" );
		loadSounds( soundNames, spPointRemove );
		
		soundNames.clear();
		soundNames.push_back( "sounds/point_shuffle/PointShuffle1.caf" );
		soundNames.push_back( "sounds/point_shuffle/PointShuffle3.caf" );
		loadSounds( soundNames, spPointShuffle );
		
#else
		
		loadSounds( "sounds/background",	spBackground, true );
		loadSounds( "sounds/mesh_drag",		spMeshDrag );
		loadSounds( "sounds/point_add",		spPointAdd );
		loadSounds( "sounds/point_collide", spPointCollide );
		loadSounds( "sounds/point_remove",	spPointRemove );
		loadSounds( "sounds/point_shuffle",	spPointShuffle );
		
#endif
		
		collisionSoundTimeout		= 0;
		collisionSoundTimeoutLimit	= 0;
		bCollisionSoundTimeout		= false;
		
		backgroundSoundIndex		= -1;
		
		ofAddListener( ofEvents.update, this, &ColorSound :: update	);
	}
	
	
#ifdef TARGET_OF_IPHONE
	
	//////////////////////////////////////////
	//	IPHONE METHODS.
	//////////////////////////////////////////
	
	void loadSounds ( const vector<string>& fileNames, vector<ofxALSoundPlayer*>& sounds, bool loop = false )
	{
		for( int i=0; i<fileNames.size(); i++ )
		{
			ofxALSoundPlayer* sp	= new ofxALSoundPlayer();
			string fileName			= fileNames[ i ];
			
			if( loop )
				sp->loadLoopingSound( fileName );
			else 
				sp->loadSound( fileName );
			
			sounds.push_back( sp );
		}
	}
	
	void loadNumOfSounds ( const vector<string>& fileNames, vector<ofxALSoundPlayer*>& sounds, int numOfSounds = 10, bool loop = false )
	{
		for( int i=0; i<numOfSounds; i++ )
		{
			int j = ofRandom( 0, fileNames.size() - 1 );
			
			ofxALSoundPlayer* sp	= new ofxALSoundPlayer();
			string fileName			= fileNames[ j ];
			
			if( loop )
				sp->loadLoopingSound( fileName );
			else 
				sp->loadSound( fileName );
			
			sounds.push_back( sp );
		}
	}
	
	int playRandomSound ( vector<ofxALSoundPlayer*>& sounds, float volume = 1.0 )
	{
		if( sounds.size() == 0 )
			return -1;
		
		int i = ofRandom( 0, sounds.size() );
		sounds[ i ]->setVolume( volume );
		sounds[ i ]->play();
		
		return i;
	}
	
	void playSoundAtIndex ( vector<ofxALSoundPlayer*>& sounds, int index, float volume = 1.0 )
	{
		if( sounds.size() == 0 )
			return;
		
		int i = ofClamp( index, 0, MIN( index, sounds.size() - 1 ) );
		
		sounds[ i ]->setVolume( volume );
		sounds[ i ]->play();
	}
	
#else
	
	//////////////////////////////////////////
	//	PC METHODS.
	//////////////////////////////////////////
	
	void loadSounds( string dirPath, vector<ofSoundPlayer*>& soundVector, bool loop = false )
	{
		int t = 0;
		t = DIR.listDir( dirPath );
		for( int i=0; i<t; i++ )
		{
			ofSoundPlayer* sp;
			sp = new ofSoundPlayer();
			sp->loadSound( DIR.getPath(i) );
			sp->setLoop( loop );
			sp->setMultiPlay( true );
			
			soundVector.push_back( sp );
		}
	}
	
	void loadNumOfSounds ( string dirPath, vector<ofSoundPlayer*>& soundVector, int numOfSounds = 10, bool loop = false )
	{
		int t = 0;
		t = DIR.listDir( dirPath );
		for( int i=0; i<numOfSounds; i++ )
		{
			int j = ofRandom( 0, t );
			
			ofSoundPlayer* sp;
			sp = new ofSoundPlayer();
			sp->loadSound( DIR.getPath( j ) );
			sp->setLoop( loop );
			
			soundVector.push_back( sp );
		}
	}
	
	int playRandomSound ( vector<ofSoundPlayer*>& sounds, float volume = 1.0 )
	{
		if( sounds.size() == 0 )
			return -1;
		
		int i = ofRandom( 0, sounds.size() );
		sounds[ i ]->setVolume( volume );
		sounds[ i ]->play();
		
		return i;
	}
	
	void playSoundAtIndex ( vector<ofSoundPlayer*>& sounds, int index, float volume = 1.0 )
	{
		if( sounds.size() == 0 )
			return;
		
		int i = ofClamp( index, 0, MIN( index, sounds.size() - 1 ) );
		
		sounds[ i ]->setVolume( volume );
		sounds[ i ]->play();
	}
	
#endif

	//////////////////////////////////////////
	//	NEXT.
	//////////////////////////////////////////
	
	void playNextBackgroundSound ()
	{
		stopBackgroundSound();
		
		if( ++backgroundSoundIndex > spBackground.size() - 1 )
			backgroundSoundIndex = 0;
		
		playBackgroundSoundAtIndex( backgroundSoundIndex );
	}
	
	//////////////////////////////////////////
	//	STOP.
	//////////////////////////////////////////
	
	void stopBackgroundSound ()
	{
		if( backgroundSoundIndex != -1 )
		{
			spBackground[ backgroundSoundIndex ]->stop();
		}
	}
	
	//////////////////////////////////////////
	//	RANDOM.
	//////////////////////////////////////////
	
	void playRandomBackgroundSound ( float volume = 1.0 )
	{
		backgroundSoundIndex = playRandomSound( spBackground, volume );
	}
	
	void playRandomMeshDragSound ( float volume = 1.0 )
	{
		playRandomSound( spMeshDrag, volume );
	}
	
	void playRandomPointAddSound ( float volume = 1.0 )
	{
		playRandomSound( spPointAdd, volume );
	}
	
	void playRandomPointRemoveSound ( float volume = 1.0 )
	{
		playRandomSound( spPointRemove, volume );
	}
	
	void playRandomPointCollisionSound ( float volume = 1.0 )
	{
		if( bCollisionSoundTimeout )
			return;
		
		playRandomSound( spPointCollide, volume );
	}
	
	void playRandomPointShuffleSound ( float volume = 1.0 )
	{
		playRandomSound( spPointShuffle, volume );
	}
	
	//////////////////////////////////////////
	//	INDEX.
	//////////////////////////////////////////
	
	void playBackgroundSoundAtIndex ( int index, float volume = 1.0 )
	{
		playSoundAtIndex( spBackground, index, volume );
	}
	
	void playMeshDragSoundAtIndex ( int index, float volume = 1.0 )
	{
		playSoundAtIndex( spMeshDrag, index, volume );
	}
	
	void playPointAddSoundAtIndex ( int index, float volume = 1.0 )
	{
		playSoundAtIndex( spPointAdd, index, volume );
	}
	
	void playPointRemoveSoundAtIndex ( int index, float volume = 1.0 )
	{
		playSoundAtIndex( spPointRemove, index, volume );
	}
	
	void playPointCollisionSoundAtIndex ( int index, float volume = 1.0 )
	{
		if( bCollisionSoundTimeout )
			return;
		
		playSoundAtIndex( spPointCollide, index, volume );
	}
	
	void playPointShuffleSoundAtIndex ( int index, float volume = 1.0 )
	{
		playSoundAtIndex( spPointShuffle, index, volume );
	}
	
	//////////////////////////////////////////
	//	TIMEOUTS.
	//////////////////////////////////////////
	
	void startCollisionSoundTimeout ( float timeoutInSeconds = 0.3 )
	{
		collisionSoundTimeout		= ofGetElapsedTimef();
		collisionSoundTimeoutLimit	= collisionSoundTimeout + timeoutInSeconds;
		bCollisionSoundTimeout		= true;
	}
	
	void checkCollisionSoundTimeout ()
	{
		if( !bCollisionSoundTimeout )
			return;
		
		float time = ofGetElapsedTimef();
		if( time > collisionSoundTimeoutLimit )
		{
			bCollisionSoundTimeout = false;
		}
	}
	
	//////////////////////////////////////////
	//	UPDATE
	//////////////////////////////////////////
	
	void update ( ofEventArgs &e )
	{
		update();
	}
	
	void update ()
	{
		checkCollisionSoundTimeout();
	}
};