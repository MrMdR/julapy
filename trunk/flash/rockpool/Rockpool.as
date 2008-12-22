package
{
	import com.holler.controls.VideoView;
	import com.holler.events.VideoViewEvent;
	import com.holler.utils.MathUtil;
	import com.quasimondo.geom.ColorMatrix;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.BlendMode;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageQuality;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	[SWF(backgroundColor="0x000000", frameRate="31", width="950", height="580")] 
	
	public class Rockpool extends Sprite
	{
		[Embed(source="assets/tile_06.png")]
		private var Tile:Class;
		
		private var stageWidth			:uint = 0;
		private var stageHeight			:uint = 0;
		
//		private var videoWidth			:int = 500;
//		private var videoHeight			:int = 333;
		private var videoWidth			:int = 1000;
		private var videoHeight			:int = 666;
		private var videoPixelSize		:int = 50;
		private var videoPixelCount		:int = 5;

		private var video				:VideoView;
		private var container			:Sprite = new Sprite();
		private var videoContainer		:Sprite = new Sprite();
		private var videoPixelate		:Sprite = new Sprite();
		private var videoOverlay		:Sprite = new Sprite();
		private var videoOverlayBitmap	:Bitmap;
		private var videoPixelateBitmap	:Bitmap;
		private var videoFrames			:Array = new Array();
		private var videoToStageRatio	:Number;
		
		private var stereoLayer			:Sprite = new Sprite();
		private var stereoBm01			:Bitmap;
		private var stereoBm02			:Bitmap;
		private var stereoRect			:Rectangle;
		private var stereoItems			:Array = new Array();
		
		public function Rockpool()
		{
			stage.align			= StageAlign.TOP_LEFT;
			stage.quality		= StageQuality.BEST;
			stage.scaleMode 	= StageScaleMode.NO_SCALE;
			
			video				= new VideoView( videoContainer, new Rectangle( 0, 0, videoWidth, videoHeight ), 1 );
			video.addEventListener( VideoViewEvent.READY,	onVideoReady );
			video.loop			= true;
			video.videoURI		= "assets/rockpool_1000x666.flv";
			
			videoOverlayBitmap	= new Tile();
			
			videoOverlay.buttonMode = true;
			stereoLayer.buttonMode	= true;

//			videoPixelate.alpha		= 0.95;
//			videoPixelate.blendMode	= BlendMode.SUBTRACT;
			stereoLayer.blendMode	= BlendMode.DIFFERENCE;
			
			var colorFilter:ColorMatrix = new ColorMatrix( null );
			colorFilter.adjustSaturation( 0 );
			
			videoPixelateBitmap	= new Bitmap();
			videoPixelateBitmap.filters = [ colorFilter.filter ];
			videoPixelate.addChild( videoPixelateBitmap );
			
			addChild( container );
			container.addChild( videoContainer );
			container.addChild( videoPixelate );
			container.addChild( stereoLayer );
			container.addChild( videoOverlay );
			
			stage.addEventListener( MouseEvent.MOUSE_DOWN,	stageMouseDownHandler );
			stage.addEventListener( MouseEvent.MOUSE_UP,	stageMouseUpHandler );
			stage.addEventListener( KeyboardEvent.KEY_DOWN,	keyPressHandler );
			stage.addEventListener( Event.RESIZE,			stageResizeHandler );
			stageResizeHandler();
		}
		
		/////////////////////////////////////////////
		//	OVERLAY.
		/////////////////////////////////////////////
		
		private function drawOverlay ( w:int, h:int ):void
		{
			videoOverlay.graphics.clear();
			videoOverlay.graphics.beginBitmapFill( videoOverlayBitmap.bitmapData, null, true );
			videoOverlay.graphics.drawRect( 0, 0, w, h );
			videoOverlay.graphics.endFill();
		}

		/////////////////////////////////////////////
		//	RECORD FRAMES.
		/////////////////////////////////////////////

		private function addEnterFrame ():void
		{
			stage.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}
		
		private function removeEnterFrame ():void
		{
			stage.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}
		
		private function enterFrameHandler ( e:Event=null ):void
		{
			var srcBmd:BitmapData;
			var r:Number;
			var w:int, h:int;
			
			r	= videoToStageRatio;
			w	= videoWidth * r;
			h	= videoHeight * r;
			
			srcBmd = new BitmapData( videoWidth, videoHeight, false, 0xFFFFFF );
			srcBmd.draw( videoContainer );
			
			videoPixelateBitmap.bitmapData = pixelate( srcBmd, w, h, videoPixelSize );
		}
		
		/////////////////////////////////////////////
		//	PIXELATE.
		/////////////////////////////////////////////
		
		public function pixelate ( bmd:BitmapData, outBmdWidth:int, outBmdHeight:int, pixelSize:uint=1 ):BitmapData
		{
			var srcBmd:BitmapData	= bmd.clone();
			var outBmd:BitmapData	= new BitmapData( outBmdWidth, outBmdHeight );
			var rx:Number			= bmd.width / outBmdWidth;
			var ry:Number			= bmd.height / outBmdHeight;
			
			for( var y:uint=0; y < outBmdHeight; y+=pixelSize )
			{  
				for( var x:uint=0; x < outBmdWidth; x+=pixelSize )
				{
 					if( x + pixelSize <= outBmdWidth && y + pixelSize <= outBmdHeight )
						outBmd.fillRect	(	
											new Rectangle	( x, y, pixelSize, pixelSize ),
											
											bmd.getPixel32	(
																(int)( rx * ( x + (int)(pixelSize/2) ) ), 
																(int)( ry * ( y + (int)(pixelSize/2) ) ) 
															) 
										);

					if( x + pixelSize > outBmdWidth )
						outBmd.fillRect	(
											new Rectangle	( x, y, outBmdWidth - x, pixelSize ),
											 
											bmd.getPixel32	(
																(int)( rx * ( x + (int)((outBmdWidth-x)/2) ) ),
																(int)( ry * (y + (int)(pixelSize/2) ) )
															) 
										);
					
					if( y + pixelSize > outBmdHeight )
						outBmd.fillRect	(	
											new Rectangle	( x, y, pixelSize, outBmdHeight - y ),
											
											bmd.getPixel32	(
																(int)( rx * ( x + (int)(pixelSize/2) ) ),
																(int)( ry * ( y + (int)((outBmdHeight-y)/2) ) )
															) 
										);
				}
			}
			
			return outBmd;
		}
		
		/////////////////////////////////////////////
		//	FRAME BITMAP.
		/////////////////////////////////////////////
		
		private function getStereoBitmapData ( rect:Rectangle ):BitmapData
		{
			var srcBmd:BitmapData;
			var dstBmd:BitmapData;
			var r:Number;			
			
			srcBmd = new BitmapData( videoWidth, videoHeight, false, 0xFFFFFF );
			srcBmd.draw( videoContainer );
			
			r = videoToStageRatio;
			 
			dstBmd = new BitmapData( rect.width/r, rect.height/r, false, 0xFFFFFF );
			dstBmd.copyPixels( srcBmd, new Rectangle( rect.x/r, rect.y/r, rect.width/r , rect.height/r ), new Point( 0, 0 ) );
			
			return dstBmd;
		}
		
		/////////////////////////////////////////////
		//	EVENTS.
		/////////////////////////////////////////////
		
		private function onVideoReady ( e:VideoViewEvent ):void
		{
			video.paused	= false;
			
			addEnterFrame();
		}
		
		private function stageMouseDownHandler ( e:MouseEvent ):void
		{
			var bmd:BitmapData;
			var rx	:int = 0; 
			var ry	:int = 0;
			var rw	:int = 0; 
			var rh	:int = 0;4
			
			rx = Math.max( 0, (int)( ( -videoContainer.x + e.localX ) / videoPixelSize ) - ( videoPixelCount - 1 ) * 0.5 ) * videoPixelSize;
			ry = Math.max( 0, (int)( ( -videoContainer.y + e.localY ) / videoPixelSize ) - ( videoPixelCount - 1 ) * 0.5 ) * videoPixelSize;
			
//			rx = Math.max( 0, (int)( ( e.localX ) / videoPixelSize ) - ( videoPixelCount - 1 ) * 0.5 ) * videoPixelSize;
//			ry = Math.max( 0, (int)( ( e.localY ) / videoPixelSize ) - ( videoPixelCount - 1 ) * 0.5 ) * videoPixelSize;

			rw = videoPixelSize * videoPixelCount;
			rh = videoPixelSize * videoPixelCount;
			
			stereoRect = new Rectangle( rx, ry, rw, rh );
			
			bmd = getStereoBitmapData( stereoRect );
			
			stereoBm01			= new Bitmap( bmd );
			stereoBm01.width	= stereoRect.width;
			stereoBm01.height	= stereoRect.height;
		}
		
		private function stageMouseUpHandler ( e:MouseEvent ):void
		{
			var bmd:BitmapData;
			
			bmd = getStereoBitmapData( stereoRect );
			
			stereoBm02			= new Bitmap( bmd );
			stereoBm02.width	= stereoRect.width;
			stereoBm02.height	= stereoRect.height;
			
			var stereoItem:RockpoolStereoItem;
			stereoItem = new RockpoolStereoItem( stereoLayer, stereoBm01, stereoBm02, stereoRect.clone() );
			stereoItem.addEventListener( Event.COMPLETE, stereoItemCompleteHandler );
			stereoItems.push( stereoItem );
		}
		
		private function stereoItemCompleteHandler ( e:Event ):void
		{
			var i:int;
			var stereoItem:RockpoolStereoItem;
			
			for( i=0; i<stereoItems.length; i++ )
			{
				stereoItem = stereoItems[ i ];
				
				if( stereoItem == e.target )
				{
					stereoItems.splice( i, 1 );
					stereoItem.destroy();
					stereoItem = null;
					
					break;
				}
			}
		}
				
		private function stageResizeHandler ( e:Event=null ):void
		{
			stageWidth	= stage.stageWidth;
			stageHeight	= stage.stageHeight;
			
			var r:Number;
			var rect:Rectangle;
			
			videoToStageRatio	= MathUtil.cropToFitRatio( stageWidth, stageHeight, videoWidth, videoHeight );
			r					= videoToStageRatio;

			rect				= new Rectangle( 0, 0, (int)( videoWidth * r ), (int)( videoHeight * r ) );

			video.width		= rect.width;
			video.height	= rect.height;
			
			container.x		= (int)( ( stageWidth - rect.width ) * 0.5 );
			container.y		= (int)( ( stageHeight - rect.height ) * 0.5 );
			
			drawOverlay( stageWidth - container.x * 2, stageHeight - container.y * 2 );
		}
		
		private function keyPressHandler ( e:KeyboardEvent ):void
		{
			//
		}
		
		/////////////////////////////////////////////
		//	UTILS.
		/////////////////////////////////////////////
		
		private function getRandomRange( min:Number, max:Number ):Number
		{
			return Math.floor( Math.random() * ( max - min + 1 ) ) + min;
		}
		
	}
}