package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;
	import flash.geom.Point;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;

	public class ZoomView extends View
	{
		private var bEnabled		: Boolean = false;

		private var zoomScaleMin	: Number = 0;
		private var zoomScaleMax	: Number = 0;
		private var zoomTime		: Number = 0.5;
		public  var zoomScale		: Number = 0;
		public  var zoomOffsetX		: Number = 0;
		public  var zoomOffsetY		: Number = 0;

		public function ZoomView( sprite : Sprite = null )
		{
			super( sprite );

			zoomScaleMin	= ModelLocator.getInstance().makeupModel.zoomScaleMin;
			zoomScaleMax	= ModelLocator.getInstance().makeupModel.zoomScaleMax;
			zoomScale 		= ModelLocator.getInstance().makeupModel.zoomScale;

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,		modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,		modeZoomOutHandler );
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ( b : Boolean ):void
		{
			if( bEnabled == b )
				return;

			bEnabled	= b;

			visible		= bEnabled;
			doValidate();
		}

		//////////////////////////////////////////////////////////
		//	RESET.
		//////////////////////////////////////////////////////////

		private function reset ():void
		{
			Tweener.removeTweens( this );

			ModelLocator.getInstance().makeupModel.zoomOffset	= new Point( 0, 0 );
			ModelLocator.getInstance().makeupModel.zoomScale	= zoomScale = zoomScaleMin;
		}

		//////////////////////////////////////////////////////////
		//	ZOOM.
		//////////////////////////////////////////////////////////

		private function zoomToEyes ():void
		{
			Tweener.addTween
			(
				this,
				{
					zoomScale		: zoomScaleMax * 0.85,
					zoomOffsetY		: 30,
					time			: zoomTime,
					delay			: 0.0,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: tweenUpdateHandler,
					onComplete		: null
				}
			);
		}

		private function zoomToLips ():void
		{
			Tweener.addTween
			(
				this,
				{
					zoomScale		: zoomScaleMax,
					zoomOffsetY		: -400,
					time			: zoomTime,
					delay			: 0.0,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: tweenUpdateHandler,
					onComplete		: null
				}
			);
		}

		private function zoomToFace ():void
		{
			var z : Number;
			z = zoomScaleMax + ( zoomScaleMin - zoomScaleMax ) * 0.6;

			Tweener.addTween
			(
				this,
				{
					zoomScale		: z,
					zoomOffsetY		: -50,
					time			: zoomTime,
					delay			: 0.0,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: tweenUpdateHandler,
					onComplete		: null
				}
			);
		}

		private function zoomOut ():void
		{
			Tweener.addTween
			(
				this,
				{
					zoomScale		: zoomScaleMin,
					zoomOffsetY		: 0,
					time			: zoomTime,
					delay			: 0.0,
					transition		: Quadratic.easeOut,
					onStart			: null,
					onUpdate		: tweenUpdateHandler,
					onComplete		: null
				}
			);
		}

		private function tweenUpdateHandler ():void
		{
			ModelLocator.getInstance().makeupModel.zoomOffset	= new Point( zoomOffsetX, zoomOffsetY );
			ModelLocator.getInstance().makeupModel.zoomScale	= zoomScale;
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var girl	: int;
			var section : int;

			girl	= ModelLocator.getInstance().makeupModel.girl;
			section = ModelLocator.getInstance().makeupModel.section;

			if( section == MakeupModel.SECTION_INTRO || section == MakeupModel.SECTION_OUTRO )
			{
				enable( false );
//				reset();
			}

			if( section == MakeupModel.SECTION_PLAY )
			{
				reset();
				enable( true );
			}
		}

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
			if( !bEnabled )
				return;

			if( ModelLocator.getInstance().makeupModel.bPlayIntroPeriod )
				return;

			if( e.mode == MakeupModel.EYES_MODE )
			{
				zoomToEyes();
			}

			if( e.mode == MakeupModel.LIPS_MODE )
			{
				zoomToLips();
			}

			if( e.mode == MakeupModel.FACE_MODE )
			{
				zoomToFace();
			}
		}

		private function modeZoomOutHandler ( e : ModeEvent ):void
		{
			if( !bEnabled )
				return;

			if( ModelLocator.getInstance().makeupModel.bPlayIntroPeriod )
				return;

			zoomOut();
		}
	}
}