package com.julapy.ph.makeup.view
{
	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import flash.display.Sprite;

	public class ZoomView extends View
	{
		private var container0	: Sprite;
		private var container1	: Sprite;

		public  var zoomScale	: Number = 0;

		public function ZoomView( container0 : Sprite, container1 : Sprite )
		{
			super( new Sprite() );

			this.container0 = container0;
			this.container1 = container1;

			zoomScale = ModelLocator.getInstance().makeupModel.zoomScaleMin;

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE, modeEvent );
		}

		//////////////////////////////////////////////////////////
		//	ZOOM.
		//////////////////////////////////////////////////////////

		private function zoomToEyes ():void
		{

		}

		private function zoomToLips ():void
		{

		}

		private function zoomToFace ():void
		{

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
				zoomToFace();
			}
		}
	}
}