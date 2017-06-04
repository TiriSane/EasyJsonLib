#include "JsonNode.h"

using namespace std;

namespace EasyJson
{
	//�����ͽڵ㹹�캯�������������ֱ��ǿ��ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(const wstring &nameT, int nNULL, JsonNode *fatherT)
	{
		if (!nNULL)
		{
			type = T_NULL;
			name = nameT;
			setFatherNode(fatherT);
		}
	}

	//�����ͽڵ㹹�캯�������أ������������ֱ��ǿ��ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT)
	{
		if (!nNULL)
		{
			type = T_NULL;
			name = nameT;
			setFatherNode(fatherT);
		}
	}

	//�������ͽڵ㹹�캯�������������ֱ��ǽڵ㲼��ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(bool D_BOOL_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_BOOL;
		name = nameT;
		D_BOOL = D_BOOL_T;
		setFatherNode(fatherT);
	}

	//�������ͽڵ㹹�캯�������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(int D_INT_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_INT_T;
		setFatherNode(fatherT);
	}
	
	//�������ͽڵ㹹�캯�����޷������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(unsigned D_UINT_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_UINT_T;
		setFatherNode(fatherT);
	}

	//�������ͽڵ㹹�캯���������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(long D_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_LONG_T;
		setFatherNode(fatherT);
	}
	
	//�������ͽڵ㹹�캯�����޷��ų����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(unsigned long D_ULONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_ULONG_T;
		setFatherNode(fatherT);
	}

	//�������ͽڵ㹹�캯����˫�����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(long long D_LONG_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_LONG_LONG_T;
		setFatherNode(fatherT);
	}

	//�������ͽڵ㹹�캯�����޷���˫�����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(unsigned long long D_ULONG_LONG_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_ULONG_LONG_T;
		setFatherNode(fatherT);
	}

	//�������ͽڵ㹹�캯�������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(double D_DOUBLE_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_DOUBLE_T;
		setFatherNode(fatherT);
	}

	//�������ͽڵ㹹�캯�������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(long double D_LONG_DOUBLE_T, const wstring &nameT, JsonNode *fatherT)
	{
		type = T_NUMBER;
		name = nameT;
		D_NUMBER = D_LONG_DOUBLE_T;
		setFatherNode(fatherT);
	}

	//�ַ����ͽڵ㹹�캯�������������ֱ��ǿ��ַ������͵Ľڵ�ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT, JsonNode *fatherT)
	{
		type = T_STRING;
		name = nameT;
		D_STRING = D_STRING_T;
		setFatherNode(fatherT);
	}

	//�ַ����ͽڵ㹹�캯�������أ������������ֱ��ǿ��ַ������͵Ľڵ�ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
	JsonNode::JsonNode(const wstring &D_STRING_T, const wchar_t *nameT, JsonNode *fatherT)
	{
		type = T_STRING;
		name = nameT;
		D_STRING = D_STRING_T;
		setFatherNode(fatherT);
	}

	//����/�ṹ���ͽڵ㹹�캯�������������ֱ��ǽڵ��ǣ���'['��']'����'{'��'}��ǣ������ַ������͵Ľڵ����ƺ͸��ڵ㡣
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

	//������ڵ�ĸ��ڵ�����ΪfatherT��
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

	//��һ���ǿսڵ�����Ϊ���ڵ㣬���򷵻�false��
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
	
	//�ݹ�ز鿴��Ҫ�趨�ĸ��׽ڵ��Ƿ�Ϊ�Լ�����ϵ�ڵ㡣
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