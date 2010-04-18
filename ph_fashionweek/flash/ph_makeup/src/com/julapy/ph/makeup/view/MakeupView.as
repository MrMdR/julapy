package com.julapy.ph.makeup.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.core.View;
	import com.julapy.ph.makeup.events.BlinkEvent;
	import com.julapy.ph.makeup.events.GirlEvent;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;
	import com.julapy.ph.makeup.vo.GirlMakeupVO;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;

	public class MakeupView extends View
	{
		private var bEnabled		: Boolean;

		private var asset			: MovieClip;

		private var girlVO			: GirlMakeupVO;

		private var faceIndex		: int = -1;
		private var eyesIndex		: int = -1;
		private var lipsIndex		: int = -1;
		private var featureIndex	: int = -1;

		private var faceHolder		: MovieClip;
		private var eyesHolder		: MovieClip;
		private var lipsHolder		: MovieClip;

		private var faceAsset		: MovieClip;
		private var eyesAsset		: MovieClip;
		private var lipsAsset		: MovieClip;

		private var faceMaskView	: MakeupMaskView;
		private var eyesMaskView	: MakeupMaskView;
		private var lipsMaskView	: MakeupMaskView;

		public function MakeupView(sprite:Sprite=null)
		{
			super( null );

			setAsset( sprite as MovieClip );

			var girlID : int;
			girlID = ModelLocator.getInstance().makeupModel.girl;
			girlVO = ModelLocator.getInstance().makeupModel.girlVOs[ girlID ];

			ModelLocator.getInstance().makeupModel.addEventListener( GirlEvent.GIRL_CHANGE,			girlChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ANIM_IN,		modeEvent );
			ModelLocator.getInstance().makeupModel.addEventListener( BlinkEvent.BLINK_START,		blinkHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( BlinkEvent.BLINK_STOP,			blinkHandler );

			hideBlink();
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ( b : Boolean ):void
		{
			if( bEnabled == b )
				return;

			bEnabled	= b;

			visible		= bEnabled;
			doValidate();
		}

		/////////////////////////////////////////
		//	PUBLIC
		/////////////////////////////////////////

		public function setAsset ( asset : MovieClip ):void
		{
			this.asset	= asset;

			eyesHolder	= asset.getChildByName( "eyesHolder" ) as MovieClip;
			lipsHolder	= asset.getChildByName( "lipsHolder" ) as MovieClip;
			faceHolder	= asset.getChildByName( "faceHolder" ) as MovieClip;

			eyesHolder.cacheAsBitmap	= true;		// blending modes are lost if turned on.
			lipsHolder.cacheAsBitmap	= true;		// blending modes are lost if turned on.
			faceHolder.cacheAsBitmap	= true;		// blending modes are lost if turned on.
		}

		public function start ():void
		{
			//
		}

		public function stop ():void
		{
			faceIndex		= -1;
			eyesIndex		= -1;
			lipsIndex		= -1;
			featureIndex	= -1;

			removeFace();
			removeFaceMask();

			removeEyes();
			removeEyesMask();

			removeLips();
			removeLipsMask();
		}

		//////////////////////////////////////////////////////////
		//	FACE.
		//////////////////////////////////////////////////////////

		private function stepFace ():void
		{
			removeFace();
			incFace();
			addFace();

			removeFaceMask();
			addFaceMask();
		}

		//--

		private function removeFace ():void
		{
			if( faceAsset )
			{
				faceHolder.removeChild( faceAsset );
				faceAsset	= null;
			}
		}

		private function incFace ():void
		{
			if( ++faceIndex >= girlVO.faceLinkage.length )
				faceIndex = 0;
		}

		private function addFace ():void
		{
			faceAsset	= AssetLoader.getInstance().getClassInstance( girlVO.faceLinkage[ faceIndex ] ) as MovieClip;
			faceHolder.addChild( faceAsset );
		}

		private function removeFaceMask ():void
		{
			if( faceMaskView )
			{
				faceHolder.mask	= null;

				faceMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				faceMaskView.container = null;
				faceMaskView = null;
			}
		}

		private function addFaceMask ():void
		{
			var faceMask : MovieClip;
			faceMask		= AssetLoader.getInstance().getClassInstance( girlVO.featureMasks[ 2 ] );
			faceHolder.addChild( faceMask );

			faceMaskView	= new MakeupMaskView( faceMask );
			faceMaskView.addEventListener( Event.COMPLETE, maskCompleteHandler );
			faceMaskView.play();

			faceHolder.mask	= faceMaskView.mc;
		}

		//////////////////////////////////////////////////////////
		//	EYES.
		//////////////////////////////////////////////////////////

		private function stepEyes ():void
		{
			removeEyes();
			incEyes();
			addEyes();

			ModelLocator.getInstance().makeupModel.blinkForce = true;

			removeLipsMask();
			addEyesMask();
		}

		//--

		private function removeEyes ():void
		{
			if( eyesAsset )
			{
				eyesHolder.removeChild( eyesAsset );
				eyesAsset	= null;
			}
		}

		private function incEyes ():void
		{
			if( ++eyesIndex >= girlVO.eyesLinkage.length )
				eyesIndex = 0;
		}

		private function addEyes ():void
		{
			eyesAsset	= AssetLoader.getInstance().getClassInstance( girlVO.eyesLinkage[ eyesIndex ] ) as MovieClip;
			eyesHolder.addChild( eyesAsset );
		}

		private function removeEyesMask ():void
		{
			if( eyesMaskView )
			{
				eyesHolder.mask	= null;

				eyesMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				eyesMaskView.container = null;
				eyesMaskView = null;
			}
		}

		private function addEyesMask ():void
		{
			var eyesMask : MovieClip;
			eyesMask		= AssetLoader.getInstance().getClassInstance( girlVO.featureMasks[ 0 ] );
			eyesHolder.addChild( eyesMask );

			eyesMaskView	= new MakeupMaskView( eyesMask );
			eyesMaskView.addEventListener( Event.COMPLETE, maskCompleteHandler );
			eyesMaskView.play();

			eyesHolder.mask	= eyesMaskView.mc;
		}

		//////////////////////////////////////////////////////////
		//	LIPS.
		//////////////////////////////////////////////////////////

		private function stepLips ():void
		{
			removeLips();
			incLips();
			addLips();

			removeLipsMask();
			addLipsMask();
		}

		//--

		private function removeLips ():void
		{
			if( lipsAsset )
			{
				lipsHolder.removeChild( lipsAsset );
				lipsAsset	= null;
			}
		}

		private function incLips ():void
		{
			if( ++lipsIndex >= girlVO.lipsLinkage.length )
				lipsIndex = 0;
		}

		private function addLips ():void
		{
			lipsAsset	= AssetLoader.getInstance().getClassInstance( girlVO.lipsLinkage[ lipsIndex ] ) as MovieClip;
			lipsHolder.addChild( lipsAsset );
		}

		private function removeLipsMask ():void
		{
			if( lipsMaskView )
			{
				lipsHolder.mask	= null;

				lipsMaskView.removeEventListener( Event.COMPLETE, maskCompleteHandler );
				lipsMaskView.container = null;
				lipsMaskView = null;
			}
		}

		private function addLipsMask ():void
		{
			var lipsMask : MovieClip;
			lipsMask		= AssetLoader.getInstance().getClassInstance( girlVO.featureMasks[ 1 ] );
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
			if( eyesAsset )
				eyesAsset.gotoAndStop( 2 );
		}

		private function hideBlink ():void
		{
			if( eyesAsset )
				eyesAsset.gotoAndStop( 1 );
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function girlChangeHandler ( e : GirlEvent ):void
		{
			var girlID : int;
			girlID = ModelLocator.getInstance().makeupModel.girl;
			girlVO = ModelLocator.getInstance().makeupModel.girlVOs[ girlID ];
		}

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var section	: int;
			section = ModelLocator.getInstance().makeupModel.section;

			if( section == MakeupModel.SECTION_PLAY )
			{
				enable( true );
			}

			if( section == MakeupModel.SECTION_INTRO || section == MakeupModel.SECTION_OUTRO )
			{
				enable( false );
				stop();
			}
		}

		private function modeEvent ( e : ModeEvent ):void
		{
			if( !bEnabled )
				return;

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
			if( !bEnabled )
				return;

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
				removeEyesMask();

				ModelLocator.getInstance().makeupModel.blinkForce = false;
			}

			if( e.target == lipsMaskView )
			{
				removeLipsMask();
			}

			if( e.target == faceMaskView )
			{
				removeFaceMask();
			}

			ModelLocator.getInstance().makeupModel.mode = -1;
		}
	}
}