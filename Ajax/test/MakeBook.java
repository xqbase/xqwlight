import xqwlight.Position;

public class MakeBook {
	public static void main(String[] args) throws Exception {
		for (int i = 0; i < Position.bookSize; i ++) {
			System.out.println("  [" + Position.bookLock[i] + ", " +
					(Position.bookMove[i] & 0xffff) + ", " + (Position.bookValue[i] & 0xffff) + "],");
		}
	}
}