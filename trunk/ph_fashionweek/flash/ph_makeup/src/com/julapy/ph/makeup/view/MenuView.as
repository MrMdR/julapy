package com.julapy.ph.makeup.view
{
	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class MenuView extends View
	{
		private var btn0		: BtnView01;
		private var btn1		: BtnView01;
		private var btn2		: BtnView01;

		private var btns		: Array = new Array();
		private var btnSelected	: BtnView01;

		public function MenuView(sprite:Sprite=null)
		{
			super(sprite);

			btn0		= new BtnView01( _sprite.getChildByName( "btn0" ) as MovieClip );
			btn0.addEventListener( MouseEvent.MOUSE_DOWN, btnHandler );

			btn1		= new BtnView01( _sprite.getChildByName( "btn1" ) as MovieClip );
			btn1.addEventListener( MouseEvent.MOUSE_DOWN, btnHandler );

			btn2		= new BtnView01( _sprite.getChildByName( "btn2" ) as MovieClip );
			btn2.addEventListener( MouseEvent.MOUSE_DOWN, btnHandler );

			btns = [ btn0, btn1, btn2 ];

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,	modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,	modeZoomOutHandler );
		}

		private function btnHandler ( e : MouseEvent ):void
		{
			if( e.target == btn0 )
			{
				ModelLocator.getInstance().makeupModel.mode = MakeupModel.EYES_MODE;
			}

			if( e.target == btn1 )
			{
				ModelLocator.getInstance().makeupModel.mode = MakeupModel.LIPS_MODE;
			}

			if( e.target == btn2 )
			{
				ModelLocator.getInstance().makeupModel.mode = MakeupModel.FACE_MODE;
			}
		}

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
			var btn : BtnView01;
			btn = btns[ e.mode ];

			if( btnSelected != btn )
			{
				if( btnSelected )
				{
					btnSelected.out();
					btnSelected = null;
				}

				btnSelected = btn;
				btnSelected.over();
			}
		}

		private function modeZoomOutHandler ( e : ModeEvent ):void
		{
			if( btnSelected )
			{
				btnSelected.out();
				btnSelected = null;
			}
		}
	}
}