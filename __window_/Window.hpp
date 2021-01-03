#pragma once

#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <Windows.h>
#include <string>

#include "ThreadSafeQueue/ThreadSafeQueue.hpp"
#include "WindowTypes.hpp"

#define COMPILING_DLL

#ifdef COMPILING_DLL
	#define IATRIBUTE __declspec(dllexport)
#else
	#define IATRIBUTE __declspec(dllimport)
#endif

struct Parametrs {

	size_t width_;
	size_t height_;
	size_t x_;
	size_t y_;
	std::wstring title_;

};

IATRIBUTE class Window final{
private:

	static const std::wstring kClassName;
	HWND self_hWnd_;
	mutable HINSTANCE hInstance_;
	HDC device_context_;
	ThreadSafeQueue<Keystroke> keystrokes_queue_;
	Parametrs params_;

	void PushKeystroke(KeyAction key)noexcept(true);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM  wParam, LPARAM lParam);

public:

	//Window class constants
	static const std::wstring kDefaultTitle;
	static const std::size_t kDefaultWidth;
	static const std::size_t kDefaultHeight;
	static const std::size_t kDefaultX;
	static const std::size_t kDefaultY;

	IATRIBUTE void PlugHInstance(const HINSTANCE hInstance)const noexcept(true);
	IATRIBUTE explicit Window()noexcept(true);
	IATRIBUTE ~Window();
	IATRIBUTE void ShowFrame(const BITMAPINFO& info, const void* frame, const size_t size)noexcept(true);
	IATRIBUTE inline Keystroke PopKeystroke()noexcept(true);
	IATRIBUTE void Create()noexcept(true);
	IATRIBUTE void Show()const noexcept(true);
	IATRIBUTE WPARAM StartMessageLoop()const noexcept(true);
	IATRIBUTE void RegisterWindowClass()noexcept(true);
	IATRIBUTE inline size_t Width()const noexcept(true) { return params_.width_; };
	IATRIBUTE inline size_t Height()const noexcept(true) { return params_.height_; };
	IATRIBUTE inline void Width(const size_t width)noexcept(true) { params_.width_ = width; };
	IATRIBUTE inline void Height(const size_t height)noexcept(true) { params_.height_ = height; };
	IATRIBUTE inline HWND Handle()const noexcept(true) { return self_hWnd_; };
	IATRIBUTE inline void Destroy()noexcept(true) { SendMessage(self_hWnd_, WM_CLOSE, NULL, NULL); }

};

#endif // !_WINDOW_HPP_