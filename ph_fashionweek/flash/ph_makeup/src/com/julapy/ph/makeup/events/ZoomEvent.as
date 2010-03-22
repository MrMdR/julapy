package com.julapy.ph.makeup.events
{
	import flash.events.Event;
	import flash.geom.Point;

	public class ZoomEvent extends Event
	{
		public static const ZOOM	: String = "zoom";

		public var zoomScale 	: Number = 0;
		public var zoomOffset	: Point;

		public function ZoomEvent( zoomScale : Number, zoomOffset : Point )
		{
			super( ZOOM, false, false );

			this.zoomScale 	= zoomScale;
			this.zoomOffset	= zoomOffset.clone();
		}

	}
}