package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.SoundModel;
	import com.reintroducing.sound.SoundManager;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;
	import flash.geom.Point;

	public class MenuToolDryerView extends MenuToolView
	{
		private var bPlayingSound	: Boolean	= false;
		public  var soundVolume		: Number	= 1;

		public function MenuToolDryerView(sprite:Sprite=null)
		{
			super(sprite);

			toolIndex = HairModel.TOOL_DRYER;

			_toolPathCenter			= new Point( 288, 320 );
			_toolPathRadiusX		= 255;
			_toolPathRadiusY		= 340;
			_toolDropAreaRotation	= 29.3;
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

			updateModelWithPosition();
		}

		////////////////////////////////////
		//	TOOL SOUND.
		////////////////////////////////////

		public override function playToolSound ():void
		{
			if( !bPlayingSound )
			{
				bPlayingSound = true;

				SoundManager.getInstance().playSound( SoundModel.DRYER_LONG, 1, 0, 100 );
			}
		}

		public override function stopToolSound ():void
		{
			if( bPlayingSound )
			{
				fadeOutToolSound();
			}
		}

		private function fadeOutToolSound ():void
		{
			Tweener.addTween
			(
				this,
				{
					soundVolume	: 0,
					time		: 0.2,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: tweenCompleteHandler
				}
			);
		}

		private function tweenUpdateHandler ():void
		{
			SoundManager.getInstance().setSoundVolume( SoundModel.DRYER_LONG, soundVolume );
		}

		private function tweenCompleteHandler ():void
		{
			SoundManager.getInstance().stopSound( SoundModel.DRYER_LONG );

			bPlayingSound = false;
		}
	}
}