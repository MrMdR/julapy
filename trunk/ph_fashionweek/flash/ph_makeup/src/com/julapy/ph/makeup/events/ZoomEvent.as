package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class ZoomEvent extends Event
	{
		public static const ZOOM	: String = "zoom";

		public var zoomScale : Number = 0;

		public function ZoomEvent( zoomScale : Number )
		{
			super( ZOOM, false, false );

			this.zoomScale = zoomScale;
		}

	}
}