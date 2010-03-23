package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class BlinkEvent extends Event
	{
		public static const BLINK_START			: String = "blinkStart";
		public static const BLINK_STOP			: String = "blinkStop";

		public static const BLINK_FORCE_START	: String = "blinkForceStart";
		public static const BLINK_FORCE_STOP	: String = "blinkForceStop";

		public var blinking 	: Boolean;
		public var blinkForce	: Boolean;

		public function BlinkEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);

			//-- blinking.

			if( type == BLINK_START )
				blinking = true;

			if( type == BLINK_STOP )
				blinking = false;

			//-- blink force.

			if( type == BLINK_FORCE_START )
				blinkForce = true;

			if( type == BLINK_FORCE_STOP )
				blinkForce = false;
		}

	}
}