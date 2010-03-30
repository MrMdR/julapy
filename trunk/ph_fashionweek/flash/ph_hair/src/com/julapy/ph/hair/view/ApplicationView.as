package com.julapy.ph.hair.view
{
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;

	import flash.display.Sprite;

	public class ApplicationView
	{
		private var asset		: Sprite;

		private var model		: HairModel;

		public function ApplicationView( asset : Sprite )
		{
			this.asset	= asset;

			model		= ModelLocator.getInstance().hairModel;
		}

	}
}