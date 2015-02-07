package {
	public class RC4 {
		private var nState:Array = new Array(256);
		private var x:int, y:int;

		private function swap(i:int, j:int):void {
			var t:int = nState[i];
			nState[i] = nState[j];
			nState[j] = t;
		}

		public function RC4(nKey:Array) {
			x = y = 0;
			var i:int;
			for (i = 0; i < 256; i ++) {
				nState[i] = i;
			}
			var j:int = 0;
			for (i = 0; i < 256; i ++) {
				j = (j + nState[i] + nKey[i % nKey.length]) & 0xff;
				swap(i, j);
			}
		}

		public function nextByte():int {
			x = (x + 1) & 0xff;
			y = (y + nState[x]) & 0xff;
			swap(x, y);
			var t:int = (nState[x] + nState[y]) & 0xff;
			return nState[t];
		}

		public function nextLong():uint {
			var dw0:uint, dw1:uint, dw2:uint, dw3:uint;
			dw0 = nextByte();
			dw1 = nextByte();
			dw2 = nextByte();
			dw3 = nextByte();
			return dw0 + (dw1 << 8) + (dw2 << 16) + (dw3 << 24);
		}
	}
}