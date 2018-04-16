#include "stdafx.h"
#include "SnakeScript.h"


SnakeScript::SnakeScript(std::string fileName)
	: scriptName(fileName)
{
}

std::string SnakeScript::get_script_name()
{
	return scriptName;
}

bool SnakeScript::run()
{
	std::ifstream plik;
	plik.open(scriptName);
	char c;

	if (!plik.good())
	{
		cout << "Error! Brak pliku" + get_script_name() << endl;
		return false;
	}

	while (true)
	{
		c = plik.get();
		if (plik.eof())
			break;

		bufor += c;

	}

	plik.close();

	bool result;

	result = parse();

	Libs.push_back(scriptName); //Zabezpieczenie przed pêtl¹ uselib

	if (!result)
	{
		cout << err_str << endl;
		system("pause > nul");
		return false;
	}
	else
	{
		if (!execute_commands(Commands, 0, Commands.size() - 1))
		{
			cout << err_str << endl;
			system("pause > nul");
			return false;
		}
	}

	return true;
}

SnakeScript::~SnakeScript()
{
}

bool SnakeScript::get_token()
{
	int tmp_pos, tmp_pos2;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		cword.clear();
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	if (tmp_pos2 == NOT_FOUND)
	{
		pos = bufor.length() - 1;
		cword = bufor.substr(tmp_pos, bufor.length() - tmp_pos);

	}
	else
	{
		pos = tmp_pos2; cword = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
	}

	return true;
}

bool SnakeScript::get_logic_operator(int &operator_buff)
{
	int tmp_pos, tmp_pos2;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		cword.clear();
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	if (tmp_pos2 == NOT_FOUND)
	{
		pos = bufor.length() - 1;
		cword = bufor.substr(tmp_pos, bufor.length() - tmp_pos);

	}
	else
	{
		pos = tmp_pos2; cword = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
	}

	if (cword == "==")
		operator_buff = OP_EQUAL;

	else if (cword == "!=")
		operator_buff = OP_NOT_EQUAL;

	else if (cword == "<")
		operator_buff = OP_LESS;

	else if (cword == ">")
		operator_buff = OP_GREATER;

	else if (cword == "<=")
		operator_buff = OP_LESS_OR_EQUAL;

	else if (cword == ">=")
		operator_buff = OP_GREATER_OR_EQUAL;
	else
		return false;

	return true;
}

bool SnakeScript::get_int()
{
	int tmp_pos, tmp_pos2;
	string s_number;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		param = 0;
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	cword.clear();

	if (tmp_pos2 == NOT_FOUND)
	{
		tmp_pos2 = bufor.find_last_not_of(WS_SET, tmp_pos);
		if (tmp_pos2 == tmp_pos)
		{
			s_number = bufor[tmp_pos];

		}

		pos = tmp_pos2;

	}

	if (s_number.empty())
	{
		s_number = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
		pos = tmp_pos2;
	}



	param = atoi(s_number.c_str());

	if (param == 0 && s_number != "0")
	{
		cword = s_number;
		return false;
	}
	return true;
}

bool SnakeScript::get_param()
{
	int tmp_pos, tmp_pos2;
	string s_number;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		param = 0;
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	cword.clear();

	if (tmp_pos2 == NOT_FOUND)
	{
		tmp_pos2 = bufor.find_last_not_of(WS_SET, tmp_pos);
		if (tmp_pos2 == tmp_pos)
		{
			s_number = bufor[tmp_pos];

		}

		pos = tmp_pos2;

	}

	if (s_number.empty())
	{
		s_number = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
		pos = tmp_pos2;
	}


	param = atoi(s_number.c_str());

	return true;
}

bool SnakeScript::get_operator()
{
	int tmp_pos, tmp_pos2;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		cword.clear();
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	if (tmp_pos2 == NOT_FOUND)
	{
		pos = bufor.length() - 1;
		cword = bufor.substr(tmp_pos, bufor.length() - tmp_pos);

	}
	else
	{
		pos = tmp_pos2;
		cword = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
	}

	if (cword == "=" || cword == "+=" || cword == "-=" || cword == "*=" || cword == "/=" || cword == "^=" || cword == "++" || cword == "--")
	{
		return true;
	}

	cword.clear();
	pos = tmp_pos;
	return false;
}

bool SnakeScript::get_math_operator()
{
	int tmp_pos, tmp_pos2;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		cword.clear();
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	if (tmp_pos2 == NOT_FOUND)
	{
		pos = bufor.length() - 1;
		cword = bufor.substr(tmp_pos, bufor.length() - tmp_pos);

	}
	else
	{
		pos = tmp_pos2;
		cword = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
	}

	if (cword == "+")
	{
		return true;
	}


	if (cword == "-")
	{
		return true;
	}


	if (cword == "*")
	{
		return true;
	}

	if (cword == "/")
	{
		return true;
	}

	if (cword == "%")

		cword.clear();
	pos = tmp_pos;
	return false;
}

void SnakeScript::ignore_line()
{
	int tmp_pos; tmp_pos = bufor.find_first_of(BR_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		pos = POS_END;
		return;
	}

	tmp_pos = bufor.find_first_not_of(BR_SET, tmp_pos);

	if (tmp_pos == NOT_FOUND)
	{
		pos = POS_END;
		return;
	}
	pos = tmp_pos;
}

bool SnakeScript::save(string fileName, VARIABLE &var)
{
	std::fstream plik;
	plik.open(fileName, std::ios::out | std::ios::trunc);

	if (!plik.good())
	{
		err_str = "Error! Brak pliku " + fileName;
		return false;
	}

	plik << var.name << endl;
	plik << var.isConst << endl;
	plik << var.value << endl;

	plik.close();
	return true;
}

bool SnakeScript::load(string fileName, VARIABLE &var)
{
	std::fstream plik;
	plik.open(fileName);

	if (!plik.good())
	{
		err_str = "Error! Brak pliku " + fileName;
		return false;
	}

	int licznik = 1;
	string linia;

	while (getline(plik, linia))
	{
		switch (licznik)
		{
		case 1: var.name = linia;							break;
		case 2:	var.isConst = (bool)atoi(linia.c_str());	break;
		case 3: var.value = atoi(linia.c_str());			break;
		}
		licznik++;
	}

	return true;
}

bool SnakeScript::encrypt(const char *plikWyjsciowy, VARIABLE &var)
{
	const char * plikXorowany = "asdwe1231012sda.bff";

	std::fstream encrypt;

	encrypt.open(plikXorowany, std::ios::out);


	encrypt << var.name << endl;
	encrypt << var.isConst << endl;
	encrypt << var.value << endl;

	encrypt.close();

	FILE *plik = fopen(plikXorowany, "rb");

	//const char * plik = plikXorowany;

	FILE *zaszyfrowany = fopen(plikWyjsciowy, "wb");

	const char* klucz = "1111";

	if (plik != NULL && zaszyfrowany != NULL)
	{
		//udane otwarcie pliku do zaszyfrowania i wyjsciowego
		int length = strlen(klucz);
		int znak, xorChar;
		int mod = 0;

		//szyfrowanie XOR znak po znaku
		do
		{
			if (mod >= length)
				mod = 0;
			znak = fgetc(plik);
			xorChar = znak ^ klucz[mod];
			mod++;
			if (znak != EOF)
				fputc(xorChar, zaszyfrowany);
		} while (znak != EOF);


		//zapisywanie plikow (zamykanie)
		if (fclose(zaszyfrowany) == 0 && fclose(plik) == 0)
		{
			remove(plikXorowany);
			return true;
		}

		else
			return false;

	}
	else
		return false;
}

bool SnakeScript::decrypt(const char *plikXorowany, VARIABLE &var)
{
	const char * plikWyjsciowy = "weopqwe31023ps.bff";

	std::fstream decrypt;

	FILE *plik = fopen(plikXorowany, "rb");

	//const char * plik = plikXorowany;

	FILE *zaszyfrowany = fopen(plikWyjsciowy, "wb");

	const char* klucz = "1111";

	if (plik != NULL && zaszyfrowany != NULL)
	{
		//udane otwarcie pliku do zaszyfrowania i wyjsciowego
		int length = strlen(klucz);
		int znak, xorChar;
		int mod = 0;

		//szyfrowanie XOR znak po znaku
		do
		{
			if (mod >= length)
				mod = 0;
			znak = fgetc(plik);
			xorChar = znak ^ klucz[mod];
			mod++;
			if (znak != EOF)
				fputc(xorChar, zaszyfrowany);
		} while (znak != EOF);


		//zapisywanie plikow (zamykanie)
		if (fclose(zaszyfrowany) == 0 && fclose(plik) == 0)
		{
			if (!decrypt.good())
				return false;

			string line;
			int n_line = 1;

			decrypt.open(plikWyjsciowy);

			while (getline(decrypt, line))
			{
				switch (n_line)
				{
				case 1: var.name = line; break;
				case 2: var.isConst = (bool)atoi(line.c_str()); break;
				case 3: var.value = atoi(line.c_str());	break;
				}
				n_line++;
			}

			decrypt.close();

			remove(plikWyjsciowy);
			return true;
		}

		else
			return false;

	}
	else
		return false;
}

SnakeScript::EOpType SnakeScript::negate_operator(EOpType op)
{
	switch (op)
	{
	case OP_EQUAL: return OP_NOT_EQUAL;
	case OP_NOT_EQUAL: return OP_EQUAL;
	case OP_LESS: return OP_GREATER_OR_EQUAL;
	case OP_GREATER: return OP_LESS_OR_EQUAL;
	case OP_LESS_OR_EQUAL: return OP_GREATER;
	case OP_GREATER_OR_EQUAL: return OP_LESS;

	default: return OP_UNKNOWN;
	}

}

SnakeScript::VARIABLE SnakeScript::get_local_variable(const string& sName, bool bChangeValue)
{
	std::stack<VARIABLE> temp;
	VARIABLE var;

	bool bFound = false;

	while (LocalVariables.size())
	{
		var = LocalVariables.top();
		LocalVariables.pop();

		if (var.name == sName)
		{
			bFound = true;

			if (bChangeValue)
				++var.value;
		}

		temp.push(var);

		if (bFound)
			break;
	}

	if (!bFound)
	{
		var.status = ENCAPSULATION_UNKNOWN;
		var.value = -1;
	}

	while (temp.size())
	{
		LocalVariables.push(temp.top());
		temp.pop();
	}

	return var;
}

void SnakeScript::set_local_variable(const string& sName, int value)
{
	std::stack<VARIABLE> temp;
	VARIABLE var;

	bool bFound = false;

	while (LocalVariables.size())
	{
		var = LocalVariables.top();
		LocalVariables.pop();

		if (var.name == sName)
		{
			bFound = true;

			var.value = value;
		}

		temp.push(var);

		if (bFound)
			break;
	}

	if (!bFound)
	{
		var.status = ENCAPSULATION_UNKNOWN;
		var.value = -1;
	}

	while (temp.size())
	{
		LocalVariables.push(temp.top());
		temp.pop();
	}

}

bool SnakeScript::get_word(string word)
{
	int tmp_pos, tmp_pos2;
	tmp_pos = bufor.find_first_not_of(WS_SET, pos);

	if (tmp_pos == NOT_FOUND)
	{
		cword.clear();
		pos = POS_END;
		return false;
	}

	tmp_pos2 = bufor.find_first_of(WS_SET, tmp_pos);

	if (tmp_pos2 == NOT_FOUND)
	{
		pos = bufor.length() - 1;
		cword = bufor.substr(tmp_pos, bufor.length() - tmp_pos);

	}
	else
	{
		pos = tmp_pos2; cword = bufor.substr(tmp_pos, tmp_pos2 - tmp_pos);
	}


	if (cword == word)
		return true;
	else
	{
		cword.clear();
		pos = tmp_pos;
		return false;
	}
}

int SnakeScript::get_object_id(std::string name)
{
	for (auto i = 0; i < Objects.size(); i++)
	{
		if (Objects[i].objectName == name)
			return i;

		return -1;
	}
}

int SnakeScript::get_object_var_id(string name, string objectName)
{
	for (auto i = 0; i < Objects.size(); i++)
	{
		if (Objects[i].objectName == objectName)
		{
			for (auto j = 0; j < Objects[i].Class.Variables.size(); i++)
			{
				if (Objects[i].Class.Variables[j].name == name)
					return i;
			}

			return -1;
		}
	}

	return -1;
}

int SnakeScript::get_class_id(string className)
{
	for (auto i = 0; i < Classes.size(); i++)
	{
		if (Classes[i].name == className)
			return i;
	}

	return -1;
}
std::vector<string> SnakeScript::get_init()
{
	std::vector<string> initVec;

	if (!get_token())
	{
		return initVec;
	}

	string initVar;

	initVar = cword;

	initVec.push_back(initVar);


	while (true)
	{
		get_token();

		if (cword == "|")
			break;

		initVar = cword;

		initVec.push_back(initVar);
	}

	return initVec;
}

bool SnakeScript::is_true(int nVarValue, EOpType OpType, int nValue)
{
	switch (OpType)
	{
	case OP_EQUAL: return (nVarValue == nValue);
	case OP_NOT_EQUAL: return (nVarValue != nValue);
	case OP_LESS: return (nVarValue < nValue);
	case OP_GREATER: return (nVarValue > nValue);
	case OP_LESS_OR_EQUAL: return (nVarValue <= nValue);
	case OP_GREATER_OR_EQUAL: return (nVarValue >= nValue);
	}

	return false;
}

bool SnakeScript::execute_commands(std::vector<COMMAND_INFO>& cmd_array, int start, int stop)
{
	static int procNestingLvl;

	static bool isBreak{ false };
	static bool isContinue{ false };
	static bool isLoop{ false };
	static bool goBreak{ false };
	static bool goContinue{ false };

	static bool isReturn{ false };
	static bool isProc{ false };

	static int nNestingLevel = 0;

	static int lastReturn{ 0 };

	//static int objectNumber;
	static int procId;
	static int methodId;
	static bool lastIf{ false };

	for (int i = start; i <= stop; ++i)
	{
	InsideLoop:

		if (isReturn)
		{
			if (cmd_array[i-1].Type != CMD_CALLPROC)
				return true;
			else
				isReturn = false;
		}

		if (isLoop)
		{
			if (isBreak)
			{
				isBreak = false;
				goBreak = true;
				return true;
			}
			if (isContinue)
			{
				isContinue = false;
				goContinue = true;
				return true;
			}
		}

		switch (cmd_array[i].Type)
		{
		case CMD_ASSIGN_VALUE:
		case CMD_READ:
		{
			auto dot = cmd_array[i].s1.find('.');
			auto leftBracket = cmd_array[i].s1.find_first_of('[');

			if (cmd_array[i].Type == CMD_READ)
			{
				std::getline(std::cin, cmd_array[i].s2);
			}

			Parser parser(cmd_array[i].s2);
			Expression* expr = parser.parse_Expression();

			int value = expr->eval(*this);
			
			if (dot != NOT_FOUND)
			{
				std::string objectName = cmd_array[i].s1.substr(0, dot);
				std::string varName = cmd_array[i].s1.substr(dot + 1);
				int varId = -1;
				int objectId = -1;


				if (objectName != "this")
				{
					for (auto j = 0; j < Objects.size(); j++)
					{
						if (Objects[j].objectName == objectName)
						{
							objectId = j;
							break;
						}
					}
				}
				else
				{
					if (!isMethod)
					{
						err_str = "Error! Nie mozna stosowac 'this' poza klasa!";
						return false;
					}

					objectId = objectNumber;
				}
					
				if (objectId < 0)
				{
					err_str = "Error! Nie ma obiektu " + objectName;
				}

				for (auto j = 0; j < Objects[objectId].Class.Variables.size(); j++)
				{
					if (Objects[objectId].Class.Variables[j].name == varName)
					{
						if (Objects[objectId].Class.Variables[j].status != PUBLIC && objectName != "this")
						{
							err_str = "Error! Zmienna " + varName + " w obiekcie " + objectName + " nie jest publiczna!";
							return false;
						}

						varId = j;
						break;
					}
				}

				if (varId < 0)
				{
					err_str = "Error! Nie ma zmiennej " + varName + " w obiekcie " + Objects[objectId].objectName;
					return false;
				}

				Objects[objectId].Class.Variables[varId].value = value;
			}

			else if (leftBracket != NOT_FOUND)
			{
				auto rightBracket = cmd_array[i].s1.find_last_of(']');

				if (rightBracket == NOT_FOUND)
				{
					err_str = "Error! Brak ''['";
					return false;
				}

				int arrayId = -1;
				std::string arrayName = cmd_array[i].s1.substr(0, leftBracket);

				for (auto j = 0; j < Arrays.size(); j++)
				{
					if (Arrays[j][0].name == arrayName)
					{
						arrayId = j;
						break;
					}
				}

				if (arrayId < 0)
				{
					err_str = "Error! Nie ma tablicy " + arrayName;
					return false;
				}

				std::string inArrayString = cmd_array[i].s1.substr(leftBracket+1);
				inArrayString.pop_back();

				Parser parser{ inArrayString };
				Expression* expr = parser.parse_Expression();

				int arrayIndex = expr->eval(*this);

				Arrays[arrayId][arrayIndex].value = value;
			}

			else
			{
				int varId = get_var_id(cmd_array[i].s1);

				if (varId < 0)
				{
					VARIABLE localVar = get_local_variable(cmd_array[i].s1, false);

					if (localVar.status == ENCAPSULATION_UNKNOWN)
					{
						err_str = "Error! nie ma zmiennej " + cmd_array[i].s1;
						return false;
					}

					set_local_variable(cmd_array[i].s1, value);

				}
				else
				{
					if (Variables[varId].isConst)
					{
						err_str = "Error! Nie mozna zmienic wartosci stalej!";
						return false;
					}

					Variables[varId].value = value;
				}

			}

		}	break;

		case CMD_CREATE_OBJECT:
		{
			OBJECT object;

			object.objectName = cmd_array[i].s1;

			for (const auto &itr : Objects)
			{
				if (itr.objectName == object.objectName)
				{
					err_str = "Error! Istnieje obiekt '" + object.objectName + "' !";
					return false;
				}
			}

			int matchingClass = -1;

			if (!Classes.empty())
			{
				for (auto j = 0; j < Classes.size(); j++)
				{
					if (Classes[j].name == cmd_array[i].s2)
					{
						matchingClass = j;
						break;
					}
				}
			}
			else
			{
				err_str = "Error! W pliku nie jest zdefiniowana zadna klasa!";
				return false;
			}

			if (matchingClass < 0)
			{
				err_str = "Error! Nie ma klasy " + cmd_array[i].s2;
				return false;
			}

			if (Classes[matchingClass].isAbstract)
			{
				err_str = "Error! Nie mozna utworzyc obiektu klasy abstrakcyjnej " + Classes[matchingClass].name;
				return false;
			}

			object.Class = Classes[matchingClass];

			if (!object.Class.Default.DefaultVar.empty())
			{
				int matchingVar = -1;

				for (auto i = 0; i < object.Class.Default.DefaultVar.size(); i++)
				{
					for (auto j = 0; j < object.Class.Variables.size(); j++)
					{
						if (object.Class.Variables[j].name == object.Class.Default.DefaultVar[i])
						{
							matchingVar = j;
							break;
						}
					}

					if (matchingVar < 0)
					{
						err_str = "Error! Klasa " + object.Class.name + " nie ma zmiennej skladowej " + object.Class.Default.DefaultVar[i];
						return false;
					}

					object.Class.Variables[matchingVar].value = object.Class.Default.DefaultInt[i];
				}
			}

			if (!object.Class.Init.empty())
			{
				for (auto j = 0; j < object.Class.Init.size(); j++)
				{
					int matchingVar = -1;

					for (int k = 0; k < object.Class.Variables.size(); k++)
					{
						if (object.Class.Variables[k].name == object.Class.Init[j])
						{
							matchingVar = k;
							break;
						}
					}

					if (matchingVar < 0)
					{
						err_str = "Error! Klasas " + object.Class.name + " nie ma zmiennej " + object.Class.Init[i];
						return false;
					}

					object.Class.Variables[matchingVar].value = cmd_array[i].initValues[j];
				}


			}

			Objects.push_back(object);

		}	break;

		case CMD_CREATE_VAR:
		{
			VARIABLE var_info;

			if (cmd_array[i].s3 == "CONST")
				var_info.isConst = true;

			var_info.name = cmd_array[i].s1;
			var_info.value = cmd_array[i].n1;
			
			int varId = -1;

			varId = get_var_id(var_info.name);

			if (varId >= 0)
				Variables.erase(Variables.begin() + varId);

			Variables.push_back(var_info);

		}	break;

		case CMD_CREATE_LOCAL_VAR:
		{
			VARIABLE var;

			var.name = cmd_array[i].s1;
			var.value = cmd_array[i].n1;

			LocalVariables.push(var);

			if (isProc)
				Procs[procId].localVariables++;
			else if (isMethod)
				Objects[objectNumber].Class.Methods[methodId].localVariables++;

		}	break;

		case CMD_DELETE_VAR:
		{
			int varId = get_var_id(cmd_array[i].s1);

			if (varId < 0)
			{
				err_str = "Error! nie ma zmiennej " + cmd_array[i].s1;
				return false;
			}

			Variables.erase(Variables.begin() + varId);
		}	break;

		case CMD_SYSTEM:
		{
			system(cmd_array[i].s1.c_str());
		}	break;

		case CMD_WAIT:
		{
			Parser parser{ cmd_array[i].s1 };
			Expression* expr = parser.parse_Expression();

			int value = expr->eval(*this);

			Sleep(value);

		}	break;
		
		case CMD_WRITE:
		case CMD_PRINT:
		{
			var value;

			if (cmd_array[i].s3 != "STRING")
			{
				Parser parser{ cmd_array[i].s1 };
				Expression* expr = parser.parse_Expression();

				if (cmd_array[i].Type == CMD_WRITE)
					cout << expr->eval(*this) << endl;
				else
					cout << expr->eval(*this);

			}
			else
			{
				if (cmd_array[i].Type == CMD_WRITE)
					cout << cmd_array[i].s1 << endl;
				else
					cout << cmd_array[i].s1;
			}
				

		}	break;
		
		case CMD_CREATE_ARRAY:
		{
			std::string varName = cmd_array[i].s1;

			for (auto j = varName.length() - 1; j != 0; j--)
			{
				if (varName[j] == '[')
				{
					varName.pop_back();
					break;
				}
				
				varName.pop_back();
			}

			int arrayLength;

			int leftBracketPos = cmd_array[i].s1.find('[');

			if (leftBracketPos == NOT_FOUND)
			{
				err_str = "Error! Nie znaleziono '['";
				return false;
			}

			leftBracketPos++;
			std::string arrayLegnthS;

			for (leftBracketPos; leftBracketPos != cmd_array[i].s1.length() - 1; leftBracketPos++)
			{
				arrayLegnthS.push_back(cmd_array[i].s1[leftBracketPos]);
			}

			arrayLength = std::stoi(arrayLegnthS);

			VARIABLE variable;
			std::vector<VARIABLE> thisArray;

			variable.name = varName;
			variable.value = cmd_array[i].n1;

			for (auto i = 0; i < arrayLength; i++)
			{
				thisArray.push_back(variable);
			}

			Arrays.push_back(thisArray);

		}	break;

		case CMD_CALLPROC:
		{
			int lastProc = procId;
			procId = -1;

			for (auto j = 0; j < Procs.size(); j++)
			{
				if (Procs[j].name == cmd_array[i].s1)
				{
					procId = j;
					break;
				}
			}

			if (procId < 0)
			{
				err_str = "Error! Nie ma procedury " + cmd_array[i].s1;
				return false;
			}

			if (!cmd_array[i].yield_info.empty())
			{
				Procs[procId].Yield = cmd_array[i].yield_info;
			}

			if (isMethod)
			{
				isMethod = false;
				execute_commands(Procs[procId].Body, 0, Procs[procId].Body.size() - 1);
				isMethod = true;
			}
			else if (isProc)
			{
				std::stack<VARIABLE> temp;

				while(Procs[lastProc].localVariables > 0)
				{
					temp.push(LocalVariables.top());
					LocalVariables.pop();
					Procs[lastProc].localVariables--;
				}

				execute_commands(Procs[procId].Body, 0, Procs[procId].Body.size() - 1);
				Procs[procId].returnValue = lastReturn;

				cout << Procs[procId].returnValue << endl;

				system("pause > nul");

				while (!temp.empty())
				{
					LocalVariables.push(temp.top());
					temp.pop();
					Procs[lastProc].localVariables++;
				}

			}
			else
			{
				isProc = true;
				execute_commands(Procs[procId].Body, 0, Procs[procId].Body.size() - 1);
				Procs[procId].returnValue = lastReturn;
				isProc = false;
			}

			while (Procs[procId].localVariables > 0)
			{
				LocalVariables.pop();
				Procs[procId].localVariables--;
			}

			Procs[procId].Yield.clear();

		}	break;

		case CMD_YIELD:
		{

			if (isMethod && methodId >= 0)
			{
				if (!Objects[objectNumber].Class.Methods[methodId].Yield.empty())
				{
					execute_commands(Objects[objectNumber].Class.Methods[methodId].Yield, 0, Objects[objectNumber].Class.Methods[methodId].Yield.size() - 1);
				}
			}

			else if (procId >= 0)
			{
				if (!Procs[procId].Yield.empty())
				{
					execute_commands(Procs[procId].Yield, 0, Procs[procId].Yield.size() - 1);
				}
			}

		}	break;

		case CMD_RUN_THREAD:
		{
			int matchingThread = -1;

			for (auto j = 0; j < Threads.size(); j++)
			{
				if (Threads[j].name == cmd_array[i].s1)
				{
					matchingThread = j;
					break;
				}
			}

			if (matchingThread < 0)
			{
				err_str = "Error! Nie ma watku " + cmd_array[i].s1;
				return false;
			}

			Threads[matchingThread].thread = new std::thread([=] { execute_commands(Threads[matchingThread].Body, 0, Threads[matchingThread].Body.size() - 1); });

		}	break;

		case CMD_CALLMETHOD:
		{
			int matchingMethod = -1;
			string methodName = cmd_array[i].s2;

			if (isMethod)
			{
				for (auto j = 0; j < Objects[objectNumber].Class.Methods.size(); j++)
				{

					if (Objects[objectNumber].Class.Methods[j].name == methodName)
					{
						matchingMethod = j;
						break;
					}
				}

				if (matchingMethod < 0)
				{
					err_str = "Error w obiekcie " + Objects[objectNumber].objectName + ". Klasa " + Objects[objectNumber].Class.name + " nie ma metody " + methodName;
					return false;
				}

				methodId = matchingMethod;

				if (!cmd_array[i].yield_info.empty())
				{
					Objects[objectNumber].Class.Methods[matchingMethod].Yield = cmd_array[i].yield_info;
				}

				if (!execute_commands(Objects[objectNumber].Class.Methods[matchingMethod].Body, 0, Objects[objectNumber].Class.Methods[matchingMethod].Body.size() - 1))
				{
					err_str = "Error w metodzie " + Objects[objectNumber].Class.Methods[matchingMethod].name + " w obiekcie " + Objects[objectNumber].objectName + " klasy " + Objects[objectNumber].Class.name;
					return false;
				}

				while (Objects[objectNumber].Class.Methods[matchingMethod].localVariables > 0)
				{
					LocalVariables.pop();
					Objects[objectNumber].Class.Methods[matchingMethod].localVariables--;
				}

				Objects[objectNumber].Class.Methods[matchingMethod].Yield.clear();
			}
			else
			{
				objectNumber = -1;

				for (auto j = 0; j < Objects.size(); j++)
				{
					if (Objects[j].objectName == cmd_array[i].s1)
					{
						objectNumber = j;
						break;
					}
				}

				if (objectNumber < 0)
				{
					err_str = "Error! Nie ma obiektu " + cmd_array[i].s1;
					return false;
				}

				for (auto j = 0; j < Objects[objectNumber].Class.Methods.size(); j++)
				{

					if (Objects[objectNumber].Class.Methods[j].name == methodName)
					{
						matchingMethod = j;
						break;
					}
				}

				if (matchingMethod < 0)
				{
					err_str = "Error w obiekcie " + Objects[objectNumber].objectName + ". Klasa " + Objects[objectNumber].Class.name + " nie ma metody " + methodName;
					return false;
				}

				methodId = matchingMethod;

				if (Objects[objectNumber].Class.Methods[matchingMethod].status != PUBLIC && !isMethod)
				{
					err_str = "Error! Metoda jest private/protected!";
					return false;
				}

				objectNumber = objectNumber;

				isMethod = true;

				if (!cmd_array[i].yield_info.empty())
				{
					Objects[objectNumber].Class.Methods[matchingMethod].Yield = cmd_array[i].yield_info;
				}

				if (!execute_commands(Objects[objectNumber].Class.Methods[matchingMethod].Body, 0, Objects[objectNumber].Class.Methods[matchingMethod].Body.size() - 1))
				{
					err_str = "Error w metodzie " + Objects[objectNumber].Class.Methods[matchingMethod].name + " w obiekcie " + Objects[objectNumber].objectName + " klasy " + Objects[objectNumber].Class.name;
					return false;
				}

				while (Objects[objectNumber].Class.Methods[matchingMethod].localVariables > 0)
				{
					LocalVariables.pop();
					Objects[objectNumber].Class.Methods[matchingMethod].localVariables--;
				}

				Objects[objectNumber].Class.Methods[matchingMethod].Yield.clear();

				isMethod = false;
			}
		}	break;

		case CMD_ELSEIF:
		case CMD_ELSE:
		case CMD_IF:
		{
			int rbracket = -1, matching_if = -1;
		
			if (cmd_array[i].Type == CMD_ELSEIF)
			{
				for (int k = i; k != -1; k--)
				{
					if (cmd_array[k].Type == CMD_IF && cmd_array[k].nNestingLevel == cmd_array[k].nNestingLevel)
					{
						matching_if = k;
						break;
					}


				} //for(int j 

				if (matching_if < 0)
				{
					err_str = "Error! Else if bez pasujacego ifa'!";
					return false;
				}
			}

			else if (cmd_array[i].Type == CMD_ELSE)
			{
				for (int k = i; k != -1; k--)
				{

					if (cmd_array[k].Type == CMD_IF || (cmd_array[k].Type == CMD_ELSEIF && !lastIf) && cmd_array[k].nNestingLevel == cmd_array[k].nNestingLevel)
					{
						matching_if = k;
						break;
					}
				}

				if (matching_if < 0)
				{
					err_str = "Error! Else bez odpowiedniego if'a!";
					return false;
				}

				cmd_array[i] = cmd_array[matching_if];
				cmd_array[i].Type = CMD_ELSE;
				cmd_array[i].n2 = negate_operator((EOpType)cmd_array[matching_if].n2); //"odwrócenie" warunku 

			} //if(cmd_array[i].Type 

			for (int j = i + 1; j <= stop; ++j)
			{
				if (cmd_array[j].Type == CMD_RBRACKET && cmd_array[j].nNestingLevel == cmd_array[i].nNestingLevel)
				{
					rbracket = j;
					break;
				}
			}

			if (rbracket < 0)
			{
				err_str = "Error! Niedomkniety blok if!";
				return false;
			}

			Parser leftParser{ cmd_array[i].s1 };
			Parser rightParser{ cmd_array[i].s2 };

			Expression* expr = leftParser.parse_Expression();

			int leftValue = expr->eval(*this);

			expr = rightParser.parse_Expression();

			int rightValue = expr->eval(*this);

			if (cmd_array[i].Type == CMD_ELSEIF && !lastIf)
			{
				if (is_true(leftValue, (EOpType)cmd_array[i].n2, rightValue))
				{

					if (!execute_commands(cmd_array, i + 1, rbracket - 1))
					{
						err_str += "Blad w bloku if!";
						return false;
					}
				}


			}

			if (cmd_array[i].Type != CMD_ELSEIF)
			{
				if (is_true(leftValue, (EOpType)cmd_array[i].n2, rightValue))
				{
					if (cmd_array[i].Type == CMD_IF)
						lastIf = true;

					if (!execute_commands(cmd_array, i + 1, rbracket - 1))
					{
						err_str += "Blad w bloku if!";
						return false;
					}
				} // if(is_true...
				else if(cmd_array[i].Type == CMD_IF)
					lastIf = false;
			}

			i = rbracket;

		}	break;

		case CMD_FOR:
		{
			int rbracket = -1;

			for (int j = i + 1; j<cmd_array.size(); ++j) //szukanie prawej klamry 
			{
				if (cmd_array[j].Type == CMD_RBRACKET && cmd_array[j].nNestingLevel == cmd_array[i].nNestingLevel)
				{
					rbracket = j;
					break;
				}
			}

			if (rbracket < 0)
			{
				err_str = "Error! Niedomkniety blok for!";
				return false;
			} //wykonanie pêtli 

			Parser leftParser{ cmd_array[i].s2 };
			Parser rightParser{ cmd_array[i].s3 };

			Expression* expr = leftParser.parse_Expression();

			VARIABLE counter;
			counter.name = cmd_array[i].s1;

			counter.value = expr->eval(*this);

			if (counter.value < 0)
			{
				err_str = "Error! nieprawidlowa wartosc licznika petli!";
				return false;
			}

			expr = rightParser.parse_Expression();

			int finalValue = expr->eval(*this);

			if (finalValue <= counter.value)
			{
				err_str = "Error! Ostateczna wartosc licznika petli nie moze byc <= od poczarkowej wartosci";
				return false;
			}
				//++nNestingLevel;

				LocalVariables.push(counter);
				for (int k = counter.value; k <= finalValue; ++k)
				{
					isLoop = true;

					if (goBreak)
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						break;
					}

					if (goContinue)
					{
						isContinue = false;
						goContinue = false;
						continue;
					}

					if (!execute_commands(cmd_array, i + 1, rbracket))
						return false;

					get_local_variable(cmd_array[i].s1, true); //zwiêksz licznik pêtli 
				} // for(int k 

				LocalVariables.pop();
				i = rbracket;
				isLoop = false;
			} break;

		case CMD_BREAK:
		{
			if (isLoop)
				isBreak = true;
		}	break;

		case CMD_CONTINUE:
		{
			if (isLoop)
				isContinue = true;
		}	break;

		case CMD_DOWHILE:
		case CMD_WHILE:
		{
			int rbracket = -1;

			for (int j = i + 1; j<cmd_array.size(); ++j) //szukanie prawej klamry 
			{
				if (cmd_array[j].Type == CMD_RBRACKET && cmd_array[j].nNestingLevel == cmd_array[i].nNestingLevel)
				{
					rbracket = j;
					break;
				}
			}

			if (rbracket < 0)
			{
				err_str = "Error! Niedomkniety blok while!";
				return false;
			} //wykonanie pêtli

			int leftValue = 1;
			int rightValue = 1;

			Expression* expr;

			if (!cmd_array[i].isWhileTrue)
			{
				Parser leftParser{ cmd_array[i].s1 };
				Parser rightParser{ cmd_array[i].s2 };

				expr = leftParser.parse_Expression();

				leftValue = expr->eval(*this);

				expr = rightParser.parse_Expression();

				rightValue = expr->eval(*this);
			}

			if (cmd_array[i].Type == CMD_DOWHILE)
			{
				bool isFirstIt{ true };

				while (true)
				{
					bool isBreakGeneralLoop{ false };

					if (!isFirstIt)
					{
						if (!is_true(leftValue, (EOpType)cmd_array[i].n2, rightValue))
						{
							goBreak = false;
							isLoop = false;
							isBreak = false;
							isBreakGeneralLoop = true;
							break;
						}
					}

					isFirstIt = false;

					isLoop = true;

					if (goBreak)
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						isBreakGeneralLoop = true;
						break;
					}

					if (goContinue)
					{
						isContinue = false;
						goContinue = false;
						continue;
					}

					if (!execute_commands(cmd_array, i + 1, rbracket))
						return false;

					if (!cmd_array[i].isWhileTrue)
					{
						Parser leftParser{ cmd_array[i].s1 };
						Parser rightParser{ cmd_array[i].s2 };

						expr = leftParser.parse_Expression();

						leftValue = expr->eval(*this);

						expr = rightParser.parse_Expression();

						rightValue = expr->eval(*this);
					}


				} // while
			}
			else
			{
				while (true)
				{
					bool isBreakGeneralLoop{ false };
			
					if (!is_true(leftValue, (EOpType)cmd_array[i].n2, rightValue))
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						isBreakGeneralLoop = true;
						break;
					}

					isLoop = true;

					if (goBreak)
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						isBreakGeneralLoop = true;
						break;
					}

					if (goContinue)
					{
						isContinue = false;
						goContinue = false;
						continue;
					}

					if (!execute_commands(cmd_array, i + 1, rbracket))
						return false;

					if (!cmd_array[i].isWhileTrue)
					{
						Parser leftParser{ cmd_array[i].s1 };
						Parser rightParser{ cmd_array[i].s2 };

						expr = leftParser.parse_Expression();

						leftValue = expr->eval(*this);

						expr = rightParser.parse_Expression();

						rightValue = expr->eval(*this);
					}

				} // while
			}


			i = rbracket;
			isLoop = false;

		} break;

		case CMD_DOUNTIL:
		case CMD_UNTIL:
		{
			int rbracket = -1;

			for (int j = i + 1; j<cmd_array.size(); ++j) //szukanie prawej klamry 
			{
				if (cmd_array[j].Type == CMD_RBRACKET && cmd_array[j].nNestingLevel == cmd_array[i].nNestingLevel)
				{
					rbracket = j;
					break;
				}
			}

			if (rbracket < 0)
			{
				err_str = "Error! Niedomkniety blok until!";
				return false;
			} //wykonanie pêtli

			int leftValue = 1;
			int rightValue = 1;

			Expression* expr;

			if (!cmd_array[i].isWhileTrue)
			{
				Parser leftParser{ cmd_array[i].s1 };
				Parser rightParser{ cmd_array[i].s2 };

				expr = leftParser.parse_Expression();

				leftValue = expr->eval(*this);

				expr = rightParser.parse_Expression();

				rightValue = expr->eval(*this);
			}

			if (cmd_array[i].Type == CMD_DOUNTIL)
			{
				bool isFirstIt{ true };

				while (true)
				{
					bool isBreakGeneralLoop{ false };

					if (!isFirstIt)
					{
						if (is_true(leftValue, (EOpType)cmd_array[i].n2, rightValue))
						{
							goBreak = false;
							isLoop = false;
							isBreak = false;
							isBreakGeneralLoop = true;
							break;
						}
					}

					isFirstIt = false;

					isLoop = true;

					if (goBreak)
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						isBreakGeneralLoop = true;
						break;
					}

					if (goContinue)
					{
						isContinue = false;
						goContinue = false;
						continue;
					}

					if (!execute_commands(cmd_array, i + 1, rbracket))
						return false;

					if (!cmd_array[i].isWhileTrue)
					{
						Parser leftParser{ cmd_array[i].s1 };
						Parser rightParser{ cmd_array[i].s2 };

						expr = leftParser.parse_Expression();

						leftValue = expr->eval(*this);

						expr = rightParser.parse_Expression();

						rightValue = expr->eval(*this);
					}


				} // while
			}
			else
			{
				while (true)
				{
					bool isBreakGeneralLoop{ false };
			
					if (is_true(leftValue, (EOpType)cmd_array[i].n2, rightValue))
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						isBreakGeneralLoop = true;
						break;
					}

					isLoop = true;

					if (goBreak)
					{
						goBreak = false;
						isLoop = false;
						isBreak = false;
						isBreakGeneralLoop = true;
						break;
					}

					if (goContinue)
					{
						isContinue = false;
						goContinue = false;
						continue;
					}

					if (!execute_commands(cmd_array, i + 1, rbracket))
						return false;

					if (!cmd_array[i].isWhileTrue)
					{
						Parser leftParser{ cmd_array[i].s1 };
						Parser rightParser{ cmd_array[i].s2 };

						expr = leftParser.parse_Expression();

						leftValue = expr->eval(*this);

						expr = rightParser.parse_Expression();

						rightValue = expr->eval(*this);
					}

				} // while
			}


			i = rbracket;
			isLoop = false;

		} break;

		case CMD_LOOP:
		{
			int rbracket = -1;

			for (int j = i + 1; j<cmd_array.size(); ++j) //szukanie prawej klamry 
			{
				if (cmd_array[j].Type == CMD_RBRACKET && cmd_array[j].nNestingLevel == cmd_array[i].nNestingLevel)
				{
					rbracket = j;
					break;
				}
			}

			if (rbracket < 0)
			{
				err_str = "Error! Niedomkniety blok loop!";
				return false;
			} //wykonanie pêtli

			int loopValue;
			int thisValue{ 0 };

			Parser parser{ cmd_array[i].s1 };
			Expression* expr = parser.parse_Expression();

			loopValue = expr->eval(*this) - 1;

			if (loopValue < 0)
			{
				err_str = "Error! Petla loop nie moze wykonac sie mniej niz 1 raz!";
				return false;
			}

			while (loopValue != thisValue)
			{
				isLoop = true;

				if (goBreak)
				{
					goBreak = false;
					isLoop = false;
					isBreak = false;
					break;
				}

				if (goContinue)
				{
					isContinue = false;
					goContinue = false;
					continue;
				}

				if (!execute_commands(cmd_array, i + 1, rbracket))
					return false;

				thisValue++;

			} // while

		}	break;

		case CMD_RETURN:
		{
			isReturn = true;

			Parser parser{ cmd_array[i].s1 };

			Expression* expr = parser.parse_Expression();

			lastReturn = expr->eval(*this);

		}	break;

		case CMD_UNKNOWN:
		{
			
		}	break;

		} // g³ówny switch 

		//if (isProcButReturnAgain)
			//goto Return;

	} // g³ówna pêtla for 

	//--nNestingLevel;
	lastReturn = 0;
}

bool SnakeScript::checkVarName(std::string varName)
{
	std::string invalidChars = "@$()[].,;/*%+-1234567890|~#¿ê€óñæ¿Ÿ¹³ñœ";

	if (varName.find_first_of(invalidChars) != POS_END)
		return false;

	return true;
}

bool SnakeScript::checkArrayName(std::string arrayName)
{
	std::string invalidChars = "@$().,;/*%+-|~#¿ê€óñæ¿Ÿ¹³ñœ";

	if (arrayName.find_first_of(invalidChars) != POS_END)
		return false;

	return true;
}

int SnakeScript::get_var_id(std::string varName)
{
	if (Variables.empty())
		return -1;

	for (int i = 0; i < Variables.size(); i++) 
		if (Variables[i].name == varName)
			return i;			

	return -1;
}

bool SnakeScript::get_string()
{
	const char* CHAR_QUOTE = "\"";
	int pos2;

	pos = bufor.find_first_not_of(WS_SET, pos);

	if (pos == NOT_FOUND)
		return false;

	cword = bufor.substr(pos, 1);

	if (cword != CHAR_QUOTE)
		return false;

	pos = bufor.find_first_not_of(CHAR_QUOTE, pos);

	if (pos == NOT_FOUND)
		return false;

	pos2 = bufor.find_first_of(CHAR_QUOTE, pos + 1);

	if (pos2 == NOT_FOUND)
		return false;

	cword = bufor.substr(pos, pos2 - pos);

	pos = bufor.find_first_of(WS_SET, pos2);

	if (pos == NOT_FOUND)
		pos = bufor.size() - 1;

	return true;
}

bool SnakeScript::get_brace()
{
	const char* CHAR_START = "{";
	const char* CHAR_END = "}";
	int pos2;

	pos = bufor.find_first_not_of(WS_SET, pos);

	if (pos == NOT_FOUND)
		return false;

	cword = bufor.substr(pos, 1);

	if (cword != CHAR_START)
		return false;

	pos = bufor.find_first_not_of(CHAR_START, pos);

	if (pos == NOT_FOUND)
		return false;

	pos2 = bufor.find_first_of(CHAR_END, pos + 1);

	if (pos2 == NOT_FOUND)
		return false;

	cword = bufor.substr(pos, pos2 - pos);

	pos = bufor.find_first_of(WS_SET, pos2);

	if (pos == NOT_FOUND)
		pos = bufor.size() - 1;

	return true;
}

bool SnakeScript::parse()
{
	bool bNotCommand{ false }; //flaga 
	static bool isMethod{ false };
	bool isThread{ false };
	static bool isProc{ false };
	static bool isYield{ false };
	int nCurNestingLevel = 0;

	COMMAND_INFO cmd_info = { CMD_UNKNOWN };
	//CLASS class_info;
	static METHOD method_info;
	static std::vector<COMMAND_INFO> yield_info;
	THREAD thread_info;
	static PROC proc_info;

	do
	{
		cmd_info.yield_info.clear();
		cmd_info.initValues.clear();

		if (!get_token())
			return true;

		if (cword.length() > 1)
		{
			if (cword.substr(0, 2) == "//")
			{
				ignore_line();
				continue;
			}
		}

		if (cword == "uselib")
		{
			if (!get_token())
			{
				err_str = "Error! Brak nazwy biblioteki!";
				return false;
			}



			if (!Libs.empty())
			{
				bool isError{ false };

				for (auto thisLib : Libs)
				{
					if (thisLib == cword)
					{
						isError = true;
						break;
					}
				}

				if (isError)
					continue;
			}

			Libs.push_back(cword);

			string fileName = cword;

			int pos_lib = pos;

			std::fstream libFile;

			string bufor_lib = bufor;

			cword.clear();
			bufor.clear();

			libFile.open(fileName);

			if (!libFile.good())
			{
				err_str = "Error! Brak pliku " + fileName;
				return false;
			}

			char c;

			while (true)
			{
				c = libFile.get();

				if (libFile.eof())
					break;

				bufor += c;
			}

			libFile.close();

			pos = 0;
			c = '0';

			if (!parse())
			{
				cout << err_str << endl;
				err_str = "Error! Blad krytyczny w bibliotece " + fileName;
				return false;
			}

			bufor = bufor_lib;
			pos = pos_lib;

		}

		else if (cword == "class" || cword == "final" || cword == "abstract" || cword == "interface" || cword == "struct")
		{
			CLASS class_info;

			if (cword == "interface")
			{
				class_info.isAbstract = true;
				class_info.isInterface = true;
			}

			else if (cword == "final")
			{
				class_info.isFinal = true;

				if (!get_token() || cword != "class")
				{
					err_str = "Error! Spodziewane 'class' po 'final'";
					return false;
				}
			}

			else if (cword == "abstract")
			{
				class_info.isAbstract = true;

				if (!get_token() || cword != "class")
				{
					err_str = "Error! Spodziewane 'class' po 'abstract'";
					return false;
				}
			}

			else if (cword == "struct")
				class_info.isStruct = true;

			ENCAPSULATION encapsulation = PRIVATE;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa Classes!";
				return false;
			}

			for (auto &itr : Classes)
			{
				if (itr.name == cword)
				{
					err_str = "Error! Istnieje taka klasa!";
					return false;
				}
			}

			class_info.name = cword;

			if (get_word(":") || get_word("extends"))
			{
				if (class_info.isInterface)
				{
					err_str = "Error! Interfejs nie moze dziedziczyc z zadnej Classes!";
					return false;
				}

				bool isFirstTime{ true };

				while (true)
				{
					if (!isFirstTime)
					{
						if (!get_word("and"))
						{
							if (!get_word("{"))
							{
								err_str = "Error! Spodziewane '{'";
								return false;
							}
							else
								break;
						}
						else
							get_token();
					}
					else
					{
						get_token();

						if (cword == "{")
							break;
					}

					//int extendClassId = get_class_id(cword);
					int extendClassId = -1;
					//if (Classes[extendClassId].name == class_info.name)
					//extendClassId = -1;

					for (auto i = 0; i < Classes.size(); i++)
					{
						if (Classes[i].name == cword)
						{
							if (Classes[i].name == class_info.name)
								continue;

							extendClassId = i;
							break;
						}
					}


					if (extendClassId < 0)
					{
						err_str = "Error! Nie ma Classes " + cword + " Error przy dziedziczeniu! ";
						return false;
					}

					if (Classes[extendClassId].isFinal)
					{
						err_str = "Error! Nie mozna dziedziczyc z Classes typu 'final'!";
						return false;
					}

					if (!Classes[extendClassId].isStruct && class_info.isStruct)
					{
						err_str = "Error! Struktura nie moze dziedziczyc z Classes, ktora jest struktura!";
						return false;
					}


					for (auto itr : Classes[extendClassId].Methods)
					{
						if (itr.status == PRIVATE)
							continue;

						class_info.Methods.push_back(itr);
						class_info.Methods[class_info.Methods.size() - 1].className = class_info.name;
					}

					for (auto itr : Classes[extendClassId].Variables)
					{
						if (itr.status == PRIVATE)
							continue;

						class_info.Variables.push_back(itr);
					}

					isFirstTime = false;

				}
			}

			else if (!get_token() || cword != "{")
			{
				err_str = "Error! Spodziewane '{'!";
				return false;
			}

			while (true)
			{
				get_token();

				if (cword == "};")
					break;

				if (cword.length() > 1)
				{
					if (cword.substr(0, 2) == "//")
					{
						ignore_line();
						continue;
					}
				}

				if (cword == "def" || cword == "virtual" || cword == "override")
				{
					if (class_info.isStruct)
					{
						err_str = "Error! Struktura nie moze posiadac metod!";
						return false;
					}

					if (cword == "virtual")
					{
						method_info.isVirtual = true;

						if (!get_token() || cword != "def")
						{
							err_str = "Error! Spodziewane 'def' po 'virtual'";
							return false;
						}
					}

					else if (cword == "override")
					{
						method_info.isOverride = true;

						if (!get_token() || cword != "def")
						{
							err_str = "Error! Spodziewane 'def' po 'virtual'";
							return false;
						}
					}

					method_info.status = encapsulation;

					isMethod = true;

					if (!get_token())
					{
						err_str = "Error! Spodziewana nazwa metody!";
						return false;
					}

					method_info.name = cword;

					if (!class_info.Methods.empty())
					{
						for (auto i = 0; i < class_info.Methods.size(); i++)
						{
							if (class_info.Methods[i].name == method_info.name)
							{
								if (class_info.Methods[i].isVirtual && !class_info.Methods[i].isOverride)
								{
									class_info.Methods.erase(class_info.Methods.begin() + i);
								}
								else
								{
									if (method_info.isOverride)
									{
										class_info.Methods.erase(class_info.Methods.begin() + i);
									}
									else
									{
										err_str = "Error! Istnieje juz metoda " + method_info.name + " w klasie " + class_info.name;
										return false;
									}

								}

							}
						}
					}

					if (!get_token() || cword != "{")
					{
						err_str = "Error! Spodziewane '{'";
						return false;
					}

					method_info.className = class_info.name;

					if (!parse())
					{
						err_str += "\nBlad w metodzie!";
						return false;
					}

					isMethod = false;

					class_info.Methods.push_back(method_info);

					method_info.Body.clear();
					method_info.name.clear();
				}

				else if (cword == "var")
				{
					if (class_info.isInterface)
					{
						err_str = "Error! Interfejs nie moze posiadac zmiennych skladowych!";
						return false;
					}

					if (!get_token())
					{
						err_str = "Error! Spodziewana nazwa zmiennej!";
						return false;
					}


					if (!class_info.Variables.empty())
					{
						for (auto &itr : class_info.Variables)
						{
							if (itr.name == cword)
							{
								err_str = "Error! Istnieje juz zmienna " + cword + " w klasie " + class_info.name;
								return false;
							}
						}
					}

					VARIABLE var;
					var.status = encapsulation;

					var.name = cword;
					var.isConst = false;
					var.value = 0;

					class_info.Variables.push_back(var);
				}

				else if (cword == "init")
				{
					if (class_info.isInterface)
					{
						err_str = "Error! Interfejs nie moze miec konstruktora!";
						return false;
					}

					class_info.Init.clear();
					class_info.Init = get_init();
				}

				else if (cword == "default")
				{
					class_info.Default.DefaultInt.clear();
					class_info.Default.DefaultVar.clear();

					if (class_info.isInterface)
					{
						err_str = "Error! Interfejs nie moze miec konstruktora!";
						return false;
					}

					if (!get_word("{"))
					{
						err_str = "Error! Spodziewane '{'";
						return false;
					}

					while (!get_word("}"))
					{
						if (!get_token())
						{
							err_str = "Error! Spodziewana nazwa zmiennej sk³adowej!";
							return false;
						}

						class_info.Default.DefaultVar.push_back(cword);

						if (!get_word("="))
						{
							err_str = "Error! Spodziewany znak '='";
							return false;
						}

						if (!get_int())
						{
							err_str = "Error! Spodziewana nazwa liczbowa!";
							return false;
						}

						class_info.Default.DefaultInt.push_back(param);
					}
				}

				else if (cword == "public:")
					encapsulation = PUBLIC;
				else if (cword == "private:")
					encapsulation = PRIVATE;
				else if (cword == "protected:")
					encapsulation = PROTECTED;
			}

			Classes.push_back(class_info);

			bNotCommand = true;
		}

		else if (cword == "define")
		{
			VARIABLE var_info;
			
			if (get_word("const"))
				var_info.isConst = true;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa zmiennej!";
				return false;
			}
			
			var_info.name = cword;

			if (!checkVarName(var_info.name))
			{
				err_str = "Error! Zmienna " + cword + " zawiera nie dozwolone znaki!";
				return false;
			}

			int var_id = -1;
			var_id = get_var_id(cword);

			if (var_id >= 0)
			{
				Variables.erase(Variables.begin() + var_id);
			}

			if (get_word("="))
			{
				if (!get_int())
				{
					err_str = "Error! Spodziewana nazwa inicjalizacyjna!";
					return false;
				}

				var_info.value = param;
			}
			else
				var_info.value = 0;

			bNotCommand = true;

			Variables.push_back(var_info);
		}

		else if (cword == "var" || cword == "const")
		{
			cmd_info.Type = CMD_CREATE_VAR;

			cmd_info.s3.clear();

			if (cword == "const")
				cmd_info.s3 = "CONST";

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa zmiennej!";
				return false;
			}


			cmd_info.s1 = cword;

			if (!checkVarName(cword))
			{
				err_str = "Error! Zmienna " + cword + " zawiera nie dozwolone znaki!";
				return false;
			}

			if (get_word("="))
			{
				if (!get_int())
				{
					err_str = "Error! Spodziewana nazwa inicjalizacyjna!";
					return false;
				}

				cmd_info.n1 = param;
			}
			else
				cmd_info.n1 = 0;
		}

		else if (cword == "Wait")
		{
			cmd_info.Type = CMD_WAIT;

			if (!get_token())
			{
				err_str = "Error! Spodziewany czas oczekiwania!";
				return false;
			}

			cmd_info.s1 = cword;
		}

		else if (cword == "let")
		{
			cmd_info.Type = CMD_CREATE_LOCAL_VAR;

			if (!isProc && !isMethod)
			{
				err_str = "Error! Zmienne lokalne moga byc umieszczanie jedynie w procedurach!";
				return false;
			}

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa zmiennej!";
				return false;
			}

			cmd_info.s1 = cword;

			if (get_word("="))
			{
				if (!get_int())
				{
					err_str = "Error! Spodziewana wartosc inicjalizacyjna!";
					return false;
				}

				cmd_info.n1 = param;
			}
			else
				cmd_info.n1 = 0;
		}

		else if (cword == "Read")
		{
			cmd_info.Type = CMD_READ;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa zmiennej!";
				return false;
			}

			cmd_info.s1 = cword;
		}

		else if (cword == "var[]")
		{
			cmd_info.Type = CMD_CREATE_ARRAY;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa tablicy!";
				return false;
			}

			if (cword[cword.size() - 1] != ']' || cword.find('[') == POS_END)
			{
				err_str = "Error! Tablica musi posiadaæ []";
				return false;
			}

			cmd_info.s1 = cword;

			if (!checkArrayName(cword))
			{
				err_str = "Error! Zmienna " + cword + " zawiera nie dozwolone znaki!";
				return false;
			}

			if (get_word("="))
			{
				if (!get_int())
				{
					err_str = "Error! Spodziewana wartosc inicjalizacyjna";
					return false;
				}

				cmd_info.n1 = param;
			}
			else
				cmd_info.n1 = 0;

		}
		
		else if (cword == "delete")
		{
			cmd_info.Type = CMD_DELETE_VAR;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa usuwanej zmiennej!";
				return false;
			}

			cmd_info.s1 = cword;
		}

		else if (cword == "def")
		{
			cmd_info.Type = CMD_DEFPROC;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa procedury!";
				return false;
			}

			if (!checkVarName(cword))
			{
				err_str = "Error! Procedura " + cword + " ma niedozwolone znaki!";
				return false;
			}

			proc_info.name = cword;

			if (!get_word("{"))
			{
				err_str = "Error! Spodziewane '{'";
				return false;
			}

			isProc = true;

			if (!parse())
				return false;

			Procs.push_back(proc_info);

			proc_info.Body.clear();

			isProc = false;
		}

		else if (cword == "system")
		{
			cmd_info.Type = CMD_SYSTEM;

			if (!get_string())
			{
				err_str = "Error! Spodziewana wartosc w cudzyslowiach!";
				return false;
			}

			cmd_info.s1 = cword;
		}

		else if (cword == "Write" || cword == "Print")
		{
			cmd_info.Type = CMD_WRITE;

			if(cword == "Print")
				cmd_info.Type = CMD_PRINT;

			cmd_info.s3 = "VAR";

			if (get_string())
			{
				cmd_info.s1 = cword;
				cmd_info.s3 = "STRING";
			}
			else
			{

				if (!get_brace())
				{
					if (!get_token())
					{
						err_str = "Error! Write bez zmiennej do wyswietlenia";
						return false;
					}
				}

				cmd_info.s1 = cword;
			}
		}

		else if (cword == "Object")
		{
			cmd_info.Type = CMD_CREATE_OBJECT;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa obiektu!";
				return false;
			}

			cmd_info.s1 = cword;

			if (!get_word("="))
			{
				err_str = "Error! Spodziewany znak '='";
				return false;
			}

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa klasy!";
				return false;
			}

			int classId = -1;
			classId = get_class_id(cword);

			if (classId < 0)
			{
				err_str = "Error! Nie ma klasy " + cword;
				return false;
			}

			cmd_info.s2 = cword;

			if (!Classes[classId].Init.empty())
			{
				for (short i = 0; i < Classes[classId].Init.size(); i++)
				{
					if (!get_int())
					{
						err_str = "Error! Spodziewana wartosc inicjalizacyjna!";
						return false;
					}

					cmd_info.initValues.push_back(param);
				}
			}

		}

		else if (cword == "break")
			cmd_info.Type = CMD_BREAK;

		else if (cword == "continue")
			cmd_info.Type = CMD_CONTINUE;

		else if (cword == "}")
		{
			cmd_info.Type = CMD_RBRACKET;

			if (isMethod && nCurNestingLevel == 0)
				return true;

			if (isYield && nCurNestingLevel == 0)
			{
				isYield = false;
				return true;
			}
			if (isProc && nCurNestingLevel == 0)
			{
				proc_info.Body.push_back(cmd_info);
				return true;
			}

			if (isThread && nCurNestingLevel == 0)
			{
				isThread = false;
				Threads.push_back(thread_info);
				thread_info.Body.clear();
			}
			else
				cmd_info.nNestingLevel = nCurNestingLevel--;
		}

		else if (cword == "thread")
		{
			bNotCommand = true;
			isThread = true;

			if (nCurNestingLevel > 0)
			{
				err_str = "Error! Watek nie moze byc zagniezdzony!";
				return false;
			}

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa watku!";
				return false;
			}

			thread_info.name = cword;

			if (!get_word("{"))
			{
				err_str = "Error! Spodziewane '{'";
				return false;
			}


			if (!Threads.empty())
			{
				for (auto i = 0; i < Threads.size(); i++)
				{
					if (Threads[i].name == cword)
					{
						err_str = "Error! Istnieje juz watek " + cword;
						return false;
					}
				}
			}
		}

		else if (cword == "run")
		{
			if (!get_token())
			{
				err_str = "Error! Wymagana nazwa uruchamianego modulu!";
				return false;
			}

			if (cword == "thread")
			{
				cmd_info.Type = CMD_RUN_THREAD;

				if (!get_token())
				{
					err_str = "Error! Spodziewana nazwa watku!";
					return false;
				}

				cmd_info.s1 = cword;
			}
			else
			{
				err_str = "Error! Brak modulu " + cword = "!";
				return false;
			}
		}

		else if (cword == "if")
		{
			cmd_info.Type = CMD_IF;

			cmd_info.s1.clear();

			while (!get_logic_operator(cmd_info.n2))
			{
				cmd_info.s1 += cword;
			}

			cmd_info.s2.clear();

			while (get_token() && cword != "{")
			{
				cmd_info.s2 += cword;
			}

			cmd_info.nNestingLevel = ++nCurNestingLevel;
		}

		else if (cword == "else")
		{
			if (get_word("if"))
			{
				cmd_info.Type = CMD_ELSEIF;

				cmd_info.s1.clear();

				while (!get_logic_operator(cmd_info.n2))
				{
					cmd_info.s1 += cword;
				}

				cmd_info.s2.clear();

				while (get_token() && cword != "{")
				{
					cmd_info.s2 += cword;
				}

				cmd_info.nNestingLevel = ++nCurNestingLevel;
			}
			else
			{
				cmd_info.Type = CMD_ELSE;

				if (!get_word("{"))
				{
					err_str = "Error! Spodziewane '{'";
					return false;
				}

				cmd_info.nNestingLevel = ++nCurNestingLevel;
			}
		}

		else if (cword == "for")
		{
			cmd_info.Type = CMD_FOR;

			if (!get_token())
			{
				err_str = "Error! Spodziewana nazwa licznika petli!";
				return false;
			}

			cmd_info.s1 = cword;

			if (!get_word("="))
			{
				err_str = "Error, spodziewalem sie znaku \'=\'";
				return false;
			}

			if (!get_token())
			{
				err_str = "Error! Spodziewana wartosc poczatkowa licznika!";
				return false;
			}

			cmd_info.s2 = cword;

			if (!get_word("to"))
			{
				err_str = "Error! Spodziewane slowo 'to'";
				return false;
			}

			if (!get_token())
			{
				err_str = "Error! Spodziewana ostateczna wartosc licznika petli!";
				return false;
			}

			cmd_info.s3 = cword;

			if (!get_token() || cword != "{")
			{
				err_str = "Error! Spodziewane '{'";
				return false;
			}

			cmd_info.nNestingLevel = ++nCurNestingLevel;
		}

		else if (cword == "while" || cword == "until" || cword == "do")
		{
			cmd_info.Type = CMD_WHILE;
			cmd_info.isWhileTrue = false;

			if (cword == "until")
				cmd_info.Type = CMD_UNTIL;

			if (cword == "do")
			{
				if (!get_token() && (cword != "until" || cword != "while"))
				{
					err_str = "Error! 'do' bez instrukcji 'while'!";
					return false;
				}

				if (cword == "while")
					cmd_info.Type = CMD_DOWHILE;
				else
					cmd_info.Type = CMD_DOUNTIL;
			}

			cmd_info.s1.clear();

			if (!get_word("true"))
			{
				while (!get_logic_operator(cmd_info.n2))
				{
					cmd_info.s1 += cword;
				}

				cmd_info.s2.clear();

				while (get_token() && cword != "{")
				{
					cmd_info.s2 += cword;
				}
			}
			else
			{
				cmd_info.isWhileTrue = true;
				cmd_info.n2 = OP_EQUAL;

				if (!get_word("{"))
				{
					err_str = "Error! Spodziewane '{'";
					return false;
				}
			}
				
			
		}

		else if (cword == "loop")
		{
			cmd_info.Type = CMD_LOOP;

			if (!get_token())
			{
				err_str = "Error! Spodziewano sie wartosci ile razy powtorzy sie petla!";
				return false;
			}

			cmd_info.s1 = cword;

			if (!get_word("{"))
			{
				err_str = "Error! Spodziewane '{'";
				return false;
			}
		}

		else if (cword == "return")
		{
			if (!isProc && !isMethod)
			{
				err_str = "Error! Nie mozna stosowac return poza metodami i procedurami!";
				return false;
			}

			if (get_brace())
				cmd_info.s1 = cword;
			else
				cmd_info.s1 = "0";

			cmd_info.Type = CMD_RETURN;
		}

		else if ((isProc || isMethod) && cword == "yield")
			cmd_info.Type = CMD_YIELD;

		//Wywolanie metody/procedury
		else if (cword[cword.length() - 1] == ')')
		{
			auto dot = cword.find_first_of('.');

			if (dot != NOT_FOUND)
			{
				cmd_info.Type = CMD_CALLMETHOD;

				auto leftBracket = cword.find("(");

				if (leftBracket == NOT_FOUND)
				{
					err_str = "Error! Brak '(' przy wywolywaniu procedury " + cword;
					return false;
				}

				cmd_info.s1 = cword.substr(0, dot);
				cmd_info.s2 = cword.substr(dot + 1);

				if (isMethod && cmd_info.s1 != "this")
				{
					err_str = "Error! Nie mozna wywolac metody innego obiektu niz 'this' w klasie!";
					return false;
				}

				for (auto i = cmd_info.s2.length()-1; i != 0; i--)
				{
					if (cmd_info.s2[i] == '(')
					{
						cmd_info.s2.pop_back();
						break;
					}
					cmd_info.s2.pop_back();
				}

				if (get_word("{"))
				{
					isYield = true;

					if (!parse())
					{
						return false;
					}

					cmd_info.yield_info = yield_info;
					yield_info.clear();

					isYield = false;
				}
			}
			else
			{
				cmd_info.Type = CMD_CALLPROC;

				auto leftBracket = cword.find("(");

				if (leftBracket == NOT_FOUND)
				{
					err_str = "Error! Brak '(' przy wywolywaniu procedury " + cword;
					return false;
				}

				cmd_info.s1 = cword.substr(0, leftBracket);


				if (get_word("{"))
				{
					isYield = true;

					if (!parse())
					{
						return false;
					}

					cmd_info.yield_info = yield_info;
					yield_info.clear();

					isYield = false;
				}
			}

		}

		else
		{
			std::string error_string = cword;
			bool notFound{ true };

			cmd_info.s1 = cword;

			if (get_operator())
			{
				cmd_info.s2.clear();
				notFound = false;
				cmd_info.Type = CMD_ASSIGN_VALUE;


				std::string lastChar;
				
				if (!get_brace())
				{
					while (get_token())
					{
						cmd_info.s2 += cword;

						if (!get_math_operator())
						{
							break;
						}

						cmd_info.s2 += cword;

					}
				}
				else
					cmd_info.s2 = cword;
			}

			if (notFound)
			{
				err_str = "Error! Nieznane polecenie " + error_string;
				return false;
			}

		}

	Push:
		if (isYield && !bNotCommand)
			yield_info.push_back(cmd_info);
		else if (isMethod)
			method_info.Body.push_back(cmd_info);
		else if (isProc && !bNotCommand)
			proc_info.Body.push_back(cmd_info);
		else if (isThread && !bNotCommand)
			thread_info.Body.push_back(cmd_info);
		else if (!bNotCommand)
			Commands.push_back(cmd_info);

		bNotCommand = false;
	} while (pos < bufor.length() - 1);

	return true;
}

//Parser matematyczny

char SnakeScript::Parser::look_ahead()
{
	skip_whitespace();
	return input[position];
}

SnakeScript::Parser::Parser(std::string input) : input(input), position(0)
{
	input.push_back(EOS);
}

void SnakeScript::Parser::skip_whitespace()
{
	while (isspace(input[position]))
		position++;
}

SnakeScript::Expression*  SnakeScript::Parser::parse_paren()
{
	position++; // parse_term zapewnia, ¿e wskaŸnik
				// stoi na nawiasie otwieraj¹cym '('
	Expression* e = parse_sum();

	if (look_ahead() == ')')
	{
		position++;
		return e;
	}
	else
	{
		delete e;
		throw Not_parsed();
	}

}

bool SnakeScript::Parser::whileParseVariable(char c)
{
	if (isalnum(c))
		return true;
	if (c == '[')
		return true;
	if (c == ']')
		return true;
	if (c == '@')
		return true;
	if (c == '.')
		return true;
	if (c == '(')
		return true;
	if (c == ')')
		return true;

	return false;
}

SnakeScript::Expression*  SnakeScript::Parser::parse_Variable()
{
	std::string s;
	while (whileParseVariable(input[position]))
	{
		s.push_back(input[position]);
		position++;
	}
	return new Variable(s);
}

SnakeScript::Expression*  SnakeScript::Parser::parse_Constant()
{
	int n = 0;
	while (isdigit(input[position]))
	{
		n *= 10;
		n += input[position] - '0';
		position++;
	}


	return new Constant(n);
}

SnakeScript::Expression*  SnakeScript::Parser::parse_term()
{
	char c = look_ahead();

	if (isdigit(c))
		return parse_Constant();
	else if (isalpha(c) || c == '|')
		return parse_Variable();
	else if (c == '(')
		return parse_paren();
	else
		throw Not_parsed();
}

SnakeScript::Expression*  SnakeScript::Parser::parse_mult()
{
	Expression* e = NULL;

	try
	{
		e = parse_term();
		char c = look_ahead();

		while (c == '*' || c == '/' || c == '%')
		{
			position++;
			e = new Binary_operator(c, e, parse_term());
			c = look_ahead();
		}
	}
	catch (Not_parsed)
	{
		delete e;
		throw Not_parsed();
	}

	return e;
}

SnakeScript::Expression*  SnakeScript::Parser::parse_sum()
{
	Expression* e = NULL;

	try
	{
		e = parse_mult();
		char c = look_ahead();

		while (c == '+' || c == '-')
		{
			position++;
			e = new Binary_operator(c, e, parse_mult());
			c = look_ahead();
		}
	}
	catch (Not_parsed)
	{
		delete e;
		throw Not_parsed();
	}

	return e;
}

SnakeScript::Expression*  SnakeScript::Parser::parse_Expression()
{
	Expression *e = parse_sum();

	if (look_ahead() == EOS)
		return e;
	else
	{
		delete e;
		throw Not_parsed();
	}

}

//Reszta kodu








