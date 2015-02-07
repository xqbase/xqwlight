/*
Position.as - Source Code for XiangQi Wizard Light, Part I

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
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLRequest;
	import flash.net.URLStream;
	import flash.utils.Endian;

	public class Position {
		public static const MATE_VALUE:int = 10000;
		public static const BAN_VALUE:int = MATE_VALUE - 100;
		public static const WIN_VALUE:int = MATE_VALUE - 200;
		public static const NULL_SAFE_MARGIN:int = 400;
		public static const NULL_OKAY_MARGIN:int = 200;
		public static const DRAW_VALUE:int = 20;
		public static const ADVANCED_VALUE:int = 3;

		public static const MAX_MOVE_NUM:int = 256;
		public static const MAX_GEN_MOVES:int = 128;
		// public static const MAX_BOOK_SIZE:int = 16384;

		public static const PIECE_KING:int = 0;
		public static const PIECE_ADVISOR:int = 1;
		public static const PIECE_BISHOP:int = 2;
		public static const PIECE_KNIGHT:int = 3;
		public static const PIECE_ROOK:int = 4;
		public static const PIECE_CANNON:int = 5;
		public static const PIECE_PAWN:int = 6;

		public static const RANK_TOP:int = 3;
		public static const RANK_BOTTOM:int = 12;
		public static const FILE_LEFT:int = 3;
		public static const FILE_RIGHT:int = 11;

		public static const DEL_PIECE:Boolean = true;

		private static const cnInBoard:Array = new Array(
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		);

		private static const cnInFort:Array = new Array(
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		);

		private static const cnLegalSpan:Array = new Array(
			                     0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0
		);

		private static const cnKnightPin:Array = new Array(
			                            0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0
		);

		private static const cnKingDelta:Array = new Array(-16, -1, 1, 16);
		private static const cnAdvisorDelta:Array = new Array(-17, -15, 15, 17);
		private static const cnKnightDelta:Array = new Array(
			new Array(-33, -31), new Array(-18, 14), new Array(-14, 18), new Array(31, 33)
		);
		private static const cnKnightCheckDelta:Array = new Array(
			new Array(-33, -18), new Array(-31, -14), new Array(14, 31), new Array(18, 33)
		);
		private static const cnMvvValue:Array = new Array(50, 10, 10, 30, 40, 30, 20, 0);

		private static const cnpvKingPawn:Array = new Array(
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
			0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
			0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
			0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
			0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
			0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
			0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		);

		private static const cnpvBishopAdvisor:Array = new Array(
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0, 18,  0,  0, 20, 23, 20,  0,  0, 18,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0, 20, 20,  0, 20, 20,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		);

		private static const cnpvKnight:Array = new Array(
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
			0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
			0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
			0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
			0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
			0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
			0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
			0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
			0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
			0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		);

		private static const cnpvRook:Array = new Array(
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
			0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
			0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
			0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
			0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
			0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
			0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
			0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
			0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
			0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		);

		private static const cnpvCannon:Array = new Array(
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
			0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
			0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
			0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
			0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
			0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
			0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
			0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
			0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
			0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		);

		public static var cnPieceValue:Array = new Array(
			cnpvKingPawn, cnpvBishopAdvisor, cnpvBishopAdvisor, cnpvKnight, cnpvRook, cnpvCannon, cnpvKingPawn, null
		);

		public static function IN_BOARD(sq:int):Boolean {
			return cnInBoard[sq] != 0;
		}

		public static function IN_FORT(sq:int):Boolean {
			return cnInFort[sq] != 0;
		}

		public static function RANK_Y(sq:int):int {
			return sq >> 4;
		}

		public static function FILE_X(sq:int):int {
			return sq & 15;
		}

		public static function COORD_XY(x:int, y:int):int {
			return x + (y << 4);
		}

		public static function SQUARE_FLIP(sq:int):int {
			return 254 - sq;
		}

		public static function FILE_FLIP(x:int):int {
			return 14 - x;
		}

		public static function RANK_FLIP(y:int):int {
			return 15 - y;
		}

		public static function MIRROR_SQUARE(sq:int):int {
			return COORD_XY(FILE_FLIP(FILE_X(sq)), RANK_Y(sq));
		}

		public static function SQUARE_FORWARD(sq:int, sd:int):int {
			return sq - 16 + (sd << 5);
		}

		public static function KING_SPAN(sqSrc:int, sqDst:int):Boolean {
			return cnLegalSpan[sqDst - sqSrc + 256] == 1;
		}

		public static function ADVISOR_SPAN(sqSrc:int, sqDst:int):Boolean {
			return cnLegalSpan[sqDst - sqSrc + 256] == 2;
		}

		public static function BISHOP_SPAN(sqSrc:int, sqDst:int):Boolean {
			return cnLegalSpan[sqDst - sqSrc + 256] == 3;
		}

		public static function BISHOP_PIN(sqSrc:int, sqDst:int):int {
			return (sqSrc + sqDst) >> 1;
		}

		public static function KNIGHT_PIN(sqSrc:int, sqDst:int):int {
			return sqSrc + cnKnightPin[sqDst - sqSrc + 256];
		}

		public static function HOME_HALF(sq:int, sd:int):Boolean {
			return (sq & 0x80) != (sd << 7);
		}

		public static function AWAY_HALF(sq:int, sd:int):Boolean {
			return (sq & 0x80) == (sd << 7);
		}

		public static function SAME_HALF(sqSrc:int, sqDst:int):Boolean {
			return ((sqSrc ^ sqDst) & 0x80) == 0;
		}

		public static function SAME_RANK(sqSrc:int, sqDst:int):Boolean {
			return ((sqSrc ^ sqDst) & 0xf0) == 0;
		}

		public static function SAME_FILE(sqSrc:int, sqDst:int):Boolean {
			return ((sqSrc ^ sqDst) & 0x0f) == 0;
		}

		public static function SIDE_TAG(sd:int):int {
			return 8 + (sd << 3);
		}

		public static function OPP_SIDE_TAG(sd:int):int {
			return 16 - (sd << 3);
		}

		public static function SRC(mv:int):int {
			return mv & 255;
		}

		public static function DST(mv:int):int {
			return mv >> 8;
		}

		public static function MOVE(sqSrc:int, sqDst:int):int {
			return sqSrc + (sqDst << 8);
		}

		public static function MIRROR_MOVE(mv:int):int {
			return MOVE(MIRROR_SQUARE(SRC(mv)), MIRROR_SQUARE(DST(mv)));
		}

		public static function MVV_LVA(pc:int, nLva:int):int {
			return cnMvvValue[pc & 7] - nLva;
		}

		public static var dwKeyPlayer:uint;
		public static var dwLockPlayer:uint;
		public static var dwKeyTable:Array = new Array(14);
		public static var dwLockTable:Array = new Array(14);

		public static var nBookSize:int = -1;
		public static var dwBookLock:Array;
		public static var mvBook:Array;
		public static var vlBook:Array;

		private static var stream:URLStream = new URLStream();

		private static function loadBook(e:Event):void {
			var n:int = stream.bytesAvailable / 8;
			dwBookLock = new Array(n);
			mvBook = new Array(n);
			vlBook = new Array(n);
			var i:int;
			for (i = 0; i < n; i ++) {
				dwBookLock[i] = stream.readUnsignedInt();
				mvBook[i] = stream.readUnsignedShort();
				vlBook[i] = stream.readUnsignedShort();
			}
			nBookSize = n;
		}

		private static function noBook(e:Event):void {
			nBookSize = 0;
		}

		private static function clinit():Object {
			var rc4:RC4 = new RC4(new Array(0, 0));
			dwKeyPlayer = rc4.nextLong();
			rc4.nextLong(); // Skip ZobristLock0
			dwLockPlayer = rc4.nextLong();
			var i:int, j:int;
			for (i = 0; i < 14; i ++) {
				dwKeyTable[i] = new Array(256);
				dwLockTable[i] = new Array(256);
				for (j = 0; j < 256; j ++) {
					dwKeyTable[i][j] = rc4.nextLong();
					rc4.nextLong(); // Skip ZobristLock0
					dwLockTable[i][j] = rc4.nextLong();
				}
			}

			stream.endian = Endian.LITTLE_ENDIAN;
			stream.addEventListener(Event.COMPLETE, loadBook);
			stream.addEventListener(IOErrorEvent.IO_ERROR, noBook);
			stream.load(new URLRequest("BOOK.DAT"));
			return null;
		}

		private static const _clinit_:Object = clinit();

		public var sdPlayer:int;
		public var pcSquares:Array = new Array(256);

		public var dwKey:uint, dwLock:uint;
		public var vlWhite:int, vlBlack:int;
		public var nMoveNum:int, nDistance:int;

		public var mvList:Array = new Array(MAX_MOVE_NUM);
		public var pcList:Array = new Array(MAX_MOVE_NUM);
		public var dwKeyList:Array = new Array(MAX_MOVE_NUM);
		public var bCheckList:Array = new Array(MAX_MOVE_NUM);

		public function clearBoard():void {
			sdPlayer = 0;
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				pcSquares[sq] = 0;
			}
			dwKey = dwLock = 0;
			vlWhite = vlBlack = 0;
		}

		public function setIrrev():void {
			mvList[0] = pcList[0] = 0;
			bCheckList[0] = checked();
			nMoveNum = 1;
			nDistance = 0;
		}

		public function addPiece(sq:int, pc:int, bDel:Boolean = false):void {
			var pcAdjust:int;
			pcSquares[sq] = bDel ? 0 : pc;
			if (pc < 16) {
				pcAdjust = pc - 8;
				vlWhite += bDel ? -cnPieceValue[pcAdjust][sq] : cnPieceValue[pcAdjust][sq];
			} else {
				pcAdjust = pc - 16;
				vlBlack += bDel ? -cnPieceValue[pcAdjust][SQUARE_FLIP(sq)] :
						cnPieceValue[pcAdjust][SQUARE_FLIP(sq)];
				pcAdjust += 7;
			}
			dwKey ^= dwKeyTable[pcAdjust][sq];
			dwLock ^= dwLockTable[pcAdjust][sq];
		}

		public function movePiece():void {
			var sqSrc:int = SRC(mvList[nMoveNum]);
			var sqDst:int = DST(mvList[nMoveNum]);
			pcList[nMoveNum] = pcSquares[sqDst];
			if (pcList[nMoveNum] > 0) {
				addPiece(sqDst, pcList[nMoveNum], DEL_PIECE);
			}
			var pc:int = pcSquares[sqSrc];
			addPiece(sqSrc, pc, DEL_PIECE);
			addPiece(sqDst, pc);
		}

		public function undoMovePiece():void {
			var sqSrc:int = SRC(mvList[nMoveNum]);
			var sqDst:int = DST(mvList[nMoveNum]);
			var pc:int = pcSquares[sqDst];
			addPiece(sqDst, pc, DEL_PIECE);
			addPiece(sqSrc, pc);
			if (pcList[nMoveNum] > 0) {
				addPiece(sqDst, pcList[nMoveNum]);
			}
		}

		public function changeSide():void {
			sdPlayer = 1 - sdPlayer;
			dwKey ^= dwKeyPlayer;
			dwLock ^= dwLockPlayer;
		}

		public function makeMove(mv:int):Boolean {
			dwKeyList[nMoveNum] = dwKey;
			mvList[nMoveNum] = mv;
			movePiece();
			if (checked()) {
				undoMovePiece();
				return false;
			}
			changeSide();
			bCheckList[nMoveNum] = checked();
			nMoveNum ++;
			nDistance ++;
			return true;
		}

		public function undoMakeMove():void {
			nMoveNum --;
			nDistance --;
			changeSide();
			undoMovePiece();
		}

		public function nullMove():void {
			dwKeyList[nMoveNum] = dwKey;
			changeSide();
			mvList[nMoveNum] = pcList[nMoveNum] = 0;
			bCheckList[nMoveNum] = false;
			nMoveNum ++;
			nDistance ++;
		}

		public function undoNullMove():void {
			nMoveNum --;
			nDistance --;
			changeSide();
		}

		public function generateMoves(mvs:Array, vls:Array = null):int {
			var nMoves:int = 0;
			var pcSelfSide:int = SIDE_TAG(sdPlayer);
			var pcOppSide:int = OPP_SIDE_TAG(sdPlayer);
			var i:int, j:int, sqSrc:int, sqDst:int, pcSrc:int, pcDst:int, nDelta:int;
			for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
				pcSrc = pcSquares[sqSrc];
				if ((pcSrc & pcSelfSide) == 0) {
					continue;
				}
				switch (pcSrc - pcSelfSide) {
				case PIECE_KING:
					for (i = 0; i < 4; i ++) {
						sqDst = sqSrc + cnKingDelta[i];
						if (!IN_FORT(sqDst)) {
							continue;
						}
						pcDst = pcSquares[sqDst];
						if (vls == null) {
							if ((pcDst & pcSelfSide) == 0) {
								mvs[nMoves] = MOVE(sqSrc, sqDst);
								nMoves ++;
							}
						} else if ((pcDst & pcOppSide) != 0) {
							mvs[nMoves] = MOVE(sqSrc, sqDst);
							vls[nMoves] = MVV_LVA(pcDst, 5);
							nMoves ++;
						}
					}
					break;
				case PIECE_ADVISOR:
					for (i = 0; i < 4; i ++) {
						sqDst = sqSrc + cnAdvisorDelta[i];
						if (!IN_FORT(sqDst)) {
							continue;
						}
						pcDst = pcSquares[sqDst];
						if (vls == null) {
							if ((pcDst & pcSelfSide) == 0) {
								mvs[nMoves] = MOVE(sqSrc, sqDst);
								nMoves ++;
							}
						} else if ((pcDst & pcOppSide) != 0) {
							mvs[nMoves] = MOVE(sqSrc, sqDst);
							vls[nMoves] = MVV_LVA(pcDst, 1);
							nMoves ++;
						}
					}
					break;
				case PIECE_BISHOP:
					for (i = 0; i < 4; i ++) {
						sqDst = sqSrc + cnAdvisorDelta[i];
						if (!(IN_BOARD(sqDst) && HOME_HALF(sqDst, sdPlayer) && pcSquares[sqDst] == 0)) {
							continue;
						}
						sqDst += cnAdvisorDelta[i];
						pcDst = pcSquares[sqDst];
						if (vls == null) {
							if ((pcDst & pcSelfSide) == 0) {
								mvs[nMoves] = MOVE(sqSrc, sqDst);
								nMoves ++;
							}
						} else if ((pcDst & pcOppSide) != 0) {
							mvs[nMoves] = MOVE(sqSrc, sqDst);
							vls[nMoves] = MVV_LVA(pcDst, 1);
							nMoves ++;
						}
					}
					break;
				case PIECE_KNIGHT:
					for (i = 0; i < 4; i ++) {
						sqDst = sqSrc + cnKingDelta[i];
						if (pcSquares[sqDst] > 0) {
							continue;
						}
						for (j = 0; j < 2; j ++) {
							sqDst = sqSrc + cnKnightDelta[i][j];
							if (!IN_BOARD(sqDst)) {
								continue;
							}
							pcDst = pcSquares[sqDst];
							if (vls == null) {
								if ((pcDst & pcSelfSide) == 0) {
									mvs[nMoves] = MOVE(sqSrc, sqDst);
									nMoves ++;
								}
							} else if ((pcDst & pcOppSide) != 0) {
								mvs[nMoves] = MOVE(sqSrc, sqDst);
								vls[nMoves] = MVV_LVA(pcDst, 1);
								nMoves ++;
							}
						}
					}
					break;
				case PIECE_ROOK:
					for (i = 0; i < 4; i ++) {
						nDelta = cnKingDelta[i];
						sqDst = sqSrc + nDelta;
						while (IN_BOARD(sqDst)) {
							pcDst = pcSquares[sqDst];
							if (pcDst == 0) {
								if (vls == null) {
									mvs[nMoves] = MOVE(sqSrc, sqDst);
									nMoves ++;
								}
							} else {
								if ((pcDst & pcOppSide) != 0) {
									mvs[nMoves] = MOVE(sqSrc, sqDst);
									if (vls != null) {
										vls[nMoves] = MVV_LVA(pcDst, 4);
									}
									nMoves ++;
								}
								break;
							}
							sqDst += nDelta;
						}
					}
					break;
				case PIECE_CANNON:
					for (i = 0; i < 4; i ++) {
						nDelta = cnKingDelta[i];
						sqDst = sqSrc + nDelta;
						while (IN_BOARD(sqDst)) {
							pcDst = pcSquares[sqDst];
							if (pcDst == 0) {
								if (vls == null) {
									mvs[nMoves] = MOVE(sqSrc, sqDst);
									nMoves ++;
								}
							} else {
								break;
							}
							sqDst += nDelta;
						}
						sqDst += nDelta;
						while (IN_BOARD(sqDst)) {
							pcDst = pcSquares[sqDst];
							if (pcDst > 0) {
								if ((pcDst & pcOppSide) != 0) {
									mvs[nMoves] = MOVE(sqSrc, sqDst);
									if (vls != null) {
										vls[nMoves] = MVV_LVA(pcDst, 4);
									}
									nMoves ++;
								}
								break;
							}
							sqDst += nDelta;
						}
					}
					break;
				case PIECE_PAWN:
					sqDst = SQUARE_FORWARD(sqSrc, sdPlayer);
					if (IN_BOARD(sqDst)) {
						pcDst = pcSquares[sqDst];
						if (vls == null) {
							if ((pcDst & pcSelfSide) == 0) {
								mvs[nMoves] = MOVE(sqSrc, sqDst);
								nMoves ++;
							}
						} else if ((pcDst & pcOppSide) != 0) {
							mvs[nMoves] = MOVE(sqSrc, sqDst);
							vls[nMoves] = MVV_LVA(pcDst, 2);
							nMoves ++;
						}
					}
					if (AWAY_HALF(sqSrc, sdPlayer)) {
						for (nDelta = -1; nDelta <= 1; nDelta += 2) {
							sqDst = sqSrc + nDelta;
							if (IN_BOARD(sqDst)) {
								pcDst = pcSquares[sqDst];
								if (vls == null) {
									if ((pcDst & pcSelfSide) == 0) {
										mvs[nMoves] = MOVE(sqSrc, sqDst);
										nMoves ++;
									}
								} else if ((pcDst & pcOppSide) != 0) {
									mvs[nMoves] = MOVE(sqSrc, sqDst);
									vls[nMoves] = MVV_LVA(pcDst, 2);
									nMoves ++;
								}
							}
						}
					}
					break;
				}
			}
			return nMoves;
		}

		public function legalMove(mv:int):Boolean {
			var sqSrc:int = SRC(mv);
			var pcSrc:int = pcSquares[sqSrc];
			var pcSelfSide:int = SIDE_TAG(sdPlayer);
			if ((pcSrc & pcSelfSide) == 0) {
				return false;
			}

			var sqDst:int = DST(mv);
			var pcDst:int = pcSquares[sqDst];
			if ((pcDst & pcSelfSide) != 0) {
				return false;
			}

			var sqPin:int, nDelta:int;
			switch (pcSrc - pcSelfSide) {
			case PIECE_KING:
				return IN_FORT(sqDst) && KING_SPAN(sqSrc, sqDst);
			case PIECE_ADVISOR:
				return IN_FORT(sqDst) && ADVISOR_SPAN(sqSrc, sqDst);
			case PIECE_BISHOP:
				return SAME_HALF(sqSrc, sqDst) && BISHOP_SPAN(sqSrc, sqDst) &&
						pcSquares[BISHOP_PIN(sqSrc, sqDst)] == 0;
			case PIECE_KNIGHT:
				sqPin = KNIGHT_PIN(sqSrc, sqDst);
				return sqPin != sqSrc && pcSquares[sqPin] == 0;
			case PIECE_ROOK:
			case PIECE_CANNON:
				if (SAME_RANK(sqSrc, sqDst)) {
					nDelta = (sqDst < sqSrc ? -1 : 1);
				} else if (SAME_FILE(sqSrc, sqDst)) {
					nDelta = (sqDst < sqSrc ? -16 : 16);
				} else {
					return false;
				}
				sqPin = sqSrc + nDelta;
				while (sqPin != sqDst && pcSquares[sqPin] == 0) {
					sqPin += nDelta;
				}
				if (sqPin == sqDst) {
					return pcDst == 0 || pcSrc - pcSelfSide == PIECE_ROOK;
				}
				if (pcDst == 0 || pcSrc - pcSelfSide == PIECE_ROOK) {
					return false;
				}
				sqPin += nDelta;
				while (sqPin != sqDst && pcSquares[sqPin] == 0) {
					sqPin += nDelta;
				}
				return sqPin == sqDst;
			case PIECE_PAWN:
				if (AWAY_HALF(sqDst, sdPlayer) && (sqDst == sqSrc - 1 || sqDst == sqSrc + 1)) {
					return true;
				}
				return sqDst == SQUARE_FORWARD(sqSrc, sdPlayer);
			default:
				return false;
			}
		}

		public function checked():Boolean {
			var pcSelfSide:int = SIDE_TAG(sdPlayer);
			var pcOppSide:int = OPP_SIDE_TAG(sdPlayer);
			var i:int, j:int, sqSrc:int, sqDst:int, pcDst:int, nDelta:int;
			for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
				if (pcSquares[sqSrc] != pcSelfSide + PIECE_KING) {
					continue;
				}
				if (pcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) {
					return true;
				}
				for (nDelta = -1; nDelta <= 1; nDelta += 2) {
					if (pcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) {
						return true;
					}
				}
				for (i = 0; i < 4; i ++) {
					if (pcSquares[sqSrc + cnAdvisorDelta[i]] != 0) {
						continue;
					}
					for (j = 0; j < 2; j ++) {
						pcDst = pcSquares[sqSrc + cnKnightCheckDelta[i][j]];
						if (pcDst == pcOppSide + PIECE_KNIGHT) {
							return true;
						}
					}
				}
				for (i = 0; i < 4; i ++) {
					nDelta = cnKingDelta[i];
					sqDst = sqSrc + nDelta;
					while (IN_BOARD(sqDst)) {
						pcDst = pcSquares[sqDst];
						if (pcDst > 0) {
							if (pcDst == pcOppSide + PIECE_ROOK || pcDst == pcOppSide + PIECE_KING) {
								return true;
							}
							break;
						}
						sqDst += nDelta;
					}
					sqDst += nDelta;
					while (IN_BOARD(sqDst)) {
						pcDst = pcSquares[sqDst];
						if (pcDst > 0) {
							if (pcDst == pcOppSide + PIECE_CANNON) {
								return true;
							}
							break;
						}
						sqDst += nDelta;
					}
				}
				return false;
			}
			return false;
		}

		public function isMate():Boolean {
			var mvs:Array = new Array(MAX_GEN_MOVES);
			var nMoves:int = generateMoves(mvs);
			var i:int;
			for (i = 0; i < nMoves; i ++) {
				if (makeMove(mvs[i])) {
					undoMakeMove();
					return false;
				}
			}
			return true;
		}

		public function drawValue():int {
			return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
		}

		public function evaluate():int {
			var vl:int = (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
			return vl == drawValue() ? vl - 1 : vl;
		}

		public function nullOkay():Boolean {
			return (sdPlayer == 0 ? vlWhite : vlBlack) > NULL_OKAY_MARGIN;
		}

		public function nullSafe():Boolean {
			return (sdPlayer == 0 ? vlWhite : vlBlack) > NULL_SAFE_MARGIN;
		}

		public function inCheck():Boolean {
			return bCheckList[nMoveNum - 1];
		}

		public function captured():Boolean {
			return pcList[nMoveNum - 1] > 0;
		}

		public function repValue(vlRep:int):int {
			var vlReturn:int = ((vlRep & 2) == 0 ? 0 : nDistance - BAN_VALUE) +
					((vlRep & 4) == 0 ? 0 : BAN_VALUE - nDistance);
			return vlReturn == 0 ? drawValue() : vlReturn;
		}

		public function repStatus(nRecur:int = 1):int {
			var bSelfSide:Boolean = false;
			var bPerpCheck:Boolean = true;
			var bOppPerpCheck:Boolean = true;
			var nIndex:int = nMoveNum - 1;
			while (mvList[nIndex] > 0 && pcList[nIndex] == 0) {
				if (bSelfSide) {
					bPerpCheck = bPerpCheck && bCheckList[nIndex];
					if (dwKeyList[nIndex] == dwKey) {
						nRecur --;
						if (nRecur == 0) {
							return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
						}
					}
				} else {
					bOppPerpCheck = bOppPerpCheck && bCheckList[nIndex];
				}
				bSelfSide = !bSelfSide;
				nIndex --;
			}
			return 0;
		}

		public function mirror():Position {
			var pos:Position = new Position();
			pos.clearBoard();
			var sq:int;
			for (sq = 0; sq < 256; sq ++) {
				var pc:int = pcSquares[sq];
				if (pc > 0) {
					pos.addPiece(MIRROR_SQUARE(sq), pc);
				}
			}
			if (sdPlayer == 1) {
				pos.changeSide();
			}
			return pos;
		}

		private static function fenPiece(c:String):int {
			switch (c) {
			case 'K':
				return PIECE_KING;
			case 'A':
				return PIECE_ADVISOR;
			case 'B':
			case 'E':
				return PIECE_BISHOP;
			case 'H':
			case 'N':
				return PIECE_KNIGHT;
			case 'R':
				return PIECE_ROOK;
			case 'C':
				return PIECE_CANNON;
			case 'P':
				return PIECE_PAWN;
			default:
				return -1;
			}
		}

		public function fromFen(fen:String):void {
			clearBoard();
			var y:int = RANK_TOP;
			var x:int = FILE_LEFT;
			var index:int = 0;
			if (index == fen.length) {
				setIrrev();
				return;
			}
			var pt:int;
			var c:String = fen.charAt(index);
			while (c != ' ') {
				if (c == '/') {
					x = FILE_LEFT;
					y ++;
					if (y > RANK_BOTTOM) {
						break;
					}
				} else if (c >= '1' && c <= '9') {
					var kk:int = c.charCodeAt() - '0'.charCodeAt();
					for (var k:int = 0; k < kk; k ++) {
						if (x >= FILE_RIGHT) {
							break;
						}
						x ++;
					}
				} else if (c >= 'A' && c <= 'Z') {
					if (x <= FILE_RIGHT) {
						pt = fenPiece(c);
						if (pt >= 0) {
							addPiece(COORD_XY(x, y), pt + 8);
						}
						x ++;
					}
				} else if (c >= 'a' && c <= 'z') {
					if (x <= FILE_RIGHT) {
						pt = fenPiece(String.fromCharCode(c.charCodeAt() - 'a'.charCodeAt() + 'A'.charCodeAt()));
						if (pt >= 0) {
							addPiece(COORD_XY(x, y), pt + 16);
						}
						x ++;
					}
				}
				index ++;
				if (index == fen.length) {
					setIrrev();
					return;
				}
				c = fen.charAt(index);
			}
			index ++;
			if (index == fen.length) {
				setIrrev();
				return;
			}
			if (sdPlayer == (fen.charAt(index) == 'b' ? 0 : 1)) {
				changeSide();
			}
			setIrrev();
		}

		public function bookMove():int {
			if (nBookSize == 0) {
				return 0;
			}
			var bMirror:Boolean = false;
			var dwMirrorLock:uint = dwLock;
			var nIndex = Util.binarySearch(dwMirrorLock, dwBookLock, 0, nBookSize);
			if (nIndex < 0) {
				bMirror = true;
				dwMirrorLock = mirror().dwLock;
				nIndex = Util.binarySearch(dwMirrorLock, dwBookLock, 0, nBookSize);
			}
			if (nIndex < 0) {
				return 0;
			}
			nIndex --;
			while (nIndex >= 0 && dwBookLock[nIndex] == dwMirrorLock) {
				nIndex --;
			}
			var mvs:Array = new Array(MAX_GEN_MOVES);
			var vls:Array = new Array(MAX_GEN_MOVES);
			var vl:int = 0;
			var nMoves:int = 0;
			nIndex ++;
			while (nIndex < nBookSize && dwBookLock[nIndex] == dwMirrorLock) {
				var mv:int = mvBook[nIndex];
				mv = (bMirror ? MIRROR_MOVE(mv) : mv);
				if (legalMove(mv)) {
					mvs[nMoves] = mv;
					vls[nMoves] = vlBook[nIndex];
					vl += vls[nMoves];
					nMoves ++;
					if (nMoves == MAX_GEN_MOVES) {
						break;
					}
				}
				nIndex ++;
			}
			if (vl == 0) {
				return 0;
			}
			vl = int(Math.random() * vl);
			for (nIndex = 0; nIndex < nMoves; nIndex ++) {
				vl -= vls[nIndex];
				if (vl < 0) {
					break;
				}
			}
			return mvs[nIndex];
		}

		public function historyIndex(mv:int):int {
			return ((pcSquares[SRC(mv)] - 8) << 8) + DST(mv);
		}
	}
}