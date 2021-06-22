#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "ITexture.h"
#include <vector>

class CubeMap : ITexture {

public:

	CubeMap(const std::vector<std::string>& facesFilePath);

	~CubeMap();

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
};


#endif