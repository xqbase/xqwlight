package xqwlight.util.wicket;

import org.apache.wicket.Resource;
import org.apache.wicket.ResourceReference;
import org.apache.wicket.ajax.AjaxEventBehavior;
import org.apache.wicket.ajax.AjaxRequestTarget;
import org.apache.wicket.behavior.SimpleAttributeModifier;
import org.apache.wicket.markup.html.image.Image;
import org.apache.wicket.markup.html.panel.Panel;
import org.apache.wicket.util.value.ValueMap;

public class AjaxPlayerPanel extends Panel {
	private static final long serialVersionUID = 1L;

	public static final int MAX_VOLUME = 5;

	private static ResourceReference createImage(String imageName) {
		return new ResourceReference(AjaxPlayerPanel.class, "images/" + imageName + ".gif");
	}

	static ResourceReference rrMute0 = createImage("mute0");
	static ResourceReference rrMute1 = createImage("mute1");
	static ResourceReference[] rrVolume0 = new ResourceReference[MAX_VOLUME];
	static ResourceReference[] rrVolume1 = new ResourceReference[MAX_VOLUME];

	static {
		for (int i = 0; i < MAX_VOLUME; i ++) {
			rrVolume0[i] = createImage("volume0" + (i + 1));
			rrVolume1[i] = createImage("volume1" + (i + 1));
		}
	}

	private ResourceComponent embed = new ResourceComponent("embed");
	private boolean mute;
	private int volume;
	Image imgMute = new Image("imgMute", rrMute0);
	Image[] imgVolume = new Image[MAX_VOLUME];

	public AjaxPlayerPanel(String id) {
		super(id);
		add(embed.setOutputMarkupId(true));
		imgMute.add(new AjaxEventBehavior("onClick") {
			private static final long serialVersionUID = 1L;

			@Override
			protected void onEvent(AjaxRequestTarget target) {
				setMute(!getMute());
				target.addComponent(imgMute);
				muteChanged(target);
			}
		});
		add(imgMute.setOutputMarkupId(true));
		for (int i = 0; i < MAX_VOLUME; i ++) {
			final int currVolume = i + 1;
			imgVolume[i] = new Image("imgVolume" + currVolume, rrVolume1[i]);
			imgVolume[i].add(new AjaxEventBehavior("onClick") {
				private static final long serialVersionUID = 1L;

				@Override
				protected void onEvent(AjaxRequestTarget target) {
					setVolume(currVolume);
					for (int j = 1; j < MAX_VOLUME; j ++) {
						target.addComponent(imgVolume[j]);
					}
					volumeChanged(target);
				}
			});
			add(imgVolume[i].setOutputMarkupId(true));
		}
		setMute(false);
		setVolume(4);
	}

	protected void volumeChanged(AjaxRequestTarget target) {
		refresh(target);
	}

	protected void muteChanged(AjaxRequestTarget target) {
		refresh(target);
	}

	public AjaxPlayerPanel setEmbed(ResourceComponent embed) {
		this.embed.setOutputMarkupId(false).setVisible(false);
		this.embed = embed;
		this.embed.setOutputMarkupId(true).setVisible(true);
		return this;
	}

	public AjaxPlayerPanel setResource(Resource resource) {
		embed.setResource(resource);
		return this;
	}

	public AjaxPlayerPanel setResourceReference(ResourceReference resourceReference) {
		embed.setResourceReference(resourceReference);
		return this;
	}

	public AjaxPlayerPanel setResourceReference(ResourceReference resourceReference, ValueMap resourceParameters) {
		embed.setResourceReference(resourceReference, resourceParameters);
		return this;
	}

	public AjaxPlayerPanel setLoop(boolean loop) {
		embed.add(new SimpleAttributeModifier("loop", Boolean.toString(loop)));
		return this;
	}

	public AjaxPlayerPanel setMute(boolean mute) {
		this.mute = mute;
		imgMute.setImageResourceReference(mute ? rrMute1 : rrMute0);
		embed.add(new SimpleAttributeModifier("autostart", Boolean.toString(!mute)));
		return this;
	}

	public AjaxPlayerPanel setVolume(int volume) {
		this.volume = volume;
		for (int i = 1; i < MAX_VOLUME; i ++) {
			imgVolume[i].setImageResourceReference(i >= volume ? rrVolume0[i] : rrVolume1[i]);
		}
		embed.add(new SimpleAttributeModifier("volume", Integer.toString(-4096 >> volume)));
		return this;
	}

	public boolean getMute() {
		return mute;
	}

	public int getVolume() {
		return volume;
	}

	public void refresh(AjaxRequestTarget target) {
		target.addComponent(embed);
	}
}