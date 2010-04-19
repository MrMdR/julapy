package com.julapy.ph.makeup.model
{
	import com.julapy.ph.model.OFDataModel;
	import com.julapy.ph.vo.TrackerVO;

	import flash.events.TimerEvent;
	import flash.utils.Timer;

	public class OFDataMakeupModel extends OFDataModel
	{
		private var trackerBufferTime	: int = 300;
		private var trackerBufferTimer	: Timer;
		private var bWaitForBufferTimer	: Boolean = false;

		public function OFDataMakeupModel()
		{
			//
		}

		protected override function findPrimaryTrackerVO ():void
		{
			var trackerVO			: TrackerVO;
			var biggestTrackerVO	: TrackerVO;
			var highestTrackerVO	: TrackerVO;

			var biggestIndex		: int = -1;
			var highestIndex		: int = -1;

			var i : int;

			//-- biggest.

			var a : Number;
			a = 0;

			for( i=0; i<_ofTrackerVOs.length; i++ )
			{
				trackerVO = _ofTrackerVOs[ i ] as TrackerVO;

				if( !trackerVO.active )
					continue;

				var ta : Number;
				ta = trackerVO.rect.width * trackerVO.rect.height;

				if( ta > a )
				{
					a = ta;
					biggestIndex		= i;
					biggestTrackerVO	= trackerVO;
				}
			}

			//-- highest

			var y : Number;
			y = 1;

			for( i=0; i<_ofTrackerVOs.length; i++ )
			{
				trackerVO = _ofTrackerVOs[ i ] as TrackerVO;

				if( !trackerVO.active )
					continue;

				var ty : Number;
				ty = trackerVO.rect.y;

				if( ty < y )
				{
					y = ty;
					highestIndex		= i;
					highestTrackerVO	= trackerVO;
				}
			}

			_ofPrimaryTrackerVO = highestTrackerVO;
			_ofPrimaryIndex		= highestIndex;

			//-- check for change in tracker object.

			if( _ofLastIndex == _ofPrimaryIndex )		// no change.
				return;

			_ofLastIndex = _ofPrimaryIndex;

			//-- tracker object has changed, please continue...

			if( !checkBufferTimer() )
				return;

//			if( !checkMinTimer() )
//				return;

			//-- new tool, yey!

			if( _ofPrimaryIndex >= 0 )						// last valid tool selected.
				_ofLastValidIndex = _ofPrimaryIndex;

			ModelLocator.getInstance().makeupModel.mode = _ofPrimaryIndex;
		}

		private function checkBufferTimer ():Boolean
		{
			if( !bWaitForBufferTimer )
			{
				if( _ofPrimaryIndex == -1 )			// tool deselected, turn on buffer.
				{
					initTrackerBufferTimer();

					return false;
				}
			}

			if( bWaitForBufferTimer )
			{
				if( _ofPrimaryIndex == _ofLastValidIndex )
				{
					killTrackerBufferTimer();

					return false;
				}

				if( _ofPrimaryIndex != _ofLastValidIndex )
				{
					killTrackerBufferTimer();

					return true;
				}
			}

			return true;
		}

		/////////////////////////////////////////////////////
		//	TRACKER BUFFER TIMER.
		/////////////////////////////////////////////////////

		private function initTrackerBufferTimer ():void
		{
			if( trackerBufferTimer )
			{
				killTrackerBufferTimer();
			}

			bWaitForBufferTimer = true;

			trackerBufferTimer = new Timer( trackerBufferTime, 1 );
			trackerBufferTimer.addEventListener( TimerEvent.TIMER_COMPLETE, trackerBufferTimerHandler );
			trackerBufferTimer.start();
		}

		private function killTrackerBufferTimer ():void
		{
			bWaitForBufferTimer = false;

			if( trackerBufferTimer )
			{
				trackerBufferTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, trackerBufferTimerHandler );
				trackerBufferTimer.stop();
				trackerBufferTimer = null;
			}
		}

		private function trackerBufferTimerHandler ( e : TimerEvent ):void
		{
			killTrackerBufferTimer();

			ModelLocator.getInstance().makeupModel.mode = -1;
		}
	}
}