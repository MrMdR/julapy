package com.julapy.ph.makeup.view
{
	import flash.display.Sprite;
	import com.holler.core.View;

	public class MakeupView extends View
	{
		[Embed( source = "../_fla/assets.swf#makeup.eyes.00" )]
		var Eyes00 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.eyes.01" )]
		var Eyes01 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.face.00" )]
		var Face00 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.face.01" )]
		var Face01 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.lips.00" )]
		var Lips00 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.lips.01" )]
		var Lips01 : Class;

		public function MakeupView(sprite:Sprite=null)
		{
			super(sprite);
		}

	}
}