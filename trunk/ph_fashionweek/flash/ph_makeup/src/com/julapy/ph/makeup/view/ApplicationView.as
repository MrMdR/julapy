package com.julapy.ph.makeup.view
{
	import com.julapy.ph.makeup.events.DisconnectEvent;
	import com.julapy.ph.makeup.events.ZoomEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;
	import com.julapy.ph.of.SocketOF;
	import com.julapy.ph.of.SocketOFEvent;

	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.KeyboardEvent;
	import flash.geom.Matrix;
	import flash.ui.Keyboard;

	public class ApplicationView
	{
		private var asset		: Sprite;

		private var model		: MakeupModel;
		private var socket 		: SocketOF;

		private var faceHolder		: Sprite;
		private var faceBmHolder	: Sprite;
		private var faceBm			: Bitmap;
		private var useFaceBm		: Boolean = false;

		private var video		: VideoBaseView;
		private var menu		: MenuView;
		private var face		: FaceView;
		private var makeup		: MakeupView;
		private var zoom		: ZoomView;
		private var focus		: FocusView;
		private var blur		: FocusBlurView;
		private var grid		: GridView;
		private var timer		: TimerView;
		private var debug		: DebugView;

		public function ApplicationView( asset : Sprite )
		{
			this.asset	= asset;

			faceHolder		= asset.getChildByName( "face" ) as Sprite;
			faceBmHolder	= asset.getChildByName( "faceBm" ) as Sprite;
			faceBm			= new Bitmap( new BitmapData( 1, 1, true, 0x00FFFFFF ) );
			faceBmHolder.addChild( faceBm );

			model = ModelLocator.getInstance().makeupModel;
			model.addEventListener( ZoomEvent.ZOOM, zoomHandler );

			ModelLocator.getInstance().ofDataModel.addEventListener( DisconnectEvent.DISCONNECT, disconnectHandler );

			initModel();
			initViews();
			initSocket();

			asset.stage.addEventListener( KeyboardEvent.KEY_DOWN, keyDownHandler );

			ModelLocator.getInstance().makeupModel.girl		= MakeupModel.GIRL_ONE;
			ModelLocator.getInstance().makeupModel.section	= MakeupModel.SECTION_INTRO;
		}

		private function initViews ():void
		{
			video		= new VideoBaseView( asset.getChildByName( "video" ) as MovieClip );

			menu		= new MenuView( asset.getChildByName( "menu" ) as MovieClip );

			face		= new FaceView( faceHolder.getChildByName( "container0" ) as MovieClip );

			makeup		= new MakeupView( faceHolder.getChildByName( "container1" ) as MovieClip );

			zoom		= new ZoomView();

			grid		= new GridView( asset.getChildByName( "grid" ) as MovieClip );

			focus		= new FocusView( asset.getChildByName( "focus" ) as MovieClip );

//			blur		= new FocusBlurView( asset.getChildByName( "focus" ) as MovieClip, faceHolder );

			timer		= new TimerView( asset.getChildByName( "timer" ) as MovieClip );

			debug		= new DebugView( asset.getChildByName( "debug" ) as MovieClip );
			debug.visible = false;
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
			socket.connect();
		}

		////////////////////////////////////////////////////
		//	HANDLERS.
		////////////////////////////////////////////////////

		private function zoomHandler ( e : ZoomEvent ):void
		{
			var appW : int;
			var appH : int;

			appW = ModelLocator.getInstance().makeupModel.appWidth;
			appH = ModelLocator.getInstance().makeupModel.appHeight;

			var sx : Number = e.zoomScale;
			var sy : Number = e.zoomScale;

			if( useFaceBm )
			{
				if( !faceHolder.visible )
				{
					faceHolder.visible = false;
				}

				var m : Matrix;
				m = new Matrix();
				m.translate( (int)( appW * 0.5 ), (int)( appH * 0.5 ) );
				m.scale( sx, sy );

				faceBm.bitmapData.dispose();
				faceBm.bitmapData = new BitmapData( appW, appH, false );
				faceBm.bitmapData.draw( faceHolder, m );
			}
			else
			{
				faceHolder.scaleX	= sx;
				faceHolder.scaleY	= sy;

				faceHolder.x		= e.zoomOffset.x;
				faceHolder.y		= e.zoomOffset.y;
			}
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

		private function disconnectHandler ( e : DisconnectEvent ):void
		{
			socket.disconnect();
		}

		private function keyDownHandler ( e : KeyboardEvent ):void
		{
			if( e.keyCode == Keyboard.D )
			{
				debug.visible = !debug.visible;
			}
		}
	}
}