package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class ModeEvent extends Event
	{
		public static const MODE : String = "mode";

		public var mode : int = -1;

		public function ModeEvent( mode : int )
		{
			super( MODE, false, false );

			this.mode = mode;
		}

	}
}