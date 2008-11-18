package
{
	import flash.display.StageAlign;
	import flash.display.StageQuality;
	import flash.display.StageScaleMode;
	import flash.display.Sprite;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.BitmapDataChannel;
	import flash.events.Event;
	import flash.geom.Point;
	import com.holler.display.PerlineNoiseArea;
	import flash.display.BlendMode;
	
	[ SWF( backgroundColor='0x000000', frameRate='30', width='800', height='600' ) ]

	public class PerlineNoiseTest extends Sprite
	{
		public var pna:PerlineNoiseArea;
		public var pnaWidth:int = 100;
		public var pnas:Array;
		
		public function PerlineNoiseTest()
		{
			stage.align		= StageAlign.TOP_LEFT;
			stage.quality	= StageQuality.BEST;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			pnas = new Array();
			
			for( var i:int=0; i<stage.stageWidth; i+=(int)( pnaWidth * Math.random() ) )
			{
/* 				if( ( i / pnaWidth ) % 2 == 0 )
				{
					pna			= new PerlineNoiseArea( pnaWidth, stage.stageHeight );
					pna.xSpeed	= 2;
				}
				else
				{
					pna			= new PerlineNoiseArea( pnaWidth, stage.stageHeight );
					pna.xSpeed	= -2;
				}
 */				
				pna		= new PerlineNoiseArea( (int)( pnaWidth * Math.random() ) + 1, (int)( stage.stageHeight * Math.random() ) );
				pna.x 	= i;
				pna.y	= (int)( ( stage.stageHeight - pna.height ) * 0.5 * Math.random() );
				pna.blendMode = BlendMode.HARDLIGHT;	// nice dark effect.
//				pna.blendMode = BlendMode.LIGHTEN;		// produces some interesting results with the strips blending in.
				addChild( pna );
				
				pnas.push( pna );
			}
			
			addEventListener( Event.ENTER_FRAME, onEnterFrame );
		}
		
		
		public function onEnterFrame ( e:Event ):void
		{
			for( var i:int=0; i<pnas.length; i++ )
			{
				pnas[ i ].draw();
			}
		}
	}
}