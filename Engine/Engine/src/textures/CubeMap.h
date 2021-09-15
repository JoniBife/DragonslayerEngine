#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "ITexture.h"
#include <vector>

class CubeMap : ITexture {

private:
	bool hasMips = false;
	unsigned int width = 0;
	unsigned int height = 0;

	CubeMap();

public:

	CubeMap(const std::vector<std::string>& facesFilePath, bool hasMips = false);

	static CubeMap* fromFloatArrayFiles(const std::vector<std::string>& facesFilePath, unsigned int width, unsigned int height, bool hasMips = false);

	~CubeMap();

	void addMip(const std::vector<std::string>& facesFilePath, GLint level);
	void addMipFromFloatArray(const std::vector<std::string>& facesFilePath, GLint level);

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
};


#endif