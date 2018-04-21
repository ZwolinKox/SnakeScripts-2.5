#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <thread>
#include <queue>
#include <Windows.h>
#include <array>
#include <functional>

#define NOT_FOUND std::string::npos 
#define POS_END std::string::npos
const char EOS = NULL;


//Rzucane wyj¹tkis
class Variable_not_found { };
class Not_parsed { };
class Array_Out_Of_Range {};
class Private_Var {};

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
		ENCAPSULATION_UNKNOWN,
		PRIVATE,
		PROTECTED,
		PUBLIC
	};

	enum ECmdType
	{
		CMD_UNKNOWN,

		CMD_CREATE_LOCAL_VAR,//
		CMD_ASSIGN_VALUE,//
		CMD_WRITE,//
		CMD_DEFPROC,//
		CMD_CALLPROC,//
		CMD_YIELD, //
		CMD_RBRACKET,//
		CMD_PRINT,//
		CMD_CREATE_VAR,//
		CMD_DELETE_VAR,
		CMD_DESTRUCT, //
		CMD_DELETE_ARRAY, //
		CMD_CREATE_OBJECT, //
		CMD_CREATE_ARRAY,//
		CMD_RUN_THREAD,
		CMD_CALLMETHOD,//
		CMD_IF,//
		CMD_ELSEIF,//
		CMD_ELSE,//
		CMD_SYSTEM, //
		CMD_READ,//
		CMD_WAIT,
		CMD_RETURN,//
		CMD_FOR,//
		CMD_WHILE,//
		CMD_DOWHILE,//
		CMD_UNTIL,//
		CMD_DOUNTIL,//
		CMD_LOOP,//
		CMD_CONTINUE,//
		CMD_BREAK,//
		CMD_INSERT,//
		CMD_ERASE,//
		CMD_ADD_NEW_METHOD,//
		CMD_LAMBDA, //
		CMD_THROW,
		CMD_TESTEQ,
		CMD_ADD_STRING, 
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

	struct STRING :public Type
	{
		std::string value;
		std::string name;
		ENCAPSULATION status{ PUBLIC };
	};

	struct COMMAND_INFO
	{
		ECmdType Type;
		int nNestingLevel;
		int n1, n2, n3, n4, n5, n6, op, pow;
		string s1, s2, s3, oper;
		bool isWhileTrue{ false };
		std::vector<std::string> initValues;
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
		int localVariables{ 0 };
		var returnValue{ 0 };
	};

	struct PROC
	{
		string name;
		std::stack<VARIABLE> Arguments;
		std::vector<COMMAND_INFO> Body;
		std::vector<COMMAND_INFO> Yield;
		int localVariables{ 0 };
		var returnValue{ 0 };
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
	std::stack<VARIABLE> LocalVariables;
	std::vector<std::vector<VARIABLE>> Arrays;
	std::vector<CLASS> Classes;
	std::vector<OBJECT> Objects;
	std::vector<string> Libs;
	std::vector<PROC> Procs;
	std::vector<THREAD> Threads;
	std::vector<STRING> Strings;

	std::vector<std::function<void()>> CPP_FUNCTIONS;

	std::array<std::string, 1> STD_LIBS
	{
		"Self.sl"
	};

	bool isMethod{ false };
	int objectNumber;
	std::string scriptName;
	int lastReturn{ 0 };

	//Metody pomocnicze
	bool execute_commands(std::vector<COMMAND_INFO>& cmd_array, int start, int stop);
	bool get_token();
	bool get_int();
	bool get_param();
	void ignore_line();
	EOpType negate_operator(EOpType op);
	VARIABLE get_local_variable(const string& sName, bool bChangeValue);
	void set_local_variable(const string& sName, int value);
	bool get_operator();
	bool get_string();
	bool get_brace();
	bool get_math_operator();
	bool save(string fileName, VARIABLE &var);
	bool load(string fileName, VARIABLE &var);
	bool encrypt(const char *plikWyjsciowy, VARIABLE &var);
	bool decrypt(const char * plikXorowany, VARIABLE &var);
	bool get_word(string word);
	//bool get_color();
	int get_object_id(string name);
	int get_object_var_id(string name, string objectName);
	int get_var_id(std::string varName);
	int get_class_id(string className);
	std::vector<string> get_init();
	bool checkVarName(std::string varName);
	bool checkArrayName(std::string arrayName);
	bool get_logic_operator(int &operator_buff);
	bool is_true(int nVarValue, EOpType OpType, int nValue);
	bool loadStdLibs();

	std::string get_script_name();
	bool parse();
	bool run();
	//bool eval();

public:

	//Parser matematyczny

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

			auto isLength = name.find(".Length");
			auto itr = name.find_first_of('[');
			auto itrObj = name.find('.');
			auto findThis = name.find("this");
			auto lBracket = name.find("()");
			auto isRandom = name.find("random");

			if (isRandom != NOT_FOUND)
			{
				auto lBr = name.find_first_of("(");
				auto floor = name.find_first_of('_');

				std::string rand1;
				int rand1Val;

				std::string rand2;
				int rand2Val;

				rand1 = name.substr(lBr + 1);
				rand2 = name.substr(floor+1);

				for (auto i = rand1.length() - 1; i != 0; i--)
				{
					if (rand1[i] == '_')
					{
						rand1.pop_back();
						break;
					}
						

					rand1.pop_back();
				}

				rand2.pop_back();

				Parser firstParser{ rand1 };
				Parser secondParser{ rand2 };

				Expression* expr = firstParser.parse_Expression();

				rand1Val = expr->eval(memory);

				expr = secondParser.parse_Expression();
				
				rand2Val = expr->eval(memory);

				if (rand2Val > rand1Val)
				{
					int tmp = rand2Val;

					rand2Val = rand1Val;
					rand1Val = tmp;

					return rand() % (rand2Val - rand1Val) + rand1Val;
				}
				else if (rand2Val == rand1Val)
				{
					return rand() % rand2Val;
				}
			}

			else if (isLength != NOT_FOUND)
			{
				std::string arrayName = name.substr(0, isLength);

				for (auto i = 0; i < memory.Arrays.size(); i++)
				{
					if (memory.Arrays[i][0].name == arrayName)
						return memory.Arrays[i].size();
				}
			}

			else if (lBracket != NOT_FOUND && findThis != NOT_FOUND && itrObj != NOT_FOUND)
			{
				SnakeScript::COMMAND_INFO cmd_info;

				std::string methodName = name.substr(itrObj+1, lBracket);
				methodName.pop_back();
				methodName.pop_back();

				int methodId = -1;

				cmd_info.s2 = methodName;

				for (auto i = 0; i < memory.Objects[memory.objectNumber].Class.Methods.size(); i++)
				{
					if (methodName == memory.Objects[memory.objectNumber].Class.Methods[i].name)
					{
						methodId = i;
						break;
					}
				}

				if (methodId == -1)
					throw Variable_not_found();

				std::vector<COMMAND_INFO> cmd_array;

				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_CALLMETHOD;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);

				memory.execute_commands(cmd_array, 0, cmd_array.size() - 1);

				return memory.Objects[memory.objectNumber].Class.Methods[methodId].returnValue;
			}

			else if (lBracket != NOT_FOUND && itrObj != NOT_FOUND)
			{
				SnakeScript::COMMAND_INFO cmd_info;

				std::string methodName = name.substr(itrObj + 1, lBracket);
				std::string objectName = name.substr(0, itrObj);
				methodName.pop_back();
				methodName.pop_back();

				int methodId = -1;

				cmd_info.s1 = objectName;
				cmd_info.s2 = methodName;

				for (auto i = 0; i < memory.Objects[memory.objectNumber].Class.Methods.size(); i++)
				{
					if (methodName == memory.Objects[memory.objectNumber].Class.Methods[i].name)
					{
						methodId = i;
						break;
					}
				}

				if (methodId == -1)
					throw Variable_not_found();

				std::vector<COMMAND_INFO> cmd_array;

				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_CALLMETHOD;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);

				memory.execute_commands(cmd_array, 0, cmd_array.size() - 1);

				return memory.Objects[memory.objectNumber].Class.Methods[methodId].returnValue;
			}

			else if (lBracket != NOT_FOUND)
			{
				SnakeScript::COMMAND_INFO cmd_info;

				std::string procName = name.substr(0, lBracket);

				int procId = -1;

				cmd_info.s1 = procName;
				cmd_info.Type = CMD_CALLPROC;

				for (auto i = 0; i < memory.Procs.size(); i++)
				{
					if (procName == memory.Procs[i].name)
					{
						procId = i;
						break;
					}
				}

				if (procId == -1)
					throw Variable_not_found();

				std::vector<COMMAND_INFO> cmd_array;

				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_CALLPROC;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);

				memory.execute_commands(cmd_array, 0, cmd_array.size()-1);

				return memory.Procs[procId].returnValue;
			}

			else if (itr != NOT_FOUND)
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

				if (memory.isMethod)
				{
					for (auto &variable : memory.Objects[memory.objectNumber].Class.Variables)
					{
						if (variable.name == name)
							return variable.value;
					}
				}

				SnakeScript::VARIABLE localVar;

				localVar = memory.get_local_variable(name, false);

				if (localVar.status != ENCAPSULATION_UNKNOWN)
					return localVar.value;
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

