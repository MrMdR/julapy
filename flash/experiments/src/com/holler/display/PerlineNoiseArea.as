package com.holler.display
{
	import flash.display.BitmapData;
	import flash.display.Bitmap;
	import flash.display.BitmapDataChannel;
	import flash.geom.Point;

	public class PerlineNoiseArea extends Bitmap
	{
		public var w:Number;
		public var h:Number;
		public var seed:Number;
		public var offset:Array;
		public var xSpeed:Number = 1;
		public var ySpeed:Number = 2;
		
		public function PerlineNoiseArea( w:Number, h:Number )
		{
			super( null, "auto", false );
			
			this.w = w;
			this.h = h;
			
			seed	= Math.floor( Math.random() * 10 );
			offset	= [ new Point(0, 0), new Point(0, 0) ];
		}
		
		public function draw ():void
		{
			offset[0].x += xSpeed;
			offset[1].y += ySpeed;

			bitmapData = new BitmapData( w, h, true, 0x000000 );
			bitmapData.perlinNoise( 100, 100, 2, seed, false, true, 7.0, true, offset );
		}
	}
}