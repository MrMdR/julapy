package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class SectionEvent extends Event
	{
		public static const SECTION_CHANGE : String = "sectionChange";

		public var section : int;

		public function SectionEvent( section : int )
		{
			super( SECTION_CHANGE, false, false );

			this.section = section;
		}

	}
}