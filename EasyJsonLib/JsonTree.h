#ifndef _JSON_TREE_H_
#define _JSON_TREE_H_

//宏定义：一些错误代码
//文件打不开
#define FileCanNotOpen        1
//无法创建文件
#define FileCanNotCreate      2
//Json节点的命名不符合Json规范
#define JsonWrongNodeName     3
//Json节点的类型非法
#define JsonWrongType         4
//其他形式的Json文件格式错误
#define JsonWrongSyntax       5

#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <codecvt>

#include "JsonNode.h"

namespace EasyJson
{
	//本文件定义了JsonTree，这是Json节点聚合成的树状结构，根节点和叶节点之间用Json节点的指针双向连接。
	class JsonTree
	{
	//JsonTree的公有函数：
	public:
		//构造、复制构造、析构函数

		//这是一个默认构造函数。这个函数通过申请内存的方式创建了一个空的Json节点，
		//这个函数还把节点的类型改为Json对象(T_OBJECT)的类型，然后把这个节点的指针赋值给类成员ROOT。
		JsonTree();
		//这个构造函数通过加载指定utf-8格式的文本文件的方式,生成对应的JsonTree，
		//并把读取出的Json宽字符串保存到类成员JsonReaded中。
		JsonTree(const std::string &fileName);
		//这个构造函数通过加载指定宽字符串的方式,生成对应的JsonTree，
		//并把读取出的Json宽字符串保存到类成员JsonReaded中。
		JsonTree(const std::wstring &jsonString);
		//这个构造函数通过加载已经手动构造的根节点的方式,生成对应的JsonTree，
		//并根据rootNode的结构和Json语法，生成对应的Json宽字符串，保存到类成员JsonReaded中。
		JsonTree(JsonNode *rootNode);
		//这个复制构造函数通过重新加载Json字符串的方式，复制构造一个新的JsonTree，会消耗额外性能。
		JsonTree(JsonTree &ths);
		//这是JsonTree的析构函数，
		//当被析构的JsonTree是从文本得来的时候，
		//它通过调用DeleteJsonTree函数，
		//递归地释放JsonTree申请的所有内存。
		//当被析构的JsonTree是由用户构造的时候，
		//它什么也不做。
		~JsonTree();

		//功能性函数

		//这个函数加载指定utf-8格式的文本文件,生成对应的JsonTree，
		//并把读取出的Json宽字符串保存到类成员JsonReaded中，返回值为错误代码。
		int setJsonFromFile(const std::string &fileName);
		//这个函数加载指定宽字符串,生成对应的JsonTree，
		//并把读取出的Json宽字符串保存到类成员JsonReaded中，返回值为错误代码。
		int setJsonFromWString(const std::wstring &jsonString);

		//把内存中部署好的JsonTree对象模型转化为Json格式的宽字符串，
		//并把整理好的Json格式的宽字符串赋值给类成员中的JsonReaded, 同时返回整理好的字符串。
		std::wstring getJsonToWString();
		//把内存中部署好的JsonTree对象模型转化为Json格式的宽字符串，
		//并把字符串转为utf-8编码，保存在参数指定的文本中,
		//这个函数还把整理好的Json格式的宽字符串赋值给类成员中的JsonReaded,返回值为错误代码。
		int getJsonToFile(const std::string &fileName);

		//这是一个重载过后的判等符号，用来判断两个JsonTree是否相等。
		bool operator==(JsonTree &that);

		//删除JsonN指向的节点以及其所有子节点（通过递归的方式），返回表示删除是否成功的布尔值。
		bool deleteNode(JsonNode *JsonN);

		//把所有节点类型为空节点的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(void *ptrNULL, const std::wstring &nameT = L"");

		//把所有节点类型为布尔类型的，值为D_BOOL_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(bool D_BOOL_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_INT_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(int D_INT_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_UINT_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(unsigned D_UINT_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_LONG_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(long D_LONG_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_ULONG_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(unsigned long D_ULONG_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_LONG_LONG_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(long long D_LONG_LONG_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_ULONG_LONG_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(unsigned long long D_ULONG_LONG_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_DOUBLE_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(double D_DOUBLE_T, const std::wstring &nameT = L"");

		//把所有节点类型为数字类型的，值为D_LONG_DOUBLE_T的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(long double D_LONG_DOUBLE_T, const std::wstring &nameT = L"");

		//把所有节点类型为宽字符串类型的，值为D_STRING_T的，节点名为name（不可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(const wchar_t *D_STRING_T, const std::wstring &nameT = L"");
		
		//把所有节点类型为宽字符串类型的，值为D_STRING_T的（重载），节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(const std::wstring &D_STRING_T, const std::wstring &nameT = L"");

		//把所有节点类型为数组/结构的，节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNode(char obj, const std::wstring &nameT = L"");

		//把所有节点名为name（可以省略名字）的
		//节点指针收集到链表中返回。
		std::list<JsonNode*> findNodeWithName(const wchar_t *nameT);

		//把所有节点名为name（可以省略名字）的
		//节点指针收集到链表中返回（重载）。
		std::list<JsonNode*> findNodeWithName(const std::wstring &nameT);

	//JsonTree的私有成员：
	private:
		//根节点的指针。根节点是一个类型为T_OBJECT的JsonNode实例，父节点为空指针。
		JsonNode *ROOT;

		//JsonTree对象对应的宽字符串，当此JsonTree对象为自己构造的，
		//并且没有主动使用过GetJsonToWString/GetJsonToFile函数，这个宽字符串的内容为空。
		std::wstring jsonReaded;

		//解析Json对象过程中的运行栈，用于标记Json节点的深度。
		std::stack<wchar_t> levelStack;

		//Private函数

		//用来标记这个JsonTree的来源是否为文件或宽字符串
		bool isFromText = false;

		//递归销毁Node节点及其所有下属节点
		//JsonTree的析构函数就调用了这个函数进行递归删除的操作。
		void deleteJsonTree(JsonNode *node);

		//通过DFS的遍历方式生产Json宽字符串文本。
		void DFSMakeJsonText(JsonNode *node, std::wstring &ss);

		//通过DFS的遍历方式寻找符合特征的Json节点，并把它们加在ls中。
		void DFSJsonFindNode(JsonNode *node, std::list<JsonNode*> &ls, const DataType dt, const std::wstring &nameT, const bool boolT, const long double numberT, const std::wstring &stringT);

		//通过DFS的遍历方式寻找仅名字匹配的Json节点，并把它们加在ls中。
		void DFSJsonFindNode(JsonNode *node, std::list<JsonNode*> &ls, const std::wstring &nameT);

		//预期的处理的文本如 空白（可选的）-指定的特殊字符-空白（可选），
		//函数的最后，迭代器将会指向最后一个可选空白的后面的第一个非空白字符。
		bool haveWChar(std::wstring::const_iterator &iter, std::wstring::const_iterator &end, wchar_t character);

		//预期的处理的文本如 空白（可选的）- 双引号 - 字符串 - 双引号 - 空白（可选的），
		//函数的最后，迭代器将会指向字符串、
		//最后一个可选空白的后面的第一个非空白字符。
		bool getWStringWithQuotes(std::wstring::const_iterator &iter, std::wstring::const_iterator &end, std::wstring &theStr);
		
		//预期处理一个以L'\"'结尾的字符串，
		//函数的最后，迭代器将会指向字符串后第一个字符。
		bool getWString(std::wstring::const_iterator &iter, std::wstring::const_iterator &end, std::wstring &theString);

		//判断wchar_t是否为空白。
		bool isSpace(const wchar_t &ch);

		//判断wchar_t是否为0~9的数字。
		bool isDigit(const wchar_t &ch);

		//通过DFS的遍历方式把Json树的所有节点整理到内存中。
		void DFSWStringCollector(JsonNode *node, std::list<std::wstring> &SS);

		//刷新json字符序列。
		void refreshJsonString();
	};
}

#endif