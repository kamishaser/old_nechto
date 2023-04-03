#pragma once
//#include "connecter.h"
//#include "visualNode.h"
//#include "group.h"
//
//namespace nechto::ide
//{
//	
//	//к нулевому и первому подключаются ноды
//	//к третьему подключается vCon из nodeBoard
//	class visualConnection :public 
//	{
//		
//	public:
//		std::wstring fText = L"h";
//		std::wstring sText = L"h";
//		visualConnection() :object() {}
//		static objectPtr<visualConnection> create(
//			objectPtr<visualNode> vn0, objectPtr<visualNode> vn1)
//		{
//			objectPtr<visualConnection> vc1 = 
//				creator::createObject(true, new visualConnection());
//			NumHubConnect(vc1, vn0, 0);
//			NumHubConnect(vc1, vn1, 1);
//			return vc1;
//		}
//		static nodePtr getOtherEnd(objectPtr<visualConnection> vc1, objectPtr<visualNode> vn1)
//		{
//			if (vc1.connection(0) == vn1)
//				return vc1.connection(1);
//			else
//				return vn1.connection(0);
//		}
//		static nodePtr getNodeBoard(objectPtr<visualConnection> vc1)
//		{
//			nodePtr temp = vc1.connection(3);
//			if (!temp.exist())
//				return nullptr;
//			temp = temp.connection(0);
//			return temp;
//		}
//		virtual ~visualConnection()
//		{
//			std::wcout << L"visualConnectionDeleted" << std::endl;
//		}
//		const static std::wstring typeName;
//		virtual const std::wstring& getTypeName() const override
//		{
//			return typeName;
//		}
//		virtual void serialize(std::vector<char>& buffer, existing<nodePtr> obj) const
//		{
//			buffer.clear();
//		}
//	};
//	const std::wstring visualConnection::typeName = L"nechtoIde.visualConnection";
//}