package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;

	public class ZoomView extends View
	{
		public  var zoomScaleMin	: Number = 0;
		public  var zoomScaleMax	: Number = 0;
		public  var zoomScale		: Number = 0;
		public  var zoomTime		: Number = 0.5;

		public function ZoomView( sprite : Sprite = null )
		{
			super( sprite );

			zoomScaleMin	= ModelLocator.getInstance().makeupModel.zoomScaleMin;
			zoomScaleMax	= ModelLocator.getInstance().makeupModel.zoomScaleMax;
			zoomScale 		= ModelLocator.getInstance().makeupModel.zoomScale;

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE, modeEvent );
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
					zoomScale	: zoomScaleMax,
					time		: zoomTime,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		private function zoomToLips ():void
		{
			Tweener.addTween
			(
				this,
				{
					zoomScale	: zoomScaleMax,
					time		: zoomTime,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		private function zoomToFace ():void
		{
			Tweener.addTween
			(
				this,
				{
					zoomScale	: zoomScaleMax,
					time		: zoomTime,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		private function zoomOut ():void
		{
			Tweener.addTween
			(
				this,
				{
					zoomScale	: zoomScaleMin,
					time		: zoomTime,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		private function tweenUpdateHandler ():void
		{
			ModelLocator.getInstance().makeupModel.zoomScale = zoomScale;
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function modeEvent ( e : ModeEvent ):void
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
				zoomOut();
			}
		}
	}
}