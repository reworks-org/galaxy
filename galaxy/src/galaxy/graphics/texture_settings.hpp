auto& sampler = entt::locator<Sampler>::emplace();

// Need to create our default texture sampler object.
if (Settings::mipmap())
{
	switch (Settings::texture_filter())
	{
		case GLTextureFilter::NEAREST:
			sampler.set(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			sampler.set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case GLTextureFilter::BILINEAR:
			sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case GLTextureFilter::TRILINEAR:
			sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
	}
}
else
{
	if (Settings::texture_filter() == GLTextureFilter::NEAREST)
	{
		sampler.set(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		sampler.set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

auto ansio = std::clamp(Settings::ansiotrophy(), 1, 16);
if (ansio == 3)
{
	ansio = 4;
}
else if (ansio > 4 && ansio < 8)
{
	ansio = 8;
}
else
{
	ansio = 16;
}
sampler.setf(GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(ansio));

sampler.set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
sampler.set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
sampler.set(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
sampler.set(GL_TEXTURE_LOD_BIAS, GL_NONE);
