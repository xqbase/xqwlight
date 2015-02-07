import java.io.File;

import xqwlight.util.ClassPath;
import xqwlight.util.Integers;
import xqwlight.util.server.JettyServer;

public class Startup {
	private static final File DEFAULT_HOME = ClassPath.getInstance("../../../..");

	public static void main(String[] args) throws Exception {
		int port = (args.length == 0 ? 80 : Integers.parse(args[0], 80));
		JettyServer server = new JettyServer(DEFAULT_HOME, port);
		server.start();
		server.join();
	}
}