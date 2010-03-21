package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class BlinkEvent extends Event
	{
		public static const BLINK_START	: String = "blinkStart";
		public static const BLINK_STOP	: String = "blinkStop";

		public var blinking : Boolean;

		public function BlinkEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);

			if( type == BLINK_START )
				blinking = true;

			if( type == BLINK_STOP )
				blinking = false;
		}

	}
}