package com.julapy.ph.hair.model
{
	import com.julapy.ph.model.OFDataModel;
	import com.julapy.ph.vo.TrackerVO;

	import flash.events.TimerEvent;
	import flash.utils.Timer;

	public class OFDataHairModel extends OFDataModel
	{
		private var trackerMinTimer	: Timer;
		private var bTrackerActive	: Boolean = true;

		public function OFDataHairModel()
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

			//--

			if( ModelLocator.getInstance().hairModel.tool == _ofPrimaryIndex )		// no change.
				return;

			if( !bTrackerActive )
			{
				if( _ofPrimaryIndex == -1 )		// if all tools are de-activeated, do not delay with timer.
				{
					ModelLocator.getInstance().hairModel.tool = -1;
				}

				return;
			}

			bTrackerActive = false;

			killTrackerMinTimer();
			initTrackerMinTimer();

			ModelLocator.getInstance().hairModel.tool = _ofPrimaryIndex;
		}

		private function initTrackerMinTimer ():void
		{
			trackerMinTimer = new Timer( 1000, 1 );
			trackerMinTimer.addEventListener( TimerEvent.TIMER_COMPLETE, trackerMinTimerHandler );
			trackerMinTimer.start();
		}

		private function killTrackerMinTimer ():void
		{
			if( trackerMinTimer )
			{
				trackerMinTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, trackerMinTimerHandler );
				trackerMinTimer.stop();
				trackerMinTimer = null;
			}
		}

		private function trackerMinTimerHandler ( e : TimerEvent ):void
		{
			killTrackerMinTimer();

			bTrackerActive = true;
		}
	}
}