#include "JsonNode.h"

using namespace std;

namespace EasyJson
{
	//空类型节点构造函数，两个参数分别是宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(const wstring &nameT, int nNULL, JsonNode *fatherT)
	{
		if (!nNULL)
		{
			type = T_NULL;
			name = nameT;
			setFatherNode(fatherT);
		}
	}

	//空类型节点构造函数（重载），两个参数分别是宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT)
	{
		if (!nNULL)
		{
			type = T_NULL;
			name = nameT;
			setFatherNode(fatherT);
		}
	}

	//布尔类型节点构造函数，三个参数分别是节点布尔值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(bool D_BOOL_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_BOOL;
		name = nameT;
		D_BOOL = D_BOOL_T;
		setFatherNode(fatherT);
	}

	//数字类型节点构造函数（整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(int D_INT_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_INT_T;
		setFatherNode(fatherT);
	}
	
	//数字类型节点构造函数（无符号整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(unsigned D_UINT_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_UINT_T;
		setFatherNode(fatherT);
	}

	//数字类型节点构造函数（长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(long D_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_LONG_T;
		setFatherNode(fatherT);
	}
	
	//数字类型节点构造函数（无符号长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(unsigned long D_ULONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_ULONG_T;
		setFatherNode(fatherT);
	}

	//数字类型节点构造函数（双长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(long long D_LONG_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_LONG_LONG_T;
		setFatherNode(fatherT);
	}

	//数字类型节点构造函数（无符号双长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(unsigned long long D_ULONG_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_ULONG_LONG_T;
		setFatherNode(fatherT);
	}

	//数字类型节点构造函数，三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(double D_DOUBLE_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_DOUBLE_T;
		setFatherNode(fatherT);
	}

	//数字类型节点构造函数，三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(long double D_LONG_DOUBLE_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_LONG_DOUBLE_T;
		setFatherNode(fatherT);
	}

	//字符类型节点构造函数，三个参数分别是宽字符串类型的节点值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT, JsonNode *fatherT)
	{
		type = T_STRING;
		name = nameT;
		D_STRING = D_STRING_T;
		setFatherNode(fatherT);
	}

	//字符类型节点构造函数（重载），三个参数分别是宽字符串类型的节点值、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(const wstring &D_STRING_T, const wchar_t *nameT, JsonNode *fatherT)
	{
		type = T_STRING;
		name = nameT;
		D_STRING = D_STRING_T;
		setFatherNode(fatherT);
	}

	//数组/结构类型节点构造函数，三个参数分别是节点标记（用'['、']'或者'{'、'}标记）、宽字符串类型的节点名称和父节点。
	JsonNode::JsonNode(char obj, const wstring &nameT, JsonNode *fatherT)
	{
		if (obj == '{' || obj == '}')
		{
			type = T_OBJECT;
		}
		else if (obj == '[' || obj == ']')
		{
			type = T_VECTOR;
		}
		else
		{
			type = T_NULL;
		}

		name = nameT;
		setFatherNode(fatherT);
	}

	//把这个节点的父节点设置为fatherT。
	bool JsonNode::setFatherNode(JsonNode *fatherT)
	{
		if (Father==nullptr && fatherT!=nullptr && fatherT!=this && fatherTIsNotMySon(fatherT, this))
		{
			if (fatherT->type == T_VECTOR)
			{
				Father = fatherT;
				name = L"";
				Father->Sons.push_back(this);
				return true;
			}
			else if (fatherT->type == T_OBJECT)
			{
				Father = fatherT;
				Father->Sons.push_back(this);
				return true;
			}
		}
		return false;
	}

	//把一个非空节点设置为根节点，否则返回false。
	bool JsonNode::setNodeAsRoot()
	{
		if (Father == nullptr)
		{
			D_BOOL = false;
			D_NUMBER = 0.0;
			D_STRING = L"";
			name = L"";
			type = T_OBJECT;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//递归地查看想要设定的父亲节点是否为自己的子系节点。
	bool JsonNode::fatherTIsNotMySon(const JsonNode *fatherNode, const JsonNode *thisTurnCheckingNode)
	{
		for (const auto &sonListIter : thisTurnCheckingNode->Sons)
		{
			if (sonListIter == fatherNode)
			{
				return false;
			}

			if (thisTurnCheckingNode->type==T_VECTOR || thisTurnCheckingNode->type==T_OBJECT)
			{
				return fatherTIsNotMySon(fatherNode, sonListIter);
			}
		}
		return true;
	}

	JsonNode *JsonManualEngine::createJsonNode()
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>();
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(const wstring &nameT, int nNULL, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(nameT, nNULL,fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(nameT, nNULL, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(bool D_BOOL_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_BOOL_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(int D_INT_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_INT_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(unsigned D_UINT_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_UINT_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(long D_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_LONG_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(unsigned long D_ULONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_ULONG_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(long long D_LONG_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_LONG_LONG_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(unsigned long long D_ULONG_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_ULONG_LONG_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(double D_DOUBLE_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_DOUBLE_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(long double D_LONG_DOUBLE_T, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_LONG_DOUBLE_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_STRING_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(const wstring &D_STRING_T, const wchar_t *nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(D_STRING_T, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}

	JsonNode *JsonManualEngine::createJsonNode(char obj, const wstring &nameT, JsonNode *fatherT)
	{
		shared_ptr<JsonNode> temp = make_shared<JsonNode>(obj, nameT, fatherT);
		JNodeList.push_back(temp);
		return temp.get();
	}
}