package com.julapy.ph.makeup.events
{
	import flash.events.Event;

	public class AttractorChangeEvent extends Event
	{
		public static const ATTRACTOR_CHANGE	: String = "attractorChange";

		public var bAttractor	: Boolean;

		public function AttractorChangeEvent( bAttractor : Boolean )
		{
			super( ATTRACTOR_CHANGE );

			this.bAttractor = bAttractor;
		}

	}
}