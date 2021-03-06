#pragma once

#include "../Utility//NonMovable.h"
#include "../Common.h"
#include "GraphicsClass.h"
#include "InputClass.h"
#include <tuple>

class SystemClass : public NonMovable {
public:
	static SystemClass& GetInstance(); 

private:
	SystemClass() = default;
	~SystemClass() = default;

public:
	int Run();

private:
	bool Frame();
	
	bool Init();
	void Release();
	
	std::tuple<int, int> InitWindows();
	void ReleaseWindows();

private:
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	LPCTSTR m_appName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	std::unique_ptr<InputClass> m_input;
	std::unique_ptr<GraphicsClass> m_graphics;
};