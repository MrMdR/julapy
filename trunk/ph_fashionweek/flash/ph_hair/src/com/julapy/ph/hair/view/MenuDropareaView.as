package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.hair.events.DropAreaEvent;

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
		private var _progressPop	: Number = 0;

		private var bCrossPlayingIn		: Boolean = false;
		private var bCrossPlayingOut 	: Boolean = false;

		private var baseBmp			: Bitmap	= new Bitmap( new BitmapData( 1, 1, true, 0x00FFFFFF ) );
		private var baseBmpRect		: Rectangle = new Rectangle( -250, -250, 500, 500 );

		private var bShow			: Boolean	= true;
		private var bIsOver			: Boolean	= false;
		private var bPlayingIn		: Boolean	= false;
		private var bPlayingOut		: Boolean	= false;

		private var circleRadiusFull	: Number = 170;
		private var circleWidthFull		: Number = 25;
		private var _circleRadius		: Number = 0;
		private var _circleWidth		: Number = 0;

		public function MenuDropareaView(sprite:Sprite=null)
		{
			super(sprite);

			ghost			= _sprite.getChildByName( "ghost" ) as MovieClip;
			base			= _sprite.getChildByName( "base" ) as MovieClip;

			ghost.ir		= ghost.rotation;
			ghost.sx		= ghost.scaleX;
			ghost.sy		= ghost.scaleY;

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

//			crossRect0Mask.ih		= crossRect0Mask.mc.height;
//			crossRect1Mask.ih		= crossRect1Mask.mc.height;

			crossWidth	= 0;
			progressVal = 0;

			show( false );
		}

		///////////////////////////////////////////////////
		//	SHOW / HIDE
		///////////////////////////////////////////////////

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
			else
			{
				_sprite.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}

			visible = bShow;
		}

		///////////////////////////////////////////////////
		//	PLAY IN / OUT
		///////////////////////////////////////////////////

		public function playIn ( b : Boolean ):void
		{
			if( b )
			{
				if( bPlayingIn )
					return;

				bPlayingIn = true;

				circleRadius	= 0;
				circleWidth		= 0;

				drawCircle();
				drawCross();

				Tweener.addTween
				(
					this,
					{
						circleRadius	: circleRadiusFull,
						circleWidth		: circleWidthFull,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: null
					}
				);

				ghost.alpha		= 0;
				ghost.scaleX	= ghost.sx * 0.3;
				ghost.scaleY	= ghost.sy * 0.3;

				Tweener.addTween
				(
					ghost,
					{
						alpha			: 1.0,
						scaleX			: ghost.sx,
						scaleY			: ghost.sy,
						time			: 0.3,
						delay			: 0.2,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: playInCompleteHandler
					}
				);
			}
			else
			{
				if( bPlayingOut )
					return;

				bPlayingOut = true;

				dispatchEvent( new DropAreaEvent( DropAreaEvent.DROP_AREA_COMPLETE ) );

				Tweener.addTween
				(
					ghost,
					{
						alpha			: 0.0,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: null
					}
				);

				Tweener.addTween
				(
					progress,
					{
						alpha			: 0.0,
						time			: 0.2,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: null
					}
				);

				Tweener.addTween
				(
					this,
					{
						progressVal		: 0.0,
						time			: 0.2,
						delay			: 0.2,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: null
					}
				);

				Tweener.addTween
				(
					this,
					{
						circleRadius	: 0,
						circleWidth		: 0,
						time			: 0.2,
						delay			: 0.2,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: playOutCompleteHandler
					}
				);
			}
		}

		private function playInCompleteHandler ():void
		{
			bPlayingIn = false;
		}

		private function playOutCompleteHandler ():void
		{
			bPlayingOut = false;

			dispatchEvent( new DropAreaEvent( DropAreaEvent.DROP_AREA_PLAYED_OUT ) );
		}

		///////////////////////////////////////////////////
		//	RESET.
		///////////////////////////////////////////////////

		public function reset ():void
		{
			crossWidth		= 0;
			progressVal 	= 0;

			circleRadius	= 0;
			circleWidth		= 0;

			progress.alpha	= 1.0;

			ghost.rotation	= ghost.ir;
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
			timeOut	= 1.0;

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

				Tweener.addTween
				(
					this,
					{
						progressPop	: 1.0,
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

				Tweener.addTween
				(
					this,
					{
						progressPop	: 0.0,
						time		: 0.2,
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
			playIn( false );
		}

		///////////////////////////////////////////////////
		//	GHOST ROTATION.
		///////////////////////////////////////////////////

		public function setGhostRotation ( r : Number ):void
		{
			//
		}

		public function releaseGhostRotation ():void
		{
			//
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

		private function drawCross ():void
		{
			crossRect1Mask.rotation = 135;
			crossRect1Mask.x =  Math.sin( 0.125 * 2 * Math.PI ) * circleRadius;
			crossRect1Mask.y = -Math.cos( 0.125 * 2 * Math.PI ) * circleRadius;

			var extra : Number;
			extra = 2;

			( crossRect1Mask.mc as MovieClip ).graphics.clear();
			( crossRect1Mask.mc as MovieClip ).graphics.beginFill( 0xFFFFFF );
			( crossRect1Mask.mc as MovieClip ).graphics.drawRect
			(
				-extra,
				(int)( -circleWidth * 0.5 ),
				( circleRadius * 2 + extra * 2 ) * crossWidth,
				circleWidthFull
			);
			( crossRect1Mask.mc as MovieClip ).graphics.endFill();
		}

		///////////////////////////////////////////////////
		// PROGRESS.
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
			if( bPlayingOut )		// don't want to redrawn the progress when playing out.
				return;

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
		// PROGRESS POP.
		///////////////////////////////////////////////////

		public function set progressPop ( value : Number ):void
		{
			_progressPop = value;
		}

		public function get progressPop ():Number
		{
			return _progressPop;
		}

		///////////////////////////////////////////////////
		//	CIRCLE.
		///////////////////////////////////////////////////

		private function drawCircle ():void
		{
			var pv : Number;
			pv = 12;

			var pp : Number
			pp = 5;

			var cr : Number;
			cr = circleRadius - ( progressVal * pv ) - ( progressPop  * pp );

			var cw : Number;
			cw = circleWidth + ( progressVal * pv * 2 ) + ( progressPop * pp * 2 );

			circle.graphics.clear();
			circle.graphics.beginFill( 0xFFFFFF );
			circle.graphics.drawCircle( 0, 0, cr );
			circle.graphics.drawCircle( 0, 0, cr + cw );
			circle.graphics.endFill();

			progress.graphics.clear();
			progress.graphics.beginFill( 0xFFFFFF );
			progress.graphics.drawCircle( 0, 0, cr );
			progress.graphics.drawCircle( 0, 0, cr + cw );
			progress.graphics.endFill();
		}

		public function set circleWidth ( value : Number ):void
		{
			_circleWidth = value;
		}

		public function get circleWidth ():Number
		{
			return _circleWidth;
		}

		public function set circleRadius ( value : Number ):void
		{
			_circleRadius = value;
		}

		public function get circleRadius ():Number
		{
			return _circleRadius;
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

			drawCircle();
			drawCross();
		}
	}
}