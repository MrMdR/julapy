package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;
	import flash.geom.Point;

	public class ZoomView extends View
	{
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

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,	modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,	modeZoomOutHandler );
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

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
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
			zoomOut();
		}
	}
}