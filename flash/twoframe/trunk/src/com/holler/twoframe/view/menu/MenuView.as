package com.holler.twoframe.view.menu
{
	import com.holler.assets.AssetManager;
	import com.holler.containers.ScreenView;
	import com.holler.twoframe.model.ModelLocator;
	import com.holler.twoframe.vo.ImageSectionVO;
	import com.holler.twoframe.vo.ImageVO;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class MenuView extends ScreenView
	{
		private var items:Array = new Array();
		private var selectedItem:MenuViewButton;
		
		public function MenuView( sprite:Sprite )
		{
			super( sprite );
			
			_sprite.x	= 20;
			_sprite.y	= 20;
			
			var sectionVO:ImageSectionVO;
			var itemVOs:Array;
			var itemVO:ImageVO;
			var item:MenuViewButton;
			var itemAsset:MovieClip;
			var i:int = 0;
			var x:int = 0;
			var padX:int = 10;
			
			sectionVO	= ModelLocator.getInstance().configModel.content[ 0 ] as ImageSectionVO;
			itemVOs		= sectionVO.images;
			
			for( i=0; i<itemVOs.length; i++ )
			{
				itemVO			= itemVOs[ i ];
				
				itemAsset		= AssetManager.getClassInstance( "com.holler.twoframe.view.menu.MenuButtonView" ) as MovieClip; 
				itemAsset.x		= i * ( MenuViewButton.WIDTH + padX );
				_sprite.addChild( itemAsset );
				
				item		= new MenuViewButton( itemAsset, itemVO );
				item.addEventListener( MouseEvent.MOUSE_DOWN,	onItemClick );
			}
		}
		
		public function get selectedItemImageVO ():ImageVO
		{
			return selectedItem.imageVO;
		}
		
		private function onItemClick ( e:MouseEvent ):void
		{
			selectedItem = e.target as MenuViewButton;
			
			dispatchEvent( new Event( Event.SELECT ) );
		}

	}
}