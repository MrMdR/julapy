package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class ResetEvent extends Event
	{
		public static const RESET : String = "reset";

		public function ResetEvent()
		{
			super( RESET );
		}

	}
}