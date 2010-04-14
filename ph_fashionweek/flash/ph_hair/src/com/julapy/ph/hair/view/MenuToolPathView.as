package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
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

		private var noDots	: int = 80;
		private var dots	: Array = new Array();

		private var circle	: MovieClip;

		public function MenuToolPathView(sprite:Sprite=null)
		{
			super(sprite);

			circle			= _sprite.getChildByName( "circle" ) as MovieClip;
			circle.visible	= false;
		}

		public function setPath ( cx : Number, cy : Number, rx : Number, ry : Number ):void
		{
			center.x = cx;
			center.y = cy;

			size.x = rx * 0.5;
			size.y = ry * 0.5;

			circle.x = center.x;
			circle.y = center.y;

			removeDots();
			addDots();
		}

		private function addDots ():void
		{
			for( var i:int=0; i<noDots; i++ )
			{
				var dot : MovieClip;
				dot = AssetLoader.getInstance().getClassInstance( "hair.menu.tool.path.dot" ) as MovieClip;

				circle.addChild( dot );

				dot.x =  Math.sin( ( i / noDots ) * 2 * Math.PI ) * size.x;
				dot.y = -Math.cos( ( i / noDots ) * 2 * Math.PI ) * size.y;

				dots.push( dot );
			}
		}

		private function removeDots ():void
		{
			for( var i:int=0; i<dots.length; i++ )
			{
				var dot : MovieClip;
				dot = dots[ i ] as MovieClip;

				circle.removeChild( dot );
			}

			dots = new Array();
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
				circle.scaleX	= 0.8;
				circle.scaleY	= 0.8;

				Tweener.addTween
				(
					circle,
					{
						alpha		: 0.4,
						scaleX		: 1.0,
						scaleY		: 1.0,
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
						scaleX		: 0.9,
						scaleY		: 0.9,
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