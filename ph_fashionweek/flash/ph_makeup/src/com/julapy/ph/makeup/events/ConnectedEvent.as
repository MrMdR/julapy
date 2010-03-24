package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class ConnectedEvent extends Event
	{
		public static const CONNECTED		: String = "connected";
		public static const DISCONNECTED	: String = "disconnected";

		public var connected	: Boolean;

		public function ConnectedEvent( connected : Boolean )
		{
			this.connected = connected;

			var eType : String;

			if( connected )
			{
				eType = CONNECTED;
			}
			else
			{
				eType = DISCONNECTED;
			}

			super( eType, false, false );
		}
	}
}