package
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.utils.getTimer;
	
	public class CirclePulseSingle
	{
		public static const RADIANS_45	:Number = Math.PI * 0.25;
		public static const RADIANS_90	:Number = Math.PI * 0.50;
		
		private var container:Sprite;
		private var circles:Array		= new Array();
		private var circleRadius:Number = 200;
		private var circlePusleRes:int 	= 20;
		private var startTime:Number	= getTimer();
		private var prsntTime:Number	= 0;
		
		public function CirclePulseSingle( container:Sprite )
		{
			this.container = container;
			
			var i:int;
			var circleObj:Object;
			var angle:Number;
			
			for( i=0; i<circlePusleRes; i++ )
			{
				angle = RADIANS_90 * i / ( circlePusleRes - 1 );
				
				circleObj = { angle:angle };
				
				circles.push( circleObj );
			}
		}

		public function update ( e:Object=null ):void
		{
			clear();
			drawOutline();
			drawCircles();
		}
		
		private function clear ():void
		{
			container.graphics.clear();
		}
		
		private function drawOutline ():void
		{
			container.graphics.lineStyle( 1, 0x000000 );
			container.graphics.drawCircle( 0, 0, circleRadius );
		}
		
		private function drawCircles ():void
		{
			var i:int;
			var circleObj:Object;
			var radiusRatio:Number;
			var radius:Number;
			var circleX:Number, circleY:Number;
			
			for( i=0; i<circlePusleRes; i++ )
			{
				circleObj	= circles[ i ];
				
				circleObj.angle	+= ( ( startTime - prsntTime ) / 20000 );
				if( circleObj.angle > RADIANS_90 )
					circleObj.angle -= RADIANS_90;

				radiusRatio	= 1 - Math.cos( circleObj.angle );
				radius		= circleRadius * radiusRatio;
				circleX		= -circleRadius + radius;
				circleY		= 0;
				
				container.graphics.lineStyle( 1, 0x000000 );
				container.graphics.drawCircle( circleX, circleY, radius );
			}
		}
	}
}