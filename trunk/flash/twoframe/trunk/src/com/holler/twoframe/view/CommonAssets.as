////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.view
{
	import mx.core.SpriteAsset;
	
	public class CommonAssets
	{	
		[Embed (source ="/../html-template/swfs/Common.swf", symbol="Asset_com.holler.common.PreloaderView")]
		private static var loaderAsset : Class;
		
		public static function get loader():SpriteAsset
		{
			return SpriteAsset(new loaderAsset());
		}
	}
}