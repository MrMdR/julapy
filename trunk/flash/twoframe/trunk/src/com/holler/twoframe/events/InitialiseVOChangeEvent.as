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

	public class InitialiseVOChangeEvent extends Event
	{
		public static const INITIALISE_V_O_CHANGE:String = "initialiseVOChange";
		
		public var initialiseVO:InitialiseVO;
		
		public function InitialiseVOChangeEvent(type:String=INITIALISE_V_O_CHANGE, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
		override public function clone():Event
		{
			return new GetInitialiseVOEvent(type);
		}
		
	}
}