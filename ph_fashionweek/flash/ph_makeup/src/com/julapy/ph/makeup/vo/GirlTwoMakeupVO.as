package com.julapy.ph.makeup.vo
{
	public class GirlTwoMakeupVO extends GirlMakeupVO
	{
		public function GirlTwoMakeupVO()
		{
			faceLinkage	=
			[
				"makeup.g2.layers.face.00",
				"makeup.g2.layers.face.01"
			];

			eyesLinkage	=
			[
				"makeup.g2.layers.eyes.00",
				"makeup.g2.layers.eyes.01"
			];

			lipsLinkage	=
			[
				"makeup.g2.layers.lips.00",
				"makeup.g2.layers.lips.01"
			];

			featureMasks =
			[
				"makeup.g2.mask.eyes",
				"makeup.g2.mask.lips",
				"makeup.g2.mask.face"
			];
		}

	}
}