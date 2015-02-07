package xqwlight;

import org.apache.wicket.protocol.http.WebApplication;

import xqwlight.util.wicket.RefreshPage;
import xqwlight.web.XQWLightPage;

public class XQWLightApp extends WebApplication {
	@Override
	public Class<? extends XQWLightPage> getHomePage() {
		return XQWLightPage.class;
	}

	@Override
	protected void init() {
		getApplicationSettings().setPageExpiredErrorPage(RefreshPage.class);
	}
}