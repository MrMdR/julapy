package com.julapy.blog.event
{
	import flash.events.Event;

	public class PostScrollPositionEvent extends Event
	{
		public static const POST_SCROLL_POSITION_CHANGE	: String = "postScrollPositionEventChange";

		public var position : Number;

		public function PostScrollPositionEvent( p : Number )
		{
			super( POST_SCROLL_POSITION_CHANGE );

			position = p;
		}

	}
}