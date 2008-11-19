////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.events
{
	import com.adobe.cairngorm.control.CairngormEvent;
	import flash.events.Event;

	public class GetImagesEvent extends CairngormEvent
	{
		public static const GET_IMAGES:String = "GetImagesEvent.getImages";
		
		public function GetImagesEvent( type:String=GET_IMAGES, bubbles:Boolean=false, cancelable:Boolean=false )
		{
			super(type, bubbles, cancelable);
		}
		
		override public function clone():Event
		{
			return new GetImagesEvent(type);
		}
		
	}
}