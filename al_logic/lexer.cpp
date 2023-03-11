#include "main.h"
#include "lexer.h"


boost::any allogic::Lexer::Node::Calc(std::map<std::string, boost::any> variables, std::map<std::byte, std::shared_ptr<stOperator>> operators)
{
	auto get_val = [&variables](std::string name)-> boost::any {
		if (variables.contains(name))
			return variables[name];
		else
			return 0;
	};

	boost::any left_val;
	boost::any right_val;

	if (this->left != 0) {
		if (this->left->type == eLexemType::L_VAR)
			left_val = get_val(boost::any_cast<std::string>(this->left->info));
		else if (this->left->type == eLexemType::L_CONST)
			left_val = this->left->info;
		else if (this->left->type == eLexemType::L_OPERATION)
			left_val = this->left->Calc(variables, operators);
	}
	else
		left_val = 0;
	if (this->right != 0) {
		if (this->right->type == eLexemType::L_VAR)
			right_val = get_val(boost::any_cast<std::string>(this->right->info));
		else if (this->right->type == eLexemType::L_CONST)
			right_val = this->right->info;
		else if (this->right->type == eLexemType::L_OPERATION)
			right_val = this->right->Calc(variables, operators);
	}
	else
		right_val = 0;

	if (this->type == L_VAR)
		return get_val(boost::any_cast<std::string>(this->info));
		 
	if (this->type == L_CONST)
		return this->info;
	
	for (const auto& [priority, callback_info] : operators) {
		if (boost::any_cast<char>(this->info) == callback_info->cOperatorChar) {
			return callback_info->pCallback(left_val, right_val);
		}
	}
	
	return 0;
}

std::shared_ptr<allogic::Lexer::Node> allogic::Parser::MakeNode(std::map<std::byte, std::shared_ptr<stOperator>> operators, std::string str, std::list<std::shared_ptr<Lexer::Node>> brackets_stack)
{
	//TODO: make NOT operator !
	std::cout << str << " |MAKENODE\n";
	std::shared_ptr<Lexer::Node> complete_node = std::make_shared<Lexer::Node>();
	int open_bracket_indx = -1;
	int open_brackets_count = 0;
	int close_bracket_indx = -1;
	int close_brackets_count = 0;
	for (int i = 0, len = str.length(); i < len; i++) {
		if (str[i] == '(') { //finding first
			open_brackets_count++;
			if(open_bracket_indx == -1)
				open_bracket_indx = i;
		}
		if (str[i] == ')') { //finding last
			close_brackets_count++;
			if(open_brackets_count == close_brackets_count && close_bracket_indx == -1)
				close_bracket_indx = i;
		}
	}
	std::cout << "obi: " << open_bracket_indx << " cbi: " << close_bracket_indx << std::endl;
	
	if (open_bracket_indx != -1 && close_bracket_indx != -1) {
		std::cout << str << " |(MAKENODE)\n";
		// ( 2 + 2 )
		// 0 1 2 3 4
		// 1:4
		std::shared_ptr<Lexer::Node> in_brackets_node = MakeNode(operators, str.substr(open_bracket_indx + 1, close_bracket_indx - open_bracket_indx - 1), brackets_stack);
		brackets_stack.push_back(in_brackets_node);
		str.erase(open_bracket_indx, close_bracket_indx - open_bracket_indx + 1);
		str.insert(open_bracket_indx, "__brackets");
	}
	
	for (const auto& kv : operators) {
		size_t op_ind = str.find((char)kv.second->cOperatorChar);
		if (op_ind != -1) {
			std::shared_ptr<Lexer::Node> node = std::make_shared<Lexer::Node>();
			node->info = (char)kv.second->cOperatorChar;
			node->type = L_OPERATION;
			std::string left_operand = str.substr(0, op_ind);
			std::string right_operand = str.substr(op_ind + 1);

			if (left_operand.length() > 0) {
				node->left = MakeNode(operators, str.substr(0, op_ind), brackets_stack);
			}
			else {
				node->left = std::make_shared<Lexer::Node>();
				node->left->type = L_CONST;
				node->left->info = (double)1.0f;
			}
			if (right_operand.length() > 0) {
				node->right = MakeNode(operators, str.substr(op_ind + 1), brackets_stack);
			}
			else {
				node->right = std::make_shared<Lexer::Node>();
				node->right->type = L_CONST;
				node->right->info = (double)1.0f;
			}
			//std::cout << "Created node: " << node->type << " " << boost::any_cast<std::string>(node->left->info) << " " << node->right->info;
			return node;
		}
	}
	//2 + 2 - 4
	//0 1 2 3 4
	//len = 5
	//op_ind = 1
	//2 : 2 

	if (std::all_of(str.begin(), str.end(), ::isdigit)) {
		std::cout << "Finded const: " << str << std::endl;
		std::shared_ptr<Lexer::Node> node = std::make_shared<Lexer::Node>();
		node->type = L_CONST;
		node->info = std::stod(str);
		return node;
	}
	else {
		if (str.find("__brackets") != -1) {
			auto bracket_node = brackets_stack.back();
			brackets_stack.pop_back();
			return bracket_node;
		}
		else {
			std::shared_ptr<Lexer::Node> node = std::make_shared<Lexer::Node>();
			node->type = L_VAR;
			node->info = str;
			return node;
		}
	}

	//9*(7-4*(6+3))+5
	//in_brackets_node -> 7 - 4 * (6 + 3)
	//in_brackets_node -> in_brackets_node -> 6 + 3

}

allogic::Lexer::Lexer(std::map<std::string, boost::any> variables)
{
	m_Variables = variables;
}

void allogic::Lexer::AddOperator(std::byte priority, stOperator operator_info)
{
	m_Operators.emplace(priority, std::make_shared<stOperator>(operator_info));
}

std::map<std::byte, std::shared_ptr<allogic::stOperator>> allogic::Lexer::GetOperators()
{
	return m_Operators;
}

std::map<std::string, boost::any> allogic::Lexer::GetVariables()
{
	return m_Variables;
}

void allogic::Lexer::SetVariableValue(std::string var, boost::any value)
{
	if (m_Variables.count(var) > 0) {
		m_Variables[var] = value;
	}
	else {
		throw std::exception("There is no such variable.");
	}
}

boost::any allogic::Lexer::Calc(std::shared_ptr<Node> root)
{
	return root->Calc(m_Variables, m_Operators);
}
