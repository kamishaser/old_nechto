#pragma once
#include "GUI.h"
#include "fileSerializer.h"
#include "periodLimiter.h"
#include "button.h"
#include <filesystem>

namespace nechto::ide
{
	class fileHandler//ОСТОРОЖНО, РОССЫПИ КОСТЫЛЕЙ!!
	{
		GUI& gui;
		periodLimiter plim{ 60000ms };
		sharedButton saveButton{ L"save", [this](auto button) {this->tryToSave(); } };
		sharedButton loadButton{ L"load", [this](auto button) {this->tryToLoad(); } };
		visualNode saveButtonVNode{ L"save" };
		visualNode loadButtonVNode{ L"load" };
		const std::filesystem::path fileName;
	public:
		fileHandler(GUI& g)
			:gui(g) 
		{
			NumNumConnect(saveButton.node(), saveButtonVNode.node(), 0, 0);
			NumNumConnect(loadButton.node(), loadButtonVNode.node(), 0, 0);
			gui.interfaceBoard.addNode(saveButtonVNode.node());
			gui.interfaceBoard.addNode(loadButtonVNode.node());
			gui.topGroup.addNode(saveButtonVNode.node());
			gui.topGroup.addNode(loadButtonVNode.node());
		}

		bool load(std::filesystem::path path)
		{
			fileDeserializer ds;
			ds.open(path);
			if (!ds.isOpen())
				return false;
			nodePtr node = ds.deserialize();
			if (!objectPtr<deserializedObject>::match(node))
			{
				ds.close();
				return false;
			}
			nonTypedObjectPtr board(node);
			if (board.getObjectPtr()->getTypeName() != nodeBoard::typeName)
			{
				ds.close();
				return false;
			}
			deserializeNodeBoard(board);
			node = ds.deserialize();
			while (node.exist())
			{
				if (objectPtr<deserializedObject>::match(node))
				{//десериализация объектов
					nonTypedObjectPtr obj(node);
					const std::wstring& name = obj.getObjectPtr()->getTypeName();
					if (name == visualNode::typeName)
						deserializeVNode(obj);
					else if (name == visualConnection::typeName)
						deserializeVConnection(obj);
					else
						assert(false);
				}
				node = ds.deserialize();
			}
			ds.close();
			groupPtr vng = board.connection(0);
			groupPtr vcg = board.connection(1);
			changeVNodeGroup(vng);
			changeVConnectionGroup(vcg);
			return true;
		}
		bool save(std::filesystem::path path)
		{
			fileSerializer fs;
			fs.open(path);
			if (!fs.isOpen())
				return false;
			fs.serialize(gui.workBoard.node());
			saveVNodeGroup(fs);
			saveVConnectionGroup(fs);
			assert(gui.workBoard.vNodeGroup().exist());
			fs.close();
			std::wcout << L"saved at " << path << std::endl;
			return true;
		}

		void update()
		{
			if (plim.moreThanMin())
			{
				plim.reset();
				save(L"autosave.nechto");
			}
		}
		void tryToSave()
		{
			try
			{
				std::filesystem::path path;
				if (!gui.textBox.iText.empty())
				{
					path = gui.textBox.iText;
					gui.textBox.reset();
				}
				else if (!fileName.empty())
					path = fileName;
				else
				{
					std::wcout << L"введите имя файла" << std::endl;
					return;
				}
				if (save(path))
					std::wcout << L"сохранено успешно" << std::endl;
				return;
			}
			catch (std::exception exc)
			{
				std::cout << exc.what() << std::endl;
			}
			std::wcout << L"сохранить не удалось" << std::endl;
		}
		void tryToLoad()
		{
			try
			{
				std::filesystem::path path;
				if (!gui.textBox.iText.empty())
				{
					path = gui.textBox.iText;
					gui.textBox.reset();
				}
				else if (!fileName.empty())
					path = fileName;
				else
				{
					std::wcout << L"введите имя файла" << std::endl;
					return;
				}
				if (load(path))
					std::wcout << L"загружено успешно" << std::endl;
				return;

			}
			catch (std::exception exc)
			{
				std::cout << exc.what() << std::endl;
			}
			std::wcout << L"загрузить не удалось" << std::endl;
		}
	private:
		void saveVNodeGroup(fileSerializer& fs)
		{
			groupIterator gi(gui.workBoard.vNodeGroup());
			fs.serialize(gi.getPurpose());
			do
			{
				auto vNode = getObject<visualNode>(gi.get());
				if (vNode)
				{
					fs.serialize(vNode->node());
					fs.serialize(vNode->node().connection(0));
				}
			} while (gi.stepForward());
		}
		void saveVConnectionGroup(fileSerializer& fs)
		{
			groupIterator gi(gui.workBoard.vConnectionGroup());
			fs.serialize(gi.getPurpose());
			do
			{
				if (objectPtr<visualConnection>::match(gi.get()))
				{
					fs.serialize(gi.get());
				}
			} while (gi.stepForward());
		}
		visualNode* deserializeVNode(nonTypedObjectPtr node)
		{
			assert(objectPtr<deserializedObject>::match(node));
			std::vector<char>& buffer = objectPtr<deserializedObject>(node)->buffer;
			visualNode::frect fr;
			char* temp = reinterpret_cast<char*>(&fr);
			assert(buffer.size() == sizeof(visualNode::frect));
			assert(node.isUniqueOwner());
			for (int i = 0; i < buffer.size(); ++i)
				temp[i] = buffer[i];
			node.reset();
			auto vNode = new visualNode(node);
			vNode->frame.position = glm::vec2(fr.posx, fr.posy);
			vNode->frame.size = glm::vec2(fr.sizex, fr.sizey);
			return vNode;
		}
		void deserializeVConnection(nonTypedObjectPtr node)
		{
			assert(objectPtr<deserializedObject>::match(node));
			assert(node.isUniqueOwner());
			node.set(new visualConnection());
		}
		void deserializeNodeBoard(nonTypedObjectPtr node)
		{
			assert(objectPtr<deserializedObject>::match(node));
			if (!node.isUniqueOwner())
				delete node.getObjectPtr();
		}
		void changeVNodeGroup(groupPtr group)
		{
			groupIterator gi(group);
			do
			{
				if(objectPtr<visualNode>::match(gi.get()))
					gui.workBoard.addNode(gi.get());
			} while (gi.stepForward());
		}
		void changeVConnectionGroup(groupPtr group)
		{
			groupIterator gi(group);
			do
			{
				if (objectPtr<visualConnection>::match(gi.get()))
					gui.workBoard.addConnection(gi.get());
			} while (gi.stepForward());
		}

		
	};
}