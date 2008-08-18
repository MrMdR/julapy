package
{
	import com.holler.video.VideoPlayer;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Rectangle;

	[ SWF( backgroundColor='0x000000', frameRate='25', width='1024', height='768' ) ]

	public class Pixelate extends Sprite
	{
		public var videoWidth:int	= 320;
		public var videoHeight:int	= 240;
		public var pixelRes:int		= 10;
		public var videoContainer:Sprite;
		public var imageContainer:Sprite;
		public var imageBitmapData:BitmapData;
		public var vp:VideoPlayer;
		
		public function Pixelate()
		{
			videoContainer		= new Sprite()
			imageContainer		= new Sprite()
			imageContainer.x	= videoWidth;

			imageContainer.addChildAt( new Bitmap( ), 0 );

			addChild( videoContainer );
			addChild( imageContainer );
			
			vp = new VideoPlayer( videoContainer, videoWidth, videoHeight );
			vp.load( "../assets/moon.flv" );
			vp.play();
			vp.loop = true;
						
			addEventListener( Event.ENTER_FRAME, onEnterFrame );
		}
		
		public function pixelate ( bmd:BitmapData, pixelSize:uint=1 ):BitmapData
		{
			var srcBmd:BitmapData = bmd.clone();
			var outBmd:BitmapData = new BitmapData( srcBmd.width, srcBmd.height );
			
			for( var y:uint=0; y < bmd.height; y++ )
			{  
				for( var x:uint=0; x < bmd.width; x++ )
				{
					if( y % pixelSize == 0 && x % pixelSize == 0 )
					{
 						if( x + pixelSize <= bmd.width && y + pixelSize <= bmd.height )
							outBmd.fillRect( new Rectangle( x, y, pixelSize, pixelSize ), bmd.getPixel32( x + (int)(pixelSize/2) , y + (int)(pixelSize/2) ) );

						if( x + pixelSize > bmd.width )
							outBmd.fillRect( new Rectangle( x, y, bmd.width-x, pixelSize ), bmd.getPixel32( x + (int)((bmd.width-x)/2), y + (int)(pixelSize/2) ) );
						
						if( y + pixelSize > bmd.height )
							outBmd.fillRect( new Rectangle( x, y, pixelSize, bmd.height-y ), bmd.getPixel32( x + (int)(pixelSize/2), y + (int)((bmd.height-y)/2) ) );
					}
				}
			}
			
			return outBmd;
		}
		
		private function onEnterFrame ( e:Event ):void
		{
			var videoBitmapData:BitmapData;
			videoBitmapData = new BitmapData( videoWidth, videoHeight );
			videoBitmapData.draw( videoContainer );
			
			imageContainer.removeChildAt( 0 );
			imageContainer.addChildAt( new Bitmap( pixelate( videoBitmapData, pixelRes ) ), 0 );
		}
	}
}