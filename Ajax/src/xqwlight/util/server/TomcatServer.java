/* Unavailable
package xqwlight.util.server;

import java.io.File;

import org.apache.catalina.Context;
import org.apache.catalina.Engine;
import org.apache.catalina.Host;
import org.apache.catalina.connector.Connector;
import org.apache.catalina.startup.Embedded;

public class TomcatServer extends Embedded {
	public TomcatServer(File home) throws Exception {
		this(home, 80);
	}

	public TomcatServer(File home, int port) throws Exception {
		setCatalinaHome(home.toString());
		initEmbedded();
		Connector connector = createConnector("0.0.0.0", port, false);
		addConnector(connector);
	}

	public TomcatServer(File home, File filePfx, String password) throws Exception {
		this(home, 443, filePfx, password);
	}

	public TomcatServer(File home, int port, File filePfx, String password) throws Exception {
		setCatalinaHome(home.toString());
		initEmbedded();
		Connector connector = createConnector("0.0.0.0", port, true);
		connector.setAttribute("keystoreType", "PKCS12");
		connector.setAttribute("keystoreFile", filePfx.getPath());
		connector.setAttribute("keystorePass", password);
		addConnector(connector);
	}

	private void initEmbedded() {
		// 1. Engine
		Engine engine = createEngine();
		addEngine(engine);
		// 2. Host
		Host host = createHost("localhost", getCatalinaHome() + File.separatorChar + "webapps");
		engine.addChild(host);
		engine.setDefaultHost("localhost");
		// 3. Context
		Context context = createContext("", host.getAppBase() + File.separatorChar + "ROOT");
		host.addChild(context);
	}
}
*/