package xqboss;

import java.io.IOException;
import java.io.InputStream;

class GBLineInputStream {
	private static char[] gbCharTab = new char[32768];

	static {
		try {
			InputStream inGB = new GBLineInputStream().getClass().
					getResourceAsStream("GB.DAT");
			for (int i = 0; i < 32768; i ++) {
				gbCharTab[i] = (char) (inGB.read() << 8 | inGB.read());
			}
			inGB.close();
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}

	private InputStream in;

	private GBLineInputStream() {
		// Do Nothing
	}

	GBLineInputStream(InputStream in) {
		this.in = in;
	}

	String readLine() {
		StringBuffer sb = new StringBuffer();
		int b;
		try {
			b = in.read();
		} catch (Exception e) {
			return null;
		}
		while (b != -1) {
			if (b == '\n') {
				return sb.toString();
			} else if (b >= 128) {
				int b2;
				try {
					b2 = in.read();
				} catch (Exception e) {
					return null;
				}
				if (b2 == -1) {
					sb.append((char) b);
					break;
				}
				sb.append(gbCharTab[((b - 128) << 8) + b2]);
			} else if (b != '\r') {
				sb.append((char) b);
			}
			try {
				b = in.read();
			} catch (Exception e) {
				return null;
			}
		}
		return sb.length() == 0 ? null : sb.toString();
	}

	void close() throws IOException {
		in.close();
	}
}