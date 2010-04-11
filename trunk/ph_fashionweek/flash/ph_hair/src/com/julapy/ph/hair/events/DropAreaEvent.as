package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class DropAreaEvent extends Event
	{
		public static const DROP_AREA_COMPLETE		: String = "dropAreaComplete";
		public static const DROP_AREA_PLAYED_OUT	: String = "dropAreaPlayedOut";

		public function DropAreaEvent( type:String )
		{
			super( type );
		}

	}
}