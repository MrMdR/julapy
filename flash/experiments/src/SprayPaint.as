package
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.CapsStyle;
	import flash.display.JointStyle;
	import flash.display.LineScaleMode;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageQuality;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.ColorTransform;
	import flash.geom.Matrix;
	import flash.geom.Point;
	
	[SWF(backgroundColor='0x333333', frameRate='25', width='600', height='300' ) ]
	
	public class SprayPaint extends Sprite
	{
		[Embed(source="../assets/spray_white.png")]
		private var SparyClass:Class;
		
		private var sprayBitmap:Bitmap;
		private var sprayCanvas:BitmapData;
		private var sprayTemp:Sprite;
		
		private var isComplete:Boolean		= false;
		private var isMouseDown:Boolean		= false;
		
		private var scale:Number			= 0;		// value ranging from 0 - 1
		private var scaleRatio:Number		= 0.1;		// this is the scale of original bitmap.
		
		private var stepFunc:Function;
		private var step:int				= 1;
		private var totalSteps:int			= 3;

		private var stepOneVal:Number		= 0;
		private var stepOneInc:Number		= 0.1;
		private var stepOneLimit:Number		= Math.PI * 0.5;
		
		private var dripFrameCount:int		= 0;
		private var dripFrameThreshold:int	= 15;
		private var dripBoundRadius:int		= 20;
		private var dripDist:Number			= 0;
		private var drips:Array				= new Array();
		private var dripPoint:Point			= new Point();
		
		private var prevDist:Number			= 0;
		private var prevPoint:Point			= new Point();
		
		public function SprayPaint()
		{
			stage.align		= StageAlign.TOP_LEFT;
			stage.quality	= StageQuality.BEST;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			sprayTemp 		= new Sprite();
			sprayBitmap		= new SparyClass();
			sprayCanvas		= new BitmapData( stage.stageWidth, stage.stageHeight, true, 0xFFFFFF );
//			filters			= [ new BlurFilter( 4, 4, 1 ) ];
			addChild( new Bitmap( sprayCanvas ) );
			
			resetStepFunc();
			resetDripPoint();
			resetPrevPoint();

			SprayPaintColor.addColor( 0x009900 );
			SprayPaintColor.addColor( 0x008C00 );
			SprayPaintColor.addColor( 0x00AE00 );
			
			stage.addEventListener( Event.ENTER_FRAME, 		onEnterFrame );
			stage.addEventListener( MouseEvent.MOUSE_DOWN,	onMouseDown );
			stage.addEventListener( MouseEvent.MOUSE_UP,	onMouseUp );
		}

		//////////////////////////////////
		//	ENTER FRAME
		//////////////////////////////////

		private function onEnterFrame ( e:Event ):void
		{
			if( isMouseDown )
			{
				checkPrevPoint();
				drawLine();
				resetPrevPoint();
				
				checkDripPoint();

				var n:Number = ( 30 - prevDist ) / 30;
				var d:Number = Math.max( n, 0 );
				var s:Number = scaleRatio * scale * d;
				var m:Matrix;
				var c:ColorTransform;
				
				if( !stepFunc() )
				{
					if( incrementStepFunc() )
					{
						selectStepFunc();
					}
					else
					{
						resetStepFunc();
						isComplete = true;
					}
				}

				m = new Matrix( );
				m.scale( s, s );
				m.translate( -sprayBitmap.width * s * 0.5, -sprayBitmap.height * s * 0.5 );
				m.rotate( Math.random() * Math.PI * 2 );
				m.translate( stage.mouseX, stage.mouseY );
				
				c = SprayPaintColor.getColorTransform( SprayPaintColor.getRandomColor() );
				c.alphaMultiplier = 0.2;
				
				sprayCanvas.draw( sprayBitmap, m, c );
			}
			
			updateDrips();
		}
		
		//////////////////////////////////
		//	STEPS
		//////////////////////////////////
		
		private function selectStepFunc ():void
		{
			switch ( step )
			{
				case 1 :	stepFunc = stepOne;		break;
				case 2 :	stepFunc = stepTwo;		break;
				case 3 :	stepFunc = stepThree	break;
			}
		}
		
		private function incrementStepFunc ():Boolean
		{
			if( ++step <= totalSteps )
				return true;
			else
				return false;
		}
		
		private function resetStepFunc ():void
		{
			step = 1;
			selectStepFunc();
		}

		private function resetStepOneValues ():void
		{
			scale = 0;
			stepOneVal = 0;
		}
		
		private function stepOne ():Boolean
		{
			if( stepOneVal < stepOneLimit )
			{
				scale		= Math.sin( stepOneVal );
				stepOneVal	+= stepOneInc;
				
				return true;
			}
			else
			{
				return false;
			}
		}
		
		private function stepTwo ():Boolean
		{
			return false;
		}
		
		private function stepThree ():Boolean
		{
			return false;
		}

		//////////////////////////////////
		//	PREV POINT
		//////////////////////////////////
		
		private function resetPrevPoint ():void
		{
			prevPoint = new Point( stage.mouseX, stage.mouseY );
		}
		
		private function checkPrevPoint ():void
		{
			var dx:Number;
			var dy:Number;
			
			dx			= Math.pow( ( stage.mouseX - prevPoint.x ), 2 );
			dy			= Math.pow( ( stage.mouseY - prevPoint.y ), 2 );
			prevDist	= Math.sqrt( dx + dy );
		}
		
		private function drawLine ():void
		{
			var t:Number	= prevDist * 0.01;
			var a:Number	= Math.min( prevDist * 0.005, 0.3 );
			var c:uint		= SprayPaintColor.getRandomColor();

//			var p1x:Number;
//			var p1y:Number;
//			var p2x:Number;
//			var p2y:Number;
//			var w:Number;
//			var h:Number;
//			
//			p1x = prevPoint.x;
//			p2x = stage.mouseX;
//			p1y = prevPoint.y;
//			p2y = stage.mouseY;
//
//			if( prevPoint.x < stage.mouseX )
//			{
//				p1x = stage.mouseX;
//				p2x = prevPoint.x;
//			}
//			
//			if( prevPoint.y < stage.mouseY )
//			{
//				p1y = stage.mouseY;
//				p2y = prevPoint.y;
//			}
//			
//			w = p2x-p1x;
//			h = p2y-p1y;
//			
//			var sprayMask:Sprite	= new Sprite();
//			sprayMask.x				= p1x;
//			sprayMask.y				= p1y;
//			sprayMask.graphics.beginFill( 0x0000FF );
//			sprayMask.graphics.moveTo( 0, 0 );
//			sprayMask.graphics.lineTo( 0, h );
//			sprayMask.graphics.lineTo( w, h );
//			sprayMask.graphics.lineTo( w, 0 );
//			sprayMask.graphics.endFill();
//			sprayMask.scaleX = 0.5;
//			sprayMask.scaleY = 0.5;
//
//			addChild( sprayMask );

			sprayTemp.graphics.clear();
			sprayTemp.graphics.lineStyle( t, c, a, false, LineScaleMode.NONE, CapsStyle.NONE, JointStyle.ROUND );
			sprayTemp.graphics.moveTo( prevPoint.x, prevPoint.y );
			sprayTemp.graphics.lineTo( stage.mouseX, stage.mouseY );
//			sprayTemp.mask = sprayMask;
			
			sprayCanvas.draw( sprayTemp );
		}
		
		//////////////////////////////////
		//	DRIPS
		//////////////////////////////////
		
		private function resetDripPoint ():void
		{
			dripPoint = new Point( stage.mouseX, stage.mouseY );
		}
		
		private function checkDripPoint ():void
		{
			var dx:Number;
			var dy:Number;
			var d:Number;
			
			dx	= Math.pow( ( stage.mouseX - dripPoint.x ), 2 );
			dy	= Math.pow( ( stage.mouseY - dripPoint.y ), 2 );
			d	= Math.sqrt( dx + dy );
			
			if( d < dripBoundRadius )
			{
				if( ++dripFrameCount == dripFrameThreshold )
				{
					dripFrameCount = 0;
					createDrip();
				}
			}
			else
			{
				dripFrameCount = 0;
				resetDripPoint();
			}
		}
		
		private function createDrip ():void
		{
			var drip:SprayPaintDrip;
			
			drip = new SprayPaintDrip( sprayBitmap, sprayCanvas, new Point( stage.mouseX, stage.mouseY ) );
			drip.initialise();
			drips.push( drip );
		}
		
		private function updateDrips ():void
		{
			var i:int;
			var drip:SprayPaintDrip;
			
			for( i=0; i<drips.length; i++ )
			{
				drip = drips[ i ];
				drip.update();
				if( !drip.isValid() )
				{
					drips.splice( i, 1 );
					drip = null;
					--i;
				}
			}
		}

		//////////////////////////////////
		//	MOUSE EVENTS
		//////////////////////////////////
		
		private function onMouseDown ( e:MouseEvent ):void
		{
			isMouseDown = true;
			
			resetDripPoint();
			resetPrevPoint();
		}

		private function onMouseUp ( e:MouseEvent ):void
		{
			isMouseDown = false;
			isComplete	= false;
			
			resetStepFunc();
			resetStepOneValues();
		}
	}
}