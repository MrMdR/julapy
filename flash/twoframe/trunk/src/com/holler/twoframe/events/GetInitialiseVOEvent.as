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

	public class GetInitialiseVOEvent extends CairngormEvent
	{
		public static const GET_INITIALISE_VO:String = "GetInitialiseVOEvent.getInitialiseVO";
		
		public function GetInitialiseVOEvent(type:String=GET_INITIALISE_VO, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
		override public function clone():Event
		{
			return new GetInitialiseVOEvent(type);
		}
		
	}
}