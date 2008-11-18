package
{
	import com.holler.video.VideoPlayer;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.BlendMode;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.filters.BlurFilter;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	[ SWF( backgroundColor='0x000000', frameRate='25', width='1400', height='768' ) ]

	public class Pixelate extends Sprite
	{
		public var videoWidth:int	= 320;
		public var videoHeight:int	= 240;
		public var pixelRes:int		= 4;
		public var videoContainer:Sprite;
		public var imageContainer:Sprite;
		public var imageBitmapData:BitmapData;
		public var imageScale:Number = 2;
		public var vp:VideoPlayer;
		
		public function Pixelate()
		{
			videoContainer		= new Sprite()
			imageContainer		= new Sprite()
			imageContainer.x	= videoWidth;

			imageBitmapData		= new BitmapData( videoWidth * imageScale, videoHeight * imageScale, true, 0x000000 );
			imageContainer.addChildAt( new Bitmap( imageBitmapData ), 0 );

			addChild( videoContainer );
			addChild( imageContainer );
			
			vp = new VideoPlayer( videoContainer, videoWidth, videoHeight );
			vp.load( "../assets/video.flv" );
			vp.play();
			vp.loop		= true;
			vp.volume	= 0;
						
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
		
		public function strand (  bmd:BitmapData, pixelSize:uint=1 ):BitmapData
		{
			var srcBmd:BitmapData = bmd.clone();
			var outBmd:BitmapData = new BitmapData( srcBmd.width * imageScale, srcBmd.height * imageScale, true, 0x000000 );

			var sprite:Sprite	= new Sprite();
			sprite.blendMode	= BlendMode.ADD;
			
			for( var y:uint=0; y < bmd.height; y++ )
			{  
				for( var x:uint=0; x < bmd.width; x++ )
				{
					if( y % pixelSize == 0 && x % pixelSize == 0 )
					{
						var c:Number	= srcBmd.getPixel( x, y );
						var r:Number	= c / 0xFFFFFF;
//						var lx:Number	= Math.cos( r ) * 5 * imageScale;
//						var ly:Number	= Math.sin( r ) * 5 * imageScale;
						
//						sprite.graphics.lineStyle( pixelSize * imageScale, c, 0.4, true, LineScaleMode.HORIZONTAL, CapsStyle.NONE );
// 						sprite.graphics.moveTo( x * imageScale, y * imageScale );
// 						sprite.graphics.lineTo( ( x + lx ) * imageScale, ( y + ly ) * imageScale );

//						sprite.graphics.beginFill( c, 0.4 );
//						sprite.graphics.drawRect( x * imageScale, y * imageScale, pixelSize * imageScale, pixelSize * imageScale );
//						sprite.graphics.endFill();

						sprite.graphics.beginFill( c, 1 );
						sprite.graphics.drawEllipse( x * imageScale, y * imageScale, pixelSize * imageScale * r * 5, pixelSize * imageScale * r * 2 );
						sprite.graphics.endFill();
					}
				}
			}
			
			outBmd.draw( sprite );
			outBmd.applyFilter( outBmd, new Rectangle( 0, 0, outBmd.width, outBmd.height ), new Point( 0, 0 ), new BlurFilter() );
			
			return outBmd;
		}
		
		private function onEnterFrame ( e:Event ):void
		{
			var videoBitmapData:BitmapData;
			videoBitmapData = new BitmapData( videoWidth, videoHeight );
			videoBitmapData.draw( videoContainer );
			
			imageBitmapData.merge( strand( videoBitmapData, pixelRes ),
								   new Rectangle( 0, 0, videoWidth * imageScale, videoHeight * imageScale ), 
								   new Point( 0, 0 ), 
								   255, 255, 255, 80 );
		}
	}
}