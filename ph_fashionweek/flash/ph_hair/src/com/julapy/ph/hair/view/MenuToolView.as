package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.controls.BtnView;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class MenuToolView extends BtnView
	{
		private var _ix	: Number;
		private var _iy : Number;
		private var _sx : Number;
		private var _sy : Number;

		public function MenuToolView(sprite:Sprite=null)
		{
			super(sprite);

			_ix = ( _sprite as MovieClip ).x;
			_iy = ( _sprite as MovieClip ).y;
			_sx = ( _sprite as MovieClip ).scaleX;
			_sy = ( _sprite as MovieClip ).scaleY;
		}

		public function get ix ():Number { return _ix };
		public function get iy ():Number { return _iy };
		public function get sx ():Number { return _sx };
		public function get sy ():Number { return _sy };

		public function returnToMenu ():void
		{
			Tweener.addTween
			(
				this,
				{
					x			: ix,
					y			: iy,
					scaleX		: sx,
					scaleY		: sy,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		public function scaleUp ( b : Boolean ):void
		{
			if( b )
			{
				var scl : Number;
				scl = sx * 1.3

				Tweener.addTween
				(
					this,
					{
						scaleX		: scl,
						scaleY		: scl,
						time		: 0.2,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: tweenUpdateHandler,
						onComplete	: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					this,
					{
						scaleX		: sx,
						scaleY		: sy,
						time		: 0.2,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: tweenUpdateHandler,
						onComplete	: null
					}
				);
			}
		}

		private function tweenUpdateHandler ():void
		{
			doValidate();
		}

	}
}