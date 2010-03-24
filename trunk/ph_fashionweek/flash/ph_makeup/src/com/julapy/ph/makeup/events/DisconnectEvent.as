package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class DisconnectEvent extends Event
	{
		public static const DISCONNECT	: String = "disconnect";

		public function DisconnectEvent()
		{
			super( DISCONNECT );
		}

	}
}