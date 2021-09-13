#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "ITexture.h"
#include <vector>

class CubeMap : ITexture {

private:
	bool hasMips = false;

public:

	CubeMap(const std::vector<std::string>& facesFilePath, bool hasMips = false);

	~CubeMap();

	void addMip(const std::vector<std::string>& facesFilePath, GLint level);

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
};


#endif