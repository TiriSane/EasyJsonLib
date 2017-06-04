#ifndef _JSON_NODE_H_
#define _JSON_NODE_H_

#include <string>
#include <list>
#include <memory>

namespace EasyJson
{
	//DataTypeΪö�����ͣ��ڵ��type���Ե�ֵΪDatatype���е�һ�֣�
	//0�������ͣ�1���������ͣ�2���������ͣ�3���ַ������ͣ�4��Json�������ͣ�5��Json�������͡�
	enum DataType { T_NULL, T_BOOL, T_NUMBER, T_STRING, T_VECTOR, T_OBJECT };

	//���ļ�������JsonNode������������ʾJson�ڵ�Ľṹ�塣
	//�ھֲ���������JsonTree�Ľڵ�ʱ�����ǵ��ڵ���������ڣ�
	//�����ڶ���֮ǰʹ��static��
	struct JsonNode
	{
		//��Ԫ������
		friend class JsonTree;

		//����һ��Ĭ�Ϲ��캯�������԰������Գ�ʼ��Ϊ�ʵ��Ŀ�ֵ��
		JsonNode() = default;
		//�����ͽڵ㹹�캯�������������ֱ��ǿ��ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(const std::wstring &nameT, int nNULL, JsonNode *fatherT = nullptr);
		//�����ͽڵ㹹�캯�������أ������������ֱ��ǿ��ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯���������������������ֱ��ǽڵ㲼��ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(bool D_BOOL_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(int D_INT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�����޷������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(unsigned D_UINT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯���������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(long D_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�����޷��ų����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(unsigned long D_ULONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯����˫�����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(long long D_LONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�����޷���˫�����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(unsigned long long D_ULONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(double D_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(long double D_LONG_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�ַ����ͽڵ㹹�캯�������������ֱ��ǿ��ַ������͵Ľڵ�ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//�ַ����ͽڵ㹹�캯�������أ������������ֱ��ǿ��ַ������͵Ľڵ�ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(const std::wstring &D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//����/�ṹ���ͽڵ㹹�캯�������������ֱ��ǽڵ��ǣ���'['��']'����'{'��'}��ǣ������ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode(char obj, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);

		//������ڵ�ĸ��ڵ�����ΪfatherT��
		bool setFatherNode(JsonNode *fatherT);
		//��һ���ǿսڵ�����Ϊ���ڵ㣬���򷵻�false��
		bool setNodeAsRoot();

		//���ݲ���
	
		//type��Datatype���͵�ʵ������������ڵ�Ԫ�ص����ͣ��ᱻĬ�ϳ�ʼ��Ϊtype-NULL��
		DataType type = T_NULL;
		//name���ڵ�����ƣ�Ĭ�ϳ�ʼ�������������һ���յĿ��ַ������棬
		//�ر�أ�����Json�﷨���������ͽڵ���ӽڵ������ƣ����ǵ�nameӦ����Ϊ���ַ�����
		std::wstring name;

		//D_BOOL����typeΪ��������ʱ�����ﱣ����Ӧ�Ĳ������ԣ�true��false���ᱻĬ�ϳ�ʼ��Ϊfalse��
		bool D_BOOL = false;
		//D_NUMBER����typeΪ��������ʱ�����ﱣ����Ӧ��ֵ������Ϊlong double�����ͣ��ᱻĬ�ϳ�ʼ��Ϊ0.0��
		long double D_NUMBER = 0.0;
		//D_STRING����typeΪ�ַ�������ʱ�����ﱣ����Ӧ��ֵ���ÿ��ַ������棬�ᱻĬ�ϳ�ʼ��Ϊ��ֵ��
		std::wstring D_STRING;

	private:		
		//�ݹ�ز鿴��Ҫ�趨�ĸ��׽ڵ��Ƿ�Ϊ�Լ�����ϵ�ڵ㡣
		bool fatherTIsNotMySon(const JsonNode *fatherNode, const JsonNode *thisTurnCheckingNode);

		//���ݲ���

		//Sons����typeΪJson����/��������ʱ�����ﱣ����Ӧ��ֵ��
		//���˫������ṹ��洢�����Json�ڵ�������ӽڵ��ָ�룬�ᱻĬ�ϳ�ʼ��Ϊ�ձ�
		std::list<JsonNode*> Sons;
		//Father�����׽ڵ㣬���ڱ�ע��ǰ�ڵ�ĸ��׽ڵ㣬�ᱻĬ�ϳ�ʼ��Ϊ��ָ�롣
		JsonNode *Father = nullptr;
	};
	
	//Ϊ��ʵ�ֹ���ģʽ����
	class JsonManualEngine
	{
	public:
		JsonNode* createJsonNode();
		//�����ͽڵ㹹�캯�������������ֱ��ǿ��ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(const std::wstring &nameT, int nNULL, JsonNode *fatherT = nullptr);
		//�����ͽڵ㹹�캯�������أ������������ֱ��ǿ��ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(const wchar_t *nameT, int nNULL, JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯���������������������ֱ��ǽڵ㲼��ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(bool D_BOOL_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(int D_INT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�����޷������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(unsigned D_UINT_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯���������ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(long D_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�����޷��ų����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(unsigned long D_ULONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯����˫�����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(long long D_LONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�����޷���˫�����ͣ������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(unsigned long long D_ULONG_LONG_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(double D_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�������ͽڵ㹹�캯�������������ֱ��ǽڵ���ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(long double D_LONG_DOUBLE_T, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);
		//�ַ����ͽڵ㹹�캯�������������ֱ��ǿ��ַ������͵Ľڵ�ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(const wchar_t *D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//�ַ����ͽڵ㹹�캯�������أ������������ֱ��ǿ��ַ������͵Ľڵ�ֵ�����ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(const std::wstring &D_STRING_T, const wchar_t *nameT = L"", JsonNode *fatherT = nullptr);
		//����/�ṹ���ͽڵ㹹�캯�������������ֱ��ǽڵ��ǣ���'['��']'����'{'��'}��ǣ������ַ������͵Ľڵ����ƺ͸��ڵ㡣
		JsonNode* createJsonNode(char obj, const std::wstring &nameT = L"", JsonNode *fatherT = nullptr);

	private:
		std::list<std::shared_ptr<JsonNode>> JNodeList;
	};
}

#endif