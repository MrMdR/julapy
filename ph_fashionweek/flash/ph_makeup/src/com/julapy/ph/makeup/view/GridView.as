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
		public  var tweenValue	: Number = 0;

		public function GridView( sprite:Sprite=null )
		{
			super(sprite);

			_sprite.blendMode = BlendMode.OVERLAY;

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE, modeEvent );
		}

		private function playIn ():void
		{
			Tweener.addTween
			(
				this,
				{
					tweenValue	: 1.0,
					time		: 0.4,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdate,
					onComplete	: null
				}
			);
		}

		private function playOut ():void
		{

		}

		private function tweenUpdate ():void
		{
			var aw : int;
			var ah : int;

			aw = ModelLocator.getInstance().makeupModel.appWidth;
			ah = ModelLocator.getInstance().makeupModel.appHeight;

			var gw : int;		// grid width.
			var gh : int;		// grid height.

			gw = 20;
			gh = 20;

			var ix : int;
			var iy : int;

			ix = (int)( aw / gw );
			iy = (int)( ah / gh );

			_sprite.graphics.clear();
			_sprite.graphics.lineStyle( 1, 0xFFFFFF, 0.1 );

			var i : int;
			var w : Number;
			var h : Number;

			for( i=1; i<ix; i++ )
			{
				_sprite.graphics.moveTo( i * gw, 0 );
				_sprite.graphics.lineTo( i * gw, ah );
			}

			for( i=1; i<iy; i++ )
			{
				_sprite.graphics.moveTo(  0, i * gh );
				_sprite.graphics.lineTo( aw, i * gh );
			}
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function modeEvent ( e : ModeEvent ):void
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
				playOut();
			}
		}
	}
}