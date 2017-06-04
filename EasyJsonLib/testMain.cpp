#include <iostream>

#include "JsonTree.h"

using namespace std;
using namespace EasyJson;

int main()
{
	locale::global(locale(""));
	//一个手动创建Json文件格式的示例
	JsonManualEngine E;
	
	//构造Json结构
	auto rootNode = E.createJsonNode();
	rootNode->setNodeAsRoot();
	for (int i = 0; i < 10; ++i)
	{
		auto temp = E.createJsonNode(i, L"numberSample", rootNode);
	}
	auto os = E.createJsonNode('{', L"objectSample", rootNode);
	for (int i = 1000; i<1010; ++i)
	{
		wstring st;
		wstringstream ss; ss << 1000; ss >> st;
		auto temp = E.createJsonNode(st, L"stringSample", os);
	}
	auto bs = E.createJsonNode(true, L"boolSample", rootNode);
	auto ovs = E.createJsonNode('[', L"vectorSample", os);
	auto ns = E.createJsonNode(L"nullSample", NULL, ovs);
	auto ns2 = E.createJsonNode(L"nullSample", NULL, rootNode);

	//用构造好的结构根节点初始化JsonTree
	//ASampleTree的生存周期和JsonManualEngine一致。
	JsonTree ASampleTree(rootNode);

	cout << "Test 1" << endl;
	//输出JsonTree
	wcout << ASampleTree.getJsonToWString() << endl;
	//保存到6.json的文件中
	ASampleTree.getJsonToFile("6.json");

	//////////////////////////////////////////////////////////////////////////

	//一个读取Json文件的示例
	JsonTree BSampleTree("2.json");
	cout << "Test 2" << endl;
	//输出读入的2.json文件
	wcout << BSampleTree.getJsonToWString() << endl;

	//搜索和删除节点
	//名字匹配
	auto ln = BSampleTree.findNodeWithName(L"sign");
	//值匹配
	auto ds = BSampleTree.findNode(241168,L"id");
	//删除节点
	for (auto &so:ln)
	{
		BSampleTree.deleteNode(so);
	}

	for (auto &so : ds)
	{
		BSampleTree.deleteNode(so);
	}

	cout << "Test 3" << endl;
	//输出删除节点后的2.json
	wcout << BSampleTree.getJsonToWString() << endl;
	//保存到7.json
	BSampleTree.getJsonToFile("7.json");

	//赋值
	BSampleTree = ASampleTree;
	//或者ASampleTree = BSampleTree;

	return 0;
}