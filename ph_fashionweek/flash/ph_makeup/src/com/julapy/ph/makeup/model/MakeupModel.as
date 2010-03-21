package com.julapy.ph.makeup.model
{
	import com.julapy.ph.makeup.events.BlinkEvent;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.events.ZoomEvent;

	import flash.events.EventDispatcher;
	import flash.geom.Rectangle;

	public class MakeupModel extends EventDispatcher
	{
		public static const EYES_MODE	: int = 0;
		public static const LIPS_MODE	: int = 1;
		public static const FACE_MODE	: int = 2;

		private var _appRect		: Rectangle	= new Rectangle( 0, 0, 600, 800 );
		private var _imageRect		: Rectangle = new Rectangle( 0, 0, 1351, 1800 );

		private var _mode			: int = -1;

		private var _blinking		: Boolean	= false;

		private var _zoomScaleMin	: Number = 0;
		private var _zoomScaleMax	: Number = 0;
		private var _zoomScale		: Number = 0;

		public function MakeupModel()
		{
			//
		}

		/////////////////////////////////////
		//	APP SIZE.
		/////////////////////////////////////

		public function get appWidth ():int
		{
			return _appRect.width;
		}

		public function get appHeight ():int
		{
			return _appRect.height;
		}

		/////////////////////////////////////
		//	IMAGE SIZE.
		/////////////////////////////////////

		public function get imageWidth ():int
		{
			return _imageRect.width;
		}

		public function get imageHeight ():int
		{
			return _imageRect.height;
		}

		/////////////////////////////////////
		//	MODE.
		/////////////////////////////////////

		public function set mode ( value : int ):void
		{
			_mode = value;

			dispatchEvent( new ModeEvent( _mode ) );
		}

		public function get mode ():int
		{
			return _mode;
		}

		/////////////////////////////////////
		//	BLINKING.
		/////////////////////////////////////

		public function set blinking ( value : Boolean ):void
		{
			_blinking = value;

			if( _blinking )
			{
				dispatchEvent( new BlinkEvent( BlinkEvent.BLINK_START ) );
			}
			else
			{
				dispatchEvent( new BlinkEvent( BlinkEvent.BLINK_STOP ) );
			}
		}

		public function get blinking ():Boolean
		{
			return _blinking;
		}

		/////////////////////////////////////
		//	ZOOM SCALE.
		/////////////////////////////////////

		public function set zoomScaleMin ( value : Number ):void
		{
			_zoomScaleMin = value;
		}

		public function get zoomScaleMin ():Number
		{
			return _zoomScaleMin;
		}

		//--

		public function set zoomScaleMax ( value : Number ):void
		{
			_zoomScaleMax = value;
		}

		public function get zoomScaleMax ():Number
		{
			return _zoomScaleMax;
		}

		//--

		public function set zoomScale ( value : Number ):void
		{
			_zoomScale = value;

			dispatchEvent( new ZoomEvent( _zoomScale ) );
		}

		public function get zoomScale ():Number
		{
			return _zoomScale;
		}
	}
}