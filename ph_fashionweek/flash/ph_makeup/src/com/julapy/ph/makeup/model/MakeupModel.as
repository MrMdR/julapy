package com.julapy.ph.makeup.model
{
	import com.julapy.ph.makeup.events.AttractorChangeEvent;
	import com.julapy.ph.makeup.events.BlinkEvent;
	import com.julapy.ph.makeup.events.GirlEvent;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.events.ResetEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.events.ZoomEvent;
	import com.julapy.ph.makeup.vo.GirlMakeupVO;
	import com.julapy.ph.makeup.vo.GirlOneMakeupVO;
	import com.julapy.ph.makeup.vo.GirlTwoMakeupVO;

	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.Timer;
	import flash.utils.getQualifiedClassName;

	public class MakeupModel extends EventDispatcher
	{
		public static const GIRL_ONE		: int = 0;
		public static const GIRL_TWO		: int = 1;

		public static const SECTION_INTRO	: int = 0;
		public static const SECTION_PLAY	: int = 1;
		public static const SECTION_OUTRO	: int = 2;

		public static const EYES_MODE		: int = 0;
		public static const LIPS_MODE		: int = 1;
		public static const FACE_MODE		: int = 2;

		private var _videoRect		: Rectangle = new Rectangle( 0, 0, 576, 324 );
		private var _imageRect		: Rectangle = new Rectangle( 0, 0, 1152, 2048 );

		private var _attractors					: Array = new Array();
		private var _attractorIndex				: int	= -1;
		private var _attractorTimeoutSeconds	: int = 60;
		private var _bAttractor					: Boolean = false;

		private var _girls			: Array = [ GIRL_ONE, GIRL_TWO ];
		private var _sections		: Array = [ SECTION_INTRO, SECTION_PLAY, SECTION_OUTRO ];

		private var _girlOneVO		: GirlOneMakeupVO = new GirlOneMakeupVO();
		private var _girlTwoVO		: GirlTwoMakeupVO = new GirlTwoMakeupVO();
		private var _girlVOs		: Array = [ _girlOneVO, _girlTwoVO ];

		private var _girl			: int = -1;
		private var _section		: int = -1;

		private var _mode				: int = -1;
		private var _modeZoomInTimer	: Timer;
		private var _modeZoomInTime		: int = 500;
		private var _modeZoomOutTimer	: Timer;
		private var _modeZoomOutTime	: int = 1000;

		private var _blinking		: Boolean	= false;
		private var _blinkForce		: Boolean	= false;

		private var _zoomScaleMin	: Number = 0;
		private var _zoomScaleMax	: Number = 0;
		private var _zoomScale		: Number = 0;
		private var _zoomOffset		: Point	 = new Point();

		private var _bPlayIntroPeriod	: Boolean = false;

		public function MakeupModel()
		{
			_attractors =
			[
				"flv/attractor_makeupartist.f4v",
				"flv/attractor_models.f4v"
			];
		}

		/////////////////////////////////////
		//	VIDEO RECT.
		/////////////////////////////////////

		public function get videoRect ():Rectangle
		{
			return _videoRect.clone();
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
		//	GIRL VO.
		/////////////////////////////////////

		public function get girlVOs ():Array
		{
			return _girlVOs;
		}

		public function get girlVO ():GirlMakeupVO
		{
			return _girlVOs[ _girl ];
		}

		/////////////////////////////////////
		//	GIRL.
		/////////////////////////////////////

		public function set girl ( value : int ):void
		{
			if( _girl != value )
			{
				_girl = value;

				dispatchEvent( new GirlEvent( _girl ) );
			}
		}

		public function get girl ():int
		{
			return _girl;
		}

		public function nextGirl ():void
		{
			var girlTemp : int;
			girlTemp = _girl;

			var vo : GirlMakeupVO;		// move onto next video.
			vo = _girlVOs[ _girl ];
			vo.stepVideo();

			if( ++girlTemp >= _girls.length )
			{
				girl = 0;

				return;
			}

			girl = girlTemp;
		}

		/////////////////////////////////////
		//	SECTION.
		/////////////////////////////////////

		public function set section ( value : int ):void
		{
			if( _section != value )
			{
				_section = value;

				dispatchEvent( new SectionEvent( _section ) );
			}
		}

		public function get section ():int
		{
			return _section;
		}

		public function nextSection ():void
		{
			var sectionTemp : int;
			sectionTemp = _section;

			if( ++sectionTemp >= _sections.length )
			{
				nextGirl();
				section = 0;

				return;
			}

			section = sectionTemp;
		}

		/////////////////////////////////////
		//	MODE.
		/////////////////////////////////////

		public function set mode ( value : int ):void
		{
			if( _mode != value )
			{
				killModeZoomInTimer();
				killModeZoomOutTimer();

				if( value >= 0 && value <= 2 )
				{
					_mode = value;

					dispatchEvent( new ModeEvent( ModeEvent.MODE_ZOOM_IN, _mode ) );

					initModeZoomInTimer();
				}
				else
				{
					_mode = -1;

					initModeZoomOutTimer();
				}

				trace( getQualifiedClassName( this ) + " :: mode = " + _mode.toString() );
			}
		}

		public function get mode ():int
		{
			return _mode;
		}

		//-- mode zoom in timer.

		private function initModeZoomInTimer ():void
		{
			_modeZoomInTimer = new Timer( _modeZoomInTime, 1 );
			_modeZoomInTimer.addEventListener( TimerEvent.TIMER_COMPLETE, modeZoomInTimerHandler );
			_modeZoomInTimer.start();
		}

		private function killModeZoomInTimer ():void
		{
			if( _modeZoomInTimer )
			{
				_modeZoomInTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, modeZoomInTimerHandler );
				_modeZoomInTimer.stop();
				_modeZoomInTimer = null;
			}
		}

		private function modeZoomInTimerHandler ( e : TimerEvent ):void
		{
			killModeZoomInTimer();

			dispatchEvent( new ModeEvent( ModeEvent.MODE_ANIM_IN, _mode ) );
		}

		//-- mode zoom out timer.

		private function initModeZoomOutTimer ():void
		{
			_modeZoomOutTimer = new Timer( _modeZoomOutTime, 1 );
			_modeZoomOutTimer.addEventListener( TimerEvent.TIMER_COMPLETE, modeZoomOutTimerHandler );
			_modeZoomOutTimer.start();
		}

		private function killModeZoomOutTimer ():void
		{
			if( _modeZoomOutTimer )
			{
				_modeZoomOutTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, modeZoomOutTimerHandler );
				_modeZoomOutTimer.stop();
				_modeZoomOutTimer = null;
			}
		}

		private function modeZoomOutTimerHandler ( e : TimerEvent ):void
		{
			killModeZoomOutTimer();

			dispatchEvent( new ModeEvent( ModeEvent.MODE_ZOOM_OUT, _mode ) );
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

		//-- force blink.

		public function set blinkForce ( value : Boolean ):void
		{
			_blinkForce = value;

			if( _blinkForce )
			{
				dispatchEvent( new BlinkEvent( BlinkEvent.BLINK_FORCE_START ) );
			}
			else
			{
				dispatchEvent( new BlinkEvent( BlinkEvent.BLINK_FORCE_STOP ) );
			}
		}

		public function get blinkForce ():Boolean
		{
			return _blinkForce;
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

			dispatchEvent( new ZoomEvent( _zoomScale, _zoomOffset.clone() ) );
		}

		public function get zoomScale ():Number
		{
			return _zoomScale;
		}

		//--

		public function set zoomOffset ( value : Point ):void
		{
			_zoomOffset = value.clone();
		}

		public function get zoomOffset ():Point
		{
			return _zoomOffset;
		}

		/////////////////////////////////////
		//	ATTRACTOR.
		/////////////////////////////////////

		public function set bAttractor ( value : Boolean ):void
		{
			if( _bAttractor != value )
			{
				_bAttractor = value;

				dispatchEvent( new AttractorChangeEvent( _bAttractor ) );
			}
		}

		public function get bAttractor ():Boolean
		{
			return _bAttractor;
		}

		public function nextAttractor ():void
		{
			if( ++_attractorIndex >= _attractors.length )
				_attractorIndex = 0;
		}

		public function get attractorFlvPath ():String
		{
			return _attractors[ _attractorIndex ];
		}

		public function get attractorTimeoutSeconds ():int
		{
			return _attractorTimeoutSeconds;
		}

		/////////////////////////////////////
		//	RESET.
		/////////////////////////////////////

		public function reset ():void
		{
			_section	= -1;				// force section reset.
			section		= SECTION_INTRO;

			ModelLocator.getInstance().soundModel.stopAllSounds();

			dispatchEvent( new ResetEvent() );
		}

		/////////////////////////////////////
		//	PLAY INTRO PERIOD.
		/////////////////////////////////////

		public function set bPlayIntroPeriod ( value : Boolean ):void
		{
			_bPlayIntroPeriod = value;
		}

		public function get bPlayIntroPeriod ():Boolean
		{
			return _bPlayIntroPeriod;
		}
	}
}