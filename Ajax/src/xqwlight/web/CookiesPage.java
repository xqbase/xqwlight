package xqwlight.web;

import java.util.ArrayList;

import javax.servlet.http.Cookie;

import org.apache.wicket.markup.html.WebPage;
import org.apache.wicket.markup.html.basic.Label;
import org.apache.wicket.markup.html.form.Button;
import org.apache.wicket.markup.html.form.Form;
import org.apache.wicket.markup.html.form.TextField;
import org.apache.wicket.markup.html.list.ListItem;
import org.apache.wicket.markup.html.list.ListView;
import org.apache.wicket.protocol.http.WebRequest;
import org.apache.wicket.protocol.http.WebResponse;

import xqwlight.util.Bytes;
import xqwlight.util.wicket.RefreshPage;

public class CookiesPage extends WebPage {
	private static final long serialVersionUID = 1L;

	private static final String COOKIE_HELPER_NAME = "COOKIEHELPER";

	private ArrayList<Cookie> oldCookies = new ArrayList<>();

	ArrayList<CookieEntry> newCookieEntries = new ArrayList<>();
	boolean skipSubmit = false;

	void setCookies() {
		for (Cookie cookie : oldCookies) {
			((WebResponse) getResponse()).clearCookie(cookie);
		}
		oldCookies.clear();
		for (CookieEntry cookieEntry : newCookieEntries) {
			if (cookieEntry.getName() != null) {
				Cookie cookie = cookieEntry.getCookie();
				if (cookie != null) {
					((WebResponse) getResponse()).addCookie(cookie);
					oldCookies.add(cookie);
				}
			}
		}
		((WebResponse) getResponse()).addCookie(new
				Cookie(COOKIE_HELPER_NAME, Bytes.toHexUpper(Bytes.random(4))));
	}

	public CookiesPage(String gotoPrompt) {
		newCookieEntries.clear();
		for (Cookie cookie : ((WebRequest) getRequest()).getCookies()) {
			if (!cookie.getName().equalsIgnoreCase("JSESSIONID") &&
					!cookie.getName().equalsIgnoreCase(COOKIE_HELPER_NAME)) {
				oldCookies.add(cookie);
				newCookieEntries.add(new CookieEntry(cookie));
			}
		}
		Form<Void> frm = new Form<Void>("frm") {
			private static final long serialVersionUID = 1L;

			@Override
			protected void onSubmit() {
				if (skipSubmit) {
					skipSubmit = false;
				} else {
					setCookies();
					try {
						setResponsePage(RefreshPage.class);
					} catch (Exception e) {
						throw new RuntimeException(e);
					}
				}
			}
		};
		frm.add(new Button("btnInsert") {
			private static final long serialVersionUID = 1L;

			@Override
			public void onSubmit() {
				newCookieEntries.add(new CookieEntry(null, null));
				skipSubmit = true;
			}
		});
		frm.add(new ListView<CookieEntry>("lstCookies", newCookieEntries) {
			private static final long serialVersionUID = 1L;

			@Override
			protected void populateItem(ListItem<CookieEntry> item) {
				final int index = item.getIndex();
				CookieEntry cookieEntry = item.getModelObject();
				item.add(new TextField<>("txtName", cookieEntry.getNameModel()));
				item.add(new TextField<>("txtValue", cookieEntry.getValueModel()));
				item.add(new Button("btnDelete") {
					private static final long serialVersionUID = 1L;

					@Override
					public void onSubmit() {
						newCookieEntries.remove(index);
						skipSubmit = true;
					}
				});
			}
		});
		frm.add(new Label("lblGoto", gotoPrompt));
		add(frm);
	}
}