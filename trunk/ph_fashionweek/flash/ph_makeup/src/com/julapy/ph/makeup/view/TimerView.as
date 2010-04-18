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

	public class TimerView extends View
	{
		private var timer 			: Timer;
		private var timerMc			: MovieClip;
		private var timerText		: TextField;
		private var timerSeconds	: int = 60;
		private var timerCount		: int = 0;

		private var finalLookTimer	: Timer;
		private var idleTimer		: Timer;

		public function TimerView(sprite:Sprite=null)
		{
			super(sprite);

			timerMc		= _sprite.getChildByName( "timerMc" ) as MovieClip;
			timerText	= timerMc.getChildByName( "timerText" ) as TextField;

			setTimerCopy( timerSeconds );

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,		modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,		modeZoomOutHandler );
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

			ModelLocator.getInstance().soundModel.play( SoundModel.FINAL_LOOKS_04 );
		}

		///////////////////////////////////////////////////
		//	IDLE TIMER.
		///////////////////////////////////////////////////

		private function initIdleTimer ():void
		{
			if( idleTimer )
				killIdleTimer();

			var time : int;
			time = (int)( Math.random() * 3000 ) + 6000;

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
				initTimer();
				initFinalLookTimer();
				initIdleTimer();
			}

			if( section == MakeupModel.SECTION_INTRO || section == MakeupModel.SECTION_OUTRO )
			{
				killTimer();
				killIdleTimer();
			}
		}

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
			killIdleTimer();
		}

		private function modeZoomOutHandler ( e : ModeEvent ):void
		{
			initIdleTimer();
		}
	}
}