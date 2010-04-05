package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.BlendMode;
	import flash.display.Sprite;

	public class GridView extends View
	{
		public  var tweenInValue0	: Number = 0;
		public  var tweenInValue1	: Number = 0;
		public  var tweenOutValue	: Number = 0;

		private var tweenAlphaStart	: Number = 0.1;
		private var tweenAlphaEnd	: Number = 0.05;
		private var tweenAlpha		: Number = 0;

		public function GridView( sprite:Sprite=null )
		{
			super(sprite);

			_sprite.blendMode	= BlendMode.OVERLAY;

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,	modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,	modeZoomOutHandler );
		}

		private function playIn ():void
		{
			_sprite.alpha = tweenAlphaStart;
			_sprite.graphics.clear();

			playInCompleteHandler();
			playOutCompleteHandler();

			Tweener.removeTweens( this );

			Tweener.addTween
			(
				this,
				{
					tweenInValue0	: 1.0,
					time			: 2.0,
					delay			: 0.0,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: playInUpdateHandler,
					onComplete		: null
				}
			);

			Tweener.addTween
			(
				this,
				{
					tweenInValue1	: 1.0,
					time			: 2.0,
					delay			: 0.5,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: playInUpdateHandler,
					onComplete		: playInCompleteHandler
				}
			);
		}

		private function playInUpdateHandler ():void
		{
			_sprite.alpha = ( tweenAlphaStart - tweenAlphaEnd ) * ( 1 - tweenInValue1 ) + tweenAlphaEnd
			_sprite.graphics.clear();
			_sprite.graphics.lineStyle( 1, 0xFFFFFF );

			drawGridV();
			drawGridH();
		}

		private function playInCompleteHandler ():void
		{
			tweenInValue0 = 0;
			tweenInValue1 = 0;
		}

		private function playOut ():void
		{
			playInCompleteHandler();
			playOutCompleteHandler();

			Tweener.removeTweens( this );

			Tweener.addTween
			(
				this,
				{
					tweenOutValue	: 1.0,
					time			: 0.4,
					delay			: 0.0,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: playOutUpdateHandler,
					onComplete		: playOutCompleteHandler
				}
			);
		}

		private function playOutUpdateHandler ():void
		{
			_sprite.alpha = ( 1 - tweenOutValue ) * tweenAlphaEnd;
		}

		private function playOutCompleteHandler ():void
		{
			tweenOutValue = 0;
		}

		//////////////////////////////////////////////////////////
		//	DRAW.
		//////////////////////////////////////////////////////////

		private function drawGridV ():void
		{
			var aw : int;
			var ah : int;

			aw = ModelLocator.getInstance().commondModel.appWidth;
			ah = ModelLocator.getInstance().commondModel.appHeight;

			var gw : int;		// grid width.
			var gh : int;		// grid height.

			gw = 20;
			gh = 20;

			var ix : int;
			var iy : int;

			ix = (int)( aw / gw );
			iy = (int)( ah / gh );

			var i : int;
			var w : Number;
			var h : Number;
			var d : Number;		// delay.

			for( i=1; i<ix; i++ )
			{
				d = 1 - ( ix - 1 - i ) / ( ix - 1 );

				h = tweenInValue0 * ah * 2 - d * ah;
				h = Math.min( h, ah );

				_sprite.graphics.moveTo( i * gw, 0 );
				_sprite.graphics.lineTo( i * gw, h );
			}
		}

		private function drawGridH ():void
		{
			var aw : int;
			var ah : int;

			aw = ModelLocator.getInstance().commondModel.appWidth;
			ah = ModelLocator.getInstance().commondModel.appHeight;

			var gw : int;		// grid width.
			var gh : int;		// grid height.

			gw = 20;
			gh = 20;

			var ix : int;
			var iy : int;

			ix = (int)( aw / gw );
			iy = (int)( ah / gh );

			var i : int;
			var w : Number;
			var h : Number;
			var d : Number;		// delay.

			for( i=1; i<iy; i++ )
			{
				d = 1 - ( iy - 1 - i ) / ( iy - 1 );

				w = tweenInValue1 * aw * 2 - d * aw;
				w = Math.min( w, aw );

				_sprite.graphics.moveTo( 0, i * gh );
				_sprite.graphics.lineTo( w, i * gh );
			}
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
			if( e.mode == MakeupModel.EYES_MODE )
			{
				playIn();
			}

			if( e.mode == MakeupModel.LIPS_MODE )
			{
				playIn();
			}

			if( e.mode == MakeupModel.FACE_MODE )
			{
				playIn();
			}
		}

		private function modeZoomOutHandler ( e : ModeEvent ):void
		{
			playOut();
		}
	}
}