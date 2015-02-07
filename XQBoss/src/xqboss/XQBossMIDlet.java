/*
XQBossMIDlet.java - Source Code for XiangQi Boss, Part V

XiangQi Boss - a Chinese Chess PGN File Reader for Java ME
Designed by Morning Yellow, Version: 1.0, Last Modified: Aug. 2008
Copyright (C) 2004-2008 www.elephantbase.net

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
package xqboss;

import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.io.file.FileSystemRegistry;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.List;
import javax.microedition.lcdui.Ticker;
import javax.microedition.midlet.MIDlet;

public class XQBossMIDlet extends MIDlet {
	private static Ticker ticker = new Ticker("欢迎登录 www.elephantbase.net 免费下载PC版 象棋巫师");
	private static Image imgFolder, imgPgn;

	static {
		try {
			imgFolder = Image.createImage("/images/folder.png");
			imgPgn = Image.createImage("/images/pgn_s.png");
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}

	private boolean started = false;

	private XQBossCanvas canvas = new XQBossCanvas(this);

	String currDir = null;

	void open(String file) {
		FileConnection dir = null;
		try {
			dir = (FileConnection) Connector.open("file://localhost/" +
					currDir + "/" + file);
			GBLineInputStream in = new GBLineInputStream(dir.openInputStream());
			PgnFile pgn = new PgnFile(in);
			in.close();
			dir.close();
			canvas.load(pgn, file);
			Display.getDisplay(this).setCurrent(canvas);
		} catch (Exception e) {
			if (dir != null) {
				try {
					dir.close();
				} catch (Exception ee) {
					// Ignored
				}
			}
			destroyApp(false);
			notifyDestroyed();
		}
	}

	void enter(String strDir) {
		if (currDir == null) {
			currDir = strDir;
		} else {
			currDir += "/" + strDir;
		}
	}

	void leave() {
		// __ASSERT(currDir != null);
		int i = currDir.lastIndexOf('/');
		if (i < 0) {
			currDir = null;
		} else {
			currDir = currDir.substring(0, i);
		}
	}

	void list() {
		final Command cmdOpen = new Command("打开", Command.OK, 1);
		final Command cmdBack = new Command("返回", Command.BACK, 1);
		final Command cmdExit = new Command("退出", Command.EXIT, 1);
		final List lstDir = new List("象棋小博士", Choice.IMPLICIT);

		lstDir.setTicker(ticker);
		Enumeration enumDir;
		lstDir.addCommand(cmdOpen);
		if (currDir == null) {
			lstDir.addCommand(cmdExit);
			try {
				enumDir = FileSystemRegistry.listRoots();
			} catch (Exception e) {
				destroyApp(false);
				notifyDestroyed();
				return;
			}
		} else {
			lstDir.addCommand(cmdBack);
			FileConnection dir = null;
			try {
				dir = (FileConnection) Connector.open("file://localhost/" + currDir + "/");
				enumDir = dir.list();
				dir.close();
			} catch (Exception e) {
				if (dir != null) {
					try {
						dir.close();
					} catch (Exception ee) {
						// Ignored
					}
				}
				destroyApp(false);
				notifyDestroyed();
				return;
			}
		}
		while (enumDir.hasMoreElements()) {
			String strDir = (String) enumDir.nextElement();
			if (strDir.endsWith("/")) {
				lstDir.append("[" + strDir.substring(0, strDir.length() - 1) + "]", imgFolder);
			} else if (strDir.toLowerCase().endsWith(".pgn")) {
				lstDir.append(strDir, imgPgn);
			}
		}
		lstDir.setSelectCommand(cmdOpen);
		lstDir.setCommandListener(new CommandListener() {
			public void commandAction(Command c, Displayable d) {
				if (c == cmdOpen) {
					final String selDir = lstDir.getString(lstDir.getSelectedIndex());
					if (selDir.startsWith("[") && selDir.endsWith("]")) {
						new Thread() {
							public void run() {
								enter(selDir.substring(1, selDir.length() - 1));
								list();
							}
						}.start();
					} else {
						new Thread() {
							public void run() {
								open(selDir);
							}
						}.start();
					}
				} else if (c == cmdBack) {
					new Thread() {
						public void run() {
							leave();
							list();
						}
					}.start();
				} else if (c == cmdExit) {
					destroyApp(false);
					notifyDestroyed();
				}
			}
		});
		Display.getDisplay(this).setCurrent(lstDir);
	}

	protected void startApp() {
		if (started) {
			return;
		}
		started = true;
		list();
	}

	protected void pauseApp() {
		// Do Nothing
	}

	protected void destroyApp(boolean unc) {
		started = false;
	}
}