package com.julapy.ph.makeup.model
{
	import com.julapy.ph.model.OFDataModel;
	import com.julapy.ph.vo.TrackerVO;

	public class OFDataMakeupModel extends OFDataModel
	{
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

			ModelLocator.getInstance().makeupModel.mode = _ofPrimaryIndex;
		}
	}
}