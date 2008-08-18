package com.holler.video
{
	import flash.display.Sprite;
	import flash.net.NetConnection;
	import flash.net.NetStream;
	import flash.events.NetStatusEvent;
	import flash.media.Video;
	import flash.events.AsyncErrorEvent;
	import flash.events.SecurityErrorEvent;
	import flash.media.Sound;
	import flash.media.SoundTransform;
	import flash.events.Event;
	import flash.events.ProgressEvent;

	public class VideoPlayer extends Sprite
	{
		private var _container:Sprite;

		private var _nc:NetConnection;
		private var _ns:NetStream;

		private var _video:Video;
		private var _videoWidth:Number;
		private var _videoHeight:Number;
		private var _videoUrl:String;

		private var _bufferTime:Number = 5;
		private var _durationTime:Number = 0;
		private var _playheadTime:Number = 0;
		private var _frameRate:Number = 0;
		
		private var _isPlaying:Boolean  = false;
		private var _isLoading:Boolean  = false;
		private var _isComplete:Boolean = false;
		private var _loop:Boolean		= false;
		
		public function VideoPlayer(container:Sprite, videoWidth:Number, videoHeight:Number)
		{
			_container		= container;
			_videoWidth 	= videoWidth;
			_videoHeight 	= videoHeight;
			
			_video 			= new Video(videoWidth, videoHeight);
			_video.addEventListener(Event.ENTER_FRAME, onEnterFrame);
		
			_container.addChild(_video);

			_nc = new NetConnection();
            _nc.addEventListener(NetStatusEvent.NET_STATUS, onNetStatus);
            _nc.addEventListener(SecurityErrorEvent.SECURITY_ERROR, onSecurityError);
            _nc.connect(null);
		}
		
		public function load (videoUrl:String):void
		{
			_videoUrl = videoUrl;
			
			_ns.close();
			_ns.play(_videoUrl);
			_ns.pause();
		}
		
		public function play ():void
		{
			if(_isComplete)	_ns.seek(0);
			
			_ns.resume();
			
			_isPlaying	= true;
			_isComplete = false;
		}
		
		public function pause ():void
		{
			_ns.pause();
			
			_isPlaying = false;
		}
		
		public function seek (t:Number):void
		{
			_ns.seek(t);
			
			_isPlaying	= true;
			_isComplete = false;
		}
		
		public function resize ( width:int=0, height:int=0, keepAspectRatio:Boolean=false ):void
		{
			if( keepAspectRatio )
			{
				var videoRatio:Number	= _video.videoWidth / _video.videoHeight;
				var resizeRatio:Number	= width / height;
				
				if( resizeRatio >= videoRatio )
				{
					_video.width	= _video.videoWidth * ( height / _video.videoHeight );
					_video.height	= height;
					_video.x		= (int)( ( width - _video.width ) / 2 );
				}
				else
				{
					_video.width	= width;
					_video.height	= _video.videoHeight * ( width / _video.videoWidth );
					_video.y		= (int)( ( height - _video.height ) / 2 );
				}
			}
			else
			{
				_video.width	= width;
				_video.height	= height;
				_video.x		= 0;
				_video.y		= 0;
			}
		}
		
        private function connectStream ():void
        {
			_ns = new NetStream(_nc);
			_ns.addEventListener(NetStatusEvent.NET_STATUS, onNetStatus);
			_ns.addEventListener(AsyncErrorEvent.ASYNC_ERROR, onAsyncError);
			_ns.client = {onMetaData:onMetaData, onPlayStatus:onPlayStatus};
			
			_video.attachNetStream(_ns);
        }
		
//______________________________________________________________________________________GETTERS / SETTERS
		
		public function get video ():Video
		{
			return _video;
		}
		
		public function get container ():Sprite
		{
			return _container;
		}
		
		public function get volume ():Number
		{
			return _ns.soundTransform.volume;
		}
		
		public function set volume (value:Number):void
		{
			_ns.soundTransform = new SoundTransform(value);
		}
		
		public function get durationTime ():Number
		{
			return _durationTime;
		}
		
		public function get playheadTime ():Number
		{
			return _playheadTime;
		}
		
		public function get loop ():Boolean
		{
			return _loop;
		}
		
		public function set loop (value:Boolean):void
		{
			_loop = value;
		}
		
//______________________________________________________________________________________THE BAD BOY DESTROY
		
		public function destroy():void
		{
			if(_ns) 
			{
				_ns.close();
				_ns = null;
			}
		}

//______________________________________________________________________________________VIDEO HANDLERS
		
		private function onMetaData (info:Object):void
		{
			if(info.duration)	_durationTime	= info.duration
			if(info.framerate)	_frameRate		= info.framerate;
		}
		
		private function onPlayStatus (info:Object):void
		{
			//
		}
		
		private function onEnterFrame (e:Event):void
		{
			if(_isPlaying)
			{
				_playheadTime = _ns.time;
				
				var playEvent:VideoPlayerEvent	= new VideoPlayerEvent(VideoPlayerEvent.UPDATE);
				playEvent.time					= _playheadTime;
				playEvent.duration				= _durationTime;
				
				dispatchEvent(playEvent);
			}

			if(_isLoading)
			{
				var progEvent:ProgressEvent		= new ProgressEvent(ProgressEvent.PROGRESS);
				progEvent.bytesLoaded			= _ns.bytesLoaded;
				progEvent.bytesTotal			= _ns.bytesTotal;
				
				dispatchEvent(progEvent);
				
				if(_ns.bytesLoaded == _ns.bytesTotal)
				{
					_isLoading = false;
				}
			}
		}
		
		private function onComplete ():void
		{
			if(_loop)
			{
				_ns.seek(0);
				_ns.resume();
			}
			else
			{
				pause();
			}

			_playheadTime = _durationTime;
			
			var event:VideoPlayerEvent = new VideoPlayerEvent(VideoPlayerEvent.UPDATE);
			event.time		= _playheadTime;
			event.duration	= _durationTime;
			
			dispatchEvent(event);
		}

//______________________________________________________________________________________CONNECTION HANDLERS		

		private function onNetStatus (event:NetStatusEvent):void
		{
			trace("VideoPlayer :: onNetStatus :: " + event.info.code);
            
            switch (event.info.code) 
            {
                case "NetConnection.Connect.Success" :
					
					connectStream();
					
                    break;
                    
                case "NetStream.Play.Start" :
                
					dispatchEvent(new VideoPlayerEvent(VideoPlayerEvent.PLAY_START));
					
					_isLoading = true;
                
                	break;
                	
               	case "NetStream.Play.Stop" :
               	
               		onComplete();
               		
               		_isComplete = true;
               	
               		dispatchEvent(new VideoPlayerEvent(VideoPlayerEvent.PLAY_STOP));
               	
               		break;
            }
		}
		
		private function onSecurityError (event:SecurityError):void
		{
			trace("VideoPlayer :: onSecurityError :: + " + event.toString());
		}
		
		private function onAsyncError (event:AsyncErrorEvent):void 
		{
            // ignore AsyncErrorEvent events.
        }
	}
}