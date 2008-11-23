package
{
	import com.holler.controls.VideoView;
	import com.holler.events.VideoViewEvent;
	
	import flash.display.Sprite;
	import flash.geom.Rectangle;
	
	[SWF(backgroundColor="0x000000", frameRate="31", width="950", height="580")] 
	
	public class Rockpool extends Sprite
	{
		private var videoContainer:Sprite;
		private var video:VideoView;
		
		public function Rockpool()
		{
			addChild( videoContainer = new Sprite() );
			video			= new VideoView( videoContainer, new Rectangle( 0, 0,  500, 333 ), 1 );
			video.addEventListener( VideoViewEvent.READY, onVideoReady );
			video.addEventListener( VideoViewEvent.START, onVideoStart );
			video.videoURI	= "assets/rockpool.flv";
			video.loop		= true;
			video.paused	= false;
		}
		
		private function onVideoReady ( e:VideoViewEvent ):void
		{
			trace( "onVideoReady" );
		}
		
		private function onVideoStart ( e:VideoViewEvent ):void
		{ 
			trace( "onVideoStart" );
		}

	}
}