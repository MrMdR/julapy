/*
 *  ofxBox2dContactReceiver.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 26/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "Box2D.h"

class ofxBox2dContactReceiver
{

public :
	
	virtual void box2dContactEventHandler ( const b2ContactPoint* point ) {};
	
};