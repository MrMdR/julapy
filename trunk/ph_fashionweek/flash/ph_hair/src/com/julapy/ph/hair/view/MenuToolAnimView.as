package com.julapy.ph.hair.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.core.View;
	import com.julapy.ph.hair.events.DropAreaEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.events.ToolPathEvent;
	import com.julapy.ph.hair.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Point;

	public class MenuToolAnimView extends View
	{
		private var bEnabled			: Boolean	= false;
		private var toolIndex			: int		= -1;
		private var toolAnimLinkages	: Array		= new Array();
		private var toolAnims			: Array		= new Array();
		private var toolPosition		: Point		= new Point();

		private var bAddOnce			: Boolean	= true;

		public function MenuToolAnimView(sprite:Sprite=null)
		{
			super(sprite);

			ModelLocator.getInstance().hairModel.addEventListener( StylePartEvent.STYLE_PART_CHANGE,		stylePartChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_PLAYED_OUT, 		dropAreaPlayedOutHandler );
			ModelLocator.getInstance().hairModel.addEventListener( ToolPathEvent.TOOL_PATH_POSITION_CHANGE, toolPathPositionChangeHandler );

			toolAnimLinkages =
			[
				"hair.menu.tool.anim.dryer",
				"hair.menu.tool.anim.curler",
				"hair.menu.tool.anim.spray"
			];
		}

		/////////////////////////////////////////////
		//	KILL.
		/////////////////////////////////////////////

		public function kill ():void
		{
			killAnims();
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			bEnabled	= true;
			visible		= true;
			doValidate();
		}

		private function disable ():void
		{
			bEnabled	= false;
			visible		= false;
			doValidate();
		}

		//////////////////////////////////////////////////////
		//	ANIM.
		//////////////////////////////////////////////////////

		private function initAnim ():void
		{

		}

		private function killAnims ():void
		{
			for( var i:int=0; i<toolAnims.length; i++ )
			{
				var toolAnim : MovieClip;
				toolAnim	= toolAnims[ i ];

				_sprite.removeChild( toolAnim );
			}

			toolAnims = new Array();
		}

		private function addAnim ():void
		{
			var toolAnim : MovieClip;
			toolAnim	= AssetLoader.getInstance().getClassInstance( toolAnimLinkages[ toolIndex ] ) as MovieClip;
			toolAnim.x	= toolPosition.x;
			toolAnim.y	= toolPosition.y;

			_sprite.addChild( toolAnim );

			toolAnims.push( toolAnim );
		}

		//////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////

		private function stylePartChangeHandler ( e : StylePartEvent = null ):void
		{
			disable();

			toolIndex = ModelLocator.getInstance().hairModel.stylePart;
		}

		private function dropAreaPlayedOutHandler ( e : DropAreaEvent ):void
		{
			enable();
		}

		private function toolPathPositionChangeHandler ( e : ToolPathEvent ):void
		{
			if( !bEnabled )
				return;

			toolPosition.x = e.p.x;
			toolPosition.y = e.p.y;
		}
	}
}