#pragma once
#include "GUI.h"
#include "fileSerializer.h"
#include <filesystem>

namespace nechto::ide
{
	class fileHandler
	{
		GUI& gui;
	public:
		fileHandler(GUI& g)
			:gui(g) {}

		bool load(std::filesystem::path path)
		{
			fileDeserializer ds;
			ds.open(path);
			if (!ds.isOpen())
				return false;
			nodePtr temp = ds.deserialize();
			while (temp.exist())
			{
				auto vNode = new visualNode(creator::createObject(1), temp);
				vNode->frame.position = glm::vec2(100.f, 100.f);
				gui.workBoard.addNode(getObjectPtr(vNode));
				temp = ds.deserialize();
			}
			ds.close();
		}
		bool save(std::filesystem::path path, nodePtr group)
		{
			fileSerializer fs;
			fs.open(path);
			if (!fs.isOpen())
				return false;
			groupIterator gi(group);
			do
			{
				auto vNode = getObject<visualNode>(gi.get());
				if (vNode && vNode->node().connection(0).exist())
					fs.serialize(vNode->node().connection(0));
			} while (gi.stepForward());
			fs.close();
		}
	};
}