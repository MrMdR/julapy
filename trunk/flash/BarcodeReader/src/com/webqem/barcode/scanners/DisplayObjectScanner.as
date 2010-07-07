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

package com.webqem.barcode.scanners {

	import com.webqem.barcode.core.InputIsNullError;

	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.geom.Matrix;
	import flash.utils.Timer;

	[Event(name="scanned", type="com.webqem.barcode.scanners.ScannerEvent")]

	public class DisplayObjectScanner extends EventDispatcher {

		public var scaleMtrx:Matrix;

		private var scanTimer:Timer;

		private var _input:DisplayObject;
		private var _output:BitmapData;
		private var _delay:uint = 0;
		private var _scanning:Boolean = false;

		public function DisplayObjectScanner(input:DisplayObject = null, output:BitmapData = null, scaleMtrx:Matrix = null, timerDelay:uint = 500) {

			this.scaleMtrx = scaleMtrx;
			this.output = output;

			if( _delay == 0) {
				_delay = timerDelay;
			}

			if (input) {
				this.input = input;
			}

			scanTimer = new Timer( _delay );
			scanTimer.addEventListener(TimerEvent.TIMER, onScanTimer);
		}

		public function startScan():void {
			if (input) {
				_scanning = true;
				scanTimer.start();
			}
			else {
				throw new InputIsNullError();
			}
		}

		public function stopScan():void {
			_scanning = false;
			scanTimer.stop();
		}

		public function get scanning():Boolean {
			return _scanning;
		}

		private function set scanning(value:Boolean):void {
			_scanning = value;
		}

		public function get delay():uint {
			return _delay;
		}

		private function set delay(value:uint):void {
			_delay = value;
			if (scanTimer) {
				scanTimer.delay;
			}
		}

		public function get input():DisplayObject {
			return _input;
		}

		public function set input(value:DisplayObject):void {
			if (value) {
				_input = value;

				if (!output) {
					output = new BitmapData(input.width, input.height);
				}

				if (!scaleMtrx) {
					scaleMtrx = new Matrix();
					scaleMtrx.scale(input.scaleX, input.scaleY);
				}
			}
			else {
				throw new InputIsNullError();
			}
		}

		public function get output():BitmapData {
			return _output;
		}

		public function set output(value:BitmapData):void {
			_output = value;
		}

		private function scanDisplayObject():void {
			output.draw(input as DisplayObject, scaleMtrx);
			dispatchEvent(new ScannerEvent(ScannerEvent.SCANNED));
		}

		private function onScanTimer(event:TimerEvent):void {
			scanDisplayObject();
		}
	}
}