package
{
	import flash.display.BlendMode;
	import flash.display.Sprite;
	import flash.events.Event;
	
	[SWF(backgroundColor="0xAAAAAA", frameRate="31", width="950", height="580")]

	public class CirclePulse extends Sprite
	{
		private var bg:Sprite;
		private var container:Sprite;
		private var circles:Array = new Array();
		private var circlesTotal:int = 8;
		
		public function CirclePulse()
		{
			stage.quality	= "high";
			stage.align		= "tl";
			stage.scaleMode	= "noScale";
			
//			blendMode		= BlendMode.DIFFERENCE;

			bg = new Sprite();
			bg.graphics.beginFill( 0xAAAAAA );
			bg.graphics.drawRect( 0, 0, 950, 580 );
			bg.graphics.endFill();
//			bg.blendMode	= BlendMode.ADD;
			addChild( bg );

			container			= new Sprite();
			container.x			= (int)( 950 * 0.5 );
			container.y			= (int)( 580 * 0.5 );
//			container.blendMode	= BlendMode.DIFFERENCE;
			addChild( container );
			
			var i:int;
			var circle:CirclePulseSingle;
			var circleContainer:Sprite;
			
			for( i=0; i<circlesTotal; i++ )
			{
				circleContainer				= new Sprite();
				circleContainer.rotation	= i * ( 360 / circlesTotal );
//				circleContainer.blendMode	= BlendMode.LIGHTEN;
				container.addChild( circleContainer );
				
				circle						= new CirclePulseSingle( circleContainer );
				circles.push( circle );
			}
			
			addEventListener( Event.ENTER_FRAME, onEnterFrame );
		}
		
		private function onEnterFrame ( e:Event ):void
		{
			var i:int;
			var circle:CirclePulseSingle;

			for( i=0; i<circles.length; i++ )
			{
				circle	= circles[ i ];
				circle.update( );
			}
		}
		
	}
}