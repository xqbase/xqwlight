package xqwlight.util.server;

import java.io.File;

import org.eclipse.jetty.deploy.WebAppDeployer;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.bio.SocketConnector;
import org.eclipse.jetty.server.handler.HandlerCollection;
import org.eclipse.jetty.server.ssl.SslSocketConnector;

public class JettyServer extends Server {
	public JettyServer(File home) throws Exception {
		this(home, 80);
	}

	public JettyServer(File home, int port) throws Exception {
		SocketConnector connector = new SocketConnector();
		connector.setPort(port);
		addConnector(connector);
		initWebApp(home);
	}

	public JettyServer(File home, File filePfx, String password) throws Exception {
		this(home, 443, filePfx, password);
	}

	public JettyServer(File home, int nPort, File filePfx, String password) throws Exception {
		SslSocketConnector connector = new SslSocketConnector();
		connector.setPassword(password);
		connector.setKeystoreType("PKCS12");
		connector.setKeystore(filePfx.getPath());
		connector.setKeyPassword(password);
		connector.setTruststoreType("PKCS12");
		connector.setTruststore(filePfx.getPath());
		connector.setTrustPassword(password);
		connector.setWantClientAuth(true);
		connector.setPort(nPort);
		addConnector(connector);
		initWebApp(home);
	}

	private void initWebApp(File home) throws Exception {
		HandlerCollection handler = new HandlerCollection();
		setHandler(handler);
		WebAppDeployer webapp = new WebAppDeployer();
		webapp.setContexts(handler);
		webapp.setWebAppDir(home + File.separator + "webapps");
		webapp.start();
	}
}