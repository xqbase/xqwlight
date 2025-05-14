/*
Search.as - Source Code for XiangQi Wizard Light, Part II

XiangQi Wizard Light - a Flash Chinese Chess Program
Designed by Morning Yellow, Version: 1.0, Last Modified: Jul. 2008
Copyright (C) 2004-2008 www.xqbase.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

package {
	public class Search {
		private static const HASH_ALPHA:int = 1;
		private static const HASH_BETA:int = 2;
		private static const HASH_PV:int = 3;
		private static const LIMIT_DEPTH:int = 64;
		private static const NULL_DEPTH:int = 2;
		private static const RANDOM_MASK:int = 7;
		private static const MAX_GEN_MOVES:int = Position.MAX_GEN_MOVES;
		private static const MATE_VALUE:int = Position.MATE_VALUE;
		private static const BAN_VALUE:int = Position.BAN_VALUE;
		private static const WIN_VALUE:int = Position.WIN_VALUE;
		private static const NO_NULL:Boolean = true;

		private var nHashMask:int, mvResult:int, nAllNodes:int, nAllMillis:int;
		private var hshTable:Array;

		public var pos:Position;
		public var nHistoryTable = new Array(4096);
		public var mvKiller1 = new Array(LIMIT_DEPTH);
		public var mvKiller2 = new Array(LIMIT_DEPTH);

		public function Search(pos_:Position, nHashLevel:int) {
			pos = pos_;
			nHashMask = (1 << nHashLevel) - 1;
			hshTable = new Array(nHashMask + 1);
			var i:int;
			for (i = 0; i <= nHashMask; i ++) {
				hshTable[i] = new HashItem();
			}
		}

		private function getHashItem():HashItem {
			return hshTable[pos.dwKey & nHashMask];
		}

		private function probeHash(vlAlpha:int, vlBeta:int, nDepth:int, mv:Array):int {
			var hsh:HashItem = getHashItem();
			if (hsh.dwLock != pos.dwLock) {
				mv[0] = 0;
				return -MATE_VALUE;
			}
			mv[0] = hsh.mv;
			var bMate:Boolean = false;
			if (hsh.vl > WIN_VALUE) {
				if (hsh.vl <= BAN_VALUE) {
					return -MATE_VALUE;
				}
				hsh.vl -= pos.nDistance;
				bMate = true;
			} else if (hsh.vl < -WIN_VALUE) {
				if (hsh.vl >= -BAN_VALUE) {
					return -MATE_VALUE;
				}
				hsh.vl += pos.nDistance;
				bMate = true;
			} else if (hsh.vl == pos.drawValue()) {
				return -MATE_VALUE;
			}
			if (hsh.nDepth >= nDepth || bMate) {
				if (hsh.nFlag == HASH_BETA) {
					return (hsh.vl >= vlBeta ? hsh.vl : -MATE_VALUE);
				} else if (hsh.nFlag == HASH_ALPHA) {
					return (hsh.vl <= vlAlpha ? hsh.vl : -MATE_VALUE);
				}
				return hsh.vl;
			}
			return -MATE_VALUE;
		}

		private function recordHash(nFlag:int, vl:int, nDepth:int, mv:int):void {
			var hsh:HashItem = getHashItem();
			if (hsh.nDepth > nDepth) {
				return;
			}
			hsh.nFlag = nFlag;
			hsh.nDepth = nDepth;
			if (vl > WIN_VALUE) {
				if (mv == 0 && vl <= BAN_VALUE) {
					return;
				}
				hsh.vl = vl + pos.nDistance;
			} else if (vl < -WIN_VALUE) {
				if (mv == 0 && vl >= -BAN_VALUE) {
					return;
				}
				hsh.vl = vl - pos.nDistance;
			} else if (vl == pos.drawValue() && mv == 0) {
				return;
			} else {
				hsh.vl = vl;
			}
			hsh.mv = mv;
			hsh.dwLock = pos.dwLock;
		}

		private function setBestMove(mv:int, nDepth:int):void {
			nHistoryTable[pos.historyIndex(mv)] += nDepth * nDepth;
			if (mvKiller1[pos.nDistance] != mv) {
				mvKiller2[pos.nDistance] = mvKiller1[pos.nDistance];
				mvKiller1[pos.nDistance] = mv;
			}
		}

		private function searchQuiesc(vlAlpha:int, vlBeta:int):int {
			nAllNodes ++;
			var vl:int = pos.nDistance - MATE_VALUE;
			if (vl >= vlBeta) {
				return vl;
			}
			var vlRep:int = pos.repStatus();
			if (vlRep > 0) {
				return pos.repValue(vlRep);
			}
			if (pos.nDistance == LIMIT_DEPTH) {
				return pos.evaluate();
			}
			var vlBest:int = -MATE_VALUE;
			var i:int, nGenMoves:int;
			var mvs:Array = new Array(MAX_GEN_MOVES);
			var vls:Array = new Array(MAX_GEN_MOVES);
			if (pos.inCheck()) {
				nGenMoves = pos.generateMoves(mvs);
				for (i = 0; i < nGenMoves; i ++) {
					vls[i] = nHistoryTable[pos.historyIndex(mvs[i])];
				}
				Util.shellSort(mvs, vls, 0, nGenMoves);
			} else {
				vl = pos.evaluate();
				if (vl > vlBest) {
					if (vl >= vlBeta) {
						return vl;
					}
					vlBest = vl;
					if (vl > vlAlpha) {
						vlAlpha = vl;
					}
				}
				nGenMoves = pos.generateMoves(mvs, vls);
				Util.shellSort(mvs, vls, 0, nGenMoves);
				for (i = 0; i < nGenMoves; i ++) {
					if (vls[i] < 10 || (vls[i] < 20 && Position.HOME_HALF(Position.DST(mvs[i]), pos.sdPlayer))) {
						nGenMoves = i;
						break;
					}
				}
			}
			for (i = 0; i < nGenMoves; i ++) {
				if (!pos.makeMove(mvs[i])) {
					continue;
				}
				vl = -searchQuiesc(-vlBeta, -vlAlpha);
				pos.undoMakeMove();
				if (vl > vlBest) {
					if (vl >= vlBeta) {
						return vl;
					}
					vlBest = vl;
					if (vl > vlAlpha) {
						vlAlpha = vl;
					}
				}
			}
			return vlBest == -MATE_VALUE ? pos.nDistance - MATE_VALUE: vlBest;
		}

		private function searchFull(vlAlpha:int, vlBeta:int, nDepth:int, bNoNull:Boolean = false):int {
			if (nDepth <= 0) {
				return searchQuiesc(vlAlpha, vlBeta);
			}
			nAllNodes ++;
			var vl:int = pos.nDistance - MATE_VALUE;
			if (vl >= vlBeta) {
				return vl;
			}
			var vlRep:int = pos.repStatus();
			if (vlRep > 0) {
				return pos.repValue(vlRep);
			}
			var mvHash:Array = new Array(1);
			vl = probeHash(vlAlpha, vlBeta, nDepth, mvHash);
			if (vl > -MATE_VALUE) {
				return vl;
			}
			if (pos.nDistance == LIMIT_DEPTH) {
				return pos.evaluate();
			}
			if (!bNoNull && !pos.inCheck() && pos.nullOkay()) {
				pos.nullMove();
				vl = -searchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
				pos.undoNullMove();
				if (vl >= vlBeta && (pos.nullSafe() || searchFull(vlAlpha, vlBeta, nDepth - NULL_DEPTH, NO_NULL) >= vlBeta)) {
					return vl;
				}
			}
			var nHashFlag:int = HASH_ALPHA;
			var vlBest:int = -MATE_VALUE;
			var mvBest:int = 0;
			var sort:SortItem = new SortItem(this, mvHash[0]);
			var mv:int;
			while ((mv = sort.nextMove()) > 0) {
				if (!pos.makeMove(mv)) {
					continue;
				}
				var nNewDepth:int = pos.inCheck() ? nDepth : nDepth - 1;
				if (vlBest == -MATE_VALUE) {
					vl = -searchFull(-vlBeta, -vlAlpha, nNewDepth);
				} else {
					vl = -searchFull(-vlAlpha - 1, -vlAlpha, nNewDepth);
					if (vl > vlAlpha && vl < vlBeta) {
						vl = -searchFull(-vlBeta, -vlAlpha, nNewDepth);
					}
				}
				pos.undoMakeMove();
				if (vl > vlBest) {
					vlBest = vl;
					if (vl >= vlBeta) {
						nHashFlag = HASH_BETA;
						mvBest = mv;
						break;
					}
					if (vl > vlAlpha) {
						vlAlpha = vl;
						nHashFlag = HASH_PV;
						mvBest = mv;
					}
				}
			}
			if (vlBest == -MATE_VALUE) {
				return pos.nDistance - MATE_VALUE;
			}
			recordHash(nHashFlag, vlBest, nDepth, mvBest);
			if (mvBest > 0) {
				setBestMove(mvBest, nDepth);
			}
			return vlBest;
		}

		private function searchRoot(nDepth):int {
			var vlBest:int = -MATE_VALUE;
			var sort:SortItem = new SortItem(this, mvResult);
			var mv:int;
			while ((mv = sort.nextMove()) > 0) {
				if (!pos.makeMove(mv)) {
					continue;
				}
				var nNewDepth = pos.inCheck() ? nDepth : nDepth - 1;
				var vl:int;
				if (vlBest == -MATE_VALUE) {
					vl = -searchFull(-MATE_VALUE, MATE_VALUE, nNewDepth, NO_NULL);
				} else {
					vl = -searchFull(-vlBest - 1, -vlBest, nNewDepth);
					if (vl > vlBest) {
						vl = -searchFull(-MATE_VALUE, -vlBest, nNewDepth, NO_NULL);
					}
				}
				pos.undoMakeMove();
				if (vl > vlBest) {
					vlBest = vl;
					mvResult = mv;
					if (vlBest > -WIN_VALUE && vlBest < WIN_VALUE) {
						vlBest += int(Math.random() * RANDOM_MASK) - int(Math.random() * RANDOM_MASK);
						vlBest = (vlBest == pos.drawValue() ? vlBest - 1 : vlBest);
					}
				}
			}
			setBestMove(mvResult, nDepth);
			return vlBest;
		}

		public function searchMain(nMillis:int, nDepth:int = LIMIT_DEPTH):int {
			mvResult = pos.bookMove();
			if (mvResult > 0) {
				pos.makeMove(mvResult);
				if (pos.repStatus(3) == 0) {
					pos.undoMakeMove();
					return mvResult;
				}
				pos.undoMakeMove();
			}
			var vl:int = 0;
			var mvs:Array = new Array(MAX_GEN_MOVES);
			var nGenMoves:int = pos.generateMoves(mvs);
			var i:int;
			for (i = 0; i < nGenMoves; i ++) {
				if (pos.makeMove(mvs[i])) {
					pos.undoMakeMove();
					mvResult = mvs[i];
					vl ++;
				}
			}
			if (vl == 1) {
				return mvResult;
			}
			for (i = 0; i <= nHashMask; i ++) {
				var hsh:HashItem = hshTable[i];
				hsh.nDepth = hsh.nFlag = hsh.vl = hsh.mv = 0;
				hsh.dwLock = 0;
			}
			for (i = 0; i < LIMIT_DEPTH; i ++) {
				mvKiller1[i] = mvKiller2[i] = 0;
			}
			for (i = 0; i < 4096; i ++) {
				nHistoryTable[i] = 0;
			}
			mvResult = 0;
			nAllNodes = 0;
			pos.nDistance = 0;
			var t:Number = new Date().getTime();
			for (i = 1; i <= nDepth; i ++) {
				vl = searchRoot(i);
				if (vl > WIN_VALUE || vl < -WIN_VALUE) {
					break;
				}
				nAllMillis = new Date().getTime() - t;
				if (nAllMillis > nMillis) {
					break;
				}
			}
			return mvResult;
		}

		public function getKNPS():int {
			return nAllNodes / nAllMillis;
		}
	}
}