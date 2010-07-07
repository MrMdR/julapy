package {
	import com.holler.webcam.Webcam;
	import com.holler.webcam.WebcamEvent;
	import com.webqem.barcode.readers.BitmapReader;
	import com.webqem.barcode.readers.OneDimEncodedData;
	import com.webqem.barcode.scanners.DisplayObjectScanner;
	import com.webqem.barcode.scanners.ScannerEvent;
	import com.webqem.barcode.symbology.EAN13.EAN13DecodedData;
	import com.webqem.barcode.symbology.EAN13.EAN13Symbology;

	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.geom.Rectangle;
	import flash.media.Video;
	import flash.text.TextField;

	[ SWF( backgroundColor='0xCCCCCC', frameRate='25', width='1024', height='768' ) ]

	public class BarcodeReader extends Sprite
	{
		public var webcam			: Webcam = Webcam.getInstance();
		public var video			: Video;
		public var videoContainer	: Sprite = new Sprite();
		public var scanLines		: Sprite = new Sprite();
		public var tf				: TextField = new TextField();

		public var scanner			: DisplayObjectScanner;
		public var reader			: BitmapReader;
		public var ean13			: EAN13Symbology;

		public function BarcodeReader()
		{
			stage.scaleMode		= StageScaleMode.NO_SCALE;
			stage.align			= StageAlign.TOP_LEFT;

			addChild( videoContainer );
			addChild( scanLines );
			addChild( tf );

			var webcamRect : Rectangle;
//			webcamRect = new Rectangle( 0, 0, 160, 120 );
//			webcamRect = new Rectangle( 0, 0, 320, 240 );
			webcamRect = new Rectangle( 0, 0, 640, 480 );

			tf.x = 20;
			tf.y = webcamRect.height + 40;

			webcam.setStage( stage );
			webcam.setSize( webcamRect.width, webcamRect.height );
			webcam.addEventListener( WebcamEvent.WEBCAM_READY_EVENT, webcamHandler );
			webcam.connect();
		}

		private function webcamHandler ( e : WebcamEvent ):void
		{
			videoContainer.addChild( webcam.getVideo() as Video );

			initReader();
			initSymbology();
			initScanner();
		}

		private function initReader ():void
		{
			reader = new BitmapReader();
			reader.checkHorizontalScanLines( true );
			reader.checkVerticalScanLines( false );
			reader.checkUpsideDownBarcodes( true );
		}

		private function initSymbology ():void
		{
			ean13 = new EAN13Symbology();
		}

		private function initScanner ():void
		{
			scanner = new DisplayObjectScanner( null, null, null, 500 );
			scanner.input = videoContainer;
			scanner.addEventListener( ScannerEvent.SCANNED, scannerHandler );
			scanner.startScan();
		}

		private function scannerHandler ( e : ScannerEvent ):void
		{
			var data : OneDimEncodedData;
			data = reader.readInput( scanner.output ) as OneDimEncodedData;

			var decoded : EAN13DecodedData;
			decoded = ean13.decodeBarcode( data ) as EAN13DecodedData;

			reader.drawScanLines( scanLines );

			tf.text = decoded.toString();
		}
	}
}
