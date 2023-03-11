#include "main.h"

//https://stackoverflow.com/questions/31273715/how-to-boostany-cast-into-stdstring
bool is_char_ptr(const boost::any& operand)
{
	try {
		boost::any_cast<char*>(operand);
		return true;
	}
	catch (const boost::bad_any_cast&) {
		return false;
	}
}

std::string any2string(boost::any anything)
{
	try {
		if (anything.type() == typeid(int)) {
			return std::to_string(boost::any_cast<int>(anything));
		}
		if (anything.type() == typeid(double)) {
			return std::to_string(boost::any_cast<double>(anything));
		}
		if (anything.type() == typeid(std::byte)) {
			return std::to_string((char)boost::any_cast<std::byte>(anything));
		}
		if (anything.type() == typeid(char)) {
			char inf[2];
			inf[1] = '\0';
			inf[0] = boost::any_cast<char>(anything);
			return std::string(inf);
		}
		if (is_char_ptr(anything)) {
			return std::string(boost::any_cast<char*>(anything));
		}
		return boost::any_cast<std::string>(anything);
	}
	catch (const boost::bad_any_cast&) {
		return "null";
	}
}

void PrintTree(std::shared_ptr<allogic::Lexer::Node> q, long n)
{
	long i;
	if (q)
	{
		PrintTree(q->right, n + n);
		for (i = 0; i < n; i++)
			printf(" ");
		printf("%d\n", q->type);
		for (i = 0; i < n; i++)
			printf(" ");
		printf("%s\n", any2string(q->info).c_str());
		PrintTree(q->left, n + n);
	}
}

#ifndef DLL

int main() {
	std::cout << "Initialize..\n";
	std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"P", (double)0}, { "Q" ,(double)0 }, { "R" , (double)0 } });

	lexer->AddOperator(std::byte('0'), allogic::stOperator('=', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "=" << std::endl;
			return (double)(boost::any_cast<double>(left) == boost::any_cast<double>(right));
		}));
	lexer->AddOperator(std::byte('1'), allogic::stOperator('>', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << ">" << std::endl;
			if (boost::any_cast<double>(left) == 1 && boost::any_cast<double>(right) == 0)
				return (double)0;
			else
				return (double)1;
		}));
	lexer->AddOperator(std::byte('2'), allogic::stOperator('|', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "|" << std::endl;
			if (boost::any_cast<double>(left) == 0 && boost::any_cast<double>(right) == 0)
				return (double)0;
			else
				return (double)1;
		}));
	lexer->AddOperator(std::byte('3'), allogic::stOperator('+', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "+" << std::endl;
			if (boost::any_cast<double>(left) == boost::any_cast<double>(right))
				return (double)0;
			else
				return (double)1;
		}));
	lexer->AddOperator(std::byte('4'), allogic::stOperator('&', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "&" << std::endl;
			if ((boost::any_cast<double>(left) == 1) && (boost::any_cast<double>(right) == 1))
				return (double)1;
			else
				return (double)0;
		}));
	lexer->AddOperator(std::byte('5'), allogic::stOperator('!', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "!" << std::endl;
			if (boost::any_cast<double>(right) == 1)
				return (double)0;
			else
				return (double)1;
		}));

	std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();

	auto node = parser->MakeNode(lexer->GetOperators(), "P&Q>(Q&!P>R&Q)");
	PrintTree(node, 4);

	std::cout << boost::any_cast<double>(node->Calc(lexer->GetVariables(), lexer->GetOperators()));

	return 0;
}

#else
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpReserved )
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

int Calculate(const char* expr, const char** variables, int* variables_values, int variables_count)
{
	std::map<std::string, boost::any> vars;
	
	for (int i = 0; i < variables_count; i++) {
		vars.emplace(std::make_pair<std::string, boost::any>(variables[i], boost::any((double)variables_values[i]) ));
	}

	std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(vars);

	lexer->AddOperator(std::byte('0'), allogic::stOperator('=', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "=" << std::endl;
			return (double)(boost::any_cast<double>(left) == boost::any_cast<double>(right));
		}));
	lexer->AddOperator(std::byte('1'), allogic::stOperator('>', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << ">" << std::endl;
			if (boost::any_cast<double>(left) == 1 && boost::any_cast<double>(right) == 0)
				return (double)0;
			else
				return (double)1;
		}));
	lexer->AddOperator(std::byte('2'), allogic::stOperator('|', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "|" << std::endl;
			if ((boost::any_cast<double>(left) == 0) && (boost::any_cast<double>(right) == 0))
				return (double)0;
			else
				return (double)1;
		}));
	lexer->AddOperator(std::byte('3'), allogic::stOperator('+', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "+" << std::endl;
			if (boost::any_cast<double>(left) == boost::any_cast<double>(right))
				return (double)0;
			else
				return (double)1;
		}));
	lexer->AddOperator(std::byte('4'), allogic::stOperator('&', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "&" << std::endl;
			if ((boost::any_cast<double>(left) == 1) && (boost::any_cast<double>(right) == 1))
				return (double)1;
			else
				return (double)0;
		}));
	lexer->AddOperator(std::byte('5'), allogic::stOperator('!', [](boost::any left, boost::any right) -> boost::any
		{
			std::cout << "!" << std::endl;
			if (boost::any_cast<double>(right) == 1)
				return (double)0;
			else
				return (double)1;
		}));

	std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();
	auto node = parser->MakeNode(lexer->GetOperators(), expr);

	return (int)boost::any_cast<double>(node->Calc(lexer->GetVariables(), lexer->GetOperators()));
}

#endif