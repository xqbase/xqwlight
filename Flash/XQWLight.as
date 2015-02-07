/*
XQWLight.as - Source Code for XiangQi Wizard Light, Part III

XiangQi Wizard Light - a Flash Chinese Chess Program
Designed by Morning Yellow, Version: 1.42, Last Modified: May 2010
Copyright (C) 2004-2010 www.xqbase.com

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
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.external.ExternalInterface;
	import flash.media.Sound;
	import flash.text.TextField;
	import flash.utils.Timer;

	public class XQWLight extends Sprite {
		private static const STARTUP_FEN = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";

		private static const PHASE_LOADING:int = 0;
		private static const PHASE_WAITING:int = 1;
		private static const PHASE_THINKING:int = 2;

		private static const COMPUTER_BLACK:int = 0;
		private static const COMPUTER_RED:int = 1;
		private static const COMPUTER_NONE:int = 2;

		private static const RESP_CLICK:int = 0;
		private static const RESP_ILLEGAL:int = 1;
		private static const RESP_MOVE:int = 2;
		private static const RESP_MOVE2:int = 3;
		private static const RESP_CAPTURE:int = 4;
		private static const RESP_CAPTURE2:int = 5;
		private static const RESP_CHECK:int = 6;
		private static const RESP_CHECK2:int = 7;
		private static const RESP_WIN:int = 8;
		private static const RESP_DRAW:int = 9;
		private static const RESP_LOSS:int = 10;

		private static const BOARD_EDGE:int = 8;
		private static const SQUARE_SIZE:int = 56;
		private static const BITMAP_SIZE:int = 57;
		private static const THINKING_SIZE:int = 48;
		private static const BOARD_WIDTH:int = BOARD_EDGE + SQUARE_SIZE * 9 + BOARD_EDGE;
		private static const BOARD_HEIGHT:int = BOARD_EDGE + SQUARE_SIZE * 10 + BOARD_EDGE;

		private static const bmpOo:BitmapData = new EmptySquare(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRk:BitmapData = new RedKing(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRa:BitmapData = new RedAdvisor(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRb:BitmapData = new RedBishop(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRn:BitmapData = new RedKnight(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRr:BitmapData = new RedRook(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRc:BitmapData = new RedCannon(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRp:BitmapData = new RedPawn(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBk:BitmapData = new BlackKing(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBa:BitmapData = new BlackAdvisor(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBb:BitmapData = new BlackBishop(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBn:BitmapData = new BlackKnight(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBr:BitmapData = new BlackRook(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBc:BitmapData = new BlackCannon(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBp:BitmapData = new BlackPawn(BITMAP_SIZE, BITMAP_SIZE);

		private static const bmpOos:BitmapData = new EmptySquareSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRks:BitmapData = new RedKingSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRas:BitmapData = new RedAdvisorSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRbs:BitmapData = new RedBishopSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRns:BitmapData = new RedKnightSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRrs:BitmapData = new RedRookSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRcs:BitmapData = new RedCannonSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpRps:BitmapData = new RedPawnSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBks:BitmapData = new BlackKingSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBas:BitmapData = new BlackAdvisorSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBbs:BitmapData = new BlackBishopSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBns:BitmapData = new BlackKnightSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBrs:BitmapData = new BlackRookSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBcs:BitmapData = new BlackCannonSelected(BITMAP_SIZE, BITMAP_SIZE);
		private static const bmpBps:BitmapData = new BlackPawnSelected(BITMAP_SIZE, BITMAP_SIZE);

		private static const sndClick:Sound = new ClickSound();
		private static const sndIllegal:Sound = new IllegalSound();
		private static const sndMove:Sound = new MoveSound();
		private static const sndMove2:Sound = new Move2Sound();
		private static const sndCapture:Sound = new CaptureSound();
		private static const sndCapture2:Sound = new Capture2Sound();
		private static const sndCheck:Sound = new CheckSound();
		private static const sndCheck2:Sound = new Check2Sound();
		private static const sndWin:Sound = new WinSound();
		private static const sndDraw:Sound = new DrawSound();
		private static const sndLoss:Sound = new LossSound();

		private static const bmpPieces:Array = new Array(
			bmpOo, null, null, null, null, null, null, null,
			bmpRk, bmpRa, bmpRb, bmpRn, bmpRr, bmpRc, bmpRp, null,
			bmpBk, bmpBa, bmpBb, bmpBn, bmpBr, bmpBc, bmpBp, null
		);

		private static const bmpSelected:Array = new Array(
			bmpOos, null, null, null, null, null, null, null,
			bmpRks, bmpRas, bmpRbs, bmpRns, bmpRrs, bmpRcs, bmpRps, null,
			bmpBks, bmpBas, bmpBbs, bmpBns, bmpBrs, bmpBcs, bmpBps, null
		);

		private static const sndResponse:Array = new Array(
			sndClick, sndIllegal, sndMove, sndMove2, sndCapture, sndCapture2,
			sndCheck, sndCheck2, sndWin, sndDraw, sndLoss
		);

		private var bmpThinking:Bitmap = new Bitmap();
		private var bmpSquares:Array = new Array(256);
		private var pos:Position = new Position();
		private var search:Search = new Search(pos, 16);
		private var bSound:Boolean = true;
		private var nMoveMode:int = 0, nLevel:int = 0;
		private var sqSelected:int = 0, mvLast:int = 0;
		private var sdCurrent:int, sdRetract:int = 0;
		private var pcCurrent:Array = new Array(256), pcRetract:Array = new Array(256);
		private var nPhase:int = PHASE_LOADING;

		private static const DRAW_SELECTED:Boolean = true;

		private function drawSquare(sq:int, bSelected:Boolean = false):void {
			var pc:int = pos.pcSquares[sq];
			sq = nMoveMode == COMPUTER_RED ? Position.SQUARE_FLIP(sq) : sq;
			bmpSquares[sq].bitmapData = bSelected ? bmpSelected[pc] : bmpPieces[pc];
		}

		private function drawMove(mv:int, bSelected:Boolean = false):void {
			drawSquare(Position.SRC(mvLast), bSelected);
			drawSquare(Position.DST(mvLast), bSelected);
		}

		private function playSound(nResponse:int):void {
			if (bSound) {
				Sound(sndResponse[nResponse]).play();
			}
		}

		private function setMessage(s:String):void {
			lblMessage.htmlText = "<b><i>" + s + "</b></i>";
			lblMessage.visible = true;
		}

		private function saveCurrent():void {
			sdCurrent = pos.sdPlayer;
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				pcCurrent[sq] = pos.pcSquares[sq];
			}
		}

		private function saveRetract():void {
			sdRetract = sdCurrent;
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				pcRetract[sq] = pcCurrent[sq];
			}
			saveCurrent();
		}

		private function getResult(nResponse:int = -1):Boolean {
			if (pos.isMate()) {
				playSound(nResponse < 0 ? RESP_WIN : RESP_LOSS);
				setMessage(nResponse < 0 ? "祝贺你取得胜利！" : "请再接再厉！");
				return true;
			}
			var vlRep:int = pos.repStatus(3);
			if (vlRep > 0) {
				vlRep = (nResponse < 0 ? pos.repValue(vlRep) : -pos.repValue(vlRep));
				playSound(vlRep > Position.WIN_VALUE ? RESP_LOSS :
						vlRep < -Position.WIN_VALUE ? RESP_WIN : RESP_DRAW);
				setMessage(vlRep > Position.WIN_VALUE ? "长打作负，请不要气馁！" :
						vlRep < -Position.WIN_VALUE ? "电脑长打作负，祝贺你取得胜利！" : "双方不变作和，辛苦了！");
				return true;
			}
			if (pos.nMoveNum > 100) {
				playSound(RESP_DRAW);
				setMessage("超过自然限着作和，辛苦了！");
				return true;
			}
			if (nResponse >= 0) {
				playSound(nResponse);
				saveRetract();
			}
			return false;
		}

		private function addMove(mv:int):Boolean {
			if (pos.legalMove(mv)) {
				if (pos.makeMove(mv)) {
					mvLast = mv;
					drawMove(mvLast, DRAW_SELECTED);
					sqSelected = 0;
					playSound(pos.inCheck() ? RESP_CHECK : pos.captured() ? RESP_CAPTURE : RESP_MOVE);
					if (pos.captured()) {
						pos.setIrrev();
					}
					return true;
				} else {
					playSound(RESP_ILLEGAL);
				}
			}
			return false;
		}

		private function responseMove(e:TimerEvent):void {
			var mv:int = search.searchMain(1000 << (nLevel << 1));
			if (mvLast > 0) {
				drawMove(mvLast);
			}
			mvLast = mv;
			pos.makeMove(mvLast);
			drawMove(mvLast, DRAW_SELECTED);
			var nResponse:int = pos.inCheck() ? RESP_CHECK2 : pos.captured() ? RESP_CAPTURE2 : RESP_MOVE2;
			if (pos.captured()) {
				pos.setIrrev();
			}
			bmpThinking.visible = false;
			nPhase = getResult(nResponse) ? PHASE_LOADING : PHASE_WAITING;
		}

		private function thinking():void {
			nPhase = PHASE_THINKING;
			var sq:int = Position.DST(mvLast);
			bmpThinking.visible = true;
			var timer:Timer = new Timer(100, 1);
			timer.addEventListener(TimerEvent.TIMER, responseMove);
			timer.start();
		}

		private function clickSquare(sq:int):void {
			sq = nMoveMode == COMPUTER_RED ? Position.SQUARE_FLIP(sq) : sq;
			var pc:int = pos.pcSquares[sq];
			if ((pc & Position.SIDE_TAG(pos.sdPlayer)) != 0) {
				if (sqSelected != 0) {
					drawSquare(sqSelected);
				}
				sqSelected = sq;
				drawSquare(sq, DRAW_SELECTED);
				if (mvLast != 0) {
					drawMove(mvLast);
				}
				playSound(RESP_CLICK);
			} else if (sqSelected != 0) {
				var mv:int = Position.MOVE(sqSelected, sq);
				if (addMove(mv)) {
					if (getResult()) {
						nPhase = PHASE_LOADING;
					} else if (nMoveMode == COMPUTER_NONE) {
						saveRetract();
					} else {
						thinking();
					}
				}
			}
		}

		private function onClick(e:MouseEvent):void {
			if (Position.nBookSize >= 0 && nPhase == PHASE_WAITING) {
				var xx:int = Position.FILE_LEFT + (e.localX - BOARD_EDGE) / SQUARE_SIZE;
				var yy:int = Position.RANK_TOP + (e.localY - BOARD_EDGE) / SQUARE_SIZE;
				if (xx >= Position.FILE_LEFT && xx <= Position.FILE_RIGHT &&
						yy >= Position.RANK_TOP && yy <= Position.RANK_BOTTOM) {
					clickSquare(Position.COORD_XY(xx, yy));
				}
			}
		}

		private function drawBoard():void {
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				if (Position.IN_BOARD(sq)) {
					drawSquare(sq);
				}
			}
		}

		private function restart(nMoveMode_:int, szFen:String):void {
			if (nPhase == PHASE_THINKING) {
				return;
			}
			nPhase = PHASE_LOADING;
			nMoveMode = nMoveMode_;
			pos.fromFen(szFen);
			sdCurrent = pos.sdPlayer;
			saveCurrent();
			saveRetract();
			sqSelected = mvLast = 0;
			drawBoard();
			lblMessage.visible = false;
			if (sdCurrent == 0 ? nMoveMode == COMPUTER_RED : nMoveMode == COMPUTER_BLACK) {
				thinking();
			} else {
				nPhase = PHASE_WAITING;
			}
		}

		private function retract():void {
			lblMessage.visible = false;
			pos.clearBoard();
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				if (pcRetract[sq] > 0) {
					pos.addPiece(sq, pcRetract[sq]);
				}
			}
			if (sdRetract == 1) {
				pos.changeSide();
			}
			pos.setIrrev();
			saveCurrent();
			sqSelected = mvLast = 0;
			drawBoard();
			if (sdCurrent == 0 ? nMoveMode == COMPUTER_RED : nMoveMode == COMPUTER_BLACK) {
				thinking();
			} else {
				nPhase = PHASE_WAITING
			}
		}

		private function setLevel(nLevel_:int):void {
			nLevel = nLevel_;
		}

		private function setSound(bSound_:Boolean):void {
			bSound = bSound_;
			playSound(RESP_CLICK);
		}

		public function XQWLight() {
			var board:Sprite = new Sprite();
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				if (Position.IN_BOARD(sq)) {
					bmpSquares[sq] = new Bitmap();
					bmpSquares[sq].x = BOARD_EDGE + (Position.FILE_X(sq) - Position.FILE_LEFT) * SQUARE_SIZE;
					bmpSquares[sq].y = BOARD_EDGE + (Position.RANK_Y(sq) - Position.RANK_TOP) * SQUARE_SIZE;
					board.addChild(bmpSquares[sq]);
				}
			}
			board.addEventListener(MouseEvent.MOUSE_DOWN, onClick);
			bmpThinking.bitmapData = new ThinkingImage(THINKING_SIZE, THINKING_SIZE);
			bmpThinking.x = (BOARD_WIDTH - THINKING_SIZE) / 2;
			bmpThinking.y = (BOARD_HEIGHT - THINKING_SIZE) / 2;
			bmpThinking.visible = false;
			board.addChild(bmpThinking);
			addChild(board);
			setChildIndex(lblMessage, numChildren - 1);
			var nMode:int = loaderInfo.parameters.mode;
			var szFen:String = loaderInfo.parameters.fen;
			if (szFen == null) {
				szFen = STARTUP_FEN;
			}
			restart(nMode, szFen);
			ExternalInterface.addCallback("restart", restart);
			ExternalInterface.addCallback("retract", retract);
			ExternalInterface.addCallback("setLevel", setLevel);
			ExternalInterface.addCallback("setSound", setSound);
		}
	}
}