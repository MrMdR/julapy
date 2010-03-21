package com.julapy.ph.makeup.of
{
	import flash.events.Event;

	public class SocketOFEvent extends Event
	{
		public static const CONNECTED			: String = "connected";
		public static const TRYING_TO_CONNECT	: String = "tryingToConnect";
		public static const DISCONNECTED		: String = "disconnected";
		public static const DATA_RECEIVED		: String = "dataReceived";

		public var connected	: Boolean	= false;
		public var dataString	: String	= "";

		public function SocketOFEvent( type : String )
		{
			super( type, false, false );
		}

	}
}