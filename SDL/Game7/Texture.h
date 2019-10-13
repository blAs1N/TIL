#pragma once

#include <string>

class Texture {
public:
	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

private:
	unsigned int textureId;

	int width;
	int height;
};

