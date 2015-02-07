package {
	public class SortItem {
		private static const PHASE_HASH:int = 0;
		private static const PHASE_KILLER_1:int = 1;
		private static const PHASE_KILLER_2:int = 2;
		private static const PHASE_GEN_MOVES:int = 3;
		private static const PHASE_REST:int = 4;

		private var search:Search;
		private var nIndex:int, nMoves:int, nPhase:int;
		private var mvHash:int, mvKiller1:int, mvKiller2:int;
		private var mvs:Array, vls:Array;

		public function SortItem(search_:Search, mvHash_:int) {
			search = search_;
			mvHash = mvHash_;
			nPhase = PHASE_HASH;
			mvKiller1 = search.mvKiller1[search.pos.nDistance];
			mvKiller2 = search.mvKiller2[search.pos.nDistance];
		}

		public function nextMove():int {
			if (nPhase == PHASE_HASH) {
				nPhase = PHASE_KILLER_1;
				if (mvHash > 0) {
					return mvHash;
				}
			}
			if (nPhase == PHASE_KILLER_1) {
				nPhase = PHASE_KILLER_2;
				if (mvKiller1 != mvHash && mvKiller1 > 0 && search.pos.legalMove(mvKiller1)) {
					return mvKiller1;
				}
			}
			if (nPhase == PHASE_KILLER_2) {
				nPhase = PHASE_GEN_MOVES;
				if (mvKiller2 != mvHash && mvKiller2 > 0 && search.pos.legalMove(mvKiller2)) {
					return mvKiller2;
				}
			}
			if (nPhase == PHASE_GEN_MOVES) {
				nPhase = PHASE_REST;
				mvs = new Array(Position.MAX_GEN_MOVES);
				vls = new Array(Position.MAX_GEN_MOVES);
				nMoves = search.pos.generateMoves(mvs);
				var i:int;
				for (i = 0; i < nMoves; i ++) {
					vls[i] = search.nHistoryTable[search.pos.historyIndex(mvs[i])];
				}
				Util.shellSort(mvs, vls, 0, nMoves);
				nIndex = 0;
			}
			while (nIndex < nMoves) {
				var mv:int = mvs[nIndex];
				nIndex ++;
				if (mv != mvHash && mv != mvKiller1 && mv != mvKiller2) {
					return mv;
				}
			}
			return 0;
		}
	}
}