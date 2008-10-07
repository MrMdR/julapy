/*
	Papervision 2.0a Example
	Lee Felarca
	12/12/2007, plus
	
	Examine the source for examples of ...
	
		- Setting up and rendering a simple 3d scene
		
		- Utilizing new materials found in the org.papervision3d.materials.shadematerials package (GouraudMaterial, PhongMaterial, CellMaterial, FlatShadeMaterial, EnvMapMaterial). Plus, CompositeMaterial.
		
		- Utilizing Shaders (viz., EnvMapShader)
		
		- Utilizing a second viewport (used here to create the 'reflection' on the floor)

		- Handling simple user interaction with the 3d scene with InteractiveSceneManager 
*/

package
{
	import caurina.transitions.*;
	
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.filters.BlurFilter;
	import flash.geom.Matrix;
	
	import org.papervision3d.cameras.Camera3D;
	import org.papervision3d.cameras.CameraType;
	import org.papervision3d.core.math.Number3D;
	import org.papervision3d.core.proto.MaterialObject3D;
	import org.papervision3d.core.utils.InteractiveSceneManager;
	import org.papervision3d.core.utils.Mouse3D;
	import org.papervision3d.events.InteractiveScene3DEvent;
	import org.papervision3d.lights.PointLight3D;
	import org.papervision3d.materials.BitmapMaterial;
	import org.papervision3d.materials.ColorMaterial;
	import org.papervision3d.materials.WireframeMaterial;
	import org.papervision3d.materials.shadematerials.CellMaterial;
	import org.papervision3d.materials.shadematerials.EnvMapMaterial;
	import org.papervision3d.materials.shadematerials.FlatShadeMaterial;
	import org.papervision3d.materials.shadematerials.GouraudMaterial;
	import org.papervision3d.materials.shadematerials.PhongMaterial;
	import org.papervision3d.materials.shaders.EnvMapShader;
	import org.papervision3d.materials.shaders.ShadedMaterial;
	import org.papervision3d.materials.special.CompositeMaterial;
	import org.papervision3d.objects.DisplayObject3D;
	import org.papervision3d.objects.primitives.Plane;
	import org.papervision3d.objects.primitives.Sphere;
	import org.papervision3d.render.BasicRenderEngine;
	import org.papervision3d.scenes.Scene3D;
	import org.papervision3d.view.Viewport3D;
	

	[SWF(width="950", height="500", frameRate="45", backgroundColor="#444444", pageTitle="PV2 Sandbox")]

	public class ShadersExample extends Sprite
	{
		private static const DEGREE:Number = Math.PI/180;
		private const NUMSPHERES:int = 7;

		private const FLOORBMP_SIDE:Number = 500;
		private const ENVSPHEREBMP_WIDTH:Number = stage.stageWidth/2;
		private const ENVSPHEREBMP_HEIGHT:Number = stage.stageHeight/2;

		private var sprScene:Sprite;

		// 3d-related variables
		private var scene:Scene3D;
		private var ism:InteractiveSceneManager;
		private var renderer:BasicRenderEngine;
		private var viewportMain:Viewport3D;
		private var viewportFloor:Viewport3D;
		private var cameraMain:Camera3D;
		private var cameraFloor:Camera3D;
		private var light:PointLight3D;
		private var lightSphereEnv:PointLight3D;
		private var floor:Plane;
		private var matSphereEnv:EnvMapMaterial;
		private var matSphereComposite:CompositeMaterial;
		private var matFloor:CompositeMaterial;
		private var matFloorBmp:BitmapMaterial;

		private var bmdEnvSphere:BitmapData;
		private var bmdFloor:BitmapData;
		private var matrixEnvSphere:Matrix;
		private var matrixFloor:Matrix;
		
		private var counter:int;
		
		
		public function ShadersExample()
		{
			init();
			init3d();
		}
		
		private function init():void
		{
			this.stage.scaleMode = StageScaleMode.NO_SCALE;
			sprScene = new Sprite();
			this.addChild(sprScene);
			this.addEventListener(Event.ENTER_FRAME, onEnterFrame, false,0,true);
		}
		
		
		private function init3d():void
		{
			renderer = new BasicRenderEngine();
			
			scene = new Scene3D();

			viewportMain = new Viewport3D(stage.stageWidth, stage.stageHeight, false, true); // interactive
			viewportMain.opaqueBackground = 0;
			this.addChild(viewportMain);
			
			ism = viewportMain.interactiveSceneManager;
			Mouse3D.enabled = true;
			
			light = new PointLight3D(true);
			lightSphereEnv = new PointLight3D(false);

			cameraMain = new Camera3D();
			cameraMain.zoom = 8;
			cameraMain.y = 100;
			cameraMain.z = -1000;
			
			matrixEnvSphere = new Matrix();
			matrixEnvSphere.scale(0.5,-0.5);
			matrixEnvSphere.translate(0, ENVSPHEREBMP_HEIGHT);

			// Set up spheres
			
			for (var i:int = 0; i < NUMSPHERES; i++) 
			{
				// [a] materials
				
				var m:MaterialObject3D;
				
				switch (i)
				{
					case 0:
						m = new GouraudMaterial(light, 0xFF3300, 0x000000);
						break;
					case 1:
						m = new PhongMaterial(light, 0xFF3300, 0, 3);
						break;
					case 2:
						m = new CellMaterial(light, 0xFF3300, 0x000000, 4);
						break;
					case 3:
						m  = new FlatShadeMaterial(light, 0x000000, 0xFF3300);
						break;
					case 4:
						m = matSphereComposite = new CompositeMaterial();
						matSphereComposite.addMaterial( new ColorMaterial(0x0, 0.7) );
						matSphereComposite.addMaterial( new WireframeMaterial(0xFF3300, 100) );
						break;					
					case 5: 
						// An environment map material whose lightmap is 
						// updated with the BitmapData of the main scene
						bmdEnvSphere = new BitmapData(ENVSPHEREBMP_WIDTH,ENVSPHEREBMP_HEIGHT);
						m = matSphereEnv = new EnvMapMaterial(lightSphereEnv, bmdEnvSphere, null, 0);
						matSphereEnv.lightMap = bmdEnvSphere;
						break;
					case 6:
						// A ShadedMaterial using an environment map shader. Uses perlin noise generator 
						// for the 'base' bitmap, the environment map bitmap, and the bumpmap. 
						var bmdBase:BitmapData = new BitmapData(250,250,true,0x0);
						bmdBase.perlinNoise(75,75, 5, 0x000001, true, true, 1, true);
						var b:BitmapMaterial = new BitmapMaterial(bmdBase);
						
						var bmdEnv:BitmapData = new BitmapData(250,250,false,0x888888);
						bmdEnv.perlinNoise(150,150, 5, 0x000002, true, false, 1|2|4 , true);

						var bmdBump:BitmapData = new BitmapData(250,250,false,0x888888);
						bmdBump.perlinNoise(150,150, 5, 0x000003, true, false, 1|2|4 , true);

						var s:Shader = new EnvMapShader(light, bmdEnv, bmdEnv, 0x0, bmdBump, null);
						m = new ShadedMaterial(b, s, 0); 
						break;
				}
				m.interactive = true;
				
				// [b] object properties
				
				var d:DisplayObject3D = new Sphere( m, 100, 8,6 );
				d.x = 325;
				d.y = 100;
				d.z = 0;
				var p:Number3D = new Number3D(d.x, d.y, d.z); 
				p = rotateY(p , i/NUMSPHERES * 360 * DEGREE );
				applyNumber3D(p, d);

				d.addEventListener(InteractiveScene3DEvent.OBJECT_CLICK, onSphereClick);

				scene.addChild(d, "s" + i.toString());
			}
			
			// Set up floor:

			// [a] viewport
			viewportFloor = new Viewport3D(FLOORBMP_SIDE,FLOORBMP_SIDE, false);
			viewportFloor.opaqueBackground = 0x222222;
			viewportFloor.filters = [ new BlurFilter(15,15,1)];

			// [b] material
			bmdFloor = new BitmapData(FLOORBMP_SIDE,FLOORBMP_SIDE,true);
			matFloorBmp = new BitmapMaterial(bmdFloor);
			matFloor = new CompositeMaterial();
			matFloor.addMaterial(matFloorBmp);
			matFloor.addMaterial( new WireframeMaterial(0x444444,100) );
			
			// [c] matrix
			matrixFloor = new Matrix();
			matrixFloor.scale(1,-1);
			matrixFloor.translate(0,FLOORBMP_SIDE);

			// [d] camera (looks up at the spheres from below)
			cameraFloor = new Camera3D();
			cameraFloor.zoom = 11.2; // bigger zoom in hopes of minimizing effects of perspective a little
			cameraFloor.focus = 50;
			cameraFloor.x = 0;
			cameraFloor.y = -1000;
			cameraFloor.z = 0;
			cameraFloor.pitch(-90);

			// [e] object setup
			floor = new Plane(matFloor, 1000,1000, 10,10);
			floor.x = floor.y = floor.z = 0;
			floor.rotationX = -90;
			scene.addChild(floor);
		}
		
		private function onSphereClick(e:InteractiveScene3DEvent):void
		{
			// Make sphere bounce...
			Tweener.addTween(e.target, { y:100, _bezier:{ y:600 }, time:3, transition:"easeoutbounce"} );
		}

		private function onEnterFrame(e:Event):void
		{
			this.stage.focus = this;

			// Move camera based on mouse position
			var x:Number = (stage.mouseX - stage.stageWidth/2) / 4;
			cameraMain.moveLeft(x);
			var y:Number = (stage.mouseY - stage.stageHeight/2) / 6;
			cameraMain.moveDown(y);
			if (cameraMain.y > 1000 || cameraMain.y < -200) cameraMain.moveDown(-y);

			// Rotate light source around camera
			counter += 3;
			var lightPos:Number3D = new Number3D(cameraMain.x, cameraMain.y, cameraMain.z);
			var r:Number = Math.sin( counter*DEGREE ) * 180;
			lightPos = rotateY( lightPos, r*DEGREE );
			applyNumber3D(lightPos, light);
			
			// 'Render' and draw floor
			// [* Alternate method: Use BitmapViewportMaterial + BitmapViewport3D]  
			renderer.renderScene(scene, cameraFloor, viewportFloor);
			bmdFloor.draw(viewportFloor, matrixFloor);
			matFloorBmp.bitmap = bmdFloor;
			matFloorBmp.updateBitmap();

			// Update light source for the env. map sphere and then update its bitmap
			applyNumber3D(lightPos, lightSphereEnv);
			bmdEnvSphere.draw(viewportMain, matrixEnvSphere);

			// Render scene
			renderer.renderScene(scene, cameraMain, viewportMain);
		}

		
		// Helper functions:
		
		public function rotateY(p:Number3D, angleY:Number) : Number3D {
			var x1:Number = Math.cos(angleY) * p.x - Math.sin(angleY) * p.z;
			var z1:Number = Math.cos(angleY) * p.z + Math.sin(angleY) * p.x;
			var y1:Number = p.y;
			return new Number3D(x1, y1, z1);
		}
		
		public function applyNumber3D(p:Number3D, d:DisplayObject3D):void
		{
			d.x = p.x;
			d.y = p.y;
			d.z = p.z;
		} 
	}
}
