#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <map>
using namespace std;

//строки анализируемой программы
deque<string> expLines;
//операнды
map<string, int> expOperands;
//операторы
map<string, int> expOperators;

//инициализация массивов
void init()
{	
	expLines = deque<string>();
	expOperands = map<string, int>();
	expOperators = map<string, int>();
}

//тип переменной
bool isNativeDataType(string word)
{
	return word == "int" || word == "char" || word == "double" || word == "float";
}

//форматируем строку
string formateExpLine(string expLine)
{
	// Удаление пробелов с начала и с конца. 
	while (expLine.front() == ' ')
	{
		if (expLine == " ")
		{
			break;
		}
		else
		{
			expLine.erase(expLine.begin());
		}
	}
	while (expLine.back() == ' ')
	{
		if (expLine == " ")
		{
			break;
		}
		else
		{
			expLine.erase(expLine.end() - 1);
		}
	}
	if (expLine == " ")
	{
		return "";
	}

	// Удаление двойных пробелов.
	for (int i = 0; i < expLine.length() - 1; ++i)
	{
		if (expLine[i] == ' ' && expLine[i + 1] == ' ')
		{
			expLine.erase(i--, 1);
		}
	}
	// Удаление знака "точка с запятой".
	if (expLine.back() == ';')
	{
		expLine.erase(expLine.end() - 1);
	}
	// Удаление знаков табуляции.
	while (expLine.front() == '\t')
	{
		if (expLine == "\t")
		{
			break;
		}
		else
		{
			expLine.erase(expLine.begin());
		}
	}
	if (expLine == "\t")
	{
		return "";
	}

	return expLine;
}

//получем код анализируемой программы
void readInput()
{
	ifstream expInput("Sourse.txt");

	if (!expInput.is_open())
	{
		cout << "Ошибка при открытии файла." << endl;
		exit(1);
	}

	string expLine;
	while (getline(expInput, expLine))
	{
		expLine = formateExpLine(expLine);

		if (expLine == "")
		{
			continue;
		}
		else
		{
			expLines.push_back(expLine);
		}
	}

	expInput.close();
}

//разбиваем строку на слова
vector<string> splitExpLine(string expLine)
{
	vector<string> expWords(0);
	string expWord;

	for (char expSymbole : expLine)
	{
		if (expSymbole == ' ')
		{
			expWords.push_back(expWord);
			expWord.clear();
		}
		else
		{
			expWord = expWord + expSymbole;
		}
	}
	expWords.push_back(expWord); // Последнее слово.

	return expWords;
}

//простой оператор
bool isSimpleOperator(string expOperator)
{
	return expOperator == "=" || expOperator == "+" ||
		expOperator == "-" || expOperator == "*" ||
		expOperator == "/" || expOperator == "%";
}

//сложный оператор
bool isComplexOperator(string expOperator)
{
	if (expOperator == "sin(temp)")
	{
		expOperands.at("temp")++;
		return true;
	}
	else if (expOperator == "pow(temp,3)")
	{
		expOperands.at("temp")++;
		return true;
	}
	else if (expOperator == "cos(y)")
	{
		expOperands.at("y")++;
		return true;
	}
	return false;
}

//разбиваем строку на операторы и операнды
void analyzeExp(const vector<string> &expWords)
{
	for (const string expWord : expWords)
	{
		if (isSimpleOperator(expWord))
		{
			if (expOperators.find(expWord) == expOperators.end())
			{
				expOperators.insert(make_pair(expWord, 0));
			}
			expOperators.at(expWord)++;
		}
		else if (isComplexOperator(expWord))
		{
			string complexOperator = expWord.substr(0, 3);

			if (expOperators.find(complexOperator) == expOperators.end())
			{
				expOperators.insert(make_pair(complexOperator, 0));
			}
			expOperators.at(complexOperator)++;
		}
		else
		{
			if (expOperands.find(expWord) == expOperands.end())
			{
				expOperands.insert(make_pair(expWord, 0));
			}
			expOperands.at(expWord)++;
		}
	}
}

//анализируем программу
void analуze()
{
	for (string expLine : expLines)
	{
		//строка разбитая на слова
		vector<string> expWords = splitExpLine(expLine);

		//начало программы, ничего не делаем
		if (expWords.size() == 2 && expWords[0] == "int" && expWords[1] == "main()")
		{
			continue;
		}
		else if (expWords.size() == 1 && (expWords[0] == "{" || expWords[0] == "}"))
		{
			continue;
		}

		//добавляем в массив операндов объявленные переменные
		else if (expWords.size() == 2 && isNativeDataType(expWords[0]))
		{
			expOperands.insert(make_pair(expWords[1], 0));
		}

		//анализируем строки
		else if (expWords.size() > 2 && expWords[1] == "=")
		{
			analyzeExp(expWords);
		}

		//если это вывод или конец программы ничего не делаем
		else if (expWords[0] == "cout")
		{
			continue;
		}
		else if (expWords[0] == "return")
		{
			continue;
		}
	}
}

//Метрики
int uniqueOperands;
int totalOperands;
int uniqueOperators;
int totalOperators;
int progVocabulary;
int implementLength;
double programValue;

//вычисляем метрики
void countMetrics()
{
	uniqueOperands = expOperands.size();


	totalOperands = 0;
	for (pair<string, int> operand : expOperands)
	{
		totalOperands += operand.second;
	}

	uniqueOperators = expOperators.size();

	totalOperators = 0;
	for (pair<string, int> expOperator : expOperators)
	{
		totalOperators += expOperator.second;
	}

	progVocabulary = uniqueOperands + uniqueOperators;
	implementLength = totalOperands + totalOperators;
	programValue = implementLength * log2(progVocabulary);
}

//вывод результатов
void writeOutput()
{
	setlocale(LC_ALL, "Russian");

	cout << "============================" << endl;
	cout << "|  " << "Информация об операндах" << " |" << endl;
	cout << "----------------------------" << endl;
	cout << "| ";
	cout << setw(4) << "Имя";
	cout << setw(15) << "Количество";
	cout << setw(7) << "|" << endl;
	for (pair<string, int> operand : expOperands)
	{
		cout << "|";
		cout << setw(5) << operand.first;
		cout << setw(15) << operand.second;
		cout << setw(7) << "|" << endl;
	}
	cout << "============================" << endl;



	cout << "==========================" << endl;
	cout << "|" << "Информация об операторах" << "|" << endl;
	cout << "--------------------------" << endl;
	cout << "|";
	cout << setw(4) << "Имя";
	cout << setw(15) << "Количество";
	cout << setw(6) << "|" << endl;
	for (pair<string, int> expOperator : expOperators)
	{
		cout << "|";
		cout << setw(4) << expOperator.first;
		cout << setw(15) << expOperator.second;
		cout << setw(6) << "|" << endl;
	}
	cout << "==========================" << endl;
	cout << endl;

	cout << "=======================================" << endl;
	cout << "|        " << "Информация о метриках" << "        |" << endl;
	cout << "---------------------------------------" << endl;
	cout << "| " << "Количество уникальных операндов: " << uniqueOperands << "  |" << endl;
	cout << "| " << "Количество уникальных операторов: " <<
		uniqueOperators << " |" << endl;
	cout << "| " << "Всего операндов: " <<
		totalOperands << "                 |" << endl;
	cout << "| " << "Всего операторов: " <<
		totalOperators << "                |" << endl;
	cout << "| " << "Словарь программы: " <<
		progVocabulary << "               |" << endl;
	cout << "| " << "Длина реализации: " <<
		implementLength << "                |" << endl;
	cout << "| " << "Объем программы: " <<
		programValue << "                |" << endl;
	cout << "=======================================";
	cout << endl;

	cout << "Готово!" << endl;
}

int main()
{
	init();

	readInput();
	analуze();
	countMetrics();
	writeOutput();

	system("pause");
	return 0;
}
