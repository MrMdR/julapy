package com.julapy.ph.makeup.vo
{
	public class GirlOneMakeupVO extends GirlMakeupVO
	{
		public function GirlOneMakeupVO()
		{
			faceLinkage	=
			[
				"makeup.g1.layers.face.00",
				"makeup.g1.layers.face.01",
				"makeup.g1.layers.face.02"
			];

			eyesLinkage	=
			[
				"makeup.g1.layers.eyes.00",
				"makeup.g1.layers.eyes.01",
				"makeup.g1.layers.eyes.02"
			];

			lipsLinkage	=
			[
				"makeup.g1.layers.lips.00",
				"makeup.g1.layers.lips.01",
				"makeup.g1.layers.lips.02"
			];

			featureMasks =
			[
				"makeup.g1.mask.eyes",
				"makeup.g1.mask.lips",
				"makeup.g1.mask.face"
			];
		}

	}
}