package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class ToolTriggerEvent extends Event
	{
		public static const TOOL_TRIGGER_ON		: String = "toolTriggerOn";
		public static const TOOL_TRIGGER_OFF	: String = "toolTriggerOff";

		public var toolTrigger : int;

		public function ToolTriggerEvent( type : String, toolTrigger : int )
		{
			super( type );

			this.toolTrigger = toolTrigger;
		}

	}
}