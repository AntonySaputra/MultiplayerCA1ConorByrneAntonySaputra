#include <stdexcept>
#include <iostream>
#include "Application.hpp"

int main()
{
	try 
	{
		Application AntonyAndConor;
		AntonyAndConor.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\n EXCEPTION" << e.what() << std::endl;
		int x = 10;
	}
}