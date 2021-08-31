#include "../math/Vec2.h"
#ifndef IM_GUI_EXTENSIONS
#define IM_GUI_EXTENSIONS

namespace ImGui {

	bool InputVec2(const char* label, Vec2& v);
	bool InputVec3(const char* label, Vec3& v);
	bool InputVec4(const char* label, Vec4& v);

}



#endif
