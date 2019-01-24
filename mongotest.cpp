// mongotest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "bsoncxx/builder/stream/document.hpp"
#include "mongocxx/instance.hpp"
#include "mongocxx/uri.hpp"
#include "mongocxx/client.hpp"

int main()
{
	mongocxx::instance instance{};
	mongocxx::uri uri("mongodb://127.0.0.1:27017");
	mongocxx::client client(uri);

	// 数据库
	mongocxx::database db = client["mytestdb"];
	// 集合
	mongocxx::collection coll = db["testcoll"];
	// 插入
	try
	{
		for (int i=1; i<=10; ++i)
		{
			auto builder = bsoncxx::builder::stream::document{};
			builder << "userid" << i
				<< "name" << "username";

			coll.insert_one(builder.view());
			std::cout << "insert : " << i << "\n";
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "insert error : " << e.what();
	}

	// 查找
	try
	{
		auto cursor = coll.find(bsoncxx::builder::stream::document{} << "name" << "username" << bsoncxx::builder::stream::finalize);
		if (cursor.begin() != cursor.end())
		{
			for (auto & doc : cursor)
			{
				std::string Name = doc["name"].get_utf8().value.to_string();
				std::cout << "find : userid[" << doc["userid"].get_int32() << "] name[" << Name << "]\n";
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "find error : " << e.what();
	}

	// 更新
	try
	{
		coll.update_one(bsoncxx::builder::stream::document{} <<
		"userid" << 3 << bsoncxx::builder::stream::finalize,
		bsoncxx::builder::stream::document{} << "$set" << bsoncxx::builder::stream::open_document
		<< "name" << "username3333333"
		<< bsoncxx::builder::stream::close_document
			<< bsoncxx::builder::stream::finalize);
		std::cout << "update : userid[3] name[username3333333]" << "\n";
	}
	catch (const std::exception& e)
	{
		std::cout << "update error : " << e.what();
	}

	// 删除数据
	try
	{
		auto filter_builder = bsoncxx::builder::stream::document{};
		bsoncxx::document::value filter_value = filter_builder << "userid" << 7 << bsoncxx::builder::stream::finalize;
		coll.delete_one(filter_value.view());
		std::cout << "delete : userid[7]" << "\n";
	}
	catch (const std::exception& e)
	{
		std::cout << "delete error : " << e.what();
	}

	// 再次查找
	try
	{
		auto cursor = coll.find(bsoncxx::builder::stream::document{} <<  bsoncxx::builder::stream::finalize);
		if (cursor.begin() != cursor.end())
		{
			for (auto & doc : cursor)
			{
				std::string Name = doc["name"].get_utf8().value.to_string();
				std::cout << "find again : userid[" << doc["userid"].get_int32() << "] name[" << Name << "]\n";
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "find error : " << e.what();
	}

	getchar();
}
