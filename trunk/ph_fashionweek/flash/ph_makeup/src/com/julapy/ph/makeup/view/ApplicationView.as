package com.julapy.ph.makeup.view
{
	import com.julapy.ph.makeup.events.ZoomEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;
	import com.julapy.ph.makeup.of.SocketOF;
	import com.julapy.ph.makeup.of.SocketOFEvent;

	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class ApplicationView
	{
		private var asset		: Sprite;

		private var model		: MakeupModel;
		private var socket 		: SocketOF;

		private var container0	: Sprite;
		private var container1	: Sprite;
		private var container2	: Sprite;
		private var container3	: Sprite;

		private var menu		: MenuView;
		private var face		: FaceView;
		private var makeup		: MakeupView;
		private var zoom		: ZoomView;
		private var focus		: FocusView;
		private var grid		: GridView;

		public function ApplicationView( asset : Sprite )
		{
			this.asset	= asset;

			container0	= asset.getChildByName( "container0" ) as MovieClip;
			container1	= asset.getChildByName( "container1" ) as MovieClip;
			container2	= asset.getChildByName( "container2" ) as MovieClip;
			container3	= asset.getChildByName( "container3" ) as MovieClip;

			model = ModelLocator.getInstance().makeupModel;
			model.addEventListener( ZoomEvent.ZOOM, zoomHandler );

			initModel();
			initViews();
			initSocket();
		}

		private function initViews ():void
		{
			menu		= new MenuView( asset.getChildByName( "menu" ) as MovieClip );

			face		= new FaceView( container0 );

			makeup		= new MakeupView( container1 );

			zoom		= new ZoomView();

			grid		= new GridView( container2 );

			focus		= new FocusView( container3 );
		}

		private function initModel ():void
		{
			var zoomScaleMin : Number;
			zoomScaleMin = model.appWidth / model.imageWidth;
			model.zoomScaleMin	= zoomScaleMin;
			model.zoomScaleMax	= 1.0;
			model.zoomScale		= zoomScaleMin;
		}

		private function initSocket ():void
		{
			socket = new SocketOF();
			socket.addEventListener( SocketOFEvent.CONNECTED,			socketHandler );
			socket.addEventListener( SocketOFEvent.DISCONNECTED,		socketHandler );
			socket.addEventListener( SocketOFEvent.TRYING_TO_CONNECT,	socketHandler );
			socket.addEventListener( SocketOFEvent.DATA_RECEIVED,		socketHandler );
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

			container0.x		= e.zoomOffset.x;
			container0.y		= e.zoomOffset.y;

			container1.x		= e.zoomOffset.x;
			container1.y		= e.zoomOffset.y;
		}

		private function socketHandler ( e : SocketOFEvent ):void
		{
			if( e.type == SocketOFEvent.CONNECTED )
			{
				ModelLocator.getInstance().ofDataModel.connected = true;
			}

			if( e.type == SocketOFEvent.DISCONNECTED )
			{
				ModelLocator.getInstance().ofDataModel.connected = false;
			}

			if( e.type == SocketOFEvent.TRYING_TO_CONNECT )
			{
				// trying and trying.
			}

			if( e.type == SocketOFEvent.DATA_RECEIVED )
			{
				ModelLocator.getInstance().ofDataModel.ofStringData = e.dataString;
			}
		}
	}
}