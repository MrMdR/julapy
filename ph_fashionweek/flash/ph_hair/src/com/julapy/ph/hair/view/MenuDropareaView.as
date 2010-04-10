package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;

	import fl.motion.easing.Quadratic;

	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.ColorTransform;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	public class MenuDropareaView extends View
	{
		private var ghost			: MovieClip;
		private var base			: MovieClip;

		private var circle			: MovieClip;
		private var progress		: MovieClip;
		private var progressMask	: MovieClip;
		private var cross			: MovieClip;

		private var crossRect0		: MovieClip;
		private var crossRect1		: MovieClip;
		private var crossRect0Mask	: MovieClip;
		private var crossRect1Mask	: MovieClip;

		private var _crossWidth		: Number = 0;
		private var _progressVal	: Number = 0;

		private var bCrossPlayingIn		: Boolean = false;
		private var bCrossPlayingOut 	: Boolean = false;

		private var baseBmp			: Bitmap	= new Bitmap( new BitmapData( 1, 1, true, 0x00FFFFFF ) );
		private var baseBmpRect		: Rectangle = new Rectangle( -250, -250, 500, 500 );

		private var bIsOver			: Boolean	= false;

		public function MenuDropareaView(sprite:Sprite=null)
		{
			super(sprite);

			ghost			= _sprite.getChildByName( "ghost" ) as MovieClip;
			base			= _sprite.getChildByName( "base" ) as MovieClip;

			baseBmp.x		= (int)( -baseBmpRect.width  * 0.5 );
			baseBmp.y		= (int)( -baseBmpRect.height * 0.5 );
			baseBmp.alpha	= 0.2;
			_sprite.addChild( baseBmp );

			circle			= base.getChildByName( "circle" ) as MovieClip;
			progress		= base.getChildByName( "progress" ) as MovieClip;
			progressMask	= base.getChildByName( "progressMask" ) as MovieClip;
			cross			= base.getChildByName( "cross" ) as MovieClip;

			circle.visible	= false;
			cross.visible	= false;

			crossRect0		= cross.getChildByName( "rect0" ) as MovieClip;
			crossRect1		= cross.getChildByName( "rect1" ) as MovieClip;
			crossRect0Mask	= cross.getChildByName( "rect0Mask" ) as MovieClip;
			crossRect1Mask	= cross.getChildByName( "rect1Mask" ) as MovieClip;

			crossRect0Mask.ih		= crossRect0Mask.mc.height;
			crossRect1Mask.ih		= crossRect1Mask.mc.height;

			crossWidth = 0;
			progressVal = 0;

			show( false );
		}

		///////////////////////////////////////////////////
		//	SHOW / HIDE
		///////////////////////////////////////////////////

		public function show ( b : Boolean ):void
		{
			if( b )
			{
				_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
			else
			{
				_sprite.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}

			visible = b;
		}

		///////////////////////////////////////////////////
		//	OVER
		///////////////////////////////////////////////////

		public function over ( b : Boolean ):void
		{
			if( bIsOver == b )
				return;

			bIsOver = b;

			var time	: Number;
			var timeIn	: Number
			var timeOut	: Number;

			time	= 0;
			timeIn	= 2.0;
			timeOut	= 2.0;

			if( b )
			{
				time = ( 1 - progressVal ) * timeIn;

				Tweener.addTween
				(
					this,
					{
						progressVal	: 1.0,
						time		: time,
						delay		: 0.0,
						transition	: Quadratic.easeIn,
						onStart		: null,
						onUpdate	: null,
						onComplete	: overCompleteHandler
					}
				);
			}
			else
			{
				time = progressVal * timeOut;

				Tweener.addTween
				(
					this,
					{
						progressVal	: 0.0,
						time		: time,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);
			}
		}

		private function overCompleteHandler ():void
		{
			dispatchEvent( new Event( Event.COMPLETE ) );
		}

		///////////////////////////////////////////////////
		//	CROSS.
		///////////////////////////////////////////////////

		public function showCross ( show : Boolean ):void
		{
			if( show )
			{
				if( bCrossPlayingIn )
					return;

				bCrossPlayingIn		= true;
				bCrossPlayingOut	= false;

				Tweener.addTween
				(
					this,
					{
						crossWidth	: 1.0,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: crossShowCompleteHandler
					}
				);
			}
			else
			{
				if( bCrossPlayingOut )
					return;

				bCrossPlayingOut	= true;
				bCrossPlayingIn		= false;

				Tweener.addTween
				(
					this,
					{
						crossWidth	: 0.0,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: crossHideCompleteHandler
					}
				);
			}
		}

		public function set crossWidth ( value : Number ):void
		{
			_crossWidth = value;

//			crossRect0Mask.mc.height = _crossWidth * crossRect0Mask.ih;
			crossRect0Mask.mc.height = 0;
			crossRect1Mask.mc.height = _crossWidth * crossRect1Mask.ih;
		}

		public function get crossWidth ():Number
		{
			return _crossWidth;
		}

		private function crossShowCompleteHandler ():void
		{
			bCrossPlayingIn = false;
		}

		private function crossHideCompleteHandler ():void
		{
			bCrossPlayingOut = false;
		}

		///////////////////////////////////////////////////
		// HANDLERS.
		///////////////////////////////////////////////////

		public function set progressVal ( value : Number ):void
		{
			_progressVal = value;

			drawWedge();
		}

		public function get progressVal ():Number
		{
			return _progressVal;
		}

		private function drawWedge ():void
		{
			progressMask.graphics.clear();
			progressMask.graphics.beginFill( 0xFF0000 );
			progressMask.graphics.moveTo( 0, 0 );

			var steps : int;
			steps = 720;

			var ang : int;
			ang = (int)( steps * _progressVal );

			for( var i:int=0; i<ang+1; i++ )
			{
				var p : Point;
				p	= new Point();
				p.x = Math.sin( ( i / steps ) * 2 * Math.PI ) * 250;
				p.y = -Math.cos( ( i / steps ) * 2 * Math.PI ) * 250;

				progressMask.graphics.lineTo( p.x, p.y );
			}

			progressMask.graphics.moveTo( 0, 0 );
			progressMask.graphics.endFill();
   		}

		///////////////////////////////////////////////////
		// HANDLERS.
		///////////////////////////////////////////////////

		private function enterFrameHandler ( e : Event ):void
		{
			var m : Matrix;
			m = new Matrix();
			m.tx = (int)( baseBmpRect.width  * 0.5 );
			m.ty = (int)( baseBmpRect.height * 0.5 );

			var ct : ColorTransform;
			ct = new ColorTransform();
//			ct.color			= 0xFFFFFF;

			baseBmp.bitmapData.dispose();
			baseBmp.bitmapData = new BitmapData( baseBmpRect.width, baseBmpRect.height, true, 0x00FFFFFF );
			baseBmp.bitmapData.draw( circle, m, ct );
			baseBmp.bitmapData.draw( cross, m, ct );

			ghost.rotation -= 0.2;
		}
	}
}