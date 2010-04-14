package com.julapy.ph.hair.events
{
	import flash.events.Event;
	import flash.geom.Point;

	public class ToolPathEvent extends Event
	{
		public static const TOOL_PATH_POSITION_CHANGE	: String = "toolPathPositionChange";

		public var p : Point;

		public function ToolPathEvent( p : Point )
		{
			super( TOOL_PATH_POSITION_CHANGE );

			this.p = p.clone();
		}

	}
}