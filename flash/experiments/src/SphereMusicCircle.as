package
{
	import flash.display.Sprite;
	import flash.display.Shape;
	import flash.filters.GlowFilter;
	
	class SphereMusicCircle extends Sprite 
	{
		public var dot1:Shape;
		public var dot2:Shape;
		public var line1:Shape;
		public var line2:Shape;
		
		public function SphereMusicCircle(scale:Number):void 
		{
			dot1 = new Shape();
			dot1.graphics.beginFill(0x9900CC,1);
			dot1.graphics.drawCircle(0,0,10/scale);
			dot1.graphics.endFill();
			dot1.graphics.beginFill(0x000000,1);
			dot1.graphics.drawCircle(0,0,8/scale);
			dot2 = new Shape();
			dot2.graphics.beginFill(0x9900CC,1);
			dot2.graphics.drawCircle(0,0,4/scale);
			line1 = new Shape();
			line1.graphics.beginFill(0x6600FF,0.3);
			line1.graphics.drawRect(-1,0,2,100);
			line1.z = 200;
			line1.rotationX = 90;
			line2 = new Shape();
			line2.graphics.beginFill(0x551FFF,Math.random()*0.5+0.5);
			line2.graphics.drawRect(-1,-25,2,35);
			line2.blendMode = "add";
			line2.z = 0;
			line2.rotationX = line2.rotationY = 90;
			addChild(dot1);
			addChild(dot2);
			addChild(line1);
			addChild(line2);
			dot1.scaleX = dot1.scaleY = dot2.scaleX = dot2.scaleY = scale;
		}
	}
}