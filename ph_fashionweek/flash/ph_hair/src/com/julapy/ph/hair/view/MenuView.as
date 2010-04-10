package com.julapy.ph.hair.view
{
	import com.holler.core.View;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.events.ToolEvent;
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.hair.vo.StyleVO;
	import com.julapy.ph.vo.TrackerVO;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	public class MenuView extends View
	{
		private var dropArea		: MenuDropareaView;
		private var dropAreas		: Array = new Array();

		private var tool			: MenuToolView;
		private var tools			: Array = new Array();
		private var toolIndex		: int = -1;
		private var toolProximity	: int = 100;

		public function MenuView(sprite:Sprite=null)
		{
			super(sprite);

			initDropAreas();
			initTools();

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,				girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,				styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,			sectionChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StylePartEvent.STYLE_PART_CHANGE,	stylePartChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( ToolEvent.TOOL_CHANGE,				toolChangeHandler );
		}

		private function initDropAreas ():void
		{
			dropAreas =
			[
				new MenuDropareaView( _sprite.getChildByName( "dropAreaDryer" ) as MovieClip ),
				new MenuDropareaView( _sprite.getChildByName( "dropAreaCurlers" ) as MovieClip ),
				new MenuDropareaView( _sprite.getChildByName( "dropAreaSpray" ) as MovieClip )
			];

			for( var i:int=0; i<dropAreas.length; i++ )
			{
				var d : MenuDropareaView;
				d = dropAreas[ i ] as MenuDropareaView;

				d.addEventListener( Event.COMPLETE, dropAreaHandler );
			}

			selectDropArea( -1 );
		}

		private function initTools ():void
		{
			tools =
			[
				new MenuToolView( _sprite.getChildByName( "toolDryer" ) as MovieClip ),
				new MenuToolView( _sprite.getChildByName( "toolCurler" ) as MovieClip ),
				new MenuToolView( _sprite.getChildByName( "toolSpray" ) as MovieClip )
			];

			for( var i:int=0; i<tools.length; i++ )
			{
				var t : MenuToolView;
				t = tools[ i ] as MenuToolView;

				t.addEventListener( MouseEvent.MOUSE_DOWN,	toolBtnHandler );
				t.addEventListener( MouseEvent.MOUSE_UP,	toolBtnHandler );
			}
		}

		/////////////////////////////////////////////
		//	DROP AREAS.
		/////////////////////////////////////////////

		private function selectDropArea ( id : int ):void
		{
			dropArea = null;

			for( var i:int=0; i<dropAreas.length; i++ )
			{
				if( i == id )
				{
					dropArea = dropAreas[ i ] as MenuDropareaView;
				}

				( dropAreas[ i ] as MenuDropareaView ).show( i == id );
			}
		}

		/////////////////////////////////////////////
		//	TOOLS.
		/////////////////////////////////////////////

		private function startToolDrag ( t : MenuToolView ):void
		{
			tool = t;

			_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}

		private function stopToolDrag ( t : MenuToolView ):void
		{
			_sprite.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

			t.returnToMenu();
		}

		private function enterFrameHandler ( e : Event ):void
		{
			updateToolPosition();
			updateToolScale();
		}

		private function updateToolPosition ():void
		{
			if( ModelLocator.getInstance().ofDataModel.connected )
			{
				var trackerVO : TrackerVO;
				trackerVO = ModelLocator.getInstance().ofDataModel.primaryTrackerVO;

				var area : Rectangle;
				area = ModelLocator.getInstance().hairModel.videoIntRect;

				var tx : int;
				var ty : int;

				tx = trackerVO.rect.x * area.width;
				ty = trackerVO.rect.y * area.height;

				var ease : Number;
				ease = 0.5;

				tool.x += ( tx - tool.x ) * ease;
				tool.y += ( ty - tool.y ) * ease;
			}
			else
			{
				tool.x = _sprite.mouseX;
				tool.y = _sprite.mouseY;
			}

			tool.doValidate();
		}

		private function updateToolScale ():void
		{
			if( !dropArea )
				return;

			var isRightTool		: Boolean;
			var isInProximity	: Boolean;

			isRightTool		= checkIsRightTool();
			isInProximity	= checkIsInProximity();

			if( isInProximity )
			{
				if( isRightTool )
				{
					dropArea.over( true );
					dropArea.showCross( false );
				}
				else
				{
					dropArea.showCross( true );
				}
			}
			else
			{
				dropArea.over( false );
				dropArea.showCross( false );
			}

			tool.scaleUp( isRightTool && isInProximity );
		}

		private function checkIsRightTool ():Boolean
		{
			if( !dropArea )
				return false;

			var isRightTool : Boolean;
			isRightTool = ( toolIndex == ModelLocator.getInstance().hairModel.stylePart );

			return isRightTool;
		}

		private function checkIsInProximity ():Boolean
		{
			if( !dropArea )
				return false;

			var t : MenuToolView;
			t = tools[ toolIndex ] as MenuToolView;

			var p1 : Point;
			p1 = new Point( dropArea.x, dropArea.y );

			var p2 : Point;
			p2 = new Point( t.x, t.y );

			var p3 : Point;
			p3 = p1.subtract( p2 );

			var isInProximity : Boolean;
			isInProximity = ( p3.length < toolProximity );

			return isInProximity;
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			visible = true;
			doValidate();
		}

		private function disable ():void
		{
			visible = false;
			doValidate();
		}

		/////////////////////////////////////////////
		//	MODEL HANDLERS.
		/////////////////////////////////////////////

		private function girlChangeHandler ( e : GirlEvent ):void
		{
			//
		}

		private function styleChangeHandler ( e : StyleEvent ):void
		{

		}

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var sec : int;
			sec		= ModelLocator.getInstance().hairModel.section;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			if( sec == HairModel.SECTION_PLAY )
			{
				enable();
			}
			else
			{
				disable();
			}
		}

		private function stylePartChangeHandler ( e : StylePartEvent ):void
		{
			var stylePart : int;
			stylePart = e.stylePart;

			selectDropArea( stylePart );
		}

		private function toolChangeHandler ( e : ToolEvent ):void
		{
			if( toolIndex >= 0 )
			{
				stopToolDrag( tools[ toolIndex ] );
			}

			toolIndex = e.tool;

			if( toolIndex >= 0 )
			{
				startToolDrag( tools[ toolIndex ] );
			}
		}

		/////////////////////////////////////////////
		//	LOCAL HANDLERS.
		/////////////////////////////////////////////

		private function dropAreaHandler ( e : Event ):void
		{
			selectDropArea( -1 );

			ModelLocator.getInstance().hairModel.menuSelection = toolIndex;
		}

		private function toolBtnHandler ( e : MouseEvent ):void
		{
			if( ModelLocator.getInstance().ofDataModel.connected )
				return;

			for( var i:int=0; i<tools.length; i++ )
			{
				if( tools[ i ] == e.target )
				{
					toolIndex = i;
					break;
				}
			}

			if( e.type == MouseEvent.MOUSE_DOWN )
			{
				startToolDrag( tools[ toolIndex ] as MenuToolView );
			}

			if( e.type == MouseEvent.MOUSE_UP )
			{
				stopToolDrag( tools[ toolIndex ] as MenuToolView );
			}
		}

	}
}