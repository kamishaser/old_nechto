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
				auto vNode = new visualNode(newExObjNode(), temp);
				vNode->frame.position = glm::vec2(100.f, 100.f);
				gui.workBoard.addNode(vNode);
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
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode && vNode->getConnection(0).exist())
					fs.serialize(vNode->getConnection(0));
			} while (gi.stepForward());
			fs.close();
		}
	};
}