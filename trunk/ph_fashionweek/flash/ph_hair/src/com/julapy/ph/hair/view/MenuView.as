package com.julapy.ph.hair.view
{
	import com.holler.core.View;
	import com.julapy.ph.hair.events.DropAreaEvent;
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
		private var toolCovers		: Array = new Array();
		private var toolIndex		: int = -1;
		private var toolSelected	: int = -1;
		private var toolProximity	: int = 100;

		private var toolPath		: MovieClip;
		private var toolPathCircle	: MovieClip;

		private var isRightTool		: Boolean = false;
		private var isInProximity	: Boolean = false;

		private var bEnabled		: Boolean = false;
		private var bVideoPlaying	: Boolean = false;

		public function MenuView(sprite:Sprite=null)
		{
			super(sprite);

			toolPath				= _sprite.getChildByName( "toolPath" ) as MovieClip;
			toolPathCircle			= toolPath.getChildByName( "circle" ) as MovieClip;
			toolPathCircle.iw		= toolPathCircle.width;
			toolPathCircle.ih		= toolPathCircle.height;
			toolPathCircle.visible	= false;
			toolPathCircle.alpha	= 0.2;

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

				d.addEventListener( DropAreaEvent.DROP_AREA_COMPLETE,	dropAreaCompleteHandler );
				d.addEventListener( DropAreaEvent.DROP_AREA_PLAYED_OUT, dropAreaPlayedOutHandler );
			}
		}

		private function initTools ():void
		{
			tools =
			[
				new MenuToolDryerView(  _sprite.getChildByName( "toolDryer" )  as MovieClip ),
				new MenuToolCurlerView( _sprite.getChildByName( "toolCurler" ) as MovieClip ),
				new MenuToolSprayTool(  _sprite.getChildByName( "toolSpray" )  as MovieClip )
			];

			for( var i:int=0; i<tools.length; i++ )
			{
				var t : MenuToolView;
				t = tools[ i ] as MenuToolView;

				t.addEventListener( MouseEvent.MOUSE_DOWN,	toolBtnHandler );
				t.addEventListener( MouseEvent.MOUSE_UP,	toolBtnHandler );
			}

			toolCovers =
			[
				new MenuToolCoverView( _sprite.getChildByName( "toolCover0" ) as MovieClip ),
				new MenuToolCoverView( _sprite.getChildByName( "toolCover1" ) as MovieClip ),
				new MenuToolCoverView( _sprite.getChildByName( "toolCover2" ) as MovieClip ),
			];
		}

		/////////////////////////////////////////////
		//	DROP AREAS.
		/////////////////////////////////////////////

		private function selectDropArea ( id : int ):void
		{
			if( !dropArea && id == -1 )		// all already off, nothing to turn off.
				return;

			if( dropArea )
			{
				dropArea = null;
			}

			for( var i:int=0; i<dropAreas.length; i++ )
			{
				var da : MenuDropareaView;
				da = dropAreas[ i ] as MenuDropareaView;

				var match : Boolean;
				match = ( i == id );

				if( match )
				{
					dropArea = da;
					dropArea.reset();
					dropArea.playIn( true );
				}

				da.show( match );
			}
		}

		/////////////////////////////////////////////
		//	TOOL COVERS
		/////////////////////////////////////////////

		private function selectToolCover ( j : int ):void
		{
			for( var i:int=0; i<toolCovers.length; i++ )
			{
				( toolCovers[ i ] as MenuToolCoverView ).show( ( i != j ) && ( j != -1 ) );
			}
		}

		/////////////////////////////////////////////
		//	LOGIC.
		/////////////////////////////////////////////

		private function startToolDrag ( t : MenuToolView ):void
		{
			if( toolSelected > -1 && toolSelected != toolIndex )
				return;

			tool = t;
			tool.rotateToDropArea();

			_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}

		private function stopToolDrag ( t : MenuToolView ):void
		{
			if( !t )
				return;

			_sprite.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

			t.scaleUp( false );
			t.returnToMenu();

			if( dropArea )		// if tool is deselects, reset dropArea.
			{
				dropArea.over( false );
				dropArea.showCross( false );
			}
		}

		private function enterFrameHandler ( e : Event ):void
		{
			if( bVideoPlaying )
			{
				drawToolPath();
				positionToolAlongPath();
			}
			else
			{
				isRightTool		= checkIsRightTool();
				isInProximity	= checkIsInProximity();

				updateToolPosition();
				updateToolScale();
				updateDropArea();
			}
		}

		/////////////////////////////////////////////
		//	PUT TOOL IN DROP AREA.
		/////////////////////////////////////////////

		private function checkIsRightTool ():Boolean
		{
			if( !dropArea )
				return false;

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

			isInProximity = ( p3.length < toolProximity );

			return isInProximity;
		}

		//--

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
			tool.scaleUp( isRightTool && isInProximity );
		}

		private function updateDropArea ():void
		{
			if( !dropArea )
				return;

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
		}

		/////////////////////////////////////////////
		//	USE TOOL WHILE VIDEO IS PLAYING.
		/////////////////////////////////////////////

		private function drawToolPath ():void
		{
			var t : MenuToolView;
			t = tools[ toolSelected ];

			toolPath.graphics.clear();
			toolPath.graphics.lineStyle( 1.0, 0xFFFFFF );

			var bLineCircle : Boolean;
			bLineCircle = false;

			if( bLineCircle )
			{
				var steps : int;
				steps = 360;

				var ang : int;
				ang = (int)( steps * 1.0 );

				for( var i:int=0; i<ang+1; i++ )		// starts at top center and moves around clock wise.
				{
					var p : Point;
					p	= new Point();
					p.x =  Math.sin( ( i / steps ) * 2 * Math.PI ) * t.toolPathRadiusX + t.toolPathCenter.x;
					p.y = -Math.cos( ( i / steps ) * 2 * Math.PI ) * t.toolPathRadiusY + t.toolPathCenter.y;

					if( i == 0 )
					{
						toolPath.graphics.moveTo( p.x, p.y );
					}
					else
					{
						toolPath.graphics.lineTo( p.x, p.y );
					}
				}
			}
			else
			{
//				toolPath.graphics.drawEllipse
//				(
//					t.toolPathCenter.x - t.toolPathRadiusX,
//					t.toolPathCenter.y - t.toolPathRadiusY,
//					t.toolPathRadiusX * 2,
//					t.toolPathRadiusY * 2
//				);

				toolPathCircle.visible	= true;
				toolPathCircle.x		= t.toolPathCenter.x;
				toolPathCircle.y		= t.toolPathCenter.y;
				toolPathCircle.width	= t.toolPathRadiusX * 2;
				toolPathCircle.height	= t.toolPathRadiusY * 2;
			}
		}

		private function clearToolPath ():void
		{
			toolPath.graphics.clear();
			toolPathCircle.visible = false;
		}

		private function positionToolAlongPath ():void
		{
			var area : Rectangle;
			area = ModelLocator.getInstance().hairModel.videoIntRect;

			var p1 : Point;
			p1 = new Point( area.width * 0.5, area.height * 0.5 );

			var p2 : Point;
			p2 = new Point();

			var tx : int;
			var ty : int;

			if( ModelLocator.getInstance().ofDataModel.connected )
			{
				var trackerVO : TrackerVO;
				trackerVO = ModelLocator.getInstance().ofDataModel.primaryTrackerVO;

				tx = trackerVO.rect.x * area.width;
				ty = trackerVO.rect.y * area.height;
			}
			else
			{
				tx = _sprite.mouseX;
				ty = _sprite.mouseY;
			}

			p2.x = tx;
			p2.y = ty;

			var p3 : Point;
			p3 = p1.subtract( p2 );

			var a : Number;
			a = Math.atan2( p3.x, p3.y );

			( tools[ toolSelected ] as MenuToolView ).angle = a;
		}

		private function restoreSelectedTool ():void
		{
			if( toolSelected == -1 )
				return;

			var t : MenuToolView;
			t = tools[ toolSelected ];

//			t.rotateBackToNormal();
			t.returnToMenu();
		}

		private function scaleToolForInteractiveVideo ():void
		{
			if( toolSelected == -1 )
				return;

			var t : MenuToolView;
			t = tools[ toolSelected ];

			t.scaleUp( false ); // scaling down here, but could be any scale.
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

				stylePartChangeHandler();
			}
			else
			{
				disable();

				stopToolDrag( tools[ toolIndex ] );
				restoreSelectedTool();
				toolSelected = -1;

				selectDropArea( -1 );
			}
		}

		private function stylePartChangeHandler ( e : StylePartEvent = null ):void
		{
			if( !bEnabled )
				return;

			bVideoPlaying = false;

			var stylePart : int;
			stylePart = ModelLocator.getInstance().hairModel.stylePart;

			stopToolDrag( tools[ toolIndex ] );			// return tool back to menu after interactive video has finished.
			restoreSelectedTool();
			toolSelected = -1;

			selectDropArea( stylePart );
			selectToolCover( -1 );
			clearToolPath();
		}

		private function toolChangeHandler ( e : ToolEvent ):void		// this happens when OF tool has changed.
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
		//	DROP AREA HANDLERS.
		/////////////////////////////////////////////

		private function dropAreaCompleteHandler ( e : DropAreaEvent ):void
		{
			toolSelected = toolIndex;
		}

		private function dropAreaPlayedOutHandler ( e : DropAreaEvent ):void
		{
			bVideoPlaying = true;

			ModelLocator.getInstance().hairModel.menuSelection = toolSelected;

			selectDropArea( -1 );
			selectToolCover( toolIndex );
			scaleToolForInteractiveVideo();
		}

		/////////////////////////////////////////////
		//	TOOL MOUSE CLICK HANDLERS.
		/////////////////////////////////////////////

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