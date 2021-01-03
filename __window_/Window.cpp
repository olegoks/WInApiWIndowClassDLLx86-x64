#include "Window.hpp"
#include <chrono>

//Window constants
const std::wstring Window::kDefaultTitle{ L"Default window title" };
const std::size_t Window::kDefaultWidth{ 1920 };
const std::size_t Window::kDefaultHeight{ 1080 };
const std::size_t Window::kDefaultX{ 0 };
const std::size_t Window::kDefaultY{ 0 };
const std::wstring Window::kClassName{ L"Main window class name." };

Window::Window()noexcept(true):
	hInstance_{ nullptr },
	params_{ kDefaultWidth, 
	kDefaultHeight, 
	kDefaultX, 
	kDefaultY, 
	kDefaultTitle },
	device_context_{ nullptr },
	self_hWnd_{ nullptr }{}

Keystroke Window::PopKeystroke()noexcept(true) {
	
	if(!keystrokes_queue_.Empty())
		return keystrokes_queue_.Pop();
	
	return Keystroke{};

}

void Window::PushKeystroke(KeyAction key)noexcept(true) {

	using namespace std::chrono;
	high_resolution_clock::time_point now_time_point = high_resolution_clock::now();
	long long  now = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
	const Keystroke keystroke = Keystroke{ key, now };
	keystrokes_queue_.Push(keystroke);

}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT Message, WPARAM  wParam, LPARAM lParam) {

	Window* const this_wnd = reinterpret_cast<Window*>(GetWindowLong(hWnd, 0));

	switch (Message) {

	case WM_KEYUP: {

		const unsigned long long key = wParam;
		this_wnd->PushKeystroke(KeyAction{ wParam, Message });
		break;
	};

	case WM_KEYDOWN: {

		const UINT key = wParam;
		this_wnd->PushKeystroke(KeyAction{ wParam, Message });
		break;
	}

	case WM_CLOSE: { DestroyWindow(hWnd);break; }

	case WM_DESTROY: { PostQuitMessage(0);break; }
		
	default: return DefWindowProc(hWnd, Message, wParam, lParam); 
		
	}

}

void Window::PlugHInstance(const HINSTANCE hInstance)const noexcept(true){

	if (hInstance_ == 0)
		hInstance_ = hInstance;

}
		
Window::~Window() {}

void Window::RegisterWindowClass()noexcept(true) {

	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = this->WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 4;
	wc.hInstance = hInstance_;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = kClassName.c_str();
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {

		MessageBox(nullptr,
			L"Failed to register window class.",
			L"Error.",
			MB_OK);

	}

}

void Window::Create()noexcept(true){
			
	//Creating window with default//installed parametrs
	self_hWnd_ = CreateWindowEx(WS_EX_TOPMOST, kClassName.c_str(),
		params_.title_.c_str(),
		WS_OVERLAPPEDWINDOW,//WS_POPUP,
		params_.x_,
		params_.y_, 
		params_.width_, 
		params_.height_, 
		HWND(NULL), 
		HMENU(NULL), 
		(HINSTANCE)hInstance_, 
		NULL);

	//If window didn't create, show error			
	if (!self_hWnd_) {
	
		MessageBox(NULL, L"Window didn't create.", L"Error", MB_OK);

	//Success window creation
	} else {

		SetWindowLong(self_hWnd_, 0, reinterpret_cast<long>(this));
		SetProcessDPIAware();//Вызывая эту функцию (SetProcessDPIAware), вы сообщаете системе, что интерфейс вашего приложения умеет сам правильно масштабироваться при высоких значениях DPI (точки на дюйм). Если вы не выставите этот флаг, то интерфейс вашего приложения может выглядеть размыто при высоких значениях DPI.
		device_context_ = GetDC(self_hWnd_);

	}

}

void Window::Show()const noexcept(true){

	ShowWindow(self_hWnd_, SW_MAXIMIZE);

}

WPARAM Window::StartMessageLoop()const noexcept(true){
			
	MSG msg; 
	msg = { 0 }; 

	while (GetMessage(&msg, NULL, NULL, NULL)) {
					
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return msg.wParam;

}

void Window::ShowFrame(const BITMAPINFO& info, const void* frame, const size_t size)noexcept(true) {

	#ifdef COUNT_FPS

	using namespace std::chrono;
	using time_point = high_resolution_clock::time_point;
	using duration = high_resolution_clock::duration;

	//Count current FPS one time to 1 second

	//Last counting fps time point
	static time_point last_fps_counting = high_resolution_clock::now();

	//Accumulate number of frame between counting fps
	static size_t frame_number = 0;

	//if this function is called frame is ready 
	frame_number++;

	//Last render duration
	duration from_last_fps_counting = high_resolution_clock::now() - last_fps_counting;

	//Condition: if one second passed from last counting fps
	if (from_last_fps_counting > 1s) {

		unsigned int render_duration = duration_cast<milliseconds>(from_last_fps_counting).count();
		size_t current_fps = static_cast<float>(frame_number) / static_cast<float>(render_duration) * 1000;

		SetWindowTextA(
			self_hWnd_,
			std::to_string(current_fps).c_str()
		);

		frame_number = 0;
		last_fps_counting = high_resolution_clock::now();

	}

	#endif //COUNT_FPS

	SetDIBitsToDevice(
			device_context_,
			0,
			0,
			params_.width_, 
			params_.height_,
			0,
			0,
			0,
			params_.height_,
			frame,
			&info,
			DIB_PAL_COLORS
	);

}
