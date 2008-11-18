package
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.geom.ColorTransform;
	import flash.geom.Matrix;
	import flash.geom.Point;
	
	public class SprayPaintDrip
	{
		private var sprayBitmap:Bitmap;
		private var sprayCanvas:BitmapData;
		private var dripPoint:Point;
		
		private var scale:Number			= 1;		// value ranging from 0 - 1
		private var scaleRatio:Number		= 0;		// this is the scale of original bitmap.
		
		private var dripHeight:Number	= 0;
		private var dripWidth:Number	= 0;
		private var dripRadius:Number	= 0;
		private var tx:Number 			= 0;
		private var ty:Number 			= 0;
		private var dy:Number			= 1.25;
		
		public function SprayPaintDrip( sprayBitmap:Bitmap, sprayCanvas:BitmapData, dripPoint:Point )
		{
			this.sprayBitmap	= sprayBitmap;
			this.sprayCanvas	= sprayCanvas;
			this.dripPoint		= dripPoint;
		}

		public function initialise ():void
		{
			dripHeight	= Math.random() * 100 + 50;
			dripRadius	= Math.random() * 4 + 1;
			
			scaleRatio	= dripRadius / ( sprayBitmap.width * 0.5 );
			
			tx 			= ( Math.random() * 2 - 1 ) * dripRadius;
		}
		
		public function destroy ():void
		{
			
		}
		
		public function update ():void
		{
			var s:Number;
			var m:Matrix;
			var c:ColorTransform;

			scale = Math.cos( ty / dripHeight * Math.PI * 0.5 );
			scale = Math.max( scale, 0.5 );

			s = scale * scaleRatio;

			m = new Matrix( );
			m.scale( s, s );
			m.translate( -sprayBitmap.width * s * 0.5, -sprayBitmap.height * s * 0.5 );
//			m.rotate( Math.random() * Math.PI * 2 );
			m.translate( dripPoint.x, dripPoint.y );
			m.translate( tx, ty );
			
			tx += Math.random( ) * 2 - 1;
			ty += dy;

			c = SprayPaintColor.getColorTransform( SprayPaintColor.getRandomColor() );
			c.alphaMultiplier = 0.5;

			sprayCanvas.draw( sprayBitmap, m, c );
		}
		
		public function isValid ():Boolean
		{
			if( ty < dripHeight )
				return true;
			else
				return false;
		}
	}
}