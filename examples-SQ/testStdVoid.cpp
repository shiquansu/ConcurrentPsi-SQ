#include <iostream>
#include <algorithm>

int main()
{
	std::function<void()> f;
	f();
}


std::function<void()> f()
{
	std::cout<<"I am in void f() \n";
	return 0;
}
