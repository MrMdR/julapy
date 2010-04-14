package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.controls.BtnView;
	import com.julapy.ph.hair.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Point;

	public class MenuToolView extends BtnView
	{
		private var _ix	: Number;
		private var _iy : Number;
		private var _sx : Number;
		private var _sy : Number;
		private var _ir	: Number;

		protected var _toolPathCenter		: Point	 = new Point( 288, 288 );
		protected var _toolPathRadiusX		: Number = 250;
		protected var _toolPathRadiusY		: Number = 250;
		protected var _toolPathEase			: Number = 0.2;
		protected var _toolRotationEase		: Number = 0.2;
		protected var _toolDropAreaRotation	: Number = 0;

		private var anim : MovieClip;

		public function MenuToolView(sprite:Sprite=null)
		{
			super(sprite);

			_ix = ( _sprite as MovieClip ).x;
			_iy = ( _sprite as MovieClip ).y;
			_sx = ( _sprite as MovieClip ).scaleX;
			_sy = ( _sprite as MovieClip ).scaleY;
			_ir = ( _sprite as MovieClip ).rotation;

			initToolAnim();
		}

		public function get ix ():Number { return _ix };
		public function get iy ():Number { return _iy };
		public function get sx ():Number { return _sx };
		public function get sy ():Number { return _sy };
		public function get ir ():Number { return _ir };

		public function get toolPathCenter  		():Point  { return _toolPathCenter.clone() };
		public function get toolPathRadiusX 		():Number { return _toolPathRadiusX };
		public function get toolPathRadiusY 		():Number { return _toolPathRadiusY };
		public function get toolPathEase    		():Number { return _toolPathEase };
		public function get toolRotationEase   		():Number { return _toolRotationEase };
		public function get toolDropAreaRotation    ():Number { return _toolDropAreaRotation };

		public function set angle ( value : Number ):void
		{
			//
		}

		protected function updateModelWithPosition ():void
		{
			ModelLocator.getInstance().hairModel.updateToolPathPosition( new Point( x, y ) );
		}

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
					rotation	: ir,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		public function rotateToDropArea ():void
		{
			Tweener.addTween
			(
				this,
				{
					rotation	: toolDropAreaRotation,
					time		: 0.2,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		public function rotateBackToNormal ():void
		{
			Tweener.addTween
			(
				this,
				{
					rotation	: ir,
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

		////////////////////////////////////
		//	TOOL ANIM.
		////////////////////////////////////

		private function initToolAnim ():void
		{
			anim = _sprite.getChildByName( "anim" ) as MovieClip;
			anim.gotoAndStop( 1 );
			anim.visible = false;
		}

		public function stopToolAnim ():void
		{
			anim.gotoAndStop( 1 );
			anim.visible = false;
		}

		public function playToolAnim ():void
		{
			anim.play();
			anim.visible = true;
		}
	}
}