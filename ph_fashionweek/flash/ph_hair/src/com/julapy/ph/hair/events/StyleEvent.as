package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class StyleEvent extends Event
	{
		public static const STYLE_CHANGE : String = "styleChange";

		public var style : int;

		public function StyleEvent( style : int )
		{
			super( STYLE_CHANGE, false, false );

			this.style = style;
		}

	}
}