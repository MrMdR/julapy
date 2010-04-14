package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Point;

	public class MenuToolPathView extends View
	{
		private var bShow	: Boolean = false;
		private var center	: Point = new Point();
		private var size	: Point = new Point();

		private var circle	: MovieClip;

		public function MenuToolPathView(sprite:Sprite=null)
		{
			super(sprite);

			circle			= _sprite.getChildByName( "circle" ) as MovieClip;
			circle.iw		= circle.width;
			circle.ih		= circle.height;
			circle.visible	= false;
			circle.alpha	= 0.2;
		}

		public function setPath ( cx : Number, cy : Number, rx : Number, ry : Number ):void
		{
			center.x = cx;
			center.y = cy;

			size.x = rx;
			size.y = ry;
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				circle.visible	= true;
				circle.alpha	= 0;

				circle.x		= center.x;
				circle.y		= center.y;
				circle.width	= size.x * 0.8;
				circle.height	= size.y * 0.8;

				Tweener.addTween
				(
					circle,
					{
						alpha		: 0.2,
						width		: size.x,
						height		: size.y,
						time		: 0.2,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					circle,
					{
						alpha		: 0.0,
						width		: size.x  * 0.9,
						height		: size.y * 0.9,
						time		: 0.2,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: hideCompleteHandler
					}
				);
			}
		}

		private function hideCompleteHandler ():void
		{
			circle.visible = false;
		}
	}
}