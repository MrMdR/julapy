package {
	import flash.display.Sprite;

	[SWF(backgroundColor="0", frameRate="31", width="950", height="580")]
	public class EyeTest extends Sprite
	{
		
		public function EyeTest()
		{
			stage.quality = "high";
			stage.align = "tl";
			stage.scaleMode = "noScale";
			
			var main:EyeTestBasicView = new EyeTestBasicView();
			addChild(main);
		}
	}
}
