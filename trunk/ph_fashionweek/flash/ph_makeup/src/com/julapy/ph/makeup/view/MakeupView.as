package com.julapy.ph.makeup.view
{
	import com.holler.core.View;

	import flash.display.MovieClip;
	import flash.display.Sprite;

	import mx.core.SpriteAsset;

	public class MakeupView extends View
	{
		[Embed( source = "../_fla/assets.swf#makeup.face.00" )]
		var Face00 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.face.01" )]
		var Face01 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.eyes.00" )]
		var Eyes00 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.eyes.01" )]
		var Eyes01 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.lips.00" )]
		var Lips00 : Class;

		[Embed( source = "../_fla/assets.swf#makeup.lips.01" )]
		var Lips01 : Class;

		private var faceClasses : Array;
		private var eyesClasses	: Array;
		private var lipsClasses	: Array;

		private var faceIndex	: int = -1;
		private var eyesIndex	: int = -1;
		private var lipsIndex	: int = -1;

		private var faceHolder	: Sprite = new Sprite();
		private var eyesHolder	: Sprite = new Sprite();
		private var lipsHolder	: Sprite = new Sprite();

		private var faceAsset	: SpriteAsset;
		private var eyesAsset	: SpriteAsset;
		private var lipsAsset	: SpriteAsset;

		private var faceMc		: MovieClip;
		private var eyesMc		: MovieClip;
		private var lipsMc		: MovieClip;

		public function MakeupView(sprite:Sprite=null)
		{
			super(sprite);

			_sprite.addChild( faceHolder );
			_sprite.addChild( eyesHolder );
			_sprite.addChild( lipsHolder );

			faceClasses	=
			[
				Face00,
				Face01
			];

			eyesClasses	=
			[
				Eyes00,
				Eyes01
			];

			lipsClasses	=
			[
				Lips00,
				Lips01
			];
		}

		public function stepFace ():void
		{
			if( faceAsset )
			{
				faceHolder.removeChild( faceAsset );
				faceAsset	= null;
				faceMc		= null;
			}

			if( ++faceIndex >= faceClasses.length )
				faceIndex = 0;

			faceAsset	= new faceClasses[ faceIndex ];
			faceHolder.addChild( faceAsset );
			faceMc		= faceAsset.getChildByName( "mc" ) as MovieClip;
		}

		public function stepEyes ():void
		{

		}

		public function stepLips ():void
		{

		}

	}
}