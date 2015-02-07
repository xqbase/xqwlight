/*
Test.java - Source Code for XiangQi Wizard Light, Part VI

XiangQi Wizard Light - a Chinese Chess Program for Java ME
Designed by Morning Yellow, Version: 1.21, Last Modified: Jan. 2008
Copyright (C) 2004-2008 www.elephantbase.net

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
import java.io.BufferedReader;
import java.io.InputStreamReader;

import xqwlight.Position;
import xqwlight.Search;

public class Test {
	public static void main(String[] args) throws Exception {
		int[] mvs = new int[Position.MAX_GEN_MOVES];
		int legal = 0, gened = 0, moved = 0, check = 0;
		Position pos = new Position();
		BufferedReader in = new BufferedReader(new InputStreamReader(Test.class.getResourceAsStream("/FUNNY.EPD")));
		String str = in.readLine();
		while (str != null) {
			pos.fromFen(str);
			for (int sqSrc = 0; sqSrc < 256; sqSrc ++) {
				if (Position.IN_BOARD(sqSrc)) {
					for (int sqDst = 0; sqDst < 256; sqDst ++) {
						if (Position.IN_BOARD(sqDst)) {
							legal += (pos.legalMove(Position.MOVE(sqSrc, sqDst)) ? 1 : 0);
						}
					}
				}
			}
			int moveNum = pos.generateAllMoves(mvs);
			for (int i = 0; i < moveNum; i ++) {
				if (pos.makeMove(mvs[i])) {
					moved ++;
					check += (pos.inCheck() ? 1 : 0);
					pos.undoMakeMove();
				}
			}
			gened += moveNum;
			str = in.readLine();
		}
		in.close();
		System.out.println("Legal: " + legal); // 7809
		System.out.println("Gened: " + gened); // 7809
		System.out.println("Moved: " + moved); // 7207
		System.out.println("Check: " + check); // 718
		pos.fromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/R1BAKABNR w - - 0 1");
		System.out.println(pos.toFen());
		Search search = new Search(pos, 12);
		search.searchMain(1000);
		System.out.println("NPS = " + search.getKNPS() + "K");
	}
}