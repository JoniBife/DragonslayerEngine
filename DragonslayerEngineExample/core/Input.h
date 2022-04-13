#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <LMath/Vec2.hpp>

using namespace LMath;

namespace core {
	
	enum class KeyCode {
		ZERO = GLFW_KEY_0,
		ONE = GLFW_KEY_1,
		TWO = GLFW_KEY_2,
		THREE = GLFW_KEY_3,
		FOUR = GLFW_KEY_4,
		FIVE = GLFW_KEY_5,
		SIX = GLFW_KEY_6,
		SEVEN = GLFW_KEY_7,
		EIGHT = GLFW_KEY_8,
		NINE = GLFW_KEY_9,
		RIGHT = GLFW_KEY_RIGHT,
		LEFT = GLFW_KEY_LEFT,
		UP = GLFW_KEY_UP,
		DOWN = GLFW_KEY_DOWN,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		LEFT_CTRL = GLFW_KEY_LEFT_CONTROL
		// TODO Add rest of keys
	};

	enum class MouseButtonCode {
		LEFT = GLFW_MOUSE_BUTTON_1,
		RIGHT = GLFW_MOUSE_BUTTON_2,
		MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
	};

	class Input {

	private:
		GLFWwindow* window;
		bool cursorVisible = true;
		static Input* instance;

		Input(GLFWwindow* window);
		~Input();

	public:

		static void terminate();
		static void initialize(GLFWwindow* window);

		static bool isKeyDown(const KeyCode keyCode);
		static bool isKeyUp(const KeyCode keyCode);

		static bool isMouseButtonDown(const MouseButtonCode mouseButtonCode);
		static bool isMouseButtonUp(const MouseButtonCode mouseButtonCode);

		static void setCursorVisibility(bool visible);

		/* Position is relative to the upper left corner of the main glfw window*/
		static Vec2 getMousePosition();
		static void setMousePosition(const Vec2& position);

		static float getMouseScroll();

		// TODO Extend with more functions
	};
}

#endif