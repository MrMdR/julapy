package com.julapy.ph.makeup.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.core.View;
	import com.julapy.ph.makeup.events.BlinkEvent;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;

	public class MakeupView extends View
	{
		private var faceLinkage 	: Array;
		private var eyesLinkage		: Array;
		private var lipsLinkage		: Array;

		private var faceIndex		: int = -1;
		private var eyesIndex		: int = -1;
		private var lipsIndex		: int = -1;

		private var faceHolder		: Sprite = new Sprite();
		private var eyesHolder		: Sprite = new Sprite();
		private var lipsHolder		: Sprite = new Sprite();

		private var faceAsset		: MovieClip;
		private var eyesAsset		: MovieClip;
		private var lipsAsset		: MovieClip;

		private var faceMaskView	: MakeupMaskView;
		private var eyesMaskView	: MakeupMaskView;
		private var lipsMaskView	: MakeupMaskView;

		private var blinkMask		: MovieClip;

		private var featureMasks	: Array = new Array();
		private var featureIndex	: int = -1;

		public function MakeupView(sprite:Sprite=null)
		{
			super(sprite);

			_sprite.addChild( faceHolder );
			_sprite.addChild( eyesHolder );
			_sprite.addChild( lipsHolder );

			eyesHolder.cacheAsBitmap	= true;		// blending modes are lost if turned on.
//			lipsHolder.cacheAsBitmap	= true;		// blending modes are lost if turned on.
//			faceHolder.cacheAsBitmap	= true;		// blending modes are lost if turned on.

			blinkMask = AssetLoader.getInstance().getClassInstance( "makeup.face.blink.mask" ) as MovieClip;
			blinkMask.cacheAsBitmap		= true;
			_sprite.addChild( blinkMask );

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

			//-- masks.

			featureMasks =
			[
				"makeup.mask.eyes",
				"makeup.mask.lips",
				"makeup.mask.face"
			];

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ANIM_IN,	modeEvent );
			ModelLocator.getInstance().makeupModel.addEventListener( BlinkEvent.BLINK_START,	blinkHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( BlinkEvent.BLINK_STOP,		blinkHandler );

			hideBlink();
		}

		private function stepFace ():void
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

			//-- mask.

			if( faceMaskView )
			{
				faceHolder.mask	= null;

				faceMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				faceMaskView.container = null;
				faceMaskView = null;
			}

			var faceMask : MovieClip;
			faceMask		= AssetLoader.getInstance().getClassInstance( "makeup.mask.face" );
			faceHolder.addChild( faceMask );

			faceMaskView	= new MakeupMaskView( faceMask );
			faceMaskView.addEventListener( Event.COMPLETE, maskCompleteHandler );
			faceMaskView.play();

			faceHolder.mask	= faceMaskView.mc;
		}

		private function stepEyes ():void
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

			//-- blink force.

			ModelLocator.getInstance().makeupModel.blinkForce = true;

			//-- mask.

			if( eyesMaskView )
			{
				eyesHolder.mask	= null;

				eyesMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				eyesMaskView.container = null;
				eyesMaskView = null;
			}

			var eyesMask : MovieClip;
			eyesMask		= AssetLoader.getInstance().getClassInstance( "makeup.mask.eyes" );
			eyesHolder.addChild( eyesMask );

			eyesMaskView	= new MakeupMaskView( eyesMask );
			eyesMaskView.addEventListener( Event.COMPLETE, maskCompleteHandler );
			eyesMaskView.play();

			eyesHolder.mask	= eyesMaskView.mc;
		}

		private function stepLips ():void
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

			//-- mask.

			if( lipsMaskView )
			{
				lipsHolder.mask	= null;

				lipsMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				lipsMaskView.container = null;
				lipsMaskView = null;
			}

			var lipsMask : MovieClip;
			lipsMask		= AssetLoader.getInstance().getClassInstance( "makeup.mask.lips" );
			lipsHolder.addChild( lipsMask );

			lipsMaskView	= new MakeupMaskView( lipsMask );
			lipsMaskView.addEventListener( Event.COMPLETE, maskCompleteHandler );
			lipsMaskView.play();

			lipsHolder.mask	= lipsMaskView.mc;
		}

		//////////////////////////////////////////////////////////
		//	BLINK.
		//////////////////////////////////////////////////////////

		private function showBlink ():void
		{
			blinkMask.visible	= false;
			eyesHolder.mask		= null;
		}

		private function hideBlink ():void
		{
			blinkMask.visible	= true;
			eyesHolder.mask		= blinkMask;
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function modeEvent ( e : ModeEvent ):void
		{
			featureIndex = e.mode;

			if( e.mode == MakeupModel.EYES_MODE )
			{
				stepEyes();
			}

			if( e.mode == MakeupModel.LIPS_MODE )
			{
				stepLips();
			}

			if( e.mode == MakeupModel.FACE_MODE )
			{
				stepFace();
			}
		}

		private function blinkHandler ( e : BlinkEvent ):void
		{
			if( e.blinking )
			{
				showBlink();
			}
			else
			{
				hideBlink();
			}
		}

		private function maskCompleteHandler ( e : Event ):void
		{
			if( e.target == eyesMaskView )
			{
				eyesHolder.mask	= null;

				eyesMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				eyesMaskView.container = null;
				eyesMaskView = null;

				ModelLocator.getInstance().makeupModel.blinkForce = false;
			}

			if( e.target == lipsMaskView )
			{
				lipsHolder.mask	= null;

				lipsMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				lipsMaskView.container = null;
				lipsMaskView = null;
			}

			if( e.target == faceMaskView )
			{
				faceHolder.mask	= null;

				faceMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				faceMaskView.container = null;
				faceMaskView = null;
			}

			ModelLocator.getInstance().makeupModel.mode = -1;
		}
	}
}