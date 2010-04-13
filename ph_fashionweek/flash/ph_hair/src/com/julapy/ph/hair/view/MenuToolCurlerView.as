package com.julapy.ph.hair.view
{
	import flash.display.Sprite;
	import flash.geom.Point;

	public class MenuToolCurlerView extends MenuToolView
	{
		public function MenuToolCurlerView(sprite:Sprite=null)
		{
			super(sprite);

			_toolPathCenter			= new Point( 288, 320 );
			_toolPathRadiusX		= 255;
			_toolPathRadiusY		= 340;
			_toolDropAreaRotation	= 60.2;
		}

		public override function set angle ( a : Number ):void
		{
			var p : Point;
			p = new Point();
			p.x = -Math.sin( a ) * toolPathRadiusX + toolPathCenter.x;
			p.y = -Math.cos( a ) * toolPathRadiusY + toolPathCenter.y;

			x += ( p.x - x ) * toolPathEase;
			y += ( p.y - y ) * toolPathEase;

			var r : Number;
			r = -a * ( 180.0 / Math.PI );
			if( r < 0 || r > 180 )
			{
				if( scaleY > 0 )
				{
					scaleY *= -1;
				}
			}
			else
			{
				if( scaleY < 0 )
				{
					scaleY *= -1;
				}
			}

			r -= 90;

			rotation += ( r - rotation ) * toolRotationEase;

			doValidate();
		}
	}
}