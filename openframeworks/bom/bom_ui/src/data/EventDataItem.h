/*
 *  EventDataItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class EventDataItem
{
	
public:
	
	 EventDataItem() {};
	~EventDataItem() {};
	
	int		id;
	string	title;
	int		year;
	float	lat;
	float	lon;
	string	thumb;
	string	image;
	string	video;
	string	sound;
	string	body;
	int		type;
	
};