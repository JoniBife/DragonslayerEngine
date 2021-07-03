#ifndef INPUT_H
#define INPUT_H

#include "../Engine.h"
#include <GLFW/glfw3.h>

namespace core {
	

	enum class KeyCode {
		A = GLFW_KEY_A,
		D = GLFW_KEY_D,
		S = GLFW_KEY_S,
		W = GLFW_KEY_W,
	};

	enum class MouseButtonCode {
		LEFT = GLFW_MOUSE_BUTTON_1,
		RIGHT = GLFW_MOUSE_BUTTON_2
	};

	class Input {

	private:
		GLFWwindow* window;
		static Input* instance;

		Input(GLFWwindow* window);
		~Input();

		static void initialize(GLFWwindow* window);
		static void terminate();

	public:

		static bool isKeyDown(const KeyCode keyCode);
		static bool isKeyUp(const KeyCode keyCode);

		static bool isMouseButtonDown(const MouseButtonCode mouseButtonCode);
		static bool isMouseButtonUp(const MouseButtonCode mouseButtonCode);

		/* Position is relative to the upper left corner of the main glfw window*/
		static Vec2 getMousePosition();

		// TODO Extend with more functions

		friend class Engine;
	};
}

#endif