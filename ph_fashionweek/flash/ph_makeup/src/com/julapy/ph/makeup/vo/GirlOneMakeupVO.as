package com.julapy.ph.makeup.vo
{
	public class GirlOneMakeupVO extends GirlMakeupVO
	{
		public function GirlOneMakeupVO()
		{
			name = "GirlOneMakeupVO";

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

			introVideos =								// names for videos were mixed up, so these videos are for model one.
			[
				"flv/makeupmodel02look01intro.f4v",
				"flv/makeupmodel02look02intro.f4v"
			];

			outroVideos =
			[
				"flv/makeupmodel02look01outro.f4v",
				"flv/makeupmodel02look02outro.f4v"
			];
		}

	}
}