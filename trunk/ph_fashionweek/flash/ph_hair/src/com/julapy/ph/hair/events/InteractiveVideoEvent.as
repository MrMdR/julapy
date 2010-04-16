package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class InteractiveVideoEvent extends Event
	{
		public static const PLAYING_IN		: String = "interactiveVideoPlayingIn";
		public static const PLAYED_IN		: String = "interactiveVideoPlayedIn";
		public static const PLAYING_OUT		: String = "interactiveVideoPlayingOut";
		public static const PLAYED_OUT		: String = "interactiveVideoPlayedIn";

		public function InteractiveVideoEvent( type:String )
		{
			super( type );
		}

	}
}