package {
	public class Util {
		public static function min(a:int, b:int):int {
			return a < b ? a : b;
		}

		public static function max(a:int, b:int):int {
			return a > b ? a : b;
		}

		public static function abs(a:int):int {
			return a < 0 ? -a : a;
		}

		private static var nPopCount16 = new Array(65536);

		private static function clinit():Object {
			var i:int;
			for (i = 0; i < 65536; i ++) {
				var n:int = ((i >> 1) & 0x5555) + (i & 0x5555);
				n = ((n >> 2) & 0x3333) + (n & 0x3333);
				n = ((n >> 4) & 0x0f0f) + (n & 0x0f0f);
				nPopCount16[i] = ((n >> 8) + (n & 0x00ff));
			}
			return null;
		}

		private static const _clinit_:Object = clinit();

		public static function POP_COUNT_16(nData:int):int {
			return nPopCount16[nData];
		}

		public static function binarySearch(vl:uint, vls:Array, nFrom:int, nTo:int):int {
			var vlLow:int = nFrom;
			var vlHigh:int = nTo - 1;
			while (vlLow <= vlHigh) {
				var vlMid:int = (vlLow + vlHigh) / 2;
				if (vls[vlMid] < vl) {
					vlLow = vlMid + 1;
				} else if (vls[vlMid] > vl) {
					vlHigh = vlMid - 1;
				} else {
					return vlMid;
				}
			}
			return -1;
		}

		private static const cnShellStep:Array = new Array(0, 1, 4, 13, 40, 121, 364, 1093);

		public static function shellSort(mvs:Array, vls:Array, nFrom:int, nTo:int):void {
			var nStepLevel:int = 1;
			while (cnShellStep[nStepLevel] < nTo - nFrom) {
				nStepLevel ++;
			}
			nStepLevel --;
			while (nStepLevel > 0) {
				var nStep:int = cnShellStep[nStepLevel];
				var i:int;
				for (i = nFrom + nStep; i < nTo; i ++) {
					var mvBest:int = mvs[i];
					var vlBest:int = vls[i];
					var j:int = i - nStep;
					while (j >= nFrom && vlBest > vls[j]) {
						mvs[j + nStep] = mvs[j];
						vls[j + nStep] = vls[j];
						j -= nStep;
					}
					mvs[j + nStep] = mvBest;
					vls[j + nStep] = vlBest;
				}
				nStepLevel --;
			}
		}
	}
}