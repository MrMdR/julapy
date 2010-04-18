package com.julapy.ph.makeup.vo
{
	public class GirlTwoMakeupVO extends GirlMakeupVO
	{
		public function GirlTwoMakeupVO()
		{
			name = "GirlTwoMakeupVO";

			faceLinkage	=
			[
				"makeup.g2.layers.face.00",
				"makeup.g2.layers.face.01",
				"makeup.g2.layers.face.02"
			];

			eyesLinkage	=
			[
				"makeup.g2.layers.eyes.00",
				"makeup.g2.layers.eyes.01",
				"makeup.g2.layers.eyes.02"
			];

			lipsLinkage	=
			[
				"makeup.g2.layers.lips.00",
				"makeup.g2.layers.lips.01",
				"makeup.g2.layers.lips.02"
			];

			featureMasks =
			[
				"makeup.g2.mask.eyes",
				"makeup.g2.mask.lips",
				"makeup.g2.mask.face"
			];

			introVideos =									// names for videos were mixed up, so these videos are for model two.
			[
				"flv/makeupmodel01look01intro.f4v",
				"flv/makeupmodel01look02intro.f4v"
			];

			outroVideos =
			[
				"flv/makeupmodel01look01outro.f4v",
				"flv/makeupmodel01look02outro.f4v"
			];
		}

	}
}