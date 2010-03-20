package com.julapy.ph.makeup.view
{
	import com.holler.controls.BtnView;
	import com.julapy.core.StageSize;
	import com.julapy.core.StageSizeEvent;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;

	import mx.core.SpriteAsset;

	public class ApplicationView
	{
		[Embed( source = "../_fla/assets.swf#makeup.base" )]
		var ImageBase : Class;

		private var asset		: Sprite;

		private var stageWidth	: int;
		private var stageHeight	: int;

		private var image		: SpriteAsset;
		private var imageScale	: Number = 0;

		private var container0	: Sprite;
		private var container1	: Sprite;

		private var btn0		: BtnView;
		private var btn1		: BtnView;
		private var btn2		: BtnView;

		private var makeup		: MakeupView;

		public function ApplicationView( asset : Sprite )
		{
			this.asset	= asset;

			container0	= asset.getChildByName( "container0" ) as MovieClip;
			container1	= asset.getChildByName( "container1" ) as MovieClip;

			btn0		= new BtnView( asset.getChildByName( "btn0" ) as MovieClip );
			btn0.addEventListener( MouseEvent.MOUSE_DOWN, btnHandler );

			btn1		= new BtnView( asset.getChildByName( "btn1" ) as MovieClip );
			btn1.addEventListener( MouseEvent.MOUSE_DOWN, btnHandler );

			btn2		= new BtnView( asset.getChildByName( "btn2" ) as MovieClip );
			btn2.addEventListener( MouseEvent.MOUSE_DOWN, btnHandler );

			image		= new ImageBase();
			container0.addChild( image );

			makeup		= new MakeupView( container1 );
			makeup.stepFace();

			stageResize( StageSize.getInstance().stageWidth, StageSize.getInstance().stageHeight );

			StageSize.getInstance().addEventListener( StageSizeEvent.RESIZE, stageResizeHandler );

			asset.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}

		////////////////////////////////////////////////////
		//	RESIZE.
		////////////////////////////////////////////////////

		private function stageResize ( stageWidth : int, stageHeight : int ):void
		{
			this.stageWidth		= stageWidth;
			this.stageHeight	= stageHeight;

			imageScale			= stageWidth / container0.width;

			container0.scaleX	= imageScale;
			container0.scaleY	= imageScale;
		}

		////////////////////////////////////////////////////
		//	HANDLERS.
		////////////////////////////////////////////////////

		private function stageResizeHandler ( e : StageSizeEvent ):void
		{
			stageResize( e.stageWidth, e.stageHeight );
		}

		private function enterFrameHandler ( e : Event ):void
		{
			var px : Number;
			var py : Number;

			px = ( asset.mouseX + stageWidth  * 0.5 ) / stageWidth;
			py = ( asset.mouseY + stageHeight * 0.5 ) / stageHeight;

			container0.scaleX	= ( 1 - imageScale ) * px + imageScale;
			container0.scaleY	= ( 1 - imageScale ) * px + imageScale;
		}

		private function btnHandler ( e : MouseEvent ):void
		{

		}
	}
}