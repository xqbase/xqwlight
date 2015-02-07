/*
XQBossMIDlet.java - Source Code for XiangQi Boss Advanced, Part V

XiangQi Boss Advanced - a Chinese Chess PGN File Reader for Java ME
Designed by Morning Yellow, Version: 1.31, Last Modified: Jan. 2010
Copyright (C) 2004-2010 www.xqbase.com

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

import java.util.Vector;

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
	private static Ticker ticker = new Ticker("欢迎登录 www.xqbase.com 免费下载PC版 象棋巫师");
	private static Image imgFolder, imgFile;

	static {
		try {
			imgFolder = Image.createImage("/images/folder.png");
			imgFile = Image.createImage("/images/file.png");
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}

	private XQBossCanvas canvas = new XQBossCanvas(this);
	private boolean started = false;

	String currDir = "";

	void open(String file, String title) {
		LineInputStream in = new LineInputStream(getClass().
				getResourceAsStream("/PGNS" + currDir + "/" + file));
		PgnFile pgn = new PgnFile(in);
		in.close();
		canvas.load(pgn, title);
		Display.getDisplay(this).setCurrent(canvas);
	}

	void list() {
		final Command cmdOpen = new Command("打开", Command.OK, 1);
		final Command cmdBack = new Command("返回", Command.BACK, 1);
		final Command cmdExit = new Command("退出", Command.EXIT, 1);
		final List lstDir = new List(getAppProperty("MIDlet-Name"), Choice.IMPLICIT);
		final Vector vctDir = new Vector();

		lstDir.addCommand(cmdOpen);
		lstDir.addCommand(currDir.length() == 0 ? cmdExit : cmdBack);
		lstDir.setTicker(ticker);

		LineInputStream in = new LineInputStream(getClass().
				getResourceAsStream("/PGNS" + currDir + "/FILELIST"));
		String s = in.readLine();
		while (s != null) {
			int i = s.indexOf('=');
			lstDir.append(s.substring(i + 1), s.charAt(0) == '/' ? imgFolder : imgFile);
			vctDir.addElement(s.substring(0, i));
			s = in.readLine();
		}
		in.close();
		lstDir.setSelectCommand(cmdOpen);
		lstDir.setCommandListener(new CommandListener() {
			public void commandAction(Command c, Displayable d) {
				if (c == cmdOpen) {
					int i = lstDir.getSelectedIndex();
					String selDir = (String) vctDir.elementAt(i);
					if (selDir.charAt(0) == '/') {
						currDir += selDir;
						list();
					} else {
						open(selDir, lstDir.getString(i));
					}
				} else if (c == cmdBack) {
					currDir = currDir.substring(0, currDir.lastIndexOf('/'));
					list();
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