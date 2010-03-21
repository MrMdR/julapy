package com.julapy.ph.makeup.view
{
	import com.julapy.core.StageSize;
	import com.julapy.core.StageSizeEvent;
	import com.julapy.ph.makeup.events.ZoomEvent;
	import com.julapy.ph.makeup.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;

	public class ApplicationView
	{
		private var asset		: Sprite;

		private var stageWidth	: int;
		private var stageHeight	: int;

		private var container0	: Sprite;
		private var container1	: Sprite;

		private var menu		: MenuView;
		private var face		: FaceView;
		private var makeup		: MakeupView;
		private var zoom		: ZoomView;

		public function ApplicationView( asset : Sprite )
		{
			this.asset	= asset;

			ModelLocator.getInstance().makeupModel.addEventListener( ZoomEvent.ZOOM, zoomHandler );

			initViews();
			initModel();
		}

		private function initViews ():void
		{
			container0	= asset.getChildByName( "container0" ) as MovieClip;
			container1	= asset.getChildByName( "container1" ) as MovieClip;

			menu		= new MenuView( asset.getChildByName( "menu" ) as MovieClip );

			face		= new FaceView( container0 );

			makeup		= new MakeupView( container1 );

			zoom		= new ZoomView( container0, container1 );
		}

		private function initModel ():void
		{
			var zoomScaleMin : Number;
			zoomScaleMin = ModelLocator.getInstance().makeupModel.appWidth / container0.width;
			ModelLocator.getInstance().makeupModel.zoomScaleMin	= zoomScaleMin;
			ModelLocator.getInstance().makeupModel.zoomScaleMax	= 1.0;
			ModelLocator.getInstance().makeupModel.zoomScale	= zoomScaleMin;
		}

		////////////////////////////////////////////////////
		//	HANDLERS.
		////////////////////////////////////////////////////

		private function zoomHandler ( e : ZoomEvent ):void
		{
			var sx : Number = e.zoomScale;
			var sy : Number = e.zoomScale;

			container0.scaleX	= sx;
			container0.scaleY	= sy;

			container1.scaleX	= sx;
			container1.scaleY	= sy;
		}
	}
}