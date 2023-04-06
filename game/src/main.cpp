#include <Arithmetic.h>
#include <iostream>
#include <Debug/Log.h>

int main()
{
	LibGL::Debug::Log::openFile("Debug.log");
	DEBUG_LOG("LibGL was successfully linked !");
	return LibMath::clamp(-1, 0, 1);
}