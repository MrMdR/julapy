package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class ModeEvent extends Event
	{
		public static const MODE_ZOOM_IN	: String = "modeZoomIn";
		public static const MODE_ANIM_IN	: String = "modeAnimIn";
		public static const MODE_ZOOM_OUT	: String = "modeAnimOut";

		public var mode : int = -1;

		public function ModeEvent( type : String, mode : int )
		{
			super( type, false, false );

			this.mode = mode;
		}

	}
}