#include "Input.h"

Input::Input() {
}

Input::~Input() {
}

void Input::keyDown(Key k) {
	keys[k] = STATE_DOWN;
}

void Input::keyUp(Key k) {
	keys[k] = STATE_UP;
}