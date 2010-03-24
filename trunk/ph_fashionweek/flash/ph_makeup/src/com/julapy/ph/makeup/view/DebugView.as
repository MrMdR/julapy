package com.julapy.ph.makeup.view
{
	import com.holler.controls.BtnView;
	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ConnectedEvent;
	import com.julapy.ph.makeup.model.ModelLocator;
	import com.julapy.ph.makeup.vo.TrackerVO;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.StageDisplayState;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Rectangle;
	import flash.text.TextField;

	public class DebugView extends View
	{
		private var infoText	: TextField;
		private var tracker		: MovieClip;
		private var dcBtn		: BtnView;
		private var fsBtn		: BtnView;

		public function DebugView(sprite:Sprite=null)
		{
			super(sprite);

			infoText = _sprite.getChildByName( "infoText" ) as TextField;
			infoText.text = "";

			dcBtn		= new BtnView( _sprite.getChildByName( "dcBtn" ) as MovieClip );
			dcBtn.addEventListener( MouseEvent.MOUSE_DOWN, dcBtnHander );

			fsBtn		= new BtnView( _sprite.getChildByName( "fsBtn" ) as MovieClip );
			fsBtn.addEventListener( MouseEvent.MOUSE_DOWN, fsBtnHandler );

			tracker		= _sprite.getChildByName( "tracker" ) as MovieClip;

			_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );

			ModelLocator.getInstance().ofDataModel.addEventListener( ConnectedEvent.CONNECTED,		ofDataModelHandler );
			ModelLocator.getInstance().ofDataModel.addEventListener( ConnectedEvent.DISCONNECTED,	ofDataModelHandler );
		}

		private function drawTrackerData ():void
		{
			var trackerVOs : Array;
			trackerVOs = ModelLocator.getInstance().ofDataModel.trackerVOs;

			var colours : Array;
			colours = [ 0xFF0000, 0x00FF00, 0x0000FF ];

			var appWidth 	: int;
			var appHeight	: int;
			appWidth	= ModelLocator.getInstance().makeupModel.appWidth;;
			appHeight	= ModelLocator.getInstance().makeupModel.appHeight;

			var r : Rectangle;
			r = new Rectangle();
			r.x			= 0;
			r.y			= 0;
			r.width		= 320;
			r.height	= 240;

			tracker.graphics.clear();
			tracker.graphics.beginFill( 0x333333 );
			tracker.graphics.drawRect( r.x, r.y, r.width, r.height );
			tracker.graphics.endFill();

			for( var i:int=0; i<trackerVOs.length; i++ )
			{
				var j : int;
				j = Math.min( i, colours.length - 1 );

				var trackerVO : TrackerVO;
				trackerVO = trackerVOs[ i ];

				//-- rect.

				var rectName : String;
				rectName = "rect_" + i.toString();

				var rectMc : Sprite;
				rectMc = tracker.getChildByName( rectName ) as Sprite;

				if( !rectMc )
				{
					rectMc = new Sprite();
					rectMc.name = rectName;
					tracker.addChild( rectMc );
				}

				rectMc.graphics.clear();

				if( !trackerVO.active )
					continue;

				rectMc.x = trackerVO.rect.x * r.width;
				rectMc.y = trackerVO.rect.y * r.height;

				rectMc.rotation = trackerVO.angle * 360;

				rectMc.graphics.lineStyle( 2, colours[ j ], 1.0 );
				rectMc.graphics.drawRect
				(
					-trackerVO.rect.width  * 0.5 * r.width,
					-trackerVO.rect.height * 0.5 * r.height,
					 trackerVO.rect.width  * r.width,
					 trackerVO.rect.height * r.height
				);
			}
		}

		private function enterFrameHandler ( e : Event ):void
		{
			drawTrackerData();
		}

		private function ofDataModelHandler ( e : Event ):void
		{
			if( e.type == ConnectedEvent.CONNECTED )
			{
				infoText.appendText( "Connected!" + "\n" );
			}

			if( e.type == ConnectedEvent.DISCONNECTED )
			{
				infoText.appendText( "Disconnected." + "\n" );
			}
		}

		private function dcBtnHander ( e : MouseEvent ):void
		{
			ModelLocator.getInstance().ofDataModel.disconnect();
		}

		private function fsBtnHandler ( e : MouseEvent ):void
		{
			if( _sprite.stage.displayState != StageDisplayState.FULL_SCREEN_INTERACTIVE )
			{
				_sprite.stage.displayState = StageDisplayState.FULL_SCREEN_INTERACTIVE;
			}
			else
			{
				_sprite.stage.displayState = StageDisplayState.NORMAL;
			}
		}
	}
}