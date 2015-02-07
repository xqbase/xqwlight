package xqwlight.web;

import javax.servlet.http.Cookie;

import org.apache.wicket.RequestCycle;
import org.apache.wicket.ResourceReference;
import org.apache.wicket.ajax.AbstractAjaxTimerBehavior;
import org.apache.wicket.ajax.AjaxEventBehavior;
import org.apache.wicket.ajax.AjaxRequestTarget;
import org.apache.wicket.behavior.SimpleAttributeModifier;
import org.apache.wicket.markup.html.WebMarkupContainer;
import org.apache.wicket.markup.html.WebPage;
import org.apache.wicket.markup.html.basic.Label;
import org.apache.wicket.markup.html.form.DropDownChoice;
import org.apache.wicket.markup.html.form.Form;
import org.apache.wicket.markup.html.form.RadioChoice;
import org.apache.wicket.markup.html.image.Image;
import org.apache.wicket.markup.html.link.Link;
import org.apache.wicket.model.Model;
import org.apache.wicket.protocol.http.WebRequest;
import org.apache.wicket.protocol.http.WebResponse;
import org.apache.wicket.util.string.Strings;
import org.apache.wicket.util.time.Duration;

import xqwlight.Position;
import xqwlight.Search;
import xqwlight.util.Integers;
import xqwlight.util.wicket.AjaxPlayerPanel;
import xqwlight.util.wicket.RefreshPage;
import xqwlight.util.wicket.ResourceComponent;

public class XQWLightPage extends WebPage {
	private static final long serialVersionUID = 1L;

	private static final int STATUS_READY = 0;
	private static final int STATUS_WIN = 1;
	private static final int STATUS_DRAW = 2;
	private static final int STATUS_LOSS = 3;
	private static final int STATUS_THINKING = 4;
	private static final int STATUS_TO_MOVE = 5;

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

	private static final String[] STATUS_NAME = {
		"ready", "win", "draw", "loss", "thinking"
	};

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
		"check", "check2", "win", "draw", "loss",
	};

	private static final String[] MUSIC_NAME = {
		"express", "funny", "classic", "mozart1", "mozart4", "furelise",
		"lovdream", "waltz", "humour", "pal", "cmusic"
	};

	private static ResourceReference createImage(String imageName) {
		return new ResourceReference(XQWLightPage.class, "images/" + imageName + ".gif");
	}

	private static ResourceReference createBoard(String boardName) {
		return new ResourceReference(XQWLightPage.class, "boards/" + boardName + ".gif");
	}

	private static ResourceReference createPiece(String piecesName, String pieceName) {
		return new ResourceReference(XQWLightPage.class, "pieces/" + piecesName +
				"/" + pieceName + ".gif");
	}

	private static ResourceReference createSound(String soundName) {
		return new ResourceReference(XQWLightPage.class, "sounds/" + soundName + ".wav");
	}

	private static ResourceReference createMusic(String musicName) {
		return new ResourceReference(XQWLightPage.class, "musics/" + musicName + ".mid");
	}

	static ResourceReference[] rrStatus = new ResourceReference[STATUS_NAME.length];
	static ResourceReference[] rrBoards = new ResourceReference[BOARD_NAME.length];
	static ResourceReference[][] rrPieces = new ResourceReference[PIECES_NAME.length][24];
	static ResourceReference[][] rrSelected = new ResourceReference[PIECES_NAME.length][24];
	static ResourceReference[] rrSound = new ResourceReference[SOUND_NAME.length];
	static ResourceReference[] rrMusic = new ResourceReference[MUSIC_NAME.length];
	static ResourceReference rrStar0 = createImage("star0");
	static ResourceReference rrStar1 = createImage("star1");

	static {
		for (int i = 0; i < STATUS_NAME.length; i ++) {
			rrStatus[i] = createImage(STATUS_NAME[i]);
		}
		for (int i = 0; i < BOARD_NAME.length; i ++) {
			rrBoards[i] = createBoard(BOARD_NAME[i]);
		}
		for (int i = 0; i < PIECES_NAME.length; i ++) {
			String piecesName = PIECES_NAME[i];
			ResourceReference imgOo = createPiece(piecesName, "oo");
			ResourceReference imgOos = createPiece(piecesName, "oos");
			for (int j = 0; j < 24; j ++) {
				if (PIECE_NAME[j] == null) {
					rrPieces[i][j] = imgOo;
					rrSelected[i][j] = imgOos;
				} else {
					rrPieces[i][j] = createPiece(piecesName, PIECE_NAME[j]);
					rrSelected[i][j] = createPiece(piecesName, PIECE_NAME[j] + "s");
				}
			}
		}
		for (int i = 0; i < SOUND_NAME.length; i ++) {
			rrSound[i] = createSound(SOUND_NAME[i]);
		}
		for (int i = 0; i < MUSIC_NAME.length; i ++) {
			rrMusic[i] = createMusic(MUSIC_NAME[i]);
		}
	}

	static final String[] LEVEL_STRING = {
		"入门", "业余", "专业", "大师", "特级大师",
	};

	int status = STATUS_READY;
	Position pos = new Position();
	Search search = new Search(pos, 16);
	int boardId = getCookieValue("board", 0, Choices.getBoardTypes().size() - 1, 0);
	int piecesId = getCookieValue("pieces", 0, Choices.getPiecesTypes().size() - 1, 0);
	int level = getCookieValue("level", 0, LEVEL_STRING.length - 1, 0);
	String cookieFen, retractFen;
	int sqSelected = 0, mvLast = 0, mvResult;

	Label lblTitle = new Label("lblTitle", "就绪");
	Image imgTitle = new Image("imgTitle", rrStatus[STATUS_READY]);
	Image[] imgSquares = new Image[256];
	Image[] imgLevels = new Image[LEVEL_STRING.length];
	Label lblLevel = new Label("lblLevel", LEVEL_STRING[level]);
	AjaxPlayerPanel playerSound = new AjaxPlayerPanel("playerSound") {
		private static final long serialVersionUID = 1L;

		@Override
		protected void muteChanged(AjaxRequestTarget target) {
			setResourceReference(rrSound[RESP_CLICK]);
			super.muteChanged(target);
			addCookieValue("soundMute", Boolean.toString(getMute()));
		}

		@Override
		protected void volumeChanged(AjaxRequestTarget target) {
			setResourceReference(rrSound[RESP_CLICK]);
			super.volumeChanged(target);
			addCookieValue("soundVolume", Integer.toString(getVolume()));
		}
	};

	private class ThinkingBehavior extends AbstractAjaxTimerBehavior {
		private static final long serialVersionUID = 1L;

		ThinkingBehavior() {
			super(Duration.seconds(1));
		}

		@Override
		protected void onTimer(AjaxRequestTarget target) {
			AjaxBoard board = new AjaxBoard(target);
			if (status != STATUS_TO_MOVE) {
				return;
			}
			if (mvLast > 0) {
				board.drawMove(mvLast);
			}
			mvLast = mvResult;
			pos.makeMove(mvLast);
			board.drawMove(mvLast, DRAW_SELECTED);
			int response = pos.inCheck() ? RESP_CHECK2 :
					pos.captured() ? RESP_CAPTURE2 : RESP_MOVE2;
			if (pos.captured()) {
				pos.setIrrev();
			}
			if (getResult(board, response)) {
				addCookieValue("fen", null);
			} else {
				board.playSound(response);
				board.setMessage("就绪", STATUS_READY);
				retractFen = cookieFen;
				cookieFen = pos.toFen();
				addCookieValue("fen", cookieFen);
			}
		}
	}

	ThinkingBehavior oldBehavior = null, newBehavior = null;

	private static final boolean DRAW_SELECTED = true;

	private class AjaxBoard {
		private AjaxRequestTarget target;

		AjaxBoard(AjaxRequestTarget target) {
			this.target = target;
		}

		void drawSquare(int sq) {
			drawSquare(sq, false);
		}

		void drawSquare(int sq, boolean bSelected) {
			imgSquares[sq].setImageResourceReference((bSelected ? rrSelected : rrPieces)
					[piecesId][pos.squares[sq]]);
			target.addComponent(imgSquares[sq]);
		}

		void drawMove(int mv) {
			drawMove(mv, false);
		}

		void drawMove(int mv, boolean bSelected) {
			drawSquare(Position.SRC(mv), bSelected);
			drawSquare(Position.DST(mv), bSelected);
		}

		void playSound(int response) {
			if (!playerSound.getMute()) {
				playerSound.setResourceReference(rrSound[response]);
				playerSound.refresh(target);
			}
		}

		void setMessage(String msg, int status) {
			lblTitle.setDefaultModelObject(msg);
			imgTitle.setImageResourceReference(rrStatus[status]);
			XQWLightPage.this.status = status;
			if (status == STATUS_THINKING) {
				newBehavior = new ThinkingBehavior();
				imgTitle.add(newBehavior);
				if (oldBehavior != null) {
					imgTitle.remove(oldBehavior);
				}
			} else {
				if (newBehavior != null) {
					newBehavior.stop();
					oldBehavior = newBehavior;
				}
			}
			if (target != null) {
				target.addComponent(lblTitle);
				target.addComponent(imgTitle);
			}
		}
	}

	private int getCookieValue(String name, int min, int max, int defaultValue) {
		Cookie cookie = ((WebRequest) getRequest()).getCookie(name);
		int value = (cookie == null ? defaultValue :
				Integers.parse(cookie.getValue(), defaultValue));
		return Integers.minMax(min, value, max);
	}

	private boolean getCookieValue(String name, boolean defaultValue) {
		boolean value = defaultValue;
		Cookie cookie = ((WebRequest) getRequest()).getCookie(name);
		if (cookie != null) {
			try {
				value = Boolean.parseBoolean(cookie.getValue());
			} catch (Exception e) {
				// Ignored;
			}
		}
		return value;
	}

	private String getCookieValue(String name) {
		Cookie cookie = ((WebRequest) getRequest()).getCookie(name);
		return (cookie == null ? null : cookie.getValue());
	}

	public XQWLightPage() {
		// 1. Start-Up Position ...
		boolean flipped = getCookieValue("flipped", false);
		int handicap = getCookieValue("handicap", 0, LEVEL_STRING.length - 1, 0);
		int musicId = getCookieValue("music", 0, Choices.getMusicTypes().size() - 1, 8);
		cookieFen = getCookieValue("fen");
		cookieFen = (cookieFen == null ? Position.STARTUP_FEN[handicap] : cookieFen);
		pos.fromFen(cookieFen);
		retractFen = cookieFen;
		// 2. Title ...
		add(lblTitle.setOutputMarkupId(true));
		add(imgTitle.setOutputMarkupId(true));
		// 3. Board ...
		// See LocalizedImageResource.setSrcAttribute(ComponentTag)
		CharSequence url = RequestCycle.get().urlFor(rrBoards[boardId], null);
		url = RequestCycle.get().getOriginalResponse().
				encodeURL(Strings.replaceAll(url, "&", "&amp;"));
		WebMarkupContainer tdBoard = new WebMarkupContainer("tdBoard");
		tdBoard.add(new SimpleAttributeModifier("background", url));
		tdBoard.setOutputMarkupId(true);
		add(tdBoard);
		for (int i = 0; i < 256; i ++) {
			final int sq = i;
			if (!Position.IN_BOARD(sq)) {
				continue;
			}
			int sqFlipped = flipped ? Position.SQUARE_FLIP(sq) : sq;
			String sqName = "" + (char) ('a' + Position.FILE_X(sqFlipped) - Position.FILE_LEFT) +
					(char) ('9' - Position.RANK_Y(sqFlipped) + Position.RANK_TOP);
			imgSquares[sq] = new Image(sqName, rrPieces[piecesId][pos.squares[sq]]);
			imgSquares[sq].add(new AjaxEventBehavior("onClick") {
				private static final long serialVersionUID = 1L;

				@Override
				protected void onEvent(AjaxRequestTarget target) {
					if (status != STATUS_READY) {
						return;
					}
					AjaxBoard board = new AjaxBoard(target);
					int pc = pos.squares[sq];
					if ((pc & Position.SIDE_TAG(pos.sdPlayer)) != 0) {
						if (sqSelected > 0) {
							board.drawSquare(sqSelected);
						}
						if (mvLast > 0) {
							board.drawMove(mvLast);
							mvLast = 0;
						}
						sqSelected = sq;
						board.drawSquare(sq, DRAW_SELECTED);
						board.playSound(RESP_CLICK);
					} else if (sqSelected > 0) {
						int mv = Position.MOVE(sqSelected, sq);
						if (!pos.legalMove(mv)) {
							return;
						}
						if (!pos.makeMove(mv)) {
							board.playSound(RESP_ILLEGAL);
							return;
						}
						int response = pos.inCheck() ? RESP_CHECK :
								pos.captured() ? RESP_CAPTURE : RESP_MOVE;
						if (pos.captured()) {
							pos.setIrrev();
						}
						board.drawMove(mv, DRAW_SELECTED);
						board.playSound(response);
						sqSelected = 0;
						mvLast = mv;
						if (getResult(board)) {
							addCookieValue("fen", null);
						} else {
							thinking(target);
						}
					}
				}
			});
			tdBoard.add(imgSquares[sq].setOutputMarkupId(true));
		}
		// 4. Form ...
		// 4.1. Player Moves ...
		final RadioChoice<String> selFlipped = new RadioChoice<>("selFlipped",
				new Model<>(flipped ? Choices.FLIPPED_TRUE : Choices.FLIPPED_FALSE),
				Choices.getFlippedTypes()).setSuffix("");
		// selFlipped.setModelValue(new String[] {Integer.toString(flipped ? 1 : 0)});
		// 4.2. Handicap ...
		final DropDownChoice<String> selHandicap = new DropDownChoice<>("selHandicap",
				new Model<>(Choices.getHandicapTypes().get(handicap)), Choices.getHandicapTypes());
		// selHandicap.setModelValue(new String[] {Integer.toString(handicap)});
		// 4.3. New Game ...
		Form<Void> frm = new Form<Void>("frm") {
			private static final long serialVersionUID = 1L;

			@Override
			public void onSubmit() {
				if (status != STATUS_THINKING) {
					addCookieValue("fen", null);
					addCookieValue("flipped", Boolean.toString(selFlipped.
							getModelObject().equals(Choices.FLIPPED_TRUE)));
					addCookieValue("handicap", selHandicap.getModelValue());
					setResponsePage(RefreshPage.class);
				}
			}
		};
		frm.add(selFlipped);
		frm.add(selHandicap);
		add(frm);
		// 4.4. Board ...
		final DropDownChoice<String> selBoard = new DropDownChoice<>("selBoard",
				new Model<>(Choices.getBoardTypes().get(boardId)), Choices.getBoardTypes());
		// 4.5. Pieces ...
		final DropDownChoice<String> selPieces = new DropDownChoice<>("selPieces",
				new Model<>(Choices.getPiecesTypes().get(piecesId)), Choices.getPiecesTypes());
		// 4.6. Musics ...
		final DropDownChoice<String> selMusic = new DropDownChoice<>("selMusic",
				new Model<>(Choices.getMusicTypes().get(musicId)), Choices.getMusicTypes());
		// 4.7. Apply ...
		Form<Void> frmGui = new Form<Void>("frmGui") {
			private static final long serialVersionUID = 1L;

			@Override
			protected void onSubmit() {
				addCookieValue("board", selBoard.getModelValue());
				addCookieValue("pieces", selPieces.getModelValue());
				addCookieValue("music", selMusic.getModelValue());
				setResponsePage(RefreshPage.class);
			}
		};
		frmGui.add(selBoard);
		frmGui.add(selPieces);
		frmGui.add(selMusic);
		add(frmGui);
		// 5. Retract and Cookies ...
		add(new Link<Void>("lnkRetract") {
			private static final long serialVersionUID = 1L;

			@Override
			public void onClick() {
				if (status != STATUS_THINKING) {
					addCookieValue("fen", retractFen);
					setResponsePage(RefreshPage.class);
				}
			}
		});
		add(new Link<Void>("lnkCookies") {
			private static final long serialVersionUID = 1L;

			@Override
			public void onClick() {
				if (status != STATUS_THINKING) {
					setResponsePage(new CookiesPage("Ajax Chess"));
				}
			}
		}.setVisible(getApplication().getConfigurationType().equalsIgnoreCase("DEVELOPMENT")));
		// 6. Level ...
		for (int i = 0; i < LEVEL_STRING.length; i ++) {
			final int currLevel = i;
			imgLevels[i] = new Image("imgLevel" + i, i > level ? rrStar0 : rrStar1);
			imgLevels[i].add(new AjaxEventBehavior("onClick") {
				private static final long serialVersionUID = 1L;

				@Override
				protected void onEvent(AjaxRequestTarget target) {
					level = currLevel;
					for (int j = 1; j < LEVEL_STRING.length; j ++) {
						imgLevels[j].setImageResourceReference(j > level ? rrStar0 : rrStar1);
						target.addComponent(imgLevels[j]);
					}
					lblLevel.setDefaultModelObject(LEVEL_STRING[level]);
					target.addComponent(lblLevel);
					addCookieValue("level", Integer.toString(level));
				}
			});
			add(imgLevels[i].setOutputMarkupId(true));
		}
		add(lblLevel.setOutputMarkupId(true));
		// 7. Sounds ...
		ResourceComponent embedSound = new ResourceComponent("embedSound");
		add(embedSound);
		playerSound.setEmbed(embedSound);
		playerSound.setMute(getCookieValue("soundMute", false));
		playerSound.setVolume(getCookieValue("soundVolume", 1, AjaxPlayerPanel.MAX_VOLUME, 3));
		add(playerSound);
		// 8. Musics ...
		ResourceComponent embedMusic = new ResourceComponent("embedMusic", rrMusic[musicId]);
		add(embedMusic);
		AjaxPlayerPanel playerMusic = new AjaxPlayerPanel("playerMusic") {
			private static final long serialVersionUID = 1L;

			@Override
			protected void muteChanged(AjaxRequestTarget target) {
				super.muteChanged(target);
				addCookieValue("musicMute", Boolean.toString(getMute()));
			}

			@Override
			protected void volumeChanged(AjaxRequestTarget target) {
				super.volumeChanged(target);
				addCookieValue("musicVolume", Integer.toString(getVolume()));
			}
		};
		playerMusic.setEmbed(embedMusic);
		playerMusic.setMute(getCookieValue("musicMute", false));
		playerMusic.setVolume(getCookieValue("musicVolume", 1, AjaxPlayerPanel.MAX_VOLUME, 2));
		playerMusic.setLoop(true);
		add(playerMusic);
		// 9. Computer Moves First ...
		if ((pos.sdPlayer == 0 ? flipped : !flipped) && !pos.isMate()) {
			thinking(null);
		}
	}

	void addCookieValue(String name, String value) {
		Cookie cookie = new Cookie(name, value);
		cookie.setMaxAge(365 * 86400);
		if (value == null) {
			((WebResponse) getResponse()).clearCookie(cookie);
		} else {
			((WebResponse) getResponse()).addCookie(cookie);
		}
	}

	void thinking(AjaxRequestTarget target) {
		new AjaxBoard(target).setMessage("思考中……", STATUS_THINKING);
		new Thread() {
			@Override
			public void run() {
				mvResult = search.searchMain(100 << (level << 1));
				status = STATUS_TO_MOVE;
			}
		}.start();
	}

	boolean getResult(AjaxBoard board) {
		return getResult(board, -1);
	}

	boolean getResult(AjaxBoard board, int response) {
		if (pos.isMate()) {
			if (response < 0) {
				board.playSound(RESP_WIN);
				board.setMessage("祝贺你取得胜利！", STATUS_WIN);
			} else {
				board.playSound(RESP_LOSS);
				board.setMessage("请再接再厉！", STATUS_LOSS);
			}
			return true;
		}
		int vlRep = pos.repStatus(3);
		if (vlRep > 0) {
			vlRep = (response < 0 ? pos.repValue(vlRep) : -pos.repValue(vlRep));
			if (vlRep > Position.WIN_VALUE) {
				board.playSound(RESP_LOSS);
				board.setMessage("长打作负，请不要气馁！", STATUS_LOSS);
			} else if (vlRep < -Position.WIN_VALUE) {
				board.playSound(RESP_WIN);
				board.setMessage("电脑长打作负，祝贺你取得胜利！", STATUS_WIN);
			} else {
				board.playSound(RESP_DRAW);
				board.setMessage("双方不变作和，辛苦了！", STATUS_DRAW);
			}
			return true;
		}
		if (pos.moveNum > 100) {
			board.playSound(RESP_DRAW);
			board.setMessage("超过自然限着作和，辛苦了！", STATUS_DRAW);
			return true;
		}
		return false;
	}
}