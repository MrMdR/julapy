/*
 *  SoundManager.h
 *  bom
 *
 *  Created by lukasz karluk on 30/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxAssets.h"

class SoundManager
{
	
private: 
	
	static SoundManager* _instance;
	
	SoundManager() 
	{
		playLoop();		// play on launch and only once.
	};
	
	~SoundManager() {};
	
public:
	
	static SoundManager* getInstance()
	{
		if( !_instance )
		{
			_instance = new SoundManager();
		}
		
        return _instance;
	}

	void playLoop ()
	{
		ofSoundPlayer* loop = ofxAssets :: getInstance()->getSound( "loop" );
		loop->setLoop( true );
		loop->setVolume( 0.2 );
		loop->play();
	}
	
	void playAttractor ()
	{
		ofSoundPlayer* sound;
		sound = ofxAssets :: getInstance()->getSound( "attractor" );
		sound->play();
	}
	
	void stopAllSoundsButLoop ()
	{
		vector<string> soundsToStop;
		soundsToStop.push_back( "attractor" );
		soundsToStop.push_back( "instruct_0" );
		soundsToStop.push_back( "instruct_1" );
		
		ofSoundPlayer* sound;
		
		for( int i=0; i<soundsToStop.size(); i++ )
		{
			sound = ofxAssets :: getInstance()->getSound( soundsToStop[ i ] );
			sound->stop();
		}
	}

	void playInstructionalSound ()
	{
		if( ofRandom( 0, 1 ) > 0.75 )		// not every time.
		{
			ofSoundPlayer* sound;
			sound = ofxAssets :: getInstance()->getSound( "instruct_1" );
			
			if( sound->getPosition() == 0 )		// only play if its not playing.
			{
				sound->setPosition( 0 );
				sound->play();
			}
		}
	}
	
	void playIdle ()
	{
		vector<string> idleSounds;
		idleSounds.push_back( "attractor" );
		idleSounds.push_back( "instruct_0" );
		
		ofSoundPlayer* sound;
		sound = ofxAssets :: getInstance()->getSound( idleSounds[ (int)( ofRandom( 0, idleSounds.size() ) ) ] );
		sound->setPosition( 0 );
		sound->play();
	}
	
	void playClick ()
	{
		ofSoundPlayer* sound;
		sound = ofxAssets :: getInstance()->getSound( "click_sound" );
		sound->play();
	}
};