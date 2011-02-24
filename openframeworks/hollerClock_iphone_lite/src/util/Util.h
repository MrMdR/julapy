/*
 *  Util.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 24/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class Util
{
	
public:
	
	static Util* getInstance()
	{
		if( !_instance )
		{
			_instance = new Util();
		}
		
        return _instance;
	};
	
	void launchSite ( string url );
	
private: 
	
	static Util* _instance;
	
	 Util() {};
	~Util() {};
	
};