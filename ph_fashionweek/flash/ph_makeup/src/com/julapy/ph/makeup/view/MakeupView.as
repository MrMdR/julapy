package com.julapy.ph.makeup.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.core.View;

	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class MakeupView extends View
	{
		private var faceLinkage : Array;
		private var eyesLinkage	: Array;
		private var lipsLinkage	: Array;

		private var faceIndex	: int = -1;
		private var eyesIndex	: int = -1;
		private var lipsIndex	: int = -1;

		private var faceHolder	: Sprite = new Sprite();
		private var eyesHolder	: Sprite = new Sprite();
		private var lipsHolder	: Sprite = new Sprite();

		private var faceAsset	: MovieClip;
		private var eyesAsset	: MovieClip;
		private var lipsAsset	: MovieClip;

		public function MakeupView(sprite:Sprite=null)
		{
			super(sprite);

			_sprite.addChild( faceHolder );
			_sprite.addChild( eyesHolder );
			_sprite.addChild( lipsHolder );

			faceLinkage	=
			[
				"makeup.layers.face.00",
				"makeup.layers.face.01"
			];

			eyesLinkage	=
			[
				"makeup.layers.eyes.00",
				"makeup.layers.eyes.01"
			];

			lipsLinkage	=
			[
				"makeup.layers.lips.00",
				"makeup.layers.lips.01"
			];
		}

		public function stepFace ():void
		{
			if( faceAsset )
			{
				faceHolder.removeChild( faceAsset );
				faceAsset	= null;
			}

			if( ++faceIndex >= faceLinkage.length )
				faceIndex = 0;

			faceAsset	= AssetLoader.getInstance().getClassInstance( faceLinkage[ faceIndex ] ) as MovieClip;
			faceHolder.addChild( faceAsset );
		}

		public function stepEyes ():void
		{
			if( eyesAsset )
			{
				eyesHolder.removeChild( eyesAsset );
				eyesAsset	= null;
			}

			if( ++eyesIndex >= eyesLinkage.length )
				eyesIndex = 0;

			eyesAsset	= AssetLoader.getInstance().getClassInstance( eyesLinkage[ eyesIndex ] ) as MovieClip;
			eyesHolder.addChild( eyesAsset );
		}

		public function stepLips ():void
		{
			if( lipsAsset )
			{
				lipsHolder.removeChild( lipsAsset );
				lipsAsset	= null;
			}

			if( ++lipsIndex >= lipsLinkage.length )
				lipsIndex = 0;

			lipsAsset	= AssetLoader.getInstance().getClassInstance( lipsLinkage[ lipsIndex ] ) as MovieClip;
			lipsHolder.addChild( lipsAsset );
		}

	}
}