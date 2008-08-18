package com.holler.video
{
	import flash.events.Event;

	public class VideoPlayerEvent extends Event
	{
		public static const PLAY_START:String 	= "PLAY_START";
		public static const PLAY_STOP:String	= "PLAY_STOP";
		public static const UPDATE:String 		= "UPDATE";

		public var time:Number 		= 0;
		public var duration:Number 	= 0;
		
		public function VideoPlayerEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
	}
}