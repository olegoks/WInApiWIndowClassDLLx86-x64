#pragma once
#ifndef WINDOW_TYPES_HPP
#define WINDOW_TYPES_HPP

#include <windows.h>
#include <functional>

enum class Key : uint32_t {

	Nothing = 0,
	ArrowLeft = 1,
	ArrowRight = 2,
	ArrowUp = 3,
	ArrowDown = 4,
	WheelDown = 5,
	WheelUp = 6,
	MouseLeft = 7,
	MouseRight = 8,
	A = 9,
	S = 10,
	D = 11,
	W = 12

};

enum class Action : uint32_t {

	Nothing = 0,
	Down = 1,
	Up = 2

};

class KeyAction final {
public:

	Key key_;
	Action action_;

	explicit KeyAction(unsigned long long wParam, UINT Message)noexcept(true);

	explicit KeyAction()noexcept(true);

	explicit KeyAction(Key key, Action action)noexcept(true);

	bool operator !=(const KeyAction& key_action)noexcept(true);

};

struct Keystroke {

	KeyAction key_type;
	long long time_point;

	explicit Keystroke()noexcept(true);

	explicit Keystroke(KeyAction key, long long now)noexcept(true);

};


#endif //WINDOW_TYPES_HPP