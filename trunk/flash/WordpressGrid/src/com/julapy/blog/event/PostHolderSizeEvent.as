package com.julapy.blog.event
{
	import flash.events.Event;
	import flash.geom.Rectangle;

	public class PostHolderSizeEvent extends Event
	{
		public static const POST_HOLDER_SIZE_EVENT_CHANGE	: String = "postHolderSizeEventChange";

		public var size : Rectangle;

		public function PostHolderSizeEvent( rect : Rectangle )
		{
			super( POST_HOLDER_SIZE_EVENT_CHANGE );

			size = rect.clone();
		}

	}
}