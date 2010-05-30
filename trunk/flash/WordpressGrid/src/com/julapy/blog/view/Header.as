package com.julapy.blog.view
{
	import com.holler.assets.AssetLoader;
	import com.julapy.blog.model.ConfigModel;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;

	public class Header
	{
		private var container	: Sprite;
		private var asset		: MovieClip;

		private var titleText	: TextField;

		public function Header( container : Sprite )
		{
			this.container = container;

			asset = AssetLoader.getInstance().getClassInstance( "julapy.blog.title" ) as MovieClip;
			container.addChild( asset );

			titleText = asset.getChildByName( "titleText" ) as TextField;
			titleText.text = ConfigModel.getInstance().blogVO.blogTitle;
		}

	}
}