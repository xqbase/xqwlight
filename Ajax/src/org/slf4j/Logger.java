package org.slf4j;

public interface Logger {
	public String getName();
	public boolean isTraceEnabled();
	public void trace(String s);
	public void trace(String s, Object o);
	public void trace(String s, Object o1, Object o2);
	public void trace(String s, Object o[]);
	public void trace(String s, Throwable t);
	public boolean isDebugEnabled();
	public void debug(String s);
	public void debug(String s, Object o);
	public void debug(String s, Object o1, Object o2);
	public void debug(String s, Object o[]);
	public void debug(String s, Throwable t);
	public boolean isInfoEnabled();
	public void info(String s);
	public void info(String s, Object o);
	public void info(String s, Object o1, Object o2);
	public void info(String s, Object o[]);
	public void info(String s, Throwable t);
	public boolean isWarnEnabled();
	public void warn(String s);
	public void warn(String s, Object o);
	public void warn(String s, Object o[]);
	public void warn(String s, Object o1, Object o2);
	public void warn(String s, Throwable t);
	public boolean isErrorEnabled();
	public void error(String s);
	public void error(String s, Object o);
	public void error(String s, Object o1, Object o2);
	public void error(String s, Object o[]);
	public void error(String s, Throwable t);
}