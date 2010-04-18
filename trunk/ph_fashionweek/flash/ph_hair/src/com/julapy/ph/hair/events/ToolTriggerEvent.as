package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class ToolTriggerEvent extends Event
	{
		public static const TOOL_TRIGGER	: String = "toolTrigger";

		private var toolTrigger : int;

		public function ToolTriggerEvent( toolTrigger : int )
		{
			super( TOOL_TRIGGER );

			this.toolTrigger = toolTrigger;
		}

	}
}