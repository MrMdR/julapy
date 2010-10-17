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

////////////////////////////////////////////////////////
//	OFX_ASSET_ITEM
////////////////////////////////////////////////////////

class ofxAssetItem
{

public:
	
	string			assetID;
	string			assetPath;
	int				assetType;
	ofBaseImage*	asset;
	
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
	
public:
	
	static ofxAssets* getInstance()
	{
		if( !_instance )
		{
			_instance = new ofxAssets();
		}
		
        return _instance;
	}
	
	//==================================================
	
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
	}
	
	void addImage ( string assetID, string assetPath )
	{
		ofImage* image	= new ofImage();
		image->loadImage( assetPath );
		
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID	= assetID;
		item->assetPath	= assetPath;
		item->assetType	= OFX_ASSETS_TYPE_IMAGE;
		item->asset		= image;
		
		items.push_back( item );
	}
	
	void addImage ( string assetID, ofBaseImage& image )
	{
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID	= assetID;
		item->assetPath	= "";
		item->assetType	= OFX_ASSETS_TYPE_IMAGE;
		item->asset		= &image;
		
		items.push_back( item );
	}
	
	void addVideo ( string assetID, string assetPath )
	{
		ofVideoPlayer* video;
		video = new ofVideoPlayer();
		video->loadMovie( assetPath );
		
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID	= assetID;
		item->assetPath	= assetPath;
		item->assetType	= OFX_ASSETS_TYPE_VIDEO;
		item->asset		= video;
		
		items.push_back( item );
	}
	
	void addVideo ( string assetID, ofBaseImage& video )
	{
		ofxAssetItem* item;
		item = new ofxAssetItem();
		item->assetID	= assetID;
		item->assetPath	= "";
		item->assetType	= OFX_ASSETS_TYPE_VIDEO;
		item->asset		= &video;
		
		items.push_back( item );
	}
	
	ofBaseImage* getAsset ( string assetID )
	{
		for( int i=0; i<items.size(); i++ )
		{
			ofxAssetItem& item = *items[ i ];
			
			if( item.assetID == assetID )
			{
				return item.asset;
			}
		}
		
		return NULL;
	}
};