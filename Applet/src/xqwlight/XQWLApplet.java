/*
XQWLApplet.java - Source Code for XiangQi Wizard Light, Part IV

XiangQi Wizard Light - a Chinese Chess Program for Java Applet
Designed by Morning Yellow, Version: 1.25, Last Modified: Mar. 2008
Copyright (C) 2004-2008 www.xqbase.com

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
package xqwlight;

import java.applet.Applet;
import java.applet.AudioClip;
import java.awt.Button;
import java.awt.Canvas;
import java.awt.Checkbox;
import java.awt.Choice;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Label;
import java.awt.List;
import java.awt.Scrollbar;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class XQWLApplet extends Applet {
	private static final long serialVersionUID = 1L;

	private static final int RESP_CLICK = 0;
	private static final int RESP_ILLEGAL = 1;
	private static final int RESP_MOVE = 2;
	private static final int RESP_MOVE2 = 3;
	private static final int RESP_CAPTURE = 4;
	private static final int RESP_CAPTURE2 = 5;
	private static final int RESP_CHECK = 6;
	private static final int RESP_CHECK2 = 7;
	private static final int RESP_WIN = 8;
	private static final int RESP_DRAW = 9;
	private static final int RESP_LOSS = 10;

	private static final int PIECE_MARGIN = 8;
	private static final int SQUARE_SIZE = 56;
	private static final int BOARD_WIDTH = 521;
	private static final int BOARD_HEIGHT = 577;
	private static final int ITEM_WIDTH = 100;
	private static final int ITEM_HEIGHT = 20;

	private static final String[] PIECE_NAME = {
		null, null, null, null, null, null, null, null,
		"rk", "ra", "rb", "rn", "rr", "rc", "rp", null,
		"bk", "ba", "bb", "bn", "br", "bc", "bp", null,
	};

	private static final String[] BOARD_NAME = {
		"wood", "green", "white", "sheet", "canvas", "drops", "qianhong"
	};

	private static final String[] PIECES_NAME = {
		"wood", "delicate", "polish"
	};

	private static final String[] SOUND_NAME = {
		"click", "illegal", "move", "move2", "capture", "capture2",
		"check", "check2", "win", "draw", "loss"
	};

	private static final String[] MUSIC_NAME = {
		"express", "funny", "classic", "mozart1", "mozart4", "furelise",
		"lovdream", "waltz", "humour", "pal", "cmusic"
	};

	static final int MUSIC_MUTE = MUSIC_NAME.length;

	static final String[] LEVEL_TEXT = {
		"入门", "业余", "专业", "大师", "特级大师"
	};

	Image[] imgPieces = new Image[PIECE_NAME.length];
	Image imgSelected, imgBoard;
	AudioClip[] acSounds = new AudioClip[SOUND_NAME.length];
	AudioClip acMusic;

	Canvas canvas = new Canvas() {
		private static final long serialVersionUID = 1L;

		public void paint(Graphics g) {
			g.drawImage(imgBoard, 0, 0, this);
			for (int x = Position.FILE_LEFT; x <= Position.FILE_RIGHT; x ++) {
				for (int y = Position.RANK_TOP; y <= Position.RANK_BOTTOM; y ++) {
					int sq = Position.COORD_XY(x, y);
					sq = (flipped ? Position.SQUARE_FLIP(sq) : sq);
					int xx = PIECE_MARGIN + (x - Position.FILE_LEFT) * SQUARE_SIZE;
					int yy = PIECE_MARGIN + (y - Position.RANK_TOP) * SQUARE_SIZE;
					int pc = pos.squares[sq];
					if (pc > 0) {
						g.drawImage(imgPieces[pc], xx, yy, this);
					}
					if (sq == sqSelected || sq == Position.SRC(mvLast) ||
							sq == Position.DST(mvLast)) {
						g.drawImage(imgSelected, xx, yy, this);
					}
				}
			}
		}
	};
	Button btnMessage = new Button();

	Position pos = new Position();
	Search search = new Search(pos, 16);
	String currentFen = Position.STARTUP_FEN[0], retractFen;
	int sqSelected, mvLast;

	volatile boolean thinking = false;
	boolean flipped = false, sound = true;
	int handicap = 0, level = 0, board = 0, pieces = 0, music = 8;

	{
		setLayout(null);

		btnMessage.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				currentFen = Position.STARTUP_FEN[handicap];
				restart();
				canvas.repaint();
			}
		});
		btnMessage.setBounds(BOARD_WIDTH / 4, (BOARD_HEIGHT - ITEM_HEIGHT) / 2,
				BOARD_WIDTH / 2, ITEM_HEIGHT);
		btnMessage.setVisible(false);
		add(btnMessage);

		canvas.addMouseListener(new MouseListener() {
			public void mouseClicked(MouseEvent e) {
				// Do Nothing
			}

			public void mouseEntered(MouseEvent e) {
				// Do Nothing
			}

			public void mouseExited(MouseEvent e) {
				// Do Nothing
			}

			public void mousePressed(MouseEvent e) {
				if (!thinking && !btnMessage.isVisible()) {
					int x = Util.MIN_MAX(0, (e.getX() - PIECE_MARGIN) / SQUARE_SIZE, 8);
					int y = Util.MIN_MAX(0, (e.getY() - PIECE_MARGIN) / SQUARE_SIZE, 9);
					clickSquare(Position.COORD_XY(x + Position.FILE_LEFT, y + Position.RANK_TOP));
				}
			}

			public void mouseReleased(MouseEvent e) {
				// Do Nothing
			}
		});
		canvas.setBounds(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
		add(canvas);

		addItem("谁先走：", 0);
		final List optFlipped = new List(2);
		optFlipped.add("我先走");
		optFlipped.add("电脑先走");
		optFlipped.select(flipped ? 1 : 0);
		addItem(optFlipped, 1, 2);

		addItem("先走让子：", 3);
		final List optHandicap = new List(4);
		optHandicap.add("不让子");
		optHandicap.add("让左马");
		optHandicap.add("让双马");
		optHandicap.add("让九子");
		optHandicap.select(handicap);
		addItem(optHandicap, 4, 4);

		Button btnRestart = new Button("重新开始");
		btnRestart.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (!thinking) {
					flipped = (optFlipped.getSelectedIndex() == 1);
					handicap = optHandicap.getSelectedIndex();
					currentFen = Position.STARTUP_FEN[handicap];
					restart();
					canvas.repaint();
				}
			}
		});
		addItem(btnRestart, 8);

		Button btnRetract = new Button("悔棋");
		btnRetract.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (!thinking) {
					currentFen = retractFen;
					restart();
					canvas.repaint();
				}
			}
		});
		addItem(btnRetract, 9);

		final Label lblLevel = new Label("级别：" + LEVEL_TEXT[level]);
		addItem(lblLevel, 10);
		final Scrollbar sbLevel = new Scrollbar(Scrollbar.HORIZONTAL, level, 1, 0, 5);
		sbLevel.addAdjustmentListener(new AdjustmentListener() {
			public void adjustmentValueChanged(AdjustmentEvent e) {
				level = sbLevel.getValue();
				lblLevel.setText("级别：" + LEVEL_TEXT[level]);
			}
		});
		addItem(sbLevel, 11);

		addItem("棋盘：", -7);
		final Choice optBoard = new Choice();
		optBoard.add("栎木");
		optBoard.add("绿色大理石");
		optBoard.add("白色大理石");
		optBoard.add("再生纸");
		optBoard.add("画布");
		optBoard.add("水滴");
		optBoard.add("浅红象棋");
		optBoard.select(board);
		optBoard.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				board = optBoard.getSelectedIndex();
				loadBoard();
				canvas.repaint();
			}
		});
		addItem(optBoard, -6);

		addItem("棋子：", -5);
		final Choice optPieces = new Choice();
		optPieces.add("木刻");
		optPieces.add("精致");
		optPieces.add("光泽");
		optPieces.select(pieces);
		optPieces.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				pieces = optPieces.getSelectedIndex();
				loadPieces();
				canvas.repaint();
			}
		});
		addItem(optPieces, -4);

		final Checkbox chkMute = new Checkbox("音效", sound);
		chkMute.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				sound = chkMute.getState();
				playSound(0);
			}
		});
		addItem(chkMute, -3);

		addItem("音乐：", -2);
		final Choice optMusic = new Choice();
		optMusic.add("紧张");
		optMusic.add("风趣");
		optMusic.add("巴赫");
		optMusic.add("莫扎特一");
		optMusic.add("莫扎特四");
		optMusic.add("贝多芬");
		optMusic.add("李斯特");
		optMusic.add("柴可夫斯基");
		optMusic.add("德沃夏克");
		optMusic.add("仙剑奇侠传");
		optMusic.add("望江南");
		optMusic.add("无声");
		optMusic.select(music);
		optMusic.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				music = optMusic.getSelectedIndex();
				stopMusic();
				startMusic();
			}
		});
		addItem(optMusic, -1);
	}

	public void init() {
		loadBoard();
		loadPieces();
		for (int i = 0; i < SOUND_NAME.length; i ++) {
			acSounds[i] = getAudioClip(getCodeBase(), "sounds/" + SOUND_NAME[i] + ".wav");
		}
		startMusic();
		restart();
	}

	public void destroy() {
		stopMusic();
	}

	void addItem(String label, int top) {
		addItem(new Label(label), top);
	}

	void addItem(Component component, int top) {
		addItem(component, top, 1);
	}

	void addItem(Component component, int top, int height) {
		if (top < 0) {
			component.setBounds(BOARD_WIDTH, BOARD_HEIGHT + ITEM_HEIGHT * top, ITEM_WIDTH, ITEM_HEIGHT * height);
		} else {
			component.setBounds(BOARD_WIDTH, ITEM_HEIGHT * top, ITEM_WIDTH, ITEM_HEIGHT * height);
		}
		add(component);
	}

	void loadBoard() {
		imgBoard = getImage(getCodeBase(), "boards/" + BOARD_NAME[board] + ".gif");
	}

	void loadPieces() {
		for (int i = 0; i < PIECE_NAME.length; i ++) {
			imgPieces[i] = (PIECE_NAME[i] == null ? null : getImage(getCodeBase(),
					"pieces/" + PIECES_NAME[pieces] + "/" + PIECE_NAME[i] + ".gif"));
		}
		imgSelected = getImage(getCodeBase(), "pieces/" + PIECES_NAME[pieces] + "/oos.gif");
	}

	void startMusic() {
		if (music == MUSIC_MUTE) {
			acMusic = null;
		} else {
			acMusic = getAudioClip(getCodeBase(), "musics/" + MUSIC_NAME[music] + ".mid");
			if (acMusic != null) {
				acMusic.loop();
			}
		}
	}

	void stopMusic() {
		if (acMusic != null) {
			acMusic.stop();
		}
	}

	void restart() {
		btnMessage.setVisible(false);
		pos.fromFen(currentFen);
		retractFen = currentFen;
		sqSelected = mvLast = 0;
		if (flipped && pos.sdPlayer == 0) {
			thinking();
		}
	}

	void clickSquare(int sq_) {
		int sq = (flipped ? Position.SQUARE_FLIP(sq_) : sq_);
		int pc = pos.squares[sq];
		if ((pc & Position.SIDE_TAG(pos.sdPlayer)) != 0) {
			if (sqSelected > 0) {
				drawSquare(sqSelected);
			}
			if (mvLast > 0) {
				drawMove(mvLast);
				mvLast = 0;
			}
			sqSelected = sq;
			drawSquare(sq);
			playSound(RESP_CLICK);
		} else if (sqSelected > 0) {
			int mv = Position.MOVE(sqSelected, sq);
			if (!pos.legalMove(mv)) {
				return;
			}
			if (!pos.makeMove(mv)) {
				playSound(RESP_ILLEGAL);
				return;
			}
			int response = pos.inCheck() ? RESP_CHECK :
				pos.captured() ? RESP_CAPTURE : RESP_MOVE;
			if (pos.captured()) {
				pos.setIrrev();
			}
			mvLast = mv;
			sqSelected = 0;
			drawMove(mv);
			playSound(response);
			if (!getResult()) {
				thinking();
			}
		}
	}

	void drawSquare(int sq_) {
		int sq = (flipped ? Position.SQUARE_FLIP(sq_) : sq_);
		int x = PIECE_MARGIN + (Position.FILE_X(sq) - Position.FILE_LEFT) * SQUARE_SIZE;
		int y = PIECE_MARGIN + (Position.RANK_Y(sq) - Position.RANK_TOP) * SQUARE_SIZE;
		canvas.repaint(x, y, SQUARE_SIZE, SQUARE_SIZE);
	}

	void drawMove(int mv) {
		drawSquare(Position.SRC(mv));
		drawSquare(Position.DST(mv));
	}

	void playSound(int response) {
		if (sound && acSounds[response] != null) {
			acSounds[response].play();
		}
	}

	void showMessage(String message) {
		btnMessage.setLabel(message);
		btnMessage.setVisible(true);
	}

	void thinking() {
		thinking = true;
		new Thread() {
			public void run() {
				setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
				int mv = mvLast;
				mvLast = search.searchMain(100 << (level << 1));
				pos.makeMove(mvLast);
				drawMove(mv);
				drawMove(mvLast);
				int response = pos.inCheck() ? RESP_CHECK2 :
						pos.captured() ? RESP_CAPTURE2 : RESP_MOVE2;
				if (pos.captured()) {
					pos.setIrrev();
				}
				setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
				getResult(response);
				thinking = false;
			}
		}.start();
	}

	/** Player Move Result */
	boolean getResult() {
		return getResult(-1);
	}

	/** Computer Move Result */
	boolean getResult(int response) {
		if (pos.isMate()) {
			playSound(response < 0 ? RESP_WIN : RESP_LOSS);
			showMessage(response < 0 ? "祝贺你取得胜利！" : "请再接再厉！");
			return true;
		}
		int vlRep = pos.repStatus(3);
		if (vlRep > 0) {
			vlRep = (response < 0 ? pos.repValue(vlRep) : -pos.repValue(vlRep));
			playSound(vlRep > Position.WIN_VALUE ? RESP_LOSS :
					vlRep < -Position.WIN_VALUE ? RESP_WIN : RESP_DRAW);
			showMessage(vlRep > Position.WIN_VALUE ? "长打作负，请不要气馁！" :
					vlRep < -Position.WIN_VALUE ? "电脑长打作负，祝贺你取得胜利！" : "双方不变作和，辛苦了！");
			return true;
		}
		if (pos.moveNum > 100) {
			playSound(RESP_DRAW);
			showMessage("超过自然限着作和，辛苦了！");
			return true;
		}
		if (response >= 0) {
			playSound(response);
			retractFen = currentFen;
			currentFen = pos.toFen();
		}
		return false;
	}
}