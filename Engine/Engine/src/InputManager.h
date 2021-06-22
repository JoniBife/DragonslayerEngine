#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "GLFW/glfw3.h"

#include <string>
#include <map>

class InputManager {

private:
	int currInputReceiverId = -1;

public:
	std::map<int, bool> keysPressed;
	bool receivingTextInput = false;
	std::string textInput;

	static InputManager* instance;

	InputManager(GLFWwindow* window);
	void startTextInput(int inputReceiverId);
	void resumeTextInput(int inputReceiverId, const std::string& oldInput);
	void stopTextInput(int inputReceiverId);
	std::string getCurrTextInput() const;

	int inputReceiverId = -1;

	int generateInputReceiverId();
	
	bool isKeyPressed(int key);

public:

	static void createInstance(GLFWwindow* window);

	static InputManager* getInstance();

	static void destroyInstance();

};

#endif
