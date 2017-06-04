#include "JsonTree.h"

using namespace std;

namespace EasyJson
{
	//���졢���ƹ��졢��������

	//����һ��Ĭ�Ϲ��캯�����������ͨ�������ڴ�ķ�ʽ������һ���յ�Json�ڵ㣬
	//����������ѽڵ�����͸�ΪJson����(T_OBJECT)�����ͣ�Ȼ�������ڵ��ָ�븳ֵ�����ԱROOT��
	JsonTree::JsonTree()
	{
		ROOT = new JsonNode;
		ROOT->setNodeAsRoot();
		isFromText = true;
	}

	//������캯��ͨ������ָ��utf-8��ʽ���ı��ļ��ķ�ʽ,���ɶ�Ӧ��JsonTree��
	//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�С�
	JsonTree::JsonTree(const string &fileName)
	{
		ROOT = new JsonNode;
		ROOT->setNodeAsRoot();
		setJsonFromFile(fileName);
	}

	//������캯��ͨ������ָ�����ַ����ķ�ʽ,���ɶ�Ӧ��JsonTree��
	//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�С�
	JsonTree::JsonTree(const wstring &jsonString)
	{
		ROOT = new JsonNode;
		ROOT->setNodeAsRoot();
		setJsonFromWString(jsonString);
	}

	//������캯��ͨ�������Ѿ��ֶ�����ĸ��ڵ�ķ�ʽ,���ɶ�Ӧ��JsonTree��
	//������rootNode�Ľṹ��Json�﷨�����ɶ�Ӧ��Json���ַ��������浽���ԱJsonReaded�С�
	JsonTree::JsonTree(JsonNode *rootNode)
	{
		delete ROOT;
		ROOT = rootNode;
		jsonReaded = getJsonToWString();
	}

	//������ƹ��캯��ͨ�����¼���Json�ַ����ķ�ʽ�����ƹ���һ���µ�JsonTree�������Ķ������ܡ�
	JsonTree::JsonTree(JsonTree &ths)
	{
		//�����ֵ���Ѿ����ڵ�json����������������ڴ棬��ô����Ҫ�ͷ�����,ֱ��ֻʣ��ths�Ŀյĸ��ڵ㡣
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

	//����JsonTree������������
	//����������JsonTree�Ǵ��ı�������ʱ��
	//��ͨ������DeleteJsonTree������
	//�ݹ���ͷ�JsonTree����������ڴ档
	//����������JsonTree�����û������ʱ��
	//��ʲôҲ������
	JsonTree::~JsonTree()
	{
		if (isFromText)
		{
			deleteJsonTree(ROOT);
		}
	}

	//�����Ժ���

	//�����������ָ��utf-8��ʽ���ı��ļ�,���ɶ�Ӧ��JsonTree��
	//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�У�����ֵΪ������롣
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

	//�����������ָ�����ַ���,���ɶ�Ӧ��JsonTree��
	//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�У�����ֵΪ������롣
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

		//���ڵĸ��ڵ�
		JsonNode *nowRoot = ROOT;
		//��ǰ״̬
		int status = 1;

		//�������ڹ����Json�ڵ�
		JsonNode thisOne;

		while (true)
		{
			switch (status)
			{
				//0������һ���յ�Json�ڵ�����ʼ��"ThisOne"��Ȼ��ѵ�ǰ״̬ת��Ϊ�ȴ��ڵ�����¼��״̬��
			case 0:
			{
				JsonNode temp;
				thisOne = temp;

				status = 1;
			}
			break;

			//1��׼���ý���һ���µĽڵ����֣�����" �ڵ����� ":
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

			//2�������ж�״̬������" n t f - 0~9 { [
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

			//3��¼����ֵ
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

			//4��������ֵ¼�룬�ڴ����ı���L'}' ���� L']',Ȼ��������������ж�Json�ı�¼���Ƿ����
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

	//���ڴ��в���õ�JsonTree����ģ��ת��ΪJson��ʽ�Ŀ��ַ�����
	//��������õ�Json��ʽ�Ŀ��ַ�����ֵ�����Ա�е�JsonReaded, ͬʱ��������õ��ַ�����
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

	//���ڴ��в���õ�JsonTree����ģ��ת��ΪJson��ʽ�Ŀ��ַ�����
	//�����ַ���תΪutf-8���룬�����ڲ���ָ�����ı���,
	//���������������õ�Json��ʽ�Ŀ��ַ�����ֵ�����Ա�е�JsonReaded,����ֵΪ������롣
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

	//����һ�����ع�����еȷ��ţ������ж�����JsonTree�Ƿ���ȡ�
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

	//ɾ��JsonNָ��Ľڵ��Լ��������ӽڵ㣨ͨ���ݹ�ķ�ʽ�������ر�ʾɾ���Ƿ�ɹ��Ĳ���ֵ��
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

	//�����нڵ�����Ϊ�սڵ�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(void *ptrNULL, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		if (ptrNULL==nullptr)
		{
			DFSJsonFindNode(ROOT, resultList, T_NULL, nameT, false, 0.0, L"");
		}
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_BOOL_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(bool D_BOOL_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_BOOL, nameT, D_BOOL_T, 0.0, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_INT_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(int D_INT_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_INT_T, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_UINT_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(unsigned D_UINT_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_UINT_T, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_LONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
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

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_LONG_LONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(long long D_LONG_LONG_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_LONG_LONG_T, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_ULONG_LONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(unsigned long long D_ULONG_LONG_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_ULONG_LONG_T, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_DOUBLE_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(double D_DOUBLE_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_DOUBLE_T, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ�������͵ģ�ֵΪD_LONG_DOUBLE_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(long double D_LONG_DOUBLE_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_NUMBER, nameT, false, D_LONG_DOUBLE_T, L"");
		return resultList;
	}

	//�����нڵ�����Ϊ���ַ������͵ģ�ֵΪD_STRING_T�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(const wchar_t *D_STRING_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_STRING, nameT, false, 0.0, D_STRING_T);
		return resultList;
	}

	//�����нڵ�����Ϊ���ַ������͵ģ�ֵΪD_STRING_T�ģ����ع������ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNode(const wstring &D_STRING_T, const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, T_STRING, nameT, false, 0.0, D_STRING_T);
		return resultList;
	}

	//�����нڵ�����Ϊ����/�ṹ�ģ��ڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
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

	//�����нڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��ء�
	list<JsonNode*> JsonTree::findNodeWithName(const wchar_t *nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, nameT);
		return resultList;
	}

	//�����нڵ���Ϊname������ʡ�����֣���
	//�ڵ�ָ���ռ��������з��أ����أ���
	list<JsonNode*> JsonTree::findNodeWithName(const wstring &nameT)
	{
		list<JsonNode*> resultList;
		DFSJsonFindNode(ROOT, resultList, nameT);
		return resultList;
	}	

	//Private����

	//�ݹ�����new������Node�ڵ㼰�����������ڵ�
	//JsonTree��������������������������еݹ�ɾ���Ĳ�����
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

	//ͨ��DFS�ı�����ʽ����Json���ַ����ı�
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

	//ͨ��DFS�ı�����ʽѰ�ҷ���������Json�ڵ㣬�������Ǽ���ls�С�
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

	//Ԥ�ڵĴ�����ı��� �հף���ѡ�ģ�-ָ���������ַ�-�հף���ѡ����
	//��������󣬵���������ָ�����һ����ѡ�հ׵ĺ���ĵ�һ���ǿհ��ַ���
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

	//Ԥ�ڵĴ�����ı��� �հף���ѡ�ģ�- ˫���� - �ַ��� - ˫���� - �հף���ѡ�ģ���
	//��������󣬵���������ָ���ַ������桢
	//���һ����ѡ�հ׵ĺ���ĵ�һ���ǿհ��ַ���
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

	//Ԥ�ڴ���һ����L'\"'��β���ַ�����
	//��������󣬵���������ָ���ַ������һ���ַ���
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

	//�ж�wchar_t�Ƿ�Ϊ�հס�
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

	//�ж�wchar_t�Ƿ�Ϊ0~9�����֡�
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

	//ͨ��DFS�ı�����ʽ��Json�������нڵ������ڴ��С�
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
	
	//ˢ��json�ַ����С�
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