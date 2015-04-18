#ifndef SPT_APP_H
#define SPT_APP_H

#include <memory>
#include <windows.h>
#include "Window.h"

namespace sptracer
{

	class App
	{
	public:
		static void Init(HINSTANCE hInstance);
		static App& GetInstance();
		int Run();

	protected:
		virtual ~App();

	private:
		std::string _title =
#ifdef _WIN64
			"SPTracer (64-bit)";
#else
			"SPTracer (32-bit)";
#endif

		static HINSTANCE _hInstance;
		std::unique_ptr<Window> _window;

		App();
	};

}

#endif
