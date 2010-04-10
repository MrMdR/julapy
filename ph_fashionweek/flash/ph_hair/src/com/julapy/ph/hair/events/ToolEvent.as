package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class ToolEvent extends Event
	{
		public static const TOOL_CHANGE	: String = "toolChange";

		public var tool : int;

		public function ToolEvent( tool : int )
		{
			super( TOOL_CHANGE );

			this.tool = tool;
		}

	}
}