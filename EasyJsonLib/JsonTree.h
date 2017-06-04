#ifndef _JSON_TREE_H_
#define _JSON_TREE_H_

//�궨�壺һЩ�������
//�ļ��򲻿�
#define FileCanNotOpen        1
//�޷������ļ�
#define FileCanNotCreate      2
//Json�ڵ������������Json�淶
#define JsonWrongNodeName     3
//Json�ڵ�����ͷǷ�
#define JsonWrongType         4
//������ʽ��Json�ļ���ʽ����
#define JsonWrongSyntax       5

#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <codecvt>

#include "JsonNode.h"

namespace EasyJson
{
	//���ļ�������JsonTree������Json�ڵ�ۺϳɵ���״�ṹ�����ڵ��Ҷ�ڵ�֮����Json�ڵ��ָ��˫�����ӡ�
	class JsonTree
	{
	//JsonTree�Ĺ��к�����
	public:
		//���졢���ƹ��졢��������

		//����һ��Ĭ�Ϲ��캯�����������ͨ�������ڴ�ķ�ʽ������һ���յ�Json�ڵ㣬
		//����������ѽڵ�����͸�ΪJson����(T_OBJECT)�����ͣ�Ȼ�������ڵ��ָ�븳ֵ�����ԱROOT��
		JsonTree();
		//������캯��ͨ������ָ��utf-8��ʽ���ı��ļ��ķ�ʽ,���ɶ�Ӧ��JsonTree��
		//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�С�
		JsonTree(const std::string &fileName);
		//������캯��ͨ������ָ�����ַ����ķ�ʽ,���ɶ�Ӧ��JsonTree��
		//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�С�
		JsonTree(const std::wstring &jsonString);
		//������캯��ͨ�������Ѿ��ֶ�����ĸ��ڵ�ķ�ʽ,���ɶ�Ӧ��JsonTree��
		//������rootNode�Ľṹ��Json�﷨�����ɶ�Ӧ��Json���ַ��������浽���ԱJsonReaded�С�
		JsonTree(JsonNode *rootNode);
		//������ƹ��캯��ͨ�����¼���Json�ַ����ķ�ʽ�����ƹ���һ���µ�JsonTree�������Ķ������ܡ�
		JsonTree(JsonTree &ths);
		//����JsonTree������������
		//����������JsonTree�Ǵ��ı�������ʱ��
		//��ͨ������DeleteJsonTree������
		//�ݹ���ͷ�JsonTree����������ڴ档
		//����������JsonTree�����û������ʱ��
		//��ʲôҲ������
		~JsonTree();

		//�����Ժ���

		//�����������ָ��utf-8��ʽ���ı��ļ�,���ɶ�Ӧ��JsonTree��
		//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�У�����ֵΪ������롣
		int setJsonFromFile(const std::string &fileName);
		//�����������ָ�����ַ���,���ɶ�Ӧ��JsonTree��
		//���Ѷ�ȡ����Json���ַ������浽���ԱJsonReaded�У�����ֵΪ������롣
		int setJsonFromWString(const std::wstring &jsonString);

		//���ڴ��в���õ�JsonTree����ģ��ת��ΪJson��ʽ�Ŀ��ַ�����
		//��������õ�Json��ʽ�Ŀ��ַ�����ֵ�����Ա�е�JsonReaded, ͬʱ��������õ��ַ�����
		std::wstring getJsonToWString();
		//���ڴ��в���õ�JsonTree����ģ��ת��ΪJson��ʽ�Ŀ��ַ�����
		//�����ַ���תΪutf-8���룬�����ڲ���ָ�����ı���,
		//���������������õ�Json��ʽ�Ŀ��ַ�����ֵ�����Ա�е�JsonReaded,����ֵΪ������롣
		int getJsonToFile(const std::string &fileName);

		//����һ�����ع�����еȷ��ţ������ж�����JsonTree�Ƿ���ȡ�
		bool operator==(JsonTree &that);

		//ɾ��JsonNָ��Ľڵ��Լ��������ӽڵ㣨ͨ���ݹ�ķ�ʽ�������ر�ʾɾ���Ƿ�ɹ��Ĳ���ֵ��
		bool deleteNode(JsonNode *JsonN);

		//�����нڵ�����Ϊ�սڵ�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(void *ptrNULL, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_BOOL_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(bool D_BOOL_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_INT_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(int D_INT_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_UINT_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(unsigned D_UINT_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_LONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(long D_LONG_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_ULONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(unsigned long D_ULONG_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_LONG_LONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(long long D_LONG_LONG_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_ULONG_LONG_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(unsigned long long D_ULONG_LONG_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_DOUBLE_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(double D_DOUBLE_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ�������͵ģ�ֵΪD_LONG_DOUBLE_T�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(long double D_LONG_DOUBLE_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ���ַ������͵ģ�ֵΪD_STRING_T�ģ��ڵ���Ϊname��������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(const wchar_t *D_STRING_T, const std::wstring &nameT = L"");
		
		//�����нڵ�����Ϊ���ַ������͵ģ�ֵΪD_STRING_T�ģ����أ����ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(const std::wstring &D_STRING_T, const std::wstring &nameT = L"");

		//�����нڵ�����Ϊ����/�ṹ�ģ��ڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNode(char obj, const std::wstring &nameT = L"");

		//�����нڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��ء�
		std::list<JsonNode*> findNodeWithName(const wchar_t *nameT);

		//�����нڵ���Ϊname������ʡ�����֣���
		//�ڵ�ָ���ռ��������з��أ����أ���
		std::list<JsonNode*> findNodeWithName(const std::wstring &nameT);

	//JsonTree��˽�г�Ա��
	private:
		//���ڵ��ָ�롣���ڵ���һ������ΪT_OBJECT��JsonNodeʵ�������ڵ�Ϊ��ָ�롣
		JsonNode *ROOT;

		//JsonTree�����Ӧ�Ŀ��ַ���������JsonTree����Ϊ�Լ�����ģ�
		//����û������ʹ�ù�GetJsonToWString/GetJsonToFile������������ַ���������Ϊ�ա�
		std::wstring jsonReaded;

		//����Json��������е�����ջ�����ڱ��Json�ڵ����ȡ�
		std::stack<wchar_t> levelStack;

		//Private����

		//����������JsonTree����Դ�Ƿ�Ϊ�ļ�����ַ���
		bool isFromText = false;

		//�ݹ�����Node�ڵ㼰�����������ڵ�
		//JsonTree�����������͵���������������еݹ�ɾ���Ĳ�����
		void deleteJsonTree(JsonNode *node);

		//ͨ��DFS�ı�����ʽ����Json���ַ����ı���
		void DFSMakeJsonText(JsonNode *node, std::wstring &ss);

		//ͨ��DFS�ı�����ʽѰ�ҷ���������Json�ڵ㣬�������Ǽ���ls�С�
		void DFSJsonFindNode(JsonNode *node, std::list<JsonNode*> &ls, const DataType dt, const std::wstring &nameT, const bool boolT, const long double numberT, const std::wstring &stringT);

		//ͨ��DFS�ı�����ʽѰ�ҽ�����ƥ���Json�ڵ㣬�������Ǽ���ls�С�
		void DFSJsonFindNode(JsonNode *node, std::list<JsonNode*> &ls, const std::wstring &nameT);

		//Ԥ�ڵĴ�����ı��� �հף���ѡ�ģ�-ָ���������ַ�-�հף���ѡ����
		//��������󣬵���������ָ�����һ����ѡ�հ׵ĺ���ĵ�һ���ǿհ��ַ���
		bool haveWChar(std::wstring::const_iterator &iter, std::wstring::const_iterator &end, wchar_t character);

		//Ԥ�ڵĴ�����ı��� �հף���ѡ�ģ�- ˫���� - �ַ��� - ˫���� - �հף���ѡ�ģ���
		//��������󣬵���������ָ���ַ�����
		//���һ����ѡ�հ׵ĺ���ĵ�һ���ǿհ��ַ���
		bool getWStringWithQuotes(std::wstring::const_iterator &iter, std::wstring::const_iterator &end, std::wstring &theStr);
		
		//Ԥ�ڴ���һ����L'\"'��β���ַ�����
		//��������󣬵���������ָ���ַ������һ���ַ���
		bool getWString(std::wstring::const_iterator &iter, std::wstring::const_iterator &end, std::wstring &theString);

		//�ж�wchar_t�Ƿ�Ϊ�հס�
		bool isSpace(const wchar_t &ch);

		//�ж�wchar_t�Ƿ�Ϊ0~9�����֡�
		bool isDigit(const wchar_t &ch);

		//ͨ��DFS�ı�����ʽ��Json�������нڵ������ڴ��С�
		void DFSWStringCollector(JsonNode *node, std::list<std::wstring> &SS);

		//ˢ��json�ַ����С�
		void refreshJsonString();
	};
}

#endif