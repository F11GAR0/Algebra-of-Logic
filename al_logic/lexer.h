#pragma once
#include "main.h"
#include <string>
#include <map>
#include <memory>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>


namespace allogic {
	enum eLexemType {
		L_CONST = 1,
		L_OPERATION = 2,
		L_VAR = 3,
	};
	struct stOperator {
		char cOperatorChar;
		boost::function<boost::any(boost::any, boost::any)> pCallback;
		stOperator(char c, boost::function<boost::any(boost::any, boost::any)> cb) {
			cOperatorChar = c;
			pCallback = cb;
		}
		stOperator(const stOperator& copy) {
			this->cOperatorChar = copy.cOperatorChar;
			this->pCallback = copy.pCallback;
		}
		stOperator(stOperator& copy) {
			this->cOperatorChar = copy.cOperatorChar;
			this->pCallback = copy.pCallback;
		}
	};

	class Lexer {
	private:
		//m_Operators[priority]
		std::map<std::byte, std::shared_ptr<stOperator>> m_Operators;
		std::map<std::string, boost::any> m_Variables;
	public:
		Lexer(std::map<std::string, boost::any> variables);
		void AddOperator(std::byte priority, stOperator operator_info);
		std::map<std::byte, std::shared_ptr<stOperator>> GetOperators();
		std::map<std::string, boost::any> GetVariables();
		void SetVariableValue(std::string var, boost::any value);
		class Node {
		protected:
			
		public:			
			std::shared_ptr<Node> parent;
			std::shared_ptr<Node> left;
			std::shared_ptr<Node> right;
			std::string lex;
			eLexemType type;
			boost::any info;
			boost::any Calc(std::map<std::string, boost::any> variables, std::map<std::byte, std::shared_ptr<stOperator>> operators);
		};

		boost::any Calc(std::shared_ptr<Node> root);
	};

	class Parser {
	public:
		std::shared_ptr<Lexer::Node> MakeNode(std::map<std::byte, std::shared_ptr<stOperator>> operators, std::string str, std::list<std::shared_ptr<Lexer::Node>> brackets_stack = std::list<std::shared_ptr<Lexer::Node>>());
	};
	//PRIORITY = { '+' : 3, '-' : 2, '*' : 1, '/' : 0 }
};

