package xqboss;

public class GenGBTab {
	public static void main(String[] args) throws Exception {
		String enc = "GBK";
		if (args.length > 0) {
			enc = args[0];
		}
		byte[] gbByteTab = new byte[65536];
		byte[] b = new byte[2];
		int n = 0;
		for (int i = 128; i < 256; i ++) {
			b[0] = (byte) i;
			for (int j = 0; j < 256; j ++) {
				b[1] = (byte) j;
				char c = new String(b, enc).charAt(0);
				gbByteTab[n] = (byte) ((c >>> 8) & 0xff);
				gbByteTab[n + 1] = (byte) (c & 0xff);
				n += 2;
			}
		}
		System.out.print(Base64.encode(gbByteTab));
	}
}