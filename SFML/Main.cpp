#include"Application.h"
int main()
{
	try
	{
		Application App;
		App.run();
	
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXPRESSION: " << e.what() << std::endl;
	}
	//system("pause");
}