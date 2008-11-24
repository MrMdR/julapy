package
{
	import com.holler.controls.VideoView;
	import com.holler.events.VideoViewEvent;
	import com.holler.utils.MathUtil;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageDisplayState;
	import flash.display.StageQuality;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.geom.Rectangle;
	
	[SWF(backgroundColor="0x000000", frameRate="31", width="950", height="580")] 
	
	public class Rockpool extends Sprite
	{
		private var videoContainer:Sprite;
		private var video:VideoView;
		private var videoWidth:int	= 500;
		private var videoHeight:int	= 333;
		
		public function Rockpool()
		{
			stage.align			= StageAlign.TOP_LEFT;
			stage.quality		= StageQuality.BEST;
			stage.scaleMode 	= StageScaleMode.NO_SCALE;
			
			var r:Number;
			var rect:Rectangle;
			
			r		= MathUtil.scaleToFitRatio( stage.fullScreenWidth, stage.fullScreenHeight, videoWidth, videoHeight );
			rect	= new Rectangle( 0, 0, (int)( videoWidth * r ), (int)( videoHeight * r ) );
			rect	= new Rectangle( 0, 0, videoWidth, videoHeight );
			
			videoContainer		= new Sprite()
			video				= new VideoView( videoContainer, rect, 1 );
			video.addEventListener( VideoViewEvent.READY, onVideoReady );
			video.loop			= true;
			video.videoURI		= "assets/rockpool.flv";
			
			addChild( videoContainer );

			stage.addEventListener( KeyboardEvent.KEY_DOWN,	keyPressHandler );
			stage.addEventListener( Event.RESIZE,			stageResizeHandler );
			stageResizeHandler( );
		}
		
		private function onVideoReady ( e:VideoViewEvent ):void
		{
			video.paused	= false;
		}
		
		private function stageResizeHandler ( e:Event=null ):void
		{
			var r:Number;
			var rect:Rectangle;
			
			r		= MathUtil.scaleToFitRatio( stage.fullScreenWidth, stage.fullScreenHeight, videoWidth, videoHeight ) * 5;
			rect	= new Rectangle( 0, 0, (int)( videoWidth * r ), (int)( videoHeight * r ) );

			video.width		= rect.width;
			video.height	= rect.height;
			
			videoContainer.x	= (int)( ( stage.fullScreenWidth - rect.width ) * 0.5 );
			videoContainer.y	= (int)( ( stage.fullScreenHeight - rect.height ) * 0.5 );
		}
		
		private function keyPressHandler ( e:KeyboardEvent ):void
		{
			stage.displayState = StageDisplayState.FULL_SCREEN;
		}
		
	}
}