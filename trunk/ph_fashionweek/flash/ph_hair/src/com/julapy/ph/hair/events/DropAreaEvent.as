package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class DropAreaEvent extends Event
	{
		public static const DROP_AREA_PLAYED_IN		: String = "dropAreaPlayedIn";
		public static const DROP_AREA_PLAYED_OUT	: String = "dropAreaPlayedOut";
		public static const DROP_AREA_CHARGED 		: String = "dropAreaCharged";

		public function DropAreaEvent( type:String )
		{
			super( type );
		}

	}
}