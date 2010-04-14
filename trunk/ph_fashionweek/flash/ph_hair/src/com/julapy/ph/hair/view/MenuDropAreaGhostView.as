package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.hair.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;
	import flash.geom.Point;

	public class MenuDropAreaGhostView extends View
	{
		private var bShow	: Boolean = false;
		private var scl		: Point = new Point();

		public function MenuDropAreaGhostView(sprite:Sprite=null)
		{
			super(sprite);

			scl.x = _sprite.scaleX;
			scl.y = _sprite.scaleY;
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				alpha	= 0;
				scaleX	= scl.x * 0.3;
				scaleY	= scl.y * 0.3;
				doValidate();

				Tweener.addTween
				(
					this,
					{
						alpha			: 1.0,
						scaleX			: scl.x,
						scaleY			: scl.y,
						time			: 0.3,
						delay			: 0.2,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: showCompleteHandler
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					this,
					{
						alpha			: 0.0,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: hideCompleteHandler
					}
				);
			}
		}

		private function showCompleteHandler ():void
		{
			ModelLocator.getInstance().hairModel.dropAreaPlayedIn();
		}

		private function hideCompleteHandler ():void
		{
			ModelLocator.getInstance().hairModel.dropAreaPlayedOut();
		}

	}
}