package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class GirlEvent extends Event
	{
		public static const GIRL_CHANGE	: String = "girlChange";

		public var girl : int;

		public function GirlEvent( girl : int )
		{
			super( GIRL_CHANGE, false, false );

			this.girl = girl;
		}

	}
}