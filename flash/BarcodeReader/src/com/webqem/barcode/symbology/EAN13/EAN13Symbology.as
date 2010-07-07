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

package com.webqem.barcode.symbology.EAN13 {

	import com.webqem.barcode.core.IDecodedBarcodeData;
	import com.webqem.barcode.core.IEncodedBarcodeData;
	import com.webqem.barcode.symbology.IOneDimSymbology;

	import flash.utils.Dictionary;
	import flash.utils.getTimer;

	public class EAN13Symbology implements IOneDimSymbology {

		public static const DEFINITION:String = "com.webqem.barcode.symbology.EAN13.EAN13Symbology";

		private var guards:Dictionary = new Dictionary();
		private var firstDigit:Dictionary = new Dictionary();
		private var digits:Dictionary = new Dictionary();

		public function EAN13Symbology() {
			initDictionaries();
		}

		public function hasValidCheckSum(data:IDecodedBarcodeData):Boolean {
			var ean13Data:EAN13DecodedData = data as EAN13DecodedData;

			return isValidBarcode(ean13Data.toString());
		}

		public function decodeBarcode(data:IEncodedBarcodeData):IDecodedBarcodeData {
			var startTime:int = getTimer();
			var barcode:String;

			if (data.encodedData[0] is Array) {
				for (var i:uint = 0; i < data.encodedData.length; i++) {
					barcode = processScanData(data.encodedData[i], data.indexes[i].start, data.indexes[i].end);
					if (barcode && isValidBarcode(barcode)) {
						break;
					}
				}
			}
			else {
				barcode = processScanData(data.encodedData, data.indexes[0].start, data.indexes[0].end);
			}
			trace("decodeBarcode", getTimer() - startTime);
			return new EAN13DecodedData(barcode);
		}

		public function isValidEncoding(data:IEncodedBarcodeData):Boolean {
			//endY - startY >= 94

			return false;
		}

		public function get name():String {
			return DEFINITION;
		}

		private function isValidBarcode(barcodeData:String):Boolean {
			if (!barcodeData || barcodeData.length != 13) {
				return false;
			}
			var checkSum:uint = barcodeData.charAt(12) as uint;

			return (calculateEAN13Checksum(barcodeData.substring(0,13)) == checkSum);
		}

		private function processScanData(data:Array, start:uint, end:uint):String {

			var result:String = "";
			var firstDigitStr:String = "";
			var cntr:Number = 0;
			var run:Number = 0;
			var tmp:String = "";
			var barWidth:Number = (end - start + 1) / 95;

			for (var i:Number = start; i <= end; i++) {

				if (data[i] != data[i + 1]) {
					var numDigits:Number = Math.min(Math.max(Math.round(run / barWidth), 1), 4);
					run = 0;

					if (guards[cntr] == null) {
						for (var z:Number = 0; z < numDigits; z++) {
							tmp += String(data[i]);
						}
						if (tmp.length == 7) {
							if (digits[tmp] == null) {
								return null;
							}
							result += digits[tmp].n;
							if (cntr < 33) {
								if (digits[tmp].en == null) {
									return null;
								}
								firstDigitStr += digits[tmp].en;
							}
							tmp = "";
						}
					}
					cntr++;
				}
				run++
			}
			if (firstDigit[firstDigitStr] == null) {
				return null;
			}

			return firstDigit[firstDigitStr] + result;
		}

		private function calculateEAN13Checksum(messageString:String):Number {
			var csumTotal:Number = 0; // The checksum working variable starts at zero

			// If the source message string is less than 12 characters long, we make it 12 characters
			if (messageString.length < 12) {
				var holdString:String = "000000000000" + messageString;
				messageString = holdString.substring(holdString.length - 12, holdString.length);
			}

			// Calculate the checksum value for the message
			for(var charPos:Number = messageString.length - 1; charPos >= 0; charPos--) {
				if(charPos % 2 == 0) {
					csumTotal = csumTotal + Number(messageString.substring(charPos, charPos+1));
				}
				else {
					csumTotal = csumTotal + 3 * Number(messageString.substring(charPos, charPos+1));
				}
			}

			// Calculate the checksum digit
			var remainder:Number = csumTotal % 10;
			if(remainder == 0 ) {
				return 0;
			}
			else {
				return (10 - remainder);
			}
		}

		private function initDictionaries():void {
			guards["0"] = 1;
			guards["1"] = 0;
			guards["2"] = 1;
			guards["27"] = 0;
			guards["28"] = 1;
			guards["29"] = 0;
			guards["30"] = 1;
			guards["31"] = 0;
			guards["56"] = 1;
			guards["57"] = 0;
			guards["58"] = 1;

			digits["0001101"] = {n:0, en:0};
			digits["0011001"] = {n:1, en:0};
			digits["0010011"] = {n:2, en:0};
			digits["0111101"] = {n:3, en:0};
			digits["0100011"] = {n:4, en:0};
			digits["0110001"] = {n:5, en:0};
			digits["0101111"] = {n:6, en:0};
			digits["0111011"] = {n:7, en:0};
			digits["0110111"] = {n:8, en:0};
			digits["0001011"] = {n:9, en:0};
			digits["0100111"] = {n:0, en:1};
			digits["0110011"] = {n:1, en:1};
			digits["0011011"] = {n:2, en:1};
			digits["0100001"] = {n:3, en:1};
			digits["0011101"] = {n:4, en:1};
			digits["0111001"] = {n:5, en:1};
			digits["0000101"] = {n:6, en:1};
			digits["0010001"] = {n:7, en:1};
			digits["0001001"] = {n:8, en:1};
			digits["0010111"] = {n:9, en:1};
			digits["1110010"] = {n:0, en:2};
			digits["1100110"] = {n:1, en:2};
			digits["1101100"] = {n:2, en:2};
			digits["1000010"] = {n:3, en:2};
			digits["1011100"] = {n:4, en:2};
			digits["1001110"] = {n:5, en:2};
			digits["1010000"] = {n:6, en:2};
			digits["1000100"] = {n:7, en:2};
			digits["1001000"] = {n:8, en:2};
			digits["1110100"] = {n:9, en:2};

			firstDigit["000000"] = 0;
			firstDigit["001011"] = 1;
			firstDigit["001101"] = 2;
			firstDigit["001110"] = 3;
			firstDigit["010011"] = 4;
			firstDigit["011001"] = 5;
			firstDigit["011100"] = 6;
			firstDigit["010101"] = 7;
			firstDigit["010110"] = 8;
			firstDigit["011010"] = 9;
		}

	}
}