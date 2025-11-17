#include <lib/repl/src/repl.hpp>

#include <iostream>

int main()
{
	auto repl = om636::repl::make_repl(std::cin, std::cout);

	repl->eval_loop([](auto message, auto & out, auto exit){
		out << message << std::endl;
		if (message == "exit") 
		{
			exit();
		}
	}).join();

	return 0; 
}