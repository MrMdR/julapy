package com.julapy.core
{
	import flash.events.Event;

	public class StageSizeEvent extends Event
	{
		public static const RESIZE	: String = Event.RESIZE;

		public var stageWidth	: int = 0;
		public var stageHeight	: int = 0;

		public function StageSizeEvent(type:String=RESIZE, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}

	}
}