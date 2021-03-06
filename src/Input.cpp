#include "Input.hpp"

#include <cstring>
#include <iostream>

EM_BOOL	Input::keyDown(int, const EmscriptenKeyboardEvent* keyEvent, void* data)
{
	Input* input = (Input*)data;

	input->_keyDown[keyEvent->code] = true;
	input->_keyPress[keyEvent->code] = true;
	return false;
}

EM_BOOL	Input::keyUp(int, const EmscriptenKeyboardEvent* keyEvent, void* data)
{
	Input* input = (Input*)data;

	input->_keyDown[keyEvent->code] = false;
	return false;
}

EM_BOOL Input::mouseDown(int, const EmscriptenMouseEvent* mouseEvent, void* data)
{
	Input* input = (Input*)data;

	input->_mouseDown[mouseEvent->button] = true;
	return false;
}

EM_BOOL Input::mouseUp(int, const EmscriptenMouseEvent* mouseEvent, void* data)
{
	Input* input = (Input*)data;

	input->_mouseDown[mouseEvent->button] = false;
	input->_mouseClick[mouseEvent->button] = true;
	return false;
}

EM_BOOL Input::mouseMove(int, const EmscriptenMouseEvent* mouseEvent, void* data)
{
	Input* input = (Input*)data;

	int mx = mouseEvent->targetX;
	int my = mouseEvent->targetY;
	int w = input->_window->Width();
	int h = input->_window->Height();

	input->_mousePos.x = (2.0 * float(mx) / float(w)) - 1.0;
	// mousePos.y is inverted because emscripten coordinates start from
	// top left, wheras openGL coordinates start from bottom left.
	// We use OpenGL coordinate system.
	input->_mousePos.y = 1 - (2.0 * float(my) / float(h));
	return false;
}

Input::Input(const char* element, const GLWindow* window)
{
	_window = window;
	emscripten_set_keydown_callback(element, this, false, keyDown);
	emscripten_set_keyup_callback(element, this, false, keyUp);
	emscripten_set_mousedown_callback(element, this, false, mouseDown);
	emscripten_set_mouseup_callback(element, this, false, mouseUp);
	emscripten_set_mousemove_callback(element, this, false, mouseMove);
}

void	Input::Update()
{
	_keyPress.clear();
	std::memset(_mouseClick, 0, sizeof(_mouseClick));
}

bool	Input::KeyDown(const std::string& keycode) const
{
	auto it = _keyDown.find(keycode);
	if (it != _keyDown.end())
		return (*it).second;
	return false;
}

bool	Input::KeyPress(const std::string& keycode) const
{
	if (_keyPress.count(keycode))
		return true;
	return false;
}

bool	Input::MouseDown(int button) const
{
	if (button < _mouseButtons && button >= 0)
		return _mouseDown[button];
	return false;
}

bool	Input::MouseClick(int button) const
{
	if (button < _mouseButtons && button >= 0)
		return _mouseClick[button];
	return false;
}

glm::vec2	Input::MousePos() const
{
	return _mousePos;
}
