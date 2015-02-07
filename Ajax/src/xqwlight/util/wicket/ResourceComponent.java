package xqwlight.util.wicket;

import org.apache.wicket.Resource;
import org.apache.wicket.ResourceReference;
import org.apache.wicket.markup.ComponentTag;
import org.apache.wicket.markup.html.WebComponent;
import org.apache.wicket.markup.html.image.resource.LocalizedImageResource;
import org.apache.wicket.util.value.ValueMap;

public class ResourceComponent extends WebComponent {
	private static final long serialVersionUID = 1L;

	private LocalizedImageResource lir = new LocalizedImageResource(this);

	public ResourceComponent(String id) {
		super(id);
	}

	public ResourceComponent(String id, Resource resource) {
		this(id);
		setResource(resource);
	}

	public ResourceComponent(String id, ResourceReference resourceReference) {
		this(id);
		setResourceReference(resourceReference);
	}

	public ResourceComponent(String id, ResourceReference resourceReference, ValueMap resourceParameters) {
		this(id);
		setResourceReference(resourceReference, resourceParameters);
	}

	public Resource getResource() {
		return lir.getResource();
	}

	public ResourceReference getResourceReference() {
		return lir.getResourceReference();
	}

	public ResourceComponent setResource(Resource resource) {
		lir.setResource(resource);
		return this;
	}

	public ResourceComponent setResourceReference(ResourceReference resourceReference) {
		lir.setResourceReference(resourceReference);
		return this;
	}

	public ResourceComponent setResourceReference(ResourceReference resourceReference, ValueMap resourceParameters) {
		lir.setResourceReference(resourceReference, resourceParameters);
		return this;
	}

	@Override
	protected void onComponentTag(ComponentTag tag) {
		super.onComponentTag(tag);
		lir.setSrcAttribute(tag);
	}
}