package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.hair.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Point;

	public class MenuDropAreaRingView extends View
	{
		private var bShow	: Boolean = false;
		private var bInit	: Boolean = false;
		private var bOver	: Boolean = false;
		private var bCross	: Boolean = false;

		private var mc		: MovieClip;
		private var ring	: MovieClip;
		private var ring0	: MovieClip;
		private var ring1	: MovieClip;
		private var ring2	: MovieClip;

		private var scl		: Point = new Point();

		private var ringRotation		: Number = 1;
		private var ringRotationTarget	: Number = 1;
		private var ringRotationEase	: Number = 0.3;

		public  var ringCharge			: Number = 0;

		public function MenuDropAreaRingView(sprite:Sprite=null)
		{
			super(sprite);

			mc = _sprite as MovieClip;
			mc.gotoAndStop( 1 );

			ring	= mc.getChildByName( "ring" ) as MovieClip;
			ring0	= ring.getChildByName( "ring0" ) as MovieClip;
			ring1	= ring.getChildByName( "ring1" ) as MovieClip;
			ring2	= ring.getChildByName( "ring2" ) as MovieClip;

			visible = false;
			doValidate();
		}

		///////////////////////////////////////////////////
		//	PUBLIC.
		///////////////////////////////////////////////////

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				mc.addEventListener( Event.ENTER_FRAME, enterFrameHandler );

				mc.play();

				visible = true;
				doValidate();
			}
			else
			{
				ring.play();
			}
		}

		public function over ( b : Boolean ):void
		{
			if( !bInit )
				return;

			if( bOver == b )
				return;

			bOver = b;

			if( bOver )
			{
				ringRotationTarget = 1.25;

				overPop( true );
				overCharge( true );
			}
			else
			{
				ringRotationTarget = 0.75;

				overPop( false );
				overCharge( false );
			}
		}

		public function cross ( b : Boolean ):void
		{
			if( bCross == b )
				return;

			bCross = b;

			if( bCross )
			{
				ringRotationTarget = 0.25;
			}
			else
			{
				ringRotationTarget = 0.75;
			}
		}

		///////////////////////////////////////////////////
		//	OVER POP.
		///////////////////////////////////////////////////

		private function overPop ( b : Boolean ):void
		{
			if( b )
			{
				var sclMult : Number;
				sclMult = 1.05;

				Tweener.addTween
				(
					mc,
					{
						scaleX		: scl.x * sclMult,
						scaleY		: scl.y * sclMult,
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
					mc,
					{
						scaleX		: scl.x,
						scaleY		: scl.y,
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

		///////////////////////////////////////////////////
		//	OVER CHARGE.
		///////////////////////////////////////////////////

		private function overCharge ( b : Boolean ):void
		{
			var time	: Number;
			var timeIn	: Number
			var timeOut	: Number;

			time	= 0;
			timeIn	= 2.0;
			timeOut	= 1.0;

			if( b )
			{
				time = ( 1 - ringCharge ) * timeIn;

				Tweener.addTween
				(
					this,
					{
						ringCharge	: 1.0,
						time		: time,
						delay		: 0.0,
						transition	: Quadratic.easeIn,
						onStart		: null,
						onUpdate	: overChargeUpdateHandler,
						onComplete	: overChargeFullHandler
					}
				);
			}
			else
			{
				time = ringCharge * timeOut;

				Tweener.addTween
				(
					this,
					{
						ringCharge	: 0.0,
						time		: time,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: overChargeUpdateHandler,
						onComplete	: overChargeCompleteHandler
					}
				);
			}
		}

		private function overChargeUpdateHandler ():void
		{
			drawCharge();
		}

		private function overChargeCompleteHandler ():void
		{
			clearCharge();
		}

		private function overChargeFullHandler ():void
		{
			ModelLocator.getInstance().hairModel.dropAreaCharged();
		}

		///////////////////////////////////////////////////
		//	CHARGE.
		///////////////////////////////////////////////////

		private function drawCharge ():void
		{
			ring2.graphics.clear();
			ring2.graphics.beginFill( 0xFF00FF );
			ring2.graphics.moveTo( 0, 0 );

			var radius : Number;
			radius = 250;

			var steps : int;
			steps = 720;

			var ang : int;
			ang = (int)( steps * ringCharge );

			for( var i:int=0; i<ang+1; i++ )
			{
				var p : Point;
				p	= new Point();
				p.x =  Math.sin( ( i / steps ) * 2 * Math.PI ) * radius;
				p.y = -Math.cos( ( i / steps ) * 2 * Math.PI ) * radius;

				ring2.graphics.lineTo( p.x, p.y );
			}

			ring2.graphics.moveTo( 0, 0 );
			ring2.graphics.endFill();
		}

		private function clearCharge ():void
		{
			ring2.graphics.clear();
		}

		///////////////////////////////////////////////////
		//	ENTER FRAME.
		///////////////////////////////////////////////////

		private function enterFrameHandler ( e : Event ):void
		{
			ringRotation += ( ringRotationTarget - ringRotation ) * ringRotationEase;

			ring0.rotation += ringRotation;
			ring1.rotation -= ringRotation;

			if( mc.currentFrame == 9 )
			{
				if( !bInit )
				{
					bInit = true;

					scl.x = mc.scaleX;
					scl.y = mc.scaleY;

					mc.stop();
				}
			}

			if( mc.currentFrame == mc.totalFrames )
			{
				mc.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

				visible = false;
				doValidate();
			}
		}

	}
}