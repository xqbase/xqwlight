package xqwlight.web;

import java.util.Arrays;
import java.util.List;

public class Choices {
	public static final String FLIPPED_FALSE = "我先走";
	public static final String FLIPPED_TRUE = "电脑先走";

	private static List<String> lstFlipped = Arrays.asList(new String[] {
		FLIPPED_FALSE, FLIPPED_TRUE
	});

	public static List<String> getFlippedTypes() {
		return lstFlipped;
	}

	private static List<String> lstHandicap = Arrays.asList(new String[] {
		"不让子", "让左马", "让双马", "让九子"
	});

	public static List<String> getHandicapTypes() {
		return lstHandicap;
	}

	private static List<String> lstBoard = Arrays.asList(new String[] {
		"栎木", "绿色大理石", "白色大理石", "再生纸", "画布", "水滴", "浅红象棋"
	});

	public static List<String> getBoardTypes() {
		return lstBoard;
	}

	private static List<String> lstPieces = Arrays.asList(new String[] {
		"木刻", "精致", "光泽"
	});

	public static List<String> getPiecesTypes() {
		return lstPieces;
	}

	private static List<String> lstMusic = Arrays.asList(new String[] {
		"紧张", "风趣", "巴赫", "莫扎特一", "莫扎特四", "贝多芬",
		"李斯特", "柴可夫斯基", "德沃夏克", "仙剑奇侠传", "望江南",
	});

	public static List<String> getMusicTypes() {
		return lstMusic;
	}
}