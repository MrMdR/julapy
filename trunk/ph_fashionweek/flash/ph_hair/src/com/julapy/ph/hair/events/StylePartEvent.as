package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class StylePartEvent extends Event
	{
		public static const STYLE_PART_CHANGE : String = "stylePartChange";

		public var stylePart : int;

		public function StylePartEvent( stylePart : int )
		{
			super( STYLE_PART_CHANGE, false, false );

			this.stylePart = stylePart;
		}

	}
}