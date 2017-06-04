#include <iostream>

#include "JsonTree.h"

using namespace std;
using namespace EasyJson;

int main()
{
	locale::global(locale(""));
	//һ���ֶ�����Json�ļ���ʽ��ʾ��
	JsonManualEngine E;
	
	//����Json�ṹ
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

	//�ù���õĽṹ���ڵ��ʼ��JsonTree
	//ASampleTree���������ں�JsonManualEngineһ�¡�
	JsonTree ASampleTree(rootNode);

	cout << "Test 1" << endl;
	//���JsonTree
	wcout << ASampleTree.getJsonToWString() << endl;
	//���浽6.json���ļ���
	ASampleTree.getJsonToFile("6.json");

	//////////////////////////////////////////////////////////////////////////

	//һ����ȡJson�ļ���ʾ��
	JsonTree BSampleTree("2.json");
	cout << "Test 2" << endl;
	//��������2.json�ļ�
	wcout << BSampleTree.getJsonToWString() << endl;

	//������ɾ���ڵ�
	//����ƥ��
	auto ln = BSampleTree.findNodeWithName(L"sign");
	//ֵƥ��
	auto ds = BSampleTree.findNode(241168,L"id");
	//ɾ���ڵ�
	for (auto &so:ln)
	{
		BSampleTree.deleteNode(so);
	}

	for (auto &so : ds)
	{
		BSampleTree.deleteNode(so);
	}

	cout << "Test 3" << endl;
	//���ɾ���ڵ���2.json
	wcout << BSampleTree.getJsonToWString() << endl;
	//���浽7.json
	BSampleTree.getJsonToFile("7.json");

	//��ֵ
	BSampleTree = ASampleTree;
	//����ASampleTree = BSampleTree;

	return 0;
}