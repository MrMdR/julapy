package
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.BitmapDataChannel;
	import flash.display.BlendMode;
	import flash.display.GradientType;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.BlurFilter;
	import flash.filters.DisplacementMapFilter;
	import flash.filters.GlowFilter;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import org.papervision3d.core.geom.Particles;
	import org.papervision3d.core.geom.renderables.Particle;
	import org.papervision3d.core.math.Number3D;
	import org.papervision3d.core.math.Quaternion;
	import org.papervision3d.events.FileLoadEvent;
	import org.papervision3d.lights.PointLight3D;
	import org.papervision3d.materials.BitmapMaterial;
	import org.papervision3d.materials.MovieMaterial;
	import org.papervision3d.materials.shaders.GouraudShader;
	import org.papervision3d.materials.shaders.ShadedMaterial;
	import org.papervision3d.materials.special.ParticleMaterial;
	import org.papervision3d.materials.utils.MaterialsList;
	import org.papervision3d.objects.DisplayObject3D;
	import org.papervision3d.objects.parsers.DAE;
	import org.papervision3d.objects.primitives.Sphere;
	import org.papervision3d.view.BasicView;
	import org.papervision3d.view.layer.ViewportLayer;
	import org.papervision3d.view.layer.util.ViewportLayerSortMode;

	public class EyeTestBasicView extends BasicView
	{
		
		private var mainObject:DisplayObject3D;
		private var mainObject2:Sphere = new Sphere(null, 30);
		
		[Embed(source="../assets/arrow2.DAE", mimeType="application/octet-stream")]
		private var arrowAsset:Class;
		
		[Embed(source="../assets/rust.jpg")]
		private var rustAsset:Class;
		
		private var dragTarget:DisplayObject3D = DisplayObject3D.ZERO;
		private var doLook:Boolean = false;
		private var light:PointLight3D; 
		private var containerSphere:Sphere;
		private var highlightSphere:Sphere;
		private var mainLayer:ViewportLayer;
		
		public function EyeTestBasicView(viewportWidth:Number=640, viewportHeight:Number=480, scaleToStage:Boolean=true, interactive:Boolean=false, cameraType:String="Target")
		{
			super(viewportWidth, viewportHeight, scaleToStage, interactive, cameraType);
			camera.focus = 100;
			camera.zoom = 11;
			init();

			singleRender();
			mainObject.lookAt(camera);

		}
		
		
		private function onLoad(e:FileLoadEvent):void{
			
			//setup the layers
			
			var vpl:ViewportLayer =	mainLayer = viewport.getChildLayer(mainObject.getChildByName("Line01", true), true, true);
			vpl.filters = [new GlowFilter(0xFFEEFF, 0.25, 38, 38, 1, 2)];
			vpl.layerIndex = 0;
			
			vpl = viewport.getChildLayer(containerSphere);
			vpl.filters = [new BlurFilter(16, 16, 8)];
			vpl.blendMode =  BlendMode.HARDLIGHT;
			vpl.layerIndex = 1;

		}
		

		
		public function init():void{
			
			viewport.containerSprite.sortMode = ViewportLayerSortMode.INDEX_SORT;
			
			mainObject = new DisplayObject3D();
			var dae:DAE = new DAE();
			
			//light for the arrow
			light = new PointLight3D(false);
			light.z = -10000;
			light.y = 40000;
			light.x = 30000;
			scene.addChild(light);

			var bmat:BitmapMaterial = new BitmapMaterial(Bitmap(new rustAsset()).bitmapData);
			var shader:GouraudShader = new GouraudShader(light, 0xFFFFFF, 0x101010);
			var shadeMat:ShadedMaterial = new ShadedMaterial(bmat, shader);

		
			var mList:MaterialsList = new MaterialsList();
			mList.addMaterial(shadeMat, "arrowMaterial3");
			mainObject.addChild(dae);
		
			
			dae.scale = 3;
			dae.yaw(180);
			
			
			//just stuff to make the sphere look better
			var sprite:Sprite = new Sprite();
			var matrix:Matrix = new Matrix();
			matrix.createGradientBox(256, 256, Math.PI/2);
			sprite.graphics.beginGradientFill(GradientType.LINEAR, [0xFFFFFF, 0x505050,0xFFFFFF], [0.15, 0.15, 0.15], [0,114, 255], matrix);
			sprite.graphics.drawRect(0, 0, 256, 256);
			sprite.graphics.endFill();
			

			var outerRing:Sprite = new Sprite();
			outerRing.graphics.lineStyle(1, 0xe0e0e0, 0.25);
			outerRing.graphics.drawCircle(0, 0, 210);
			outerRing.filters = [new BlurFilter(4,4,4)];
			outerRing.cacheAsBitmap = true;

			containerSphere = new Sphere(new MovieMaterial(sprite, true, false), 180, 12, 10);
			containerSphere.material.doubleSided = true;
		
			mainObject.addChild(dae);
			
			scene.addChild(mainObject);
			
			viewport.containerSprite.addChild(outerRing);
						
			var innerTint:Sprite = new Sprite();
			matrix = new Matrix();
			matrix.createGradientBox(420, 420, 0, -210, -210);
			innerTint.graphics.beginGradientFill(GradientType.RADIAL, [0x640909, 0x640909], [0.1, 1], [180, 255], matrix);
			innerTint.graphics.drawCircle(0, 0, 210);
			innerTint.blendMode = BlendMode.ADD;
			innerTint.alpha = 0.14;
			viewport.containerSprite.addChild(innerTint);
			
			//random bubbly background
			var bubbleLayer:Sprite = new Sprite();
			
			for(var i:int = 0;i<40;i++){
				var rad:Number = Math.random()*10+2;	
				var a:Number = Math.random();
				var c:Number = 0xFFFFFF;
				var xx:Number = Math.random()*1000-500;
				var yy:Number = Math.random()*1000-500;
				if(Math.sqrt(xx*xx+yy*yy) < 210){
					c = 0x442222;
				}
				bubbleLayer.graphics.beginFill(c, a);
				
				bubbleLayer.graphics.drawCircle(xx, yy, rad);
				bubbleLayer.graphics.endFill();
			}
			
			viewport.containerSprite.addChildAt(bubbleLayer, 0);
			
			bubbleLayer.filters = [new BlurFilter(24, 24, 4)];
			bubbleLayer.cacheAsBitmap = true;
			bubbleLayer.blendMode = BlendMode.SCREEN;

			mainObject.addChild(containerSphere);
			
			//load the model
			dae.addEventListener(FileLoadEvent.LOAD_COMPLETE, onLoad);
			dae.load(XML(new arrowAsset()), mList);

			addEventListener(Event.ENTER_FRAME, tick);
		}
		
		private function lookTo(ray:Number3D):void{
		
			var ease:Number = 0.25;
			var mainPosition:Number3D = new Number3D(mainObject.x, mainObject.y, mainObject.z);
			
			var dir:Number3D = Number3D.sub(ray, mainPosition);
			dir.normalize();
			
			var front:Number3D = new Number3D(mainObject.transform.n13, mainObject.transform.n23, mainObject.transform.n33);
			front.normalize();
			
			var cross:Number3D = Number3D.cross(dir, front);
			var dot:Number = Number3D.dot(front,dir);
			
			var angle:Number = Math.acos(dot);
			
			var targetQuat:Quaternion = Quaternion.createFromAxisAngle(cross.x, cross.y, cross.z, angle*ease);
 
			mainObject.transform.calculateMultiply3x3(mainObject.transform, targetQuat.matrix);
		
		}
		
		private function tick(e:Event):void{


			//find our point
			var ray:Number3D = camera.unproject(viewport.containerSprite.mouseX, viewport.containerSprite.mouseY);
			var depth:Number = 1600;
			ray.normalize();
			ray.multiplyEq(depth);
			ray = Number3D.add(ray, new Number3D(camera.x, camera.y, camera.z));
			lookTo(ray);
			
			//roll the sphere highlights to keep some motion in the scene
			containerSphere.rotationX+=0.45;
			containerSphere.rotationY+=0.15; 

			singleRender();


		}
		
	
	}
}