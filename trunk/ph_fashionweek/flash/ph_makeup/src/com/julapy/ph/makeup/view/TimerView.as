package com.julapy.ph.makeup.view
{
	import com.holler.core.View;
	import com.holler.utils.TimeFormat;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

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
		private var timerSeconds	: int = 10;
		private var timerCount		: int = 0;

		public function TimerView(sprite:Sprite=null)
		{
			super(sprite);

			timerMc		= _sprite.getChildByName( "timerMc" ) as MovieClip;
			timerText	= timerMc.getChildByName( "timerText" ) as TextField;

			setTimerCopy( timerSeconds );

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE, sectionChangeHandler );
		}

		///////////////////////////////////////////////
		//	TIMER.
		///////////////////////////////////////////////

		private function initTimer ():void
		{
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

		private function setTimerCopy ( t : int ):void
		{
			var ta : Array;
			ta = TimeFormat.formatFromSeconds( t, TimeFormat.HH_MM_SS );
			timerText.text	=	TimeFormat.intToString( ta[ 0 ], 2 ) + ":" +
								TimeFormat.intToString( ta[ 1 ], 2 ) + ":" +
								TimeFormat.intToString( ta[ 2 ], 2 );
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
			}

			if( section == MakeupModel.SECTION_INTRO || section == MakeupModel.SECTION_OUTRO )
			{
				killTimer();
			}
		}
	}
}