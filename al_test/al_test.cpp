#include "main.h"
#include "al_test.h"
#include "CppUnitTest.h"

#include "../al_logic/lexer.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace al_test
{
	inline void GetArithmeticOperations(std::shared_ptr<allogic::Lexer> lexer) {
		lexer->AddOperator(std::byte('0'), allogic::stOperator('+', [](boost::any left, boost::any right) -> boost::any { return boost::any_cast<double>(left) + boost::any_cast<double>(right); }));
		lexer->AddOperator(std::byte('1'), allogic::stOperator('-', [](boost::any left, boost::any right) -> boost::any { return boost::any_cast<double>(left) - boost::any_cast<double>(right); }));
		lexer->AddOperator(std::byte('2'), allogic::stOperator('*', [](boost::any left, boost::any right) -> boost::any { return boost::any_cast<double>(left) * boost::any_cast<double>(right); }));
		lexer->AddOperator(std::byte('3'), allogic::stOperator('/', [](boost::any left, boost::any right) -> boost::any { return boost::any_cast<double>(left) / boost::any_cast<double>(right); }));
	}
	inline void GetAlgebraOfLogicOperations(std::shared_ptr<allogic::Lexer> lexer) {
		lexer->AddOperator(std::byte('0'), allogic::stOperator('=', [](boost::any left, boost::any right) -> boost::any
			{ return (double)(boost::any_cast<double>(left) == boost::any_cast<double>(right)); }));
		lexer->AddOperator(std::byte('1'), allogic::stOperator('>', [](boost::any left, boost::any right) -> boost::any
			{
				if (boost::any_cast<double>(left) == 1 && boost::any_cast<double>(right) == 0)
					return (double)0;
				else
					return (double)1;
			}));
		lexer->AddOperator(std::byte('2'), allogic::stOperator('|', [](boost::any left, boost::any right) -> boost::any
			{
				if (boost::any_cast<double>(left) == 0 && boost::any_cast<double>(right) == 0)
					return (double)0;
				else
					return (double)1;
			}));
		lexer->AddOperator(std::byte('3'), allogic::stOperator('+', [](boost::any left, boost::any right) -> boost::any
			{
				if (boost::any_cast<double>(left) == boost::any_cast<double>(right))
					return (double)0;
				else
					return (double)1;
			}));
		lexer->AddOperator(std::byte('4'), allogic::stOperator('&', [](boost::any left, boost::any right) -> boost::any
			{
				if (boost::any_cast<double>(left) == 1 && boost::any_cast<double>(right) == 1)
					return (double)1;
				else
					return (double)0;
			}));
		lexer->AddOperator(std::byte('5'), allogic::stOperator('!', [](boost::any left, boost::any right) -> boost::any
			{
				if (boost::any_cast<double>(right) == 1)
					return (double)0;
				else
					return (double)1;
			}));
	}
	TEST_CLASS(al_calculations)
	{
	public:
		
		TEST_METHOD(testSimpleCalculationsNoVariables)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"x", 1}, { "y" ,-8 } });
			
			GetArithmeticOperations(lexer);
			
			std::shared_ptr<allogic::Lexer::Node> root = std::make_shared<allogic::Lexer::Node>();
			root->type = allogic::eLexemType::L_OPERATION;
			root->info = '+';

			root->left = std::make_shared<allogic::Lexer::Node>();
			root->left->type = allogic::eLexemType::L_CONST;
			root->left->info = (double)2;
			root->right = std::make_shared<allogic::Lexer::Node>();
			root->right->type = allogic::eLexemType::L_CONST;
			root->right->info = (double)2;

			Assert::AreEqual(boost::any_cast<double>(lexer->Calc(root)), (double)4.0f);
		}
		TEST_METHOD(testSimpleCalculationsWithVariables)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"x", (double)2}, { "y" , (double)2 } });

			GetArithmeticOperations(lexer);

			std::shared_ptr<allogic::Lexer::Node> root = std::make_shared<allogic::Lexer::Node>();
			root->type = allogic::eLexemType::L_OPERATION;
			root->info = '+';

			root->left = std::make_shared<allogic::Lexer::Node>();
			root->left->type = allogic::eLexemType::L_VAR;
			root->left->info = std::string("x");
			root->right = std::make_shared<allogic::Lexer::Node>();
			root->right->type = allogic::eLexemType::L_VAR;
			root->right->info = std::string("y");

			Assert::AreEqual(boost::any_cast<double>(lexer->Calc(root)), (double)4.0f);
		}
	};
	TEST_CLASS(al_parsing)
	{
	public:
		//There is no such variable.
		TEST_METHOD(testLexerVariables)
		{
			auto test_var = [] { 
				std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"x", (double)1}, { "y" ,(double)-8 } });
				lexer->SetVariableValue("j", 8); 
			};

			Assert::ExpectException<std::exception>(test_var, L"There is no such variable.");
		}
		TEST_METHOD(testSimpleParsing)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"x", (double)1}, { "y" ,(double)-8 } });

			GetArithmeticOperations(lexer);

			std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();

			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "2+2*2")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)6.0f);
		}
		TEST_METHOD(testHardParsing)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"x", (double)1}, { "y" ,(double)-8 } });

			GetArithmeticOperations(lexer);

			std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();

			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "(2+2)*4+7*(1+3)")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)44.0f);
		}
		TEST_METHOD(testParsingWithVariables)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"x", (double)7}, { "y" ,(double)4 } });

			GetArithmeticOperations(lexer);

			std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();

			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "y*(2+2)+x*(1+3)")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)44.0f);
		}
		TEST_METHOD(testSimpleAlgebraOfLogic)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"A", (double)0}, { "B" ,(double)0 } });

			GetAlgebraOfLogicOperations(lexer);

			std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();

			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "A|B")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)0.0f);
			lexer->SetVariableValue("A", (double)1.0f);
			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "A|B")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)1.0f);
			lexer->SetVariableValue("B", (double)1.0f);
			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "A|B")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)1.0f);
		}
		TEST_METHOD(testAlgebraOfLogic)
		{
			std::shared_ptr<allogic::Lexer> lexer = std::make_unique<allogic::Lexer>(std::map<std::string, boost::any> { {"P", (double)0}, { "Q" ,(double)0 }, { "R", (double)0 } });

			GetAlgebraOfLogicOperations(lexer);

			std::unique_ptr<allogic::Parser> parser = std::make_unique<allogic::Parser>();

			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "P&Q>(Q&!P>R&Q)")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)1.0f);
			lexer->SetVariableValue("R", (double)1.0f);
			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "P&Q>(Q&!P>R&Q)")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)1.0f);
			lexer->SetVariableValue("Q", (double)1.0f);
			lexer->SetVariableValue("R", (double)0.0f);
			Assert::AreEqual(boost::any_cast<double>(parser->MakeNode(lexer->GetOperators(), "P&Q>(Q&!P>R&Q)")->Calc(lexer->GetVariables(), lexer->GetOperators())), (double)1.0f);
		}
	};

}
