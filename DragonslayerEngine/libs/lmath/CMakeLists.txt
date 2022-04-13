cmake_minimum_required (VERSION 3.8)

project (LMath VERSION 1.0.0 DESCRIPTION "A basic math library for computer graphics and game development.")

add_library (LMath STATIC 
	"src/Mat2.cpp" 
	"src/Mat3.cpp" 
	"src/Mat4.cpp" 
	"src/MathAux.cpp" 
	"src/Qtrn.cpp"
	"src/Vec2.cpp"
	"src/Vec3.cpp"
	"src/Vec4.cpp"
)

target_include_directories(LMath PUBLIC "include" PRIVATE "src")

set_property(TARGET LMath PROPERTY CXX_STANDARD 17)

target_compile_features(LMath PUBLIC cxx_std_17)

