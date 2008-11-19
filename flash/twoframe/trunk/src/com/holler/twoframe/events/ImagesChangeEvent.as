////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.events
{
	import com.holler.twoframe.vo.InitialiseVO;
	
	import flash.events.Event;

	public class ImagesChangeEvent extends Event
	{
		public static const IMAGES_CHANGE:String = "imagesChange";
		
		public function ImagesChangeEvent( type:String=IMAGES_CHANGE, bubbles:Boolean=false, cancelable:Boolean=false )
		{
			super(type, bubbles, cancelable);
		}
		
		override public function clone():Event
		{
			return new ImagesChangeEvent(type);
		}
		
	}
}