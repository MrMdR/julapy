/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Actionscript 3 barcode decoding library.
 *
 * The Initial Developer of the Original Code is
 * webqem pty ltd. http://www.webqem.com/.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Mike Shaw <mikes@webqem.com>
 * ***** END LICENSE BLOCK ***** */

package com.webqem.barcode.readers {

	import com.webqem.barcode.core.AbstractDecoder;
	import com.webqem.barcode.core.IBarcodeReader;
	import com.webqem.barcode.core.IEncodedBarcodeData;
	import com.webqem.barcode.core.InputIsNullError;

	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.filters.ColorMatrixFilter;
	import flash.filters.ConvolutionFilter;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	import flash.utils.getTimer;

	public class BitmapReader implements IBarcodeReader {

		private static const VERTICAL_LINES_NUM_DEFAULT		: uint = 2;
		private static const HORIZONTAL_LINES_NUM_DEFAULT	: uint = 2;

		private var _input				: BitmapData;
		private var _encodedBarcodeData	: OneDimEncodedData;

		private var barcodeDecoder		: AbstractDecoder;
		private var pixelRowsIdx		: Dictionary	= new Dictionary();
		private var	pixelRows			: Array			= new Array();
		private var	indexes				: Array			= new Array();

		private var horScanLines		: Array			= new Array();
		private var verScanLines		: Array			= new Array();

		private var bCheckHorizontal	: Boolean		= true;
		private var bCheckVertical		: Boolean		= true;
		private var bCheckUpsideDown	: Boolean		= true;

		private var greyscaleArr		: Array =		// greyscale matrix
		[
			0.3, 0.59, 0.11, 0, 0,
			0.3, 0.59, 0.11, 0, 0,
			0.3, 0.59, 0.11, 0, 0,
			0,   0,    0,    1, 0
		];

		private var greyscaleFilter		: ColorMatrixFilter = new ColorMatrixFilter( greyscaleArr );

		private var sharpenFilter		: ConvolutionFilter = new ConvolutionFilter		// sharpen matrix
		(
			3,
			3,
			[
				 0, -1,  0,
				-1, 15, -1,
				 0, -1,  0
			],
			9
		);

		/////////////////////////////////////////////
		//	CONSTRUCTOR
		/////////////////////////////////////////////

		public function BitmapReader( barcodeDecoder : AbstractDecoder = null )
		{
			this.barcodeDecoder = barcodeDecoder;

			initDefaultScanLines();
		}

		public function set input(value:Object):void {
			_input = value as BitmapData;
			readInput();
		}

		public function get encodedBarcodeData ():IEncodedBarcodeData
		{
			return _encodedBarcodeData;
		}

		private function set encodedBarcodeData( value : IEncodedBarcodeData ):void
		{
			_encodedBarcodeData = value as OneDimEncodedData;
		}

		public function readInput( input : Object = null ):IEncodedBarcodeData
		{
			if( input )
			{
				_input = input as BitmapData;
			}

			if( !_input )
			{
				throw new InputIsNullError();
			}

			scanPixels();

			return _encodedBarcodeData;
		}

		/////////////////////////////////////////////
		//	SCAN LINES
		/////////////////////////////////////////////

		private function initDefaultScanLines ():void
		{
			var t : int;
			var i : int;
			var d : Number;
			var p : Number;

			//-- vertical scan lines.

			t = VERTICAL_LINES_NUM_DEFAULT;
			i = 0;
			d = 1 / ( t + 1 );

			for( i=0; i<t; i++ )
			{
				p = i * d + d;

				addVerticalScanLine( p );
			}

			//-- horizontal scan lines.

			t = HORIZONTAL_LINES_NUM_DEFAULT;
			i = 0;
			d = 1 / ( t + 1 );

			for( i=0; i<t; i++ )
			{
				p = i * d + d;

				addHorizontalScanLine( p );
			}
		}

		//-- public settings.

		public function addHorizontalScanLine ( p : Number ):void
		{
			horScanLines.push( p );
		}

		public function addVerticalScanLine ( p : Number ):void
		{
			verScanLines.push( p );
		}

		public function clearScanLines ():void
		{
			horScanLines = new Array();
			verScanLines = new Array();
		}

		public function checkVerticalScanLines ( b : Boolean ):void
		{
			bCheckVertical = b;
		}

		public function checkHorizontalScanLines ( b : Boolean ):void
		{
			bCheckHorizontal = b;
		}

		public function checkUpsideDownBarcodes	( b : Boolean ):void
		{
			bCheckUpsideDown = b;
		}

		/////////////////////////////////////////////
		//	SCAN
		/////////////////////////////////////////////

		private function scanPixels():void
		{
			var startTime : int;
			startTime = getTimer();

			var scanFrame : BitmapData;
			scanFrame = _input.clone();

			scanFrame.applyFilter( scanFrame, scanFrame.rect, new Point( 0, 0 ), sharpenFilter );
			scanFrame.applyFilter( scanFrame, scanFrame.rect, new Point( 0, 0 ), greyscaleFilter );

			pixelRowsIdx	= new Dictionary();
			pixelRows		= new Array();
			indexes			= new Array();

			for( var c:Number=0xf; c<=0xff; c += 0xf )
			{
				var threshold : Number;
				threshold = new Number( c << 16 | c << 8 | c );

				if( bCheckHorizontal )
					scanHorizontalLines( scanFrame, threshold );

				if( bCheckVertical )
					scanVerticalLines( scanFrame, threshold );
			}

			pixelRowsIdx = null;

			_encodedBarcodeData = new OneDimEncodedData( pixelRows, indexes );

			trace( "scanPixels", getTimer() - startTime );
		}

		private function scanHorizontalLines ( scanFrame : BitmapData, threshold : int ):void
		{
			var x	: uint = 0;
			var y	: uint = 0;
			var w	: uint = scanFrame.width;
			var h	: uint = scanFrame.height;
			var i	: uint = 0;

			for( i=0; i<horScanLines.length; i++ )
			{
				y = horScanLines[ i ] * h;

				var row		: Array;
				var startX	: uint;
				var endX	: uint;

				row		= new Array( scanFrame.width );
				startX	= scanFrame.width;
				endX	= 0;

				for( x=0; x<scanFrame.width; x++ )
				{
					if( scanFrame.getPixel( x, y ) <= threshold )
					{
						if( x < startX )
						{
							startX = x;
						}

						if( x > endX )
						{
							endX = x;
						}

						row[ x ] = 1;
					}
					else
					{
						row[ x ] = 0;
					}
				}

				addDataToResult( row, startX, endX );

				if( !bCheckUpsideDown )
					continue;

				row = row.slice();
				row.reverse();
				addDataToResult( row, row.length - endX, row.length - startX );
			}
		}

		private function scanVerticalLines ( scanFrame : BitmapData, threshold : int ):void
		{
			var x	: uint = 0;
			var y	: uint = 0;
			var w	: uint = scanFrame.width;
			var h	: uint = scanFrame.height;
			var i	: uint = 0;

			for( i=0; i<verScanLines.length; i++ )
			{
				x = verScanLines[ i ] * w;

				var col		: Array;
				var startY	: Number;
				var endY	: Number;

				col		= new Array( scanFrame.height );
				startY	= scanFrame.height;
				endY	= 0;

				for( y=0; y<scanFrame.height; y++ )
				{
					if( scanFrame.getPixel( x, y ) <= threshold )
					{
						if( y < startY )
						{
							startY = y;
						}
						if( y > endY )
						{
							endY = y;
						}

						col[ y ] = 1;
					}
					else
					{
						col[ y ] = 0;
					}
				}

				addDataToResult(col, startY, endY);

				if( !bCheckUpsideDown )
					continue;

				col = col.slice();
				col.reverse();
				addDataToResult(col, col.length - endY, col.length - startY);
			}
		}

		private function addDataToResult(bits:Array, start:uint, end:uint):void {
			if (!pixelRowsIdx[bits.toString()]) {
				if (barcodeDecoder) {
					if (barcodeDecoder.isValidEncoding(new OneDimEncodedData(bits, [start, end]))) {
						pixelRowsIdx[bits.toString()] = bits;
						pixelRows.push(bits);
						indexes.push({start: start, end: end});
					}
				}
				else {
					pixelRowsIdx[bits.toString()] = bits;
					pixelRows.push(bits);
					indexes.push({start: start, end: end});
				}
			}
		}

		/////////////////////////////////////////////
		//	DRAW SCAN LINES
		/////////////////////////////////////////////

		public function drawScanLines ( sprite : Sprite ):void
		{
			var x	: uint = 0;
			var y	: uint = 0;

			var w	: uint = _input.width;
			var h	: uint = _input.height;

			var i	: uint = 0;

			sprite.graphics.clear();

			if( bCheckHorizontal )
			{
				for( i=0; i<horScanLines.length; i++ )
				{
					y = horScanLines[ i ] * h;

					sprite.graphics.beginFill( 0xFF0000, 1.0 );
					sprite.graphics.drawRect( 0, y, w, 1 );
					sprite.graphics.endFill();
				}
			}

			if( bCheckVertical )
			{
				for( i=0; i<verScanLines.length; i++ )
				{
					x = verScanLines[ i ] * w;

					sprite.graphics.beginFill( 0xFF0000, 1.0 );
					sprite.graphics.drawRect( x, 0, 1, h );
					sprite.graphics.endFill();
				}
			}
		}
	}
}