/*
 *  ofxAssets.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#define OFX_ASSETS_TYPE_IMAGE	0
#define OFX_ASSETS_TYPE_VIDEO	1
#define OFX_ASSETS_TYPE_SOUND	2

////////////////////////////////////////////////////////
//	OFX_ASSET_ITEM
////////////////////////////////////////////////////////

class ofxAssetItem
{

public:
	
	string			assetID;
	string			assetPath;
	int				assetType;
	ofBaseImage*	imageAsset;
	ofSoundPlayer*	soundAsset;
	
};

////////////////////////////////////////////////////////
//	OFX_ASSETS
////////////////////////////////////////////////////////

class ofxAssets
{

private: 
	
	static ofxAssets* _instance;
	
	 ofxAssets() {};
	~ofxAssets() {};
	
	vector<ofxAssetItem*> items;
	vector<ofxAssetItem*> imageItems;
	vector<ofxAssetItem*> videoItems;
	vector<ofxAssetItem*> soundItems;
	
public:
	
	static ofxAssets* getInstance()
	{
		if( !_instance )
		{
			_instance = new ofxAssets();
		}
		
        return _instance;
	}
	
	//================================================== GENERAL.
	
	void addAsset( string assetID, string assetPath, int assetType )
	{
		if( assetType == OFX_ASSETS_TYPE_IMAGE )
		{
			addImage( assetPath, assetID );
		}
		else if( assetType == OFX_ASSETS_TYPE_VIDEO )
		{
			addVideo( assetPath, assetID );
		}
		else if( assetType == OFX_ASSETS_TYPE_SOUND )
		{
			addSound( assetPath, assetID );
		}
	}
	
	//================================================== IMAGE.
	
	void addImage ( string assetID, string assetPath )
	{
		ofImage* image	= new ofImage();
		image->loadImage( assetPath );
		
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID		= assetID;
		item->assetPath		= assetPath;
		item->assetType		= OFX_ASSETS_TYPE_IMAGE;
		item->imageAsset	= image;
		item->soundAsset	= NULL;
		
		items.push_back( item );
		imageItems.push_back( item );
	}
	
	void addImage ( string assetID, ofBaseImage& image )
	{
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID		= assetID;
		item->assetPath		= "";
		item->assetType		= OFX_ASSETS_TYPE_IMAGE;
		item->imageAsset	= &image;
		item->soundAsset	= NULL;
		
		items.push_back( item );
		imageItems.push_back( item );
	}
	
	//================================================== VIDEO.
	
	void addVideo ( string assetID, string assetPath )
	{
		ofVideoPlayer* video;
		video = new ofVideoPlayer();
		video->loadMovie( assetPath );
		
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID		= assetID;
		item->assetPath		= assetPath;
		item->assetType		= OFX_ASSETS_TYPE_VIDEO;
		item->imageAsset	= video;
		item->soundAsset	= NULL;
		
		items.push_back( item );
		videoItems.push_back( item );
	}
	
	void addVideo ( string assetID, ofBaseImage& video )
	{
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID		= assetID;
		item->assetPath		= "";
		item->assetType		= OFX_ASSETS_TYPE_VIDEO;
		item->imageAsset	= &video;
		item->soundAsset	= NULL;
		
		items.push_back( item );
		videoItems.push_back( item );
	}
	
	//================================================== SOUND.
	
	void addSound ( string assetID, string assetPath )
	{
		ofSoundPlayer* sound;
		sound = new ofSoundPlayer();
		sound->loadSound( assetPath );
		
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID		= assetID;
		item->assetPath		= assetPath;
		item->assetType		= OFX_ASSETS_TYPE_SOUND;
		item->imageAsset	= NULL;
		item->soundAsset	= sound;
		
		items.push_back( item );
		soundItems.push_back( item );
	}
	
	void addSound ( string assetID, ofSoundPlayer& sound )
	{
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID		= assetID;
		item->assetPath		= "";
		item->assetType		= OFX_ASSETS_TYPE_SOUND;
		item->imageAsset	= NULL;
		item->soundAsset	= &sound;
		
		items.push_back( item );
		soundItems.push_back( item );
	}
	
	ofSoundPlayer* getSound ( string assetID )
	{
		for( int i=0; i<soundItems.size(); i++ )
		{
			ofxAssetItem& item = *soundItems[ i ];
			
			if( item.assetID == assetID )
			{
				return item.soundAsset;
			}
		}
		
		return NULL;
	}
	
	ofSoundPlayer* getSoundByFileName ( string fileName )
	{
		if( fileName == "" )
			return NULL;
		
		for( int i=0; i<soundItems.size(); i++ )
		{
			ofxAssetItem& item = *soundItems[ i ];
			
			if( item.assetPath == fileName )
			{
				return item.soundAsset;
			}
		}
		
		return NULL;
	}
	
	//==================================================
	
	ofBaseImage* getAsset ( string assetID )
	{
		for( int i=0; i<items.size(); i++ )
		{
			ofxAssetItem& item = *items[ i ];
			
			if( item.assetID == assetID )
			{
				return item.imageAsset;
			}
		}
		
		return NULL;
	}

	ofBaseImage* getAssetByFileName ( string fileName )
	{
		if( fileName == "" )
			return NULL;
		
		for( int i=0; i<items.size(); i++ )
		{
			ofxAssetItem& item = *items[ i ];
			
			if( item.assetPath == fileName )
			{
				return item.imageAsset;
			}
		}
		
		return NULL;
	}
};