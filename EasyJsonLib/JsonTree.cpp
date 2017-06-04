#include "JsonTree.h"

using namespace std;

namespace EasyJson
{
	//构造、复制构造、析构函数

	//这是一个默认构造函数。这个函数通过申请内存的方式创建了一个空的Json节点，
	//这个函数还把节点的类型改为Json对象(T_OBJECT)的类型，然后把这个节点的指针赋值给类成员ROOT。
	JsonTree::JsonTree()
	{
		ROOT = new JsonNode;
		ROOT->setNodeAsRoot();
		isFromText = true;
	}

	//这个构造函数通过加载指定utf-8格式的文本文件的方式,生成对应的JsonTree，
	//并把读取出的Json宽字符串保存到类成员JsonReaded中。
	JsonTree::JsonTree(const string &fileName)
	{
		ROOT = new JsonNode;
		ROOT->setNodeAsRoot();
		setJsonFromFile(fileName);
	}

	//这个构造函数通过加载指定宽字符串的方式,生成对应的JsonTree，
	//并把读取出的Json宽字符串保存到类成员JsonReaded中。
	JsonTree::JsonTree(const wstring &jsonString)
	{
		ROOT = new JsonNode;
		ROOT->setNodeAsRoot();
		setJsonFromWString(jsonString);
	}

	//这个构造函数通过加载已经手动构造的根节点的方式,生成对应的JsonTree，
	//并根据rootNode的结构和Json语法，生成对应的Json宽字符串，保存到类成员JsonReaded中。
	JsonTree::JsonTree(JsonNode *rootNode)
	{
		delete ROOT;
		ROOT = rootNode;
		jsonReaded = getJsonToWString();
	}

	//这个复制构造函数通过重新加载Json字符串的方式，复制构造一个新的JsonTree，会消耗额外性能。
	JsonTree::JsonTree(JsonTree &ths)
	{
		//如果左值是已经存在的json树，并且有申请的内存，那么首先要释放它们,直到只剩下ths的空的根节点。
		if (isFromText)
		{
			for (auto &v : ROOT->Sons)
			{
				deleteJsonTree(v);
			}
			ROOT->setNodeAsRoot();
		}

		setJsonFromWString(ths.jsonReaded);
	}

	//这是JsonTree的析构函数，
	//当被析构的JsonTree是从文本得来的时候，
	//它通过调用DeleteJsonTree函数，
	//递归地释放JsonTree申请的所有内存。
	//当被析构的JsonTree是由用户构造的时候，
	//它什么也不做。
	JsonTree::~JsonTree()
	{
		if (isFromText)
		{
			deleteJsonTree(ROOT);
		}
	}

	//功能性函数

	//这个函数加载指定utf-8格式的文本文件,生成对应的JsonTree，
	//并把读取出的Json宽字符串保存到类成员JsonReaded中，返回值为错误代码。
	int JsonTree::setJsonFromFile(const string &fileName)
	{
		int returnNum = 0;

		fstream fs(fileName);
		if (!fs.is_open())
		{
			return FileCanNotOpen;
		}
		else
		{
			string jsonString;

			string temp;
			while (fs >> temp)
			{
				jsonString += temp;
			}
			fs.close();

			wstring_convert<codecvt_utf8<wchar_t>> u8_wchar_cv;
			wstring fileStr = u8_wchar_cv.from_bytes(jsonString);

			returnNum = setJsonFromWString(fileStr);
		}
		return returnNum;
	}

	//这个函数加载指定宽字符串,生成对应的JsonTree，
	//并把读取出的Json宽字符串保存到类成员JsonReaded中，返回值为错误代码。
	int JsonTree::setJsonFromWString(const wstring &jsonString)
	{
		isFromText = true;

		jsonReaded = jsonString;

		auto iter = jsonString.begin();
		auto End = jsonString.end();

		if (!haveWChar(iter, End, L'{'))
		{
			return JsonWrongSyntax;
		}

		levelStack.push(L'{');

		//现在的父节点
		JsonNode *nowRoot = ROOT;
		//当前状态
		int status = 1;

		//现在正在构造的Json节点
		JsonNode thisOne;

		while (true)
		{
			switch (status)
			{
				//0：创建一个空的Json节点来初始化"ThisOne"，然后把当前状态转换为等待节点名字录入状态。
			case 0:
			{
				JsonNode temp;
				thisOne = temp;

				status = 1;
			}
			break;

			//1：准备好接受一个新的节点名字，比如" 节点名称 ":
			case 1:
			{
				if (!getWStringWithQuotes(iter, End, thisOne.name))
				{
					return JsonWrongNodeName;
				}

				if (!haveWChar(iter, End, L':'))
				{
					return JsonWrongNodeName;
				}

				status = 2;
			}
			break;

			//2：类型判断状态，比如" n t f - 0~9 { [
			case 2:
			{
				while (isSpace(*iter))
				{
					++iter;
					if (iter == End)
					{
						return JsonWrongType;
					}
				}

				thisOne.Father = nowRoot;

				switch (*iter)
				{
				case L'n':
				{
					wstring pd(iter, iter + 4);
					if (pd != L"null")
					{
						return JsonWrongType;
					}

					thisOne.type = T_NULL;

					JsonNode* Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot->Sons.push_back(Temp);

					status = 4;
					iter += 4;
				}
				break;

				case L't':
				{
					wstring pd(iter, iter + 4);
					if (pd != L"true")
					{
						return JsonWrongType;
					}

					thisOne.type = T_BOOL;
					thisOne.D_BOOL = true;

					JsonNode* Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot->Sons.push_back(Temp);

					status = 4;
					iter += 4;
				}
				break;

				case L'f':
				{
					wstring pd(iter, iter + 5);
					if (pd != L"false")
					{
						return JsonWrongType;
					}

					thisOne.type = T_BOOL;
					thisOne.D_BOOL = false;

					JsonNode* Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot->Sons.push_back(Temp);

					status = 4;
					iter += 5;
				}
				break;

				case L'\"':
				{
					if (!getWStringWithQuotes(iter, End, thisOne.D_STRING))
					{
						return JsonWrongSyntax;
					}
					thisOne.type = T_STRING;

					JsonNode* Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot->Sons.push_back(Temp);

					status = 4;
				}
				break;

				case L'[':
				{
					thisOne.type = T_VECTOR;
					JsonNode* Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot = Temp;

					nowRoot->Father->Sons.push_back(Temp);

					JsonNode newi;
					thisOne = newi;

					levelStack.push(L'[');

					++iter;
					if (iter == End)
					{
						return JsonWrongSyntax;
					}

					status = 2;
				}
				break;

				case L'{':
				{
					levelStack.push(L'{');
					thisOne.type = T_OBJECT;
					JsonNode *Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot = Temp;

					nowRoot->Father->Sons.push_back(Temp);

					status = 0;

					++iter;
					if (iter == End)
					{
						return JsonWrongSyntax;
					}
				}
				break;

				default:
				{
					if (isDigit(*iter) || *iter == L'-')
					{
						thisOne.type = T_NUMBER;
						status = 3;
					}
					else
					{
						return JsonWrongType;
					}
				}
				break;
				}
			}
			break;

			//3：录入数值
			case 3:
			{
				auto tempIter = iter;
				while (!(isSpace(*iter) || *iter == L'}' || *iter == L',' || *iter == L']'))
				{
					++iter;
					if (iter == End)
					{
						return JsonWrongSyntax;
					}
				}

				wstring numberString(tempIter, iter);
				wstringstream ss;
				ss << numberString;
				if (ss >> thisOne.D_NUMBER)
				{
					ss.clear();

					JsonNode* Temp = new JsonNode;
					*Temp = thisOne;
					nowRoot->Sons.push_back(Temp);
				}
				else
				{
					return JsonWrongType;
				}

				status = 4;
			}
			break;

			//4：结束数值录入，期待的文本如L'}' 或者 L']',然后这个函数将会判断Json文本录入是否结束
			case 4:
			{
				while (isSpace(*iter))
				{
					++iter;
					if (iter == End)
					{
						return JsonWrongSyntax;
					}
				}

				if (*iter == L'}')
				{
					if (levelStack.top() != L'{')
					{
						return JsonWrongSyntax;
					}
					levelStack.pop();

					nowRoot = nowRoot->Father;
					if (levelStack.size() == 0)
					{
						++iter;
						if (iter == End)
						{
							return 0;
						}
						else
						{
							bool flag = false;
							while (true)
							{
								if (iter == End)
								{
									return 0;
								}

								if (isSpace(*iter))
								{
									++iter;
								}
								else
								{
									return JsonWrongSyntax;
								}
							}
						}
					}
					else
					{
						++iter;
						if (iter == End)
						{
							return JsonWrongSyntax;
						}
						status = 4;
					}
				}
				else if (*iter == L']')
				{
					if (levelStack.top() != L'[')
					{
						return JsonWrongSyntax;
					}
					levelStack.pop();

					++iter;
					if (iter == End)
					{
						return JsonWrongSyntax;
					}

					nowRoot = nowRoot->Father;
					status = 4;
				}
				else if (*iter == L',')
				{
					++iter;
					if (iter == End)
					{
						return JsonWrongSyntax;
					}

					if (levelStack.top() == L'{')
					{
						status = 0;
					}
					else
					{
						status = 2;
					}
				}
			}
			break;
			}
		}
		return 0;
	}

	//把内存中部署好的JsonTree对象模型转化为Json格式的宽字符串，
	//并把整理好的Json格式的宽字符串赋值给类成员中的JsonReaded, 同时返回整理好的字符串。
	wstring JsonTree::getJsonToWString()
	{
		if (jsonReaded.empty())
		{
			jsonReaded += L"{\n";
			for (auto &v : ROOT->Sons)
			{
				DFSMakeJsonText(v, jsonReaded);
			}
			jsonReaded.pop_back();
			jsonReaded.pop_back();
			jsonReaded += L"\n}";
		}
		return jsonReaded;
	}

	//把内存中部署好的JsonTree对象模型转化为Json格式的宽字符串，
	//并把字符串转为utf-8编码，保存在参数指定的文本中,
	//这个函数还把整理好的Json格式的宽字符串赋值给类成员中的JsonReaded,返回值为错误代码。
	int JsonTree::getJsonToFile(const string &fileName)
	{
		ofstream fout(fileName);

		if (fout.is_open())
		{
			wstring_convert<codecvt_utf8<wchar_t>> u8_wchar_cv;
			string JsonUTF8StringToFile = u8_wchar_cv.to_bytes(jsonReaded);

			fout << JsonUTF8StringToFile;
			fout.close();
		}
		else
		{
			return FileCanNotCreate;
		}
		return 0;
	}

	//这是一个重载过后的判等符号，用来判断两个JsonTree是否相等。
	bool JsonTree::operator==(JsonTree &that)
	{
		list<wstring> thisOne, thatOne;

		DFSWStringCollector(ROOT, thisOne);
		DFSWStringCollector(that.ROOT, thatOne);

		thisOne.sort(); thatOne.sort();

		if (thisOne.size() != thatOne.size())
		{
			return false;
		}

		bool AllSame = true;
		auto thisIter = thisOne.begin(), thatIter = thatOne.begin();
		while (thisIter != thisOne.end())
		{
			if (*thisIter != *thatIter)
			{
				AllSame = false;
				break;
			}
			++thisIter; ++thatIter;
		}
		return AllSame;
	}

	//删除JsonN指向的节点以及其所有子节点（通过递归的方式），返回表示删除是否成功的布尔值。
	bool JsonTree::deleteNode(JsonNode *JsonN)
	{
		if (JsonN == nullptr)
		{
			return false;
		}
		else
		{
			for (auto iter = JsonN->Father->Sons.begin(); iter != JsonN->Father->Sons.end(); ++iter)
			{
				if (*iter == JsonN)
				{
					JsonN->Father->Sons.erase(iter);
					break;
				}
			}

			if (isFromText)
			{
				deleteJsonTree(JsonN);
			}
			refreshJsonString();

			return true;
		}
	}

	//把所有节点类型为空节点的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(void *ptrNULL, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		if (ptrNULL==nullptr)
		{
			DFSJsonFindNode(ROOT, resultList, T_NULL, nameT, false, 0.0, L"");
		}
		return resultList;
	}

	//把所有节点类型为布尔类型的，值为D_BOOL_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(bool D_BOOL_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_BOOL, nameT, D_BOOL_T, 0.0, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_INT_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(int D_INT_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_INT_T, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_UINT_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(unsigned D_UINT_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_UINT_T, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_LONG_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(long D_LONG_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_LONG_T, L"");
		return resultList;
	}

	list<JsonNode*> JsonTree::findNode(unsigned long D_ULONG_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_ULONG_T, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_LONG_LONG_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(long long D_LONG_LONG_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_LONG_LONG_T, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_ULONG_LONG_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(unsigned long long D_ULONG_LONG_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_ULONG_LONG_T, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_DOUBLE_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(double D_DOUBLE_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_DOUBLE_T, L"");
		return resultList;
	}

	//把所有节点类型为数字类型的，值为D_LONG_DOUBLE_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(long double D_LONG_DOUBLE_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_LONG_DOUBLE_T, L"");
		return resultList;
	}

	//把所有节点类型为宽字符串类型的，值为D_STRING_T的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(const wchar_t *D_STRING_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_STRING, nameT, false, 0.0, D_STRING_T);
		return resultList;
	}

	//把所有节点类型为宽字符串类型的，值为D_STRING_T的（重载过），节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(const wstring &D_STRING_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_STRING, nameT, false, 0.0, D_STRING_T);
		return resultList;
	}

	//把所有节点类型为数组/结构的，节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNode(char obj, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		if (obj == '{' || obj == '}')
		{
			DFSJsonFindNode(ROOT, resultList, T_OBJECT, nameT, false, 0.0, L"");
		}
		else if (obj == '[' || obj == ']')
		{
			DFSJsonFindNode(ROOT, resultList, T_VECTOR, nameT, false, 0.0, L"");
		}
		return resultList;
	}

	//把所有节点名为name（可以省略名字）的
	//节点指针收集到链表中返回。
	list<JsonNode*> JsonTree::findNodeWithName(const wchar_t *nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, nameT);
		return resultList;
	}

	//把所有节点名为name（可以省略名字）的
	//节点指针收集到链表中返回（重载）。
	list<JsonNode*> JsonTree::findNodeWithName(const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, nameT);
		return resultList;
	}	

	//Private函数

	//递归销毁new出来的Node节点及其所有下属节点
	//JsonTree的析构函数调用了这个函数进行递归删除的操作。
	void JsonTree::deleteJsonTree(JsonNode *node)
	{
		if (node->Sons.size() == 0)
		{
			delete node;
		}
		else
		{
			for (auto &v : node->Sons)
			{
				deleteJsonTree(v);
			}
			delete node;
		}
	}

	//通过DFS的遍历方式生产Json宽字符串文本
	void JsonTree::DFSMakeJsonText(JsonNode *node, wstring &ss)
	{
		static int level = 0;
		for (int i = 0; i<(level + 5); ++i)
		{
			ss.push_back(L' ');
		}

		if (node->Father->type != T_VECTOR)
		{
			ss += L"\"";
			ss += node->name;
			ss += L"\":";
		}

		if (node->Sons.size() == 0)
		{
			switch (node->type)
			{
			case T_BOOL:
				if (node->D_BOOL)
				{
					ss += L"true,\n";
				}
				else
				{
					ss += L"false,\n";
				}
				break;

			case T_NULL:
				ss += L"null,\n";
				break;

			case T_NUMBER:
			{
				wstringstream sa;
				wstring temp;
				sa << node->D_NUMBER;
				sa >> temp;
				sa.clear();
				ss += temp;
				ss += L",\n";
			}
			break;

			case T_STRING:
				ss += L"\"";
				ss += node->D_STRING;
				ss += L"\",\n";
				break;
			}
		}
		else
		{
			if (node->type == T_VECTOR)
			{
				level += 5;
				ss += L"[\n";
			}
			else if (node->type == T_OBJECT)
			{
				level += 5;
				ss += L"{\n";
			}

			for (auto &v : node->Sons)
			{
				DFSMakeJsonText(v, ss);
			}

			if (node->type == T_VECTOR)
			{
				ss.pop_back();
				ss.pop_back();
				ss.push_back(L'\n');

				for (int i = 0; i<(level + 5); ++i)
				{
					ss.push_back(L' ');
				}
				ss += L"],\n";
				level -= 5;
			}
			else if (node->type == T_OBJECT)
			{
				ss.pop_back();
				ss.pop_back();
				ss.push_back(L'\n');

				for (int i = 0; i<(level + 5); ++i)
				{
					ss.push_back(L' ');
				}
				ss += L"},\n";
				level -= 5;
			}
		}
	}

	//通过DFS的遍历方式寻找符合特征的Json节点，并把它们加在ls中。
	void JsonTree::DFSJsonFindNode(JsonNode *node, list<JsonNode*> &ls, const DataType dt, const wstring &nameT, const bool boolT, const long double numberT, const wstring &stringT)
	{
		switch (node->type)
		{
		case T_NULL:
			if (dt == T_NULL)
			{
				if (nameT.empty() || node->name == nameT)
				{
					ls.push_back(node);
				}
			}
		
		case T_BOOL:
			if (dt == T_BOOL && node->D_BOOL == boolT)
			{
				if (nameT.empty() || node->name == nameT)
				{
					ls.push_back(node);
				}
			}
			break;

		case T_NUMBER:
			if (dt == T_NUMBER && node->D_NUMBER == numberT)
			{
				if (nameT.empty() || node->name == nameT)
				{
					ls.push_back(node);
				}
			}
			break;

		case T_STRING:
			if (dt == T_STRING && node->D_STRING == stringT)
			{
				if (nameT.empty() || node->name == nameT)
				{
					ls.push_back(node);
				}
			}
			break;

		case T_VECTOR:
			if (dt == T_VECTOR)
			{
				if (nameT.empty() || node->name == nameT)
				{
					ls.push_back(node);
				}

				if (nameT.empty())
				{
					for (auto s:node->Sons)
					{
						DFSJsonFindNode(s,ls,dt,nameT,boolT,numberT,stringT);
					}
				}
			}
			else
			{
				for (auto s : node->Sons)
				{
					DFSJsonFindNode(s, ls, dt, nameT, boolT, numberT, stringT);
				}
			}
			break;

		case T_OBJECT:
			if (dt == T_OBJECT)
			{
				if (nameT.empty() || node->name == nameT)
				{
					ls.push_back(node);
				}
			}
			for (auto s : node->Sons)
			{
				DFSJsonFindNode(s, ls, dt, nameT, boolT, numberT, stringT);
			}
			break;
		}
	}

	void JsonTree::DFSJsonFindNode(JsonNode *node, std::list<JsonNode*> &ls, const wstring &nameT)
	{
		if (node->name==nameT)
		{
			ls.push_back(node);
		}

		if (node->type==T_OBJECT)
		{
			for (auto s:node->Sons)
			{
				DFSJsonFindNode(s,ls,nameT);
			}
		}
	}

	//预期的处理的文本如 空白（可选的）-指定的特殊字符-空白（可选），
	//函数的最后，迭代器将会指向最后一个可选空白的后面的第一个非空白字符。
	bool JsonTree::haveWChar(wstring::const_iterator &iter, wstring::const_iterator &end, wchar_t character)
	{
		while (isSpace(*iter))
		{
			++iter;
			if (iter == end)
			{
				return false;
			}
		}

		if (*iter != character)
		{
			return false;
		}

		++iter;
		if (iter == end)
		{
			return false;
		}

		while (isSpace(*iter))
		{
			++iter;
			if (iter == end)
			{
				return false;
			}
		}

		return true;
	}

	//预期的处理的文本如 空白（可选的）- 双引号 - 字符串 - 双引号 - 空白（可选的），
	//函数的最后，迭代器将会指向字符串后面、
	//最后一个可选空白的后面的第一个非空白字符。
	bool JsonTree::getWStringWithQuotes(wstring::const_iterator &iter, wstring::const_iterator &end, wstring &theStr)
	{
		if (!haveWChar(iter, end, L'\"'))
		{
			return false;
		}

		if (!getWString(iter, end, theStr))
		{
			return false;
		}

		if (!haveWChar(iter, end, L'\"'))
		{
			return false;
		}

		return true;
	}

	//预期处理一个以L'\"'结尾的字符串，
	//函数的最后，迭代器将会指向字符串后第一个字符。
	bool JsonTree::getWString(wstring::const_iterator &iter, wstring::const_iterator &end, wstring &theString)
	{
		auto BeginIter = iter;
		while (*iter != L'\"' || (*iter == L'\"'&&*(iter - 1) == L'\\'))
		{
			++iter;
			if (iter == end)
			{
				return false;
			}
		}

		wstring STR(BeginIter, iter);
		theString = STR;

		return true;
	}

	//判断wchar_t是否为空白。
	bool JsonTree::isSpace(const wchar_t &ch)
	{
		if (ch == L'\n' || ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\v' || ch == L'\f')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//判断wchar_t是否为0~9的数字。
	bool JsonTree::isDigit(const wchar_t &ch)
	{
		if (ch >= L'0'&& ch <= L'9')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//通过DFS的遍历方式把Json树的所有节点整理到内存中。
	void JsonTree::DFSWStringCollector(JsonNode *node,list<wstring> &SS)
	{
		static list<wstring>prefix(1, L"ROOT");

		if (node->Sons.size() == 0)
		{
			wstring temp;
			for (auto &v : prefix)
			{
				temp += v;
			}
			temp += L"\"";

			temp += node->name;

			switch (node->type)
			{
			case T_BOOL:
				if (node->D_BOOL)
				{
					temp += L"true";
				}
				else
				{
					temp += L"false";
				}
				break;

			case T_NULL:
				temp += L"null";
				break;

			case T_NUMBER:
			{
				wstringstream sa;
				wstring t;
				sa << node->D_NUMBER;
				sa >> t;
				sa.clear();
				temp += t;
			}
			break;

			case T_STRING:
				temp += node->D_STRING;
				break;
			}

			SS.push_back(temp);
		}
		else
		{
			prefix.push_back(node->name);

			for (auto &v : node->Sons)
			{
				DFSWStringCollector(v,SS);
			}

			prefix.pop_back();
		}
	}
	
	//刷新json字符序列。
	void JsonTree::refreshJsonString()
	{
		jsonReaded.clear();
		jsonReaded += L"{\n";
		for (auto &v : ROOT->Sons)
		{
			DFSMakeJsonText(v, jsonReaded);
		}
		jsonReaded.pop_back();
		jsonReaded.pop_back();
		jsonReaded += L"\n}";
	}
}