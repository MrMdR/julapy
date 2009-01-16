package com.holler.twoframe.view.stereo
{
	import com.holler.core.View;
	import com.holler.twoframe.vo.ImageVO;
	
	import flash.display.Sprite;
	import flash.events.Event;

	public class StereoView extends View
	{
		public static const ITEM_PAD	: int = 1;
		
		private var itemVOs				: Array = new Array();
		private var stereoItemViews		: Array = new Array();
		private var stereoItemLoadIndex	: int = 0;
		private var stereoItemX			: int = ITEM_PAD;
		private var stereoItemY			: int = ITEM_PAD;
//		private var stereoItemScale		: Number = 0.8325;
		private var stereoItemScale		: Number = 0.25;		
		
		public function StereoView( sprite:Sprite, itemVOs:Array )
		{
			super(sprite);
			
			this.itemVOs = itemVOs;
			
			addStereoItem();
		}
		
		public function destroy ():void
		{
			
		}
		
		//////////////////////////////////////////////////////
		//	STEREO VIEW
		//////////////////////////////////////////////////////

		public function playIn ():void
		{
			
		}
		
		public function playOut ():void
		{
			
		}

		//////////////////////////////////////////////////////
		//	STEREO ITEMS.
		//////////////////////////////////////////////////////
		
		private function addStereoItem ( e:*=null ):void
		{
			var itemVO			: ImageVO;
			var stereoItemView	: StereoItemView;
			
			if( e is Event )
			{
				if( e.type == Event.INIT )
				{
					stereoItemView = e.target as StereoItemView;
					stereoItemView.removeEventListener( Event.INIT, addStereoItem );
					stereoItemView.x		= stereoItemX;
					stereoItemView.y		= stereoItemY;
					stereoItemView.scaleX	= stereoItemScale;
					stereoItemView.scaleY	= stereoItemScale;
					stereoItemView.visible	= true;
					
					stereoItemX			+= (int)( StereoItemView.ITEM_WIDTH * stereoItemScale ) + ITEM_PAD;
					if( stereoItemX > _sprite.stage.stageWidth - (int)( StereoItemView.ITEM_WIDTH * stereoItemScale ) )
					{
						stereoItemY		+= (int)( StereoItemView.ITEM_HEIGHT * stereoItemScale ) + ITEM_PAD;
						stereoItemX		= ITEM_PAD;
					}
					
					stereoItemView = null;
				}
			}
			
			if( stereoItemLoadIndex < itemVOs.length )
			{
				itemVO			= itemVOs[ stereoItemLoadIndex ];
				stereoItemView	= new StereoItemView( new Sprite(), itemVO );
				stereoItemView.addEventListener( Event.INIT, addStereoItem );
				stereoItemView.visible = false;
				
				addChildView( stereoItemView );
			}
			
			++stereoItemLoadIndex;
		}

	}
}