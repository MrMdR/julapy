package com.julapy.ph.hair.view
{
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.of.SocketOF;
	import com.julapy.ph.of.SocketOFEvent;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;

	public class ApplicationView
	{
		private var asset		: Sprite;

		private var model		: HairModel;
		private var socket 		: SocketOF;

		private var menu		: MenuView;
		private var videoInt	: VideoInteractiveView;
		private var videoGen	: VideoGeneralView;
		private var debug		: DebugView;

		public function ApplicationView( asset : Sprite )
		{
			this.asset	= asset;

			initModel();
			initViews();
			initSocket();

			asset.stage.addEventListener( KeyboardEvent.KEY_DOWN, keyDownHandler );

			model.girl		= HairModel.GIRL_ONE;
			model.style		= HairModel.STYLE_ONE;
			model.section	= HairModel.SECTION_INTRO;
			model.stylePart	= HairModel.STYLE_PART_ONE;
		}

		private function initModel ():void
		{
			model = ModelLocator.getInstance().hairModel;
		}

		private function initViews ():void
		{

			videoGen	= new VideoGeneralView( asset.getChildByName( "videoGen" ) as MovieClip );

			videoInt	= new VideoInteractiveView( asset.getChildByName( "videoInt" ) as MovieClip );

			menu		= new MenuView( asset.getChildByName( "menu" ) as MovieClip );

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

		private function keyDownHandler ( e : KeyboardEvent ):void
		{
			if( e.keyCode == Keyboard.D )
			{
				debug.visible = !debug.visible;
			}
		}

	}
}