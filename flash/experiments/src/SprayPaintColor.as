package
{
	import flash.geom.ColorTransform;
	
	public class SprayPaintColor
	{
		private static var colors:Array = new Array();
		
		public static function addColor ( c:uint ):void
		{
			colors.push( c );
		}
		
		public static function getRandomColor ():uint
		{
			var i:uint = (uint)( colors.length * Math.random() );
			return colors[ i ];
		}
		
		public static function getColorTransform ( c:uint ):ColorTransform
		{
			var r:uint = c >> 16;
			var g:uint = c >> 8 & 0xFF;
			var b:uint = c & 0xFF;
			
			return new ColorTransform( r/255, g/255, b/255 );
		}

	}
}