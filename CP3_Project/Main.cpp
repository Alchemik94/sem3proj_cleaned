#include "Application.h"

using namespace Application;

int main()
{
	MainApplication application = MainApplication();
	#ifdef _DEBUG
	application.Test();
	#endif
	return application.Run();
}