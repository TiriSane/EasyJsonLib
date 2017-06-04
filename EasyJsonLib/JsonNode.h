#ifndef _JSON_NODE_H_
#define _JSON_NODE_H_

#include <string>
#include <list>
#include <memory>

namespace EasyJson
{
	//DataType为枚举类型，节点的type属性的值为Datatype其中的一种，
	//0：空类型，1：布尔类型，2：数字类型，3：字符串类型，4：Json数组类型，5：Json对象类型。
	enum DataType { T_NULL, T_BOOL, T_NUMBER, T_STRING, T_VECTOR, T_OBJECT };

	//本文件定义了JsonNode，这是用来表示Json节点的结构体。
	//在局部作用域构造JsonTree的节点时，考虑到节点的生命周期，
	//建议在定义之前使用static。
	struct JsonNode
	{
		//友元类声明
		friend class JsonTree;

		//这是一个默认构造函数。可以把类属性初始化为适当的空值。
		JsonNode() = default;
		//空类型节点构造函数，两个参数分别是宽字符串类型的节点名称和父节点。
		JsonNode(const std::wstring &nameT, int nNULL, JsonNode *fatherT = nullptr);
		//空类型节点构造函数（重载），两个参数分别是宽字符串类型的节点名称和父节点。
		JsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT = nullptr);
		//布尔类型节点构造函数（布尔），三个参数分别是节点布尔值、宽字符串类型的节点名称和父节点。
		JsonNode(bool D_BOOL_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(int D_INT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（无符号整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(unsigned D_UINT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(long D_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（无符号长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(unsigned long D_ULONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（双长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(long long D_LONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（无符号双长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(unsigned long long D_ULONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数，三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(double D_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数，三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode(long double D_LONG_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//字符类型节点构造函数，三个参数分别是宽字符串类型的节点值、宽字符串类型的节点名称和父节点。
		JsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//字符类型节点构造函数（重载），三个参数分别是宽字符串类型的节点值、宽字符串类型的节点名称和父节点。
		JsonNode(const std::wstring &D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//数组/结构类型节点构造函数，三个参数分别是节点标记（用'['、']'或者'{'、'}标记）、宽字符串类型的节点名称和父节点。
		JsonNode(char obj, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);

		//把这个节点的父节点设置为fatherT。
		bool setFatherNode(JsonNode *fatherT);
		//把一个非空节点设置为根节点，否则返回false。
		bool setNodeAsRoot();

		//数据部分
	
		//type：Datatype类型的实例，保存这个节点元素的类型，会被默认初始化为type-NULL。
		DataType type = T_NULL;
		//name：节点的名称，默认初始化的情况下用了一个空的宽字符串保存，
		//特别地，根据Json语法，数组类型节点的子节点无名称，它们的name应保持为空字符串。
		std::wstring name;

		//D_BOOL：当type为布尔类型时，这里保存相应的布尔属性：true或false，会被默认初始化为false。
		bool D_BOOL = false;
		//D_NUMBER：当type为数字类型时，这里保存相应的值，类型为long double浮点型，会被默认初始化为0.0。
		long double D_NUMBER = 0.0;
		//D_STRING：当type为字符串类型时，这里保存相应的值，用宽字符串保存，会被默认初始化为空值。
		std::wstring D_STRING;

	private:		
		//递归地查看想要设定的父亲节点是否为自己的子系节点。
		bool fatherTIsNotMySon(const JsonNode *fatherNode, const JsonNode *thisTurnCheckingNode);

		//数据部分

		//Sons：当type为Json数组/对象类型时，这里保存相应的值，
		//这个双向链表结构里存储着这个Json节点的所有子节点的指针，会被默认初始化为空表。
		std::list<JsonNode*> Sons;
		//Father：父亲节点，用于标注当前节点的父亲节点，会被默认初始化为空指针。
		JsonNode *Father = nullptr;
	};
	
	//为了实现工厂模式的类
	class JsonManualEngine
	{
	public:
		JsonNode* createJsonNode();
		//空类型节点构造函数，两个参数分别是宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(const std::wstring &nameT, int nNULL, JsonNode *fatherT = nullptr);
		//空类型节点构造函数（重载），两个参数分别是宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT = nullptr);
		//布尔类型节点构造函数（布尔），三个参数分别是节点布尔值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(bool D_BOOL_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(int D_INT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（无符号整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(unsigned D_UINT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(long D_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（无符号长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(unsigned long D_ULONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（双长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(long long D_LONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数（无符号双长整型），三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(unsigned long long D_ULONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数，三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(double D_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//数字类型节点构造函数，三个参数分别是节点数值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(long double D_LONG_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//字符类型节点构造函数，三个参数分别是宽字符串类型的节点值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//字符类型节点构造函数（重载），三个参数分别是宽字符串类型的节点值、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(const std::wstring &D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//数组/结构类型节点构造函数，三个参数分别是节点标记（用'['、']'或者'{'、'}标记）、宽字符串类型的节点名称和父节点。
		JsonNode* createJsonNode(char obj, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);

	private:
		std::list<std::shared_ptr<JsonNode>> JNodeList;
	};
}

#endif