#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <thread>
#include <queue>

#define NOT_FOUND std::string::npos 
#define POS_END std::string::npos
const char EOS = NULL;


//Rzucane wyj¹tkis
class Variable_not_found { };
class Not_parsed { };

using std::string;
using std::cout;
using std::cin;
using std::endl;

class SnakeScript
{
public:


	using var = int;	//Oszczêdzi nam to w przysz³oœci problemów

	const char* WS_SET = " \t\r\n";
	const char* BR_SET = "\r\n";
	string bufor, cword, err_str;
	int pos = 0, param = 0;

	//Enumy
	enum EOpType
	{
		OP_UNKNOWN,
		OP_EQUAL,
		OP_NOT_EQUAL,
		OP_GREATER,
		OP_LESS,
		OP_GREATER_OR_EQUAL,
		OP_LESS_OR_EQUAL
	};

	enum ENCAPSULATION
	{
		PRIVATE,
		PROTECTED,
		PUBLIC
	};

	enum ECmdType
	{
		CMD_UNKNOWN,

		CMD_ASSIGN_VALUE,
		CMD_WRITE,
		CMD_DEFPROC,
		CMD_CALLPROC,
		CMD_YIELD,
		CMD_RBRACKER,
		CMD_PRINT,
		CMD_CREATE_VAR,
		CMD_DELETE_VAR,
		CMD_CREATE_OBJECT,
		CMD_CREATE_ARRAY,
		CMD_RUN_THREAD,
		CMD_CALLMETHOD
	};

	//Struktury
	class Type abstract
	{

	};

	struct VARIABLE :public Type
	{
		var value;
		std::string name;
		bool isConst{ false };
		ENCAPSULATION status{ PUBLIC };
	};

	struct COMMAND_INFO
	{
		ECmdType Type;
		int nNestingLevel;
		int n1, n2, n3, n4, n5, n6, op, pow;
		string s1, s2, s3, oper;
		bool isWhileTrue{ false };
		std::vector<int> initValues;
		std::vector<COMMAND_INFO> yield_info;
	};

	struct METHOD
	{
		ENCAPSULATION status{ PRIVATE };
		bool isVirtual{ false };
		bool isOverride{ false };
		string className;
		string name;
		std::stack<VARIABLE> Arguments;
		std::vector<COMMAND_INFO> Body;
		std::vector<COMMAND_INFO> Yield;
	};

	struct PROC
	{
		string name;
		std::stack<VARIABLE> Arguments;
		std::vector<COMMAND_INFO> Body;
		std::vector<COMMAND_INFO> Yield;
	};

	struct THREAD
	{
		std::thread *thread;
		std::string name;
		std::vector<COMMAND_INFO> Body;
	};

	struct DEFAULT_CONSTRUCTOR
	{
		std::vector<int> DefaultInt;
		std::vector<string> DefaultVar;
	};

	struct CLASS :public Type
	{
		bool isFinal{ false };
		bool isAbstract{ false };
		bool isInterface{ false };
		bool isStruct{ false };
		string name;
		std::vector<METHOD> Methods;
		std::vector<VARIABLE> Variables;
		std::vector<string> Init;
		DEFAULT_CONSTRUCTOR Default;
	};

	struct OBJECT :public Type
	{
		string objectName;
		CLASS Class;
	};

	std::vector<COMMAND_INFO> Commands;
	std::vector<VARIABLE> Variables;
	std::vector<std::vector<VARIABLE>> Arrays;
	std::vector<CLASS> Classes;
	std::vector<OBJECT> Objects;
	std::vector<string> Libs;
	std::vector<PROC> Procs;
	std::vector<THREAD> Threads;

	bool isMethod{ false };
	int objectNumber;
	std::string scriptName;

	//Metody pomocnicze
	bool execute_commands(std::vector<COMMAND_INFO>& cmd_array, int start, int stop);
	bool get_token();
	bool get_int();
	bool get_param();
	void ignore_line();
	EOpType negate_operator(EOpType op);
	VARIABLE get_local_variable(const string& sName, bool bChangeValue);
	bool get_operator();
	bool get_string();
	bool get_brace();
	bool get_math_operator();
	bool save(string fileName, VARIABLE &var);
	bool load(string fileName, VARIABLE &var);
	bool encrypt(const char *plikWyjsciowy, VARIABLE &var);
	bool decrypt(const char * plikXorowany, VARIABLE &var);
	bool get_word(string word);
	bool get_color();
	int get_object_id(string name);
	int get_object_var_id(string name, string objectName);
	int get_var_id(std::string varName);
	int get_class_id(string className);
	std::vector<string> get_init();
	bool checkVarName(std::string varName);
	bool checkArrayName(std::string arrayName);

	
	std::string get_script_name();
	bool parse();
	bool run();
	//bool eval();

public:

	//Parser matematyczny
	class Array_Out_Of_Range {};
	class Private_Var {};

	class Expression
	{
	public:

		virtual int eval(SnakeScript &memory) = 0;

		virtual ~Expression() {}
	};

	class Constant :public Expression
	{
		int value;
	public:

		Constant(int v) : value(v) {};

		int eval(SnakeScript &memory) override
		{
			return value;
		}
	};

	class Binary_operator :public Expression
	{
		char symbol;
		Expression *left, *right;

	public:

		Binary_operator(char s, Expression* l, Expression* r)
			:symbol(s), left(l), right(r) {};

		int eval(SnakeScript &memory) override
		{
			switch (symbol)
			{
			case '*': return left->eval(memory) * right->eval(memory);	break;
			case '+': return left->eval(memory) + right->eval(memory);	break;
			case '-': return left->eval(memory) - right->eval(memory);	break;
			case '/': return left->eval(memory) / right->eval(memory);	break;
			case '%': return left->eval(memory) % right->eval(memory);	break;
			}
		}

		virtual ~Binary_operator()
		{
			delete left;
			delete right;
		}
	};

	class Variable :public Expression 
	{
		std::string name;

	public:
		Variable(std::string n) : name(n) {};

		int eval(SnakeScript &memory) override
		{
			using namespace std;

			auto itr = name.find_first_of('[');
			auto itrObj = name.find('.');
			auto findThis = name.find("this");

			if (itr != NOT_FOUND)
			{
				std::string cword;
				std::string number;

				
				auto rbracket = name.find_last_of(']');

				auto lbracket = itr;

				for (auto i = lbracket+1; i != rbracket; i++)
				{
					number.push_back(name[i]);
				}

				Parser parser{ number };
				Expression* expr = parser.parse_Expression();

				cword = name.substr(0, lbracket);


				int index = expr->eval(memory);

				for (auto i = 0; i < memory.Arrays.size(); i++)
				{
					if (memory.Arrays[i][0].name == cword)
					{
						if (index >= memory.Arrays[i].size())
							throw Array_Out_Of_Range();
						return memory.Arrays[i][index].value;
					}
						
				}
			}

			else if (itrObj != NOT_FOUND && findThis != NOT_FOUND)
			{
				std::string varName = name.substr(itrObj+1);

				for (auto &objVar : memory.Objects[memory.objectNumber].Class.Variables)
				{
					if (objVar.name == varName)
						return objVar.value;
				}
			}

			else if (itrObj != NOT_FOUND)
			{
				int pos = name.find_last_of('.');

				std::string varName;
				std::string objectName;

				for (int i = 0; i < pos; i++)
				{
					objectName.push_back(name[i]);
				}

				for (int i = pos + 1; i < name.length(); i++)
				{
					varName.push_back(name[i]);
				}

				for (auto &itr : memory.Objects)
				{
					if (itr.objectName == objectName)
					{
						for (auto &itr2 : itr.Class.Variables)
						{
							if (itr2.name == varName)
							{
								if (itr2.status != PUBLIC)
									throw Private_Var();
								return itr2.value;
							}
						}
					}
				}
			}

			else
			{
				for (auto &variable : memory.Variables)
				{
					if (variable.name == name)
						return variable.value;
				}
			}

			throw Variable_not_found();

		}
	};

	class Parser
	{
		std::string input;
		size_t position;

	public:
		Parser(std::string input);

		void skip_whitespace();

		char look_ahead();

		bool whileParseVariable(char c);
		Expression* parse_Expression(); //Parsuje wyra¿enie
		Expression* parse_sum(); //parsuje sume
		Expression* parse_mult(); //Parsuje sk³adnik
		Expression* parse_term(); //Parsuje czynnik
		Expression* parse_Constant(); //Parsuje liczbe
		Expression* parse_Variable(); //Parsuje zmienn¹
		Expression* parse_paren(); //parsuje "sume" w nawiasie

	};
	//Reszta
public:

	SnakeScript(std::string fileName);
	~SnakeScript();
};

