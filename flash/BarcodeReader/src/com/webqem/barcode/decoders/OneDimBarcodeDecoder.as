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

package com.webqem.barcode.decoders {

	import com.webqem.barcode.core.AbstractDecoder;
	import com.webqem.barcode.core.ISymbology;
	import com.webqem.barcode.symbology.EAN13.EAN13Symbology;
	import com.webqem.barcode.symbology.IOneDimSymbology;

	import flash.utils.getDefinitionByName;

	public class OneDimBarcodeDecoder extends AbstractDecoder {

		public static const EAN13_SYMBOLGY:String = EAN13Symbology.DEFINITION;

		public function OneDimBarcodeDecoder(symbologyType:String) {
			super(symbologyType);
		}

		protected override function createSymbology(symbologyType:String):ISymbology {
			var Symbology:Class = getDefinitionByName(symbologyType) as Class;
			var symbology:IOneDimSymbology = new Symbology();
			return symbology;
		}

	}
}