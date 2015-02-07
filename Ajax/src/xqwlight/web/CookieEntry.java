package xqwlight.web;

import java.io.Serializable;

import javax.servlet.http.Cookie;

import org.apache.wicket.model.Model;

public class CookieEntry implements Serializable {
	private static final long serialVersionUID = 1L;

	private Model<String> name;
	private Model<String> value;

	public CookieEntry(Cookie cookie) {
		this(cookie.getName(), cookie.getValue());
	}

	public CookieEntry(String name, String value) {
		this.name = new Model<>(name);
		this.value = new Model<>(value);
	}

	public Model<String> getNameModel() {
		return name;
	}

	public Model<String> getValueModel() {
		return value;
	}

	public String getName() {
		return name.getObject();
	}

	public String getValue() {
		return value.getObject();
	}

	public Cookie getCookie() {
		try {
			return new Cookie(getName(), getValue());
		} catch (Exception e) {
			return null;
		}
	}
}