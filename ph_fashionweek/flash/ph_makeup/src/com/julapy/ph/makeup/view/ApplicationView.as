package com.julapy.ph.makeup.view
{
	import com.julapy.ph.events.DisconnectEvent;
	import com.julapy.ph.makeup.events.GirlEvent;
	import com.julapy.ph.makeup.events.ZoomEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;
	import com.julapy.ph.of.SocketOF;
	import com.julapy.ph.of.SocketOFEvent;

	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.StageDisplayState;
	import flash.events.KeyboardEvent;
	import flash.geom.Matrix;
	import flash.ui.Keyboard;

	public class ApplicationView
	{
		private var asset		: Sprite;

		private var model		: MakeupModel;
		private var socket 		: SocketOF;

		private var girl			: MovieClip;
		private var girlOne			: MovieClip;
		private var girlTwo			: MovieClip;

		private var girlBmHolder	: Sprite;
		private var girlBm			: Bitmap;
		private var useGirlBm		: Boolean = false;

		private var video		: VideoBaseView;
		private var cover		: CoverView;
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

			girlOne			= asset.getChildByName( "girlOne" ) as MovieClip;
			girlTwo			= asset.getChildByName( "girlTwo" ) as MovieClip;
			girlTwo.visible	= false;
			girl			= girlOne;

			girlBmHolder	= asset.getChildByName( "girlBm" ) as Sprite;
			girlBm			= new Bitmap( new BitmapData( 1, 1, true, 0x00FFFFFF ) );
			girlBmHolder.addChild( girlBm );

			initModel();
			initViews();
			initSocket();

			ModelLocator.getInstance().ofDataModel.addEventListener( DisconnectEvent.DISCONNECT, disconnectHandler );
			asset.stage.addEventListener( KeyboardEvent.KEY_DOWN, keyDownHandler );

			model.girl		= MakeupModel.GIRL_ONE;
			model.section	= MakeupModel.SECTION_INTRO;
		}

		private function initModel ():void
		{
			model = ModelLocator.getInstance().makeupModel;
			model.addEventListener( GirlEvent.GIRL_CHANGE,	girlChangeHandler );
			model.addEventListener( ZoomEvent.ZOOM,			zoomHandler );

			var zoomScaleMin : Number;
			zoomScaleMin = ModelLocator.getInstance().commondModel.appWidth / model.imageWidth;
			model.zoomScaleMin	= zoomScaleMin;
			model.zoomScaleMax	= 1.0;
			model.zoomScale		= zoomScaleMin;
		}

		private function initViews ():void
		{
			video		= new VideoBaseView( asset.getChildByName( "video" ) as MovieClip );

			cover		= new CoverView( asset.getChildByName( "cover" ) as MovieClip );

			menu		= new MenuView( asset.getChildByName( "menu" ) as MovieClip );

			face		= new FaceView( girl.getChildByName( "baseImage" ) as MovieClip );

			makeup		= new MakeupView( girl.getChildByName( "makeupHolder" ) as MovieClip );

			zoom		= new ZoomView();

			grid		= new GridView( asset.getChildByName( "grid" ) as MovieClip );

			focus		= new FocusView( asset.getChildByName( "focus" ) as MovieClip );

//			blur		= new FocusBlurView( asset.getChildByName( "focus" ) as MovieClip, faceHolder );

			timer		= new TimerView( asset.getChildByName( "timer" ) as MovieClip );

			debug		= new DebugView( asset.getChildByName( "debug" ) as MovieClip );
			debug.visible = false;
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

		private function girlChangeHandler ( e : GirlEvent ):void
		{
			var g : int;
			g = ModelLocator.getInstance().makeupModel.girl;

			face.stop();
			makeup.stop();

			if( g == MakeupModel.GIRL_ONE )
			{
				girlOne.visible = true;
				girlTwo.visible = false;
				girl			= girlOne;
			}

			if( g == MakeupModel.GIRL_TWO )
			{
				girlOne.visible = false;
				girlTwo.visible = true;
				girl			= girlTwo;
			}

			girl.scaleX = model.zoomScaleMin;
			girl.scaleY = model.zoomScaleMin;

			face.setAsset( girl.getChildByName( "baseImage" ) as MovieClip );
			face.start();

			makeup.setAsset( girl.getChildByName( "makeupHolder" ) as MovieClip );
			makeup.start();
		}

		private function zoomHandler ( e : ZoomEvent ):void
		{
			var appW : int;
			var appH : int;

			appW = ModelLocator.getInstance().commondModel.appWidth;
			appH = ModelLocator.getInstance().commondModel.appHeight;

			var sx : Number = e.zoomScale;
			var sy : Number = e.zoomScale;

			if( useGirlBm )
			{
				if( !girl.visible )
				{
					girl.visible = false;
				}

				var m : Matrix;
				m = new Matrix();
				m.translate( (int)( appW * 0.5 ), (int)( appH * 0.5 ) );
				m.scale( sx, sy );

				girlBm.bitmapData.dispose();
				girlBm.bitmapData = new BitmapData( appW, appH, false );
				girlBm.bitmapData.draw( girl, m );
			}
			else
			{
				girl.scaleX	= sx;
				girl.scaleY	= sy;

				girl.x		= e.zoomOffset.x;
				girl.y		= e.zoomOffset.y;
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

			if( e.keyCode == Keyboard.F )
			{
				if( asset.stage.displayState != StageDisplayState.FULL_SCREEN_INTERACTIVE )
				{
					asset.stage.displayState = StageDisplayState.FULL_SCREEN_INTERACTIVE;
				}
				else
				{
					asset.stage.displayState = StageDisplayState.NORMAL;
				}
			}
		}
	}
}