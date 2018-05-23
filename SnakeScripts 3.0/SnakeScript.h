#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <thread>
#include <queue>
#include <Windows.h>
#include <map>
#include <array>
#include <functional>
#include <fstream>

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

	enum SCRIPT_TYPE
	{
		EVAL_SCRIPT
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
		CMD_CREATE_BLOCK,
		CMD_RUN_THREAD,
		CMD_CALLMETHOD,//
		CMD_IF,//
		CMD_ELSEIF,//
		CMD_ELSE,//
		CMD_SYSTEM, //
		CMD_READ,//
		CMD_READS,//
		CMD_EXIT,
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
		CMD_EVAL,
		CMD_THROW,
		CMD_TESTEQ,
		CMD_ADD_STRING, 
		CMD_FILE_SAVE,
		CMD_FILE_LOAD,
		CMD_FILE_TRUNCATE,
		CMD_FILE_LOAD_ALL
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
		std::vector<std::string> procArguments;
	};

	struct CPP_FUNC;

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
		std::vector<CPP_FUNC> CPP_BODY;
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

	struct CPP_FUNC
	{
		int howMuchArgc{ 0 };
		std::string name;
		std::function<int(SnakeScript* script)> body;
		int returnValue;
	};

	struct PREPROC_DEF
	{
		std::string orginal;
		std::string definition;
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
	std::vector<PREPROC_DEF> PreprocDefinitions;



	std::vector<std::function<void()>> CPP_FUNCTIONS;

	std::array<std::string, 1> STD_LIBS
	{
		"Self.sl"
	};

	bool isMethod{ false };
	int objectNumber;
	std::string scriptName;
	int lastReturn{ 0 };

	SnakeScript(SCRIPT_TYPE);
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
	std::vector<std::string> get_proc_arguments();
	bool eval(std::string);
	void preproc();

	std::string get_script_name();
	bool parse();

	int blockVariablesN{ 0 };

	int allBlockCounter{ 0 };

	//Old block
	/*
	class Block
	{
		SnakeScript *skrypt = nullptr;
		int block_number{ 0 };
		std::vector<std::string> blockVariablesName;
		std::stack<VARIABLE> stack;

	public:

		Block(SnakeScript *_skrypt)
		{
			skrypt = _skrypt;
		}

		void addBlockVariable(std::string name, int value)
		{
			VARIABLE var;
			var.name = name;
			var.value = value;
			skrypt->LocalVariables.push(var);
			block_number++; 
			blockVariablesName.push_back(name);
		}

		void stackBlockVariable()
		{
			std::stack<VARIABLE> tmp;
			while (block_number > 0)
			{
				bool flag{ false };

				for (auto i = 0; i < blockVariablesName.size(); i++)
				{
					if (blockVariablesName[i] == skrypt->LocalVariables.top().name)
					{
						flag = true;
						break;
					}
				}

				if (!flag)
				{
					tmp.push(skrypt->LocalVariables.top());
				}
				else
				{
					stack.push(skrypt->LocalVariables.top());
					block_number--;
				}

				skrypt->LocalVariables.pop();
			}

			while (!tmp.empty())
			{
				skrypt->LocalVariables.push(tmp.top());
				tmp.pop();
			}
		}

		void unStackBlockVariable()
		{
			while (!stack.empty())
			{
				skrypt->LocalVariables.push(stack.top());
				stack.pop();
				block_number++;
			}
		}

		~Block()
		{
			std::stack<VARIABLE> tmp;
			while (block_number > 0)
			{
				bool flag{ false };

				for (auto i = 0; i < blockVariablesName.size(); i++)
				{
					if (blockVariablesName[i] == skrypt->LocalVariables.top().name)
					{
						flag = true;
						break;
					}
				}

				if (!flag)
				{
					tmp.push(skrypt->LocalVariables.top());
				}

				skrypt->LocalVariables.pop();
				block_number--;
			}

			while (!tmp.empty())
			{
				skrypt->LocalVariables.push(tmp.top());
				tmp.pop();
			}
		}
	};
	*/

class Block
{
	SnakeScript *skrypt = nullptr;
	int block_number{ 0 };
	std::vector<std::string> blockVariablesName;
	std::stack<VARIABLE> stack;

public:

	Block(SnakeScript *_skrypt)
	{
		skrypt = _skrypt;
	}

	void addBlockVariable(std::string name, int value)
	{
		VARIABLE var;
		var.name = name;
		var.value = value;
		skrypt->LocalVariables.push(var);
		block_number++;
		blockVariablesName.push_back(name);
	}

	void stackBlockVariable()
	{	
		while (!skrypt->LocalVariables.empty())
		{
			stack.push(skrypt->LocalVariables.top());
			skrypt->LocalVariables.pop();
		}
	}

	void unStackBlockVariable()
	{
		while (!stack.empty())
		{
			skrypt->LocalVariables.push(stack.top());
			stack.pop();
		}
	}

	~Block()
	{
		std::stack<VARIABLE> tmp;
		while (block_number > 0)
		{
			if (skrypt->LocalVariables.empty())
				break;

			bool flag{ false };


			for (auto i = 0; i < blockVariablesName.size(); i++)
			{
				if (blockVariablesName[i] == skrypt->LocalVariables.top().name)
				{
					flag = true;
					break;
				}
			}


			if (!flag)
			{
				tmp.push(skrypt->LocalVariables.top());
			}
			
			block_number--;

			skrypt->LocalVariables.pop();

		}

		while (!tmp.empty())
		{
			skrypt->LocalVariables.push(tmp.top());
			tmp.pop();
		}
	}
};


public:

	void registerFunc(std::function<int(SnakeScript*)> func, std::string name, int argcNumber)
	{
		int classID = get_class_id("cpp");

		if (classID < 0)
		{
			std::cout << "Error! Skrypt nie posiada pliku Self.sn lub klasy cpp!" << std::endl;
			return;
		}

		CPP_FUNC cpp_func;

		cpp_func.name = name;
		cpp_func.howMuchArgc = argcNumber;
		cpp_func.body = func;

		METHOD thisMethod;
		thisMethod.CPP_BODY.push_back(cpp_func);
		thisMethod.className = "cpp";
		thisMethod.name = name;
		thisMethod.status = PUBLIC;

		Classes[classID].Methods.push_back(thisMethod);
	}

	VARIABLE getVariable(std::string name)
	{
		for (auto i = 0; i < Variables.size(); i++)
		{
			if (Variables[i].name == name)
				return Variables[i];
		}

		VARIABLE var = get_local_variable(name, false);

		return var;
	}

	var getVariableValue(std::string name)
	{
		for (auto i = 0; i < Variables.size(); i++)
		{
			if (Variables[i].name == name)
				return Variables[i].value;
		}

		VARIABLE var = get_local_variable(name, false);

		return var.value;
	}

	var getArrayValue(std::string name, size_t index)
	{
		for (auto i = 0; i < Arrays.size(); i++)
		{
			if (Arrays[i][0].name == name)
				return Arrays[i][index].value;
		}

		return -1;
	}

	VARIABLE getArray(std::string name, size_t index)
	{
		for (auto i = 0; i < Arrays.size(); i++)
		{
			if (Arrays[i][0].name == name)
				return Arrays[i][index];
		}

		VARIABLE var;
		var.name = "UNKNOWN";
		var.value = -1;
		var.status = ENCAPSULATION_UNKNOWN;

		return var;
	}

	bool runProc(std::string name)
	{
		for (auto i = 0; i < Procs.size(); i++)
		{
			if (Procs[i].name == name)
			{
				if (!execute_commands(Procs[i].Body, 0, Procs[i].Body.size() - 1))
				{
					std::cout << err_str << std::endl;
					return false;
				}

				return true;
			}
		}

		return false;
	}

	bool runMethod(std::string methodName, std::string objectName)
	{
		int objectID = -1;
		int methodID = -1;

		for (auto i = 0; i < Objects.size(); i++)
		{
			if (Objects[i].objectName == objectName)
			{
				objectID = i;
				break;
			}
		}

		if (objectID < 0)
			return false;

		for (auto i = 0; i < Objects[objectID].Class.Methods.size(); i++)
		{

			if (Objects[objectID].Class.Methods[i].name == methodName)
			{
				methodID = i;
				break;
			}
			else if (Objects[objectID].Class.Methods[i].name == "missing")
				methodID = i;
		}

		if (methodID < 0)
		{
			std::cout << "Error! Obiekt " + Objects[objectID].objectName + " nie ma metody " + methodName + ", ani metody domyslnej!" << std::endl;
			return false;
		}

		if (Objects[objectID].Class.Methods[methodID].status != PUBLIC && Objects[objectID].Class.Methods[methodID].name != "missing")
		{
			std::cout << "Error! Metoda " << Objects[objectID].Class.Methods[methodID].name << " nie jest publiczna!" << std::endl;
			return false;
		}

		if (!execute_commands(Objects[objectID].Class.Methods[methodID].Body, 0, Objects[objectID].Class.Methods[methodID].Body.size() - 1))
		{
			std::cout << err_str << std::endl;
			return false;
		}

		return true;
	}

	bool runProc(std::string name, std::vector<std::string> stack)
	{
		COMMAND_INFO cmd_info;
		std::vector<COMMAND_INFO> cmd_array;
		std::stack<VARIABLE> tmp;

		cmd_info.procArguments = stack;
		cmd_info.Type = CMD_CALLPROC;
		cmd_info.s1 = name;

		cmd_array.push_back(cmd_info);
		
		for (auto i = 0; i < Procs.size(); i++)
		{
			if (Procs[i].name == name)
			{
				if (!execute_commands(cmd_array, 0, cmd_array.size()-1))
				{
					std::cout << err_str << std::endl;
					return false;
				}

				return true;
			}
		}

		return false;
	}

	bool runMethod(std::string methodName, std::string objectName, std::vector<std::string> stack)
	{
		int objectID = -1;
		int methodID = -1;

		COMMAND_INFO cmd_info;
		std::vector<COMMAND_INFO> cmd_array;
		std::stack<VARIABLE> tmp;

		cmd_info.procArguments = stack;
		cmd_info.Type = CMD_CALLMETHOD;
		cmd_info.s1 = objectName;
		cmd_info.s2 = methodName;

		cmd_array.push_back(cmd_info);

		for (auto i = 0; i < Objects.size(); i++)
		{
			if (Objects[i].objectName == objectName)
			{
				objectID = i;
				break;
			}
		}

		if (objectID < 0)
			return false;

		for (auto i = 0; i < Objects[objectID].Class.Methods.size(); i++)
		{

			if (Objects[objectID].Class.Methods[i].name == methodName)
			{
				methodID = i;
				break;
			}
			else if (Objects[objectID].Class.Methods[i].name == "missing")
				methodID = i;
		}

		if (methodID < 0)
		{
			std::cout << "Error! Obiekt " + Objects[objectID].objectName + " nie ma metody " + methodName + ", ani metody domyslnej!" << std::endl;
			return false;
		}

		if (Objects[objectID].Class.Methods[methodID].status != PUBLIC && Objects[objectID].Class.Methods[methodID].name != "missing")
		{
			std::cout << "Error! Metoda " << Objects[objectID].Class.Methods[methodID].name << " nie jest publiczna!" << std::endl;
			return false;
		}

		if (!execute_commands(cmd_array, 0, cmd_array.size() - 1))
		{
			std::cout << err_str << std::endl;
			return false;
		}

		return true;
	}

	bool run();
	void runThread();

	//Argumenty dla funkcji z C++
	std::vector<VARIABLE> CPP_ARGC;

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

			throw Not_parsed();
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
			auto lBracket = name.find_first_of("(");
			auto toString = name.find(".toInt");


			if (isLength != NOT_FOUND)
			{
				std::string arrayName = name.substr(0, isLength);

				for (auto i = 0; i < memory.Arrays.size(); i++)
				{
					if (memory.Arrays[i][0].name == arrayName)
						return memory.Arrays[i].size();
				}
			}

			else if (toString != NOT_FOUND)
			{
				std::string stringName = name.substr(0, toString);

				for (auto i = 0; i < memory.Strings.size(); i++)
				{
					if (memory.Strings[i].name == stringName)
							return std::stoi(memory.Strings[i].value);
				}
			}

			else if (lBracket != NOT_FOUND && findThis != NOT_FOUND && itrObj != NOT_FOUND)
			{
				SnakeScript::COMMAND_INFO cmd_info;

				std::string methodName = name.substr(itrObj + 1, lBracket - 3);
				methodName.pop_back();
				methodName.pop_back();

				int methodId = -1;

				int start = lBracket + 1;
				std::string toPush;

				for (auto i = start; i < name.length(); i++)
				{
					toPush.push_back(name[i]);

					if (name[i] == ',')
					{
						toPush.pop_back();
						cmd_info.procArguments.push_back(toPush);
						toPush.clear();
					}
					else if (name[i] == ')')
					{
						toPush.pop_back();
						cmd_info.procArguments.push_back(toPush);
						toPush.clear();
						break;
					}
				}


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

				cmd_info.s2 = methodName;

				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_CALLMETHOD;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);


				if (!memory.execute_commands(cmd_array, 0, cmd_array.size() - 1))
					std::cout << memory.err_str << std::endl;
				else
					return memory.Objects[memory.objectNumber].Class.Methods[methodId].returnValue;
			}

			else if (lBracket != NOT_FOUND && itrObj != NOT_FOUND)
			{
				SnakeScript::COMMAND_INFO cmd_info;

				std::string methodName = name.substr(itrObj + 1, lBracket-2);
				std::string objectName = name.substr(0, itrObj);
				methodName.pop_back();
				methodName.pop_back();

				int methodId = -1;
				int objectId = -1;

				int start = lBracket + 1;
				std::string toPush;

				for (auto i = start; i < name.length(); i++)
				{
					toPush.push_back(name[i]);

					if (name[i] == ',')
					{
						toPush.pop_back();
						cmd_info.procArguments.push_back(toPush);
						toPush.clear();
					}
					else if (name[i] == ')')
					{
						toPush.pop_back();
						cmd_info.procArguments.push_back(toPush);
						toPush.clear();
						break;
					}
				}

				for (auto i = 0; i < memory.Objects.size(); i++)
				{
					if (memory.Objects[i].objectName == objectName)
					{
						objectId = i;
						break;
					}
				}

				if (objectId < 0)
					throw Variable_not_found();

				for (auto i = 0; i < memory.Objects[objectId].Class.Methods.size(); i++)
				{
					if (methodName == memory.Objects[objectId].Class.Methods[i].name)
					{
						methodId = i;
						break;
					}
				}

				if (methodId == -1)
					throw Variable_not_found();

				std::vector<COMMAND_INFO> cmd_array;

				cmd_info.s1 = objectName;
				cmd_info.s2 = methodName;


				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_CALLMETHOD;
				cmd_array.push_back(cmd_info);
				cmd_info.Type = CMD_UNKNOWN;
				cmd_array.push_back(cmd_info);
				

				if (!memory.execute_commands(cmd_array, 0, cmd_array.size() - 1))
					std::cout << memory.err_str << std::endl;
				else
					return memory.Objects[memory.objectNumber].Class.Methods[methodId].returnValue;
			}

			else if (lBracket != NOT_FOUND)
			{
				auto rBracket = name.find_last_of(")");

			if (rBracket == NOT_FOUND)
					throw Not_parsed();

				SnakeScript::COMMAND_INFO cmd_info;

				std::string procName = name.substr(0, lBracket);

				int procId = -1;

				cmd_info.s1 = procName;
				cmd_info.Type = CMD_CALLPROC;

				int start = lBracket + 1;
				std::string toPush;

				for (auto i = start; i < name.length(); i++)
				{
					toPush.push_back(name[i]);

					if (name[i] == ',')
					{
						toPush.pop_back();
						cmd_info.procArguments.push_back(toPush);
						toPush.clear();
					}
					else if (name[i] == ')')
					{
						toPush.pop_back();
						cmd_info.procArguments.push_back(toPush);
						toPush.clear();
						break;
					}
				}
				

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

				if (!memory.execute_commands(cmd_array, 0, cmd_array.size() - 1))
					cout << memory.err_str << endl;
				

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

