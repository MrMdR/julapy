package com.julapy.net
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.events.SecurityErrorEvent;
	import flash.events.TimerEvent;
	import flash.net.Socket;
	import flash.utils.Timer;

	[Event( name="connected", 		type="com.julapy.net.SocketOFEvent" )]
	[Event( name="tryingToConnect", type="com.julapy.net.SocketOFEvent" )]
	[Event( name="disconnected",	type="com.julapy.net.SocketOFEvent" )]
	[Event( name="dataReceived",	type="com.julapy.net.SocketOFEvent" )]

	public class SocketOF extends EventDispatcher
	{
		private var socket 		: Socket;
		private var bConnected	: Boolean 	= false;
		private var dataString	: String	= "";

		public function SocketOF(target:IEventDispatcher=null)
		{
			super( target );

			initSocket();
		}

		private function initSocketDelayed ( delay : int = 100 ):void
		{
			var timer : Timer;
			timer = new Timer( delay, 1 );
			timer.addEventListener( TimerEvent.TIMER_COMPLETE, initSocketDelayedHandler );
			timer.start();
		}

		private function initSocketDelayedHandler( e : TimerEvent ):void
		{
			var timer : Timer;
			timer = e.target as Timer;
			timer.removeEventListener( TimerEvent.TIMER_COMPLETE, initSocketDelayedHandler );
			timer.stop();
			timer = null;

			initSocket();
		}

	    private function initSocket (): void
	    {
            socket = new Socket( "localhost", 11999 );
			socket.addEventListener( Event.CONNECT,						socketConnectHandler 	);
			socket.addEventListener( Event.CLOSE,						socketCloseHandler	 	);
			socket.addEventListener( IOErrorEvent.IO_ERROR,				socketIOErrorHandler 	);
			socket.addEventListener( ProgressEvent.SOCKET_DATA, 		socketDataHandler	 	);
			socket.addEventListener( SecurityErrorEvent.SECURITY_ERROR, socketSecurityHandler	);
	    }

	    private function killSocket ():void
	    {
	    	if( socket )
	    	{
	    		socket.close();
	    		socket.removeEventListener( Event.CONNECT,						socketConnectHandler 	);
	    		socket.removeEventListener( Event.CLOSE,						socketCloseHandler	 	);
				socket.removeEventListener( IOErrorEvent.IO_ERROR,				socketIOErrorHandler 	);
				socket.removeEventListener( ProgressEvent.SOCKET_DATA, 			socketDataHandler	 	);
				socket.removeEventListener( SecurityErrorEvent.SECURITY_ERROR,	socketSecurityHandler	);
				socket = null;
	    	}
	    }

	    /////////////////////////////////////////////////////
	    //	HANDLERS.
	    /////////////////////////////////////////////////////

	    private function socketConnectHandler ( e : Event ):void
	    {
			bConnected = true;

			socket.flush();

			var event : SocketOFEvent;
			event = new SocketOFEvent( SocketOFEvent.CONNECTED );
			event.connected		= bConnected;
			dispatchEvent( event );
	    }

	    private function socketCloseHandler ( e : Event ):void
	    {
			bConnected = false;

			killSocket();
			initSocketDelayed( 1000 );

			var event : SocketOFEvent;
			event = new SocketOFEvent( SocketOFEvent.DISCONNECTED );
			event.connected		= bConnected;
			dispatchEvent( event );
	    }

	    private function socketIOErrorHandler ( e : IOErrorEvent ):void
	    {
			killSocket();
			initSocketDelayed( 1000 );

			var event : SocketOFEvent;
			event = new SocketOFEvent( SocketOFEvent.TRYING_TO_CONNECT );
			event.connected		= bConnected;
			dispatchEvent( event );
	    }

	    private function socketDataHandler ( e : ProgressEvent ):void
	    {
			dataString = socket.readUTFBytes( socket.bytesAvailable );

			var event : SocketOFEvent;
			event = new SocketOFEvent( SocketOFEvent.DATA_RECEIVED );
			event.connected		= bConnected;
			event.dataString	= dataString;
			dispatchEvent( event );
	    }

	    private function socketSecurityHandler ( e : SecurityErrorEvent ):void
	    {
			trace( e );
	    }
	}
}