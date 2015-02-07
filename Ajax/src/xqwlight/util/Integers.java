package xqwlight.util;

public class Integers {
	public static int parse(String s) {
		return parse(s, 0);
	}

	public static int parse(String s, int i) {
		int n = i;
		try {
			n = Integer.parseInt(s);
		} catch (Exception e) {
			// Ignored
		}
		return n;
	}

	public static int minMax(int min, int mid, int max) {
		return mid < min ? min : mid > max ? max : mid;
	}
}