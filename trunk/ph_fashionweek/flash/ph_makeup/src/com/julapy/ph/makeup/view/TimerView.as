package com.julapy.ph.makeup.view
{
	import com.holler.core.View;
	import com.holler.utils.TimeFormat;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;
	import com.julapy.ph.makeup.model.SoundModel;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.utils.Timer;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;

	public class TimerView extends View
	{
		private var bEnabled		: Boolean;

		private var timer 			: Timer;
		private var timerMc			: MovieClip;
		private var timerText		: TextField;
		private var timerSeconds	: int = 60;
		private var timerCount		: int = 0;

		private var finalLookTimer	: Timer;
		private var idleTimer		: Timer;
		private var introPeriodIntv	: int;

		public function TimerView(sprite:Sprite=null)
		{
			super(sprite);

			timerMc		= _sprite.getChildByName( "timerMc" ) as MovieClip;
			timerText	= timerMc.getChildByName( "timerText" ) as TextField;

			timerMc.visible = false;		// hide timer.

			setTimerCopy( timerSeconds );

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,		modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,		modeZoomOutHandler );
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ( b : Boolean ):void
		{
			if( bEnabled == b )
				return;

			bEnabled	= b;

			visible		= bEnabled;
			doValidate();
		}

		///////////////////////////////////////////////
		//	TIMER.
		///////////////////////////////////////////////

		private function initTimer ():void
		{
			if( timer )
				killTimer();

			timer = new Timer( 1000, timerSeconds );
			timer.addEventListener( TimerEvent.TIMER,			timerHandler );
			timer.addEventListener( TimerEvent.TIMER_COMPLETE,	timerHandler );
			timer.start();
		}

		private function killTimer ():void
		{
			if( timer )
			{
				timer.removeEventListener( TimerEvent.TIMER,			timerHandler );
				timer.removeEventListener( TimerEvent.TIMER_COMPLETE,	timerHandler );
				timer.stop();
				timer = null;

				timerCount = 0;
				setTimerCopy( timerSeconds );
			}
		}

		private function timerHandler ( e : TimerEvent ):void
		{
			if( e.type == TimerEvent.TIMER )
			{
				++timerCount;

				var t : int;
				t = timerSeconds - timerCount;

				setTimerCopy( t );
			}

			if( e.type == TimerEvent.TIMER_COMPLETE )
			{
				killTimer();

				ModelLocator.getInstance().makeupModel.nextSection();
			}
		}

		///////////////////////////////////////////////
		//	INTRO TIMER.
		///////////////////////////////////////////////

		private function initIntroTimer ():void
		{
			ModelLocator.getInstance().makeupModel.bPlayIntroPeriod = true;

			killIntroTimer();

			introPeriodIntv = setInterval( introPeriodCompleteHandler, 1000 );
		}

		private function killIntroTimer ():void
		{
			clearInterval( introPeriodIntv );
		}

		private function introPeriodCompleteHandler ():void
		{
			killIntroTimer();

			ModelLocator.getInstance().makeupModel.bPlayIntroPeriod = false;
		}

		///////////////////////////////////////////////
		//	TIME COPY.
		///////////////////////////////////////////////

		private function setTimerCopy ( t : int ):void
		{
			var ta : Array;
			ta = TimeFormat.formatFromSeconds( t, TimeFormat.HH_MM_SS );
			timerText.text	=	TimeFormat.intToString( ta[ 0 ], 2 ) + ":" +
								TimeFormat.intToString( ta[ 1 ], 2 ) + ":" +
								TimeFormat.intToString( ta[ 2 ], 2 );
		}

		///////////////////////////////////////////////
		//	FINAL LOOK TIMER.
		///////////////////////////////////////////////

		private function initFinalLookTimer ():void
		{
			if( finalLookTimer )
				killFinalLookTimer();

			var time : int;
			time = (int)( ( timerSeconds - 7.7 ) * 1000 );

			finalLookTimer = new Timer( time, 1 );
			finalLookTimer.addEventListener( TimerEvent.TIMER_COMPLETE, finalLookTimerHandler );
			finalLookTimer.start();
		}

		private function killFinalLookTimer ():void
		{
			if( finalLookTimer )
			{
				finalLookTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, finalLookTimerHandler );
				finalLookTimer.stop();
				finalLookTimer = null;
			}
		}

		private function finalLookTimerHandler ( e : TimerEvent ):void
		{
			killFinalLookTimer();

			ModelLocator.getInstance().soundModel.playFinalLooks();
		}

		///////////////////////////////////////////////////
		//	IDLE TIMER.
		///////////////////////////////////////////////////

		private function initIdleTimer ():void
		{
			if( idleTimer )
				killIdleTimer();

			var time : int;
			time = (int)( Math.random() * 4000 ) + 8000;

			idleTimer = new Timer( time, 1 );
			idleTimer.addEventListener( TimerEvent.TIMER_COMPLETE, idleTimerHandler );
			idleTimer.start();
		}

		private function killIdleTimer ():void
		{
			if( idleTimer )
			{
				idleTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, idleTimerHandler );
				idleTimer.stop();
				idleTimer = null;
			}
		}

		private function idleTimerHandler ( e : TimerEvent ):void
		{
			initIdleTimer();

			if( timerCount > timerSeconds - 12 )		// don't play over final countdown.
				return;

			ModelLocator.getInstance().soundModel.playNextIdle();
		}

		///////////////////////////////////////////////
		//	HANDLER.
		///////////////////////////////////////////////

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var section	: int;
			section = ModelLocator.getInstance().makeupModel.section;

			if( section == MakeupModel.SECTION_PLAY )
			{
				enable( true );

				initTimer();
				initIntroTimer();
				initIdleTimer();
				initFinalLookTimer();

				ModelLocator.getInstance().soundModel.playBackground();
			}

			if( section == MakeupModel.SECTION_INTRO || section == MakeupModel.SECTION_OUTRO )
			{
				enable( false );

				killTimer();
				killIntroTimer();
				killIdleTimer();
				killFinalLookTimer();

				ModelLocator.getInstance().soundModel.stopAllSounds();
			}
		}

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
			if( !bEnabled )
				return;

			killIdleTimer();
		}

		private function modeZoomOutHandler ( e : ModeEvent ):void
		{
			if( !bEnabled )
				return;

			initIdleTimer();
		}
	}
}