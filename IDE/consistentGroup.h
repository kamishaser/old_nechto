#pragma once
#include "visualGroup.h"

namespace nechto::ide
{
	//соединение 0 - подконтрольная группа нод (обр номер 0)
	//cоединение 3 - nodeBoard (обратный - хаб)
	struct consistentGroup :public visualGroup
	{
		struct arrangeMode
		{
			bool horisontal = false; //иначе вертикально
			/*в зависимости от horisontal позиционирование справа/сверху или слева/снизу*/
			bool rightAlignment = false; 
			bool reverseDirection = false;
			bool table4 = false; //иначе список

			//отступ по x (зависимость позиции от точки)
			constexpr bool xSizeOffset() const
			{
				return(!horisontal && rightAlignment);
			}
			constexpr bool ySizeOffset() const
			{
				return(horisontal && rightAlignment);
			}
			constexpr glm::vec2 startPoint(const rect& r1) const
			{
				return glm::vec2{
					xSizeOffset() ? (r1.position.x + r1.size.x) : r1.position.x,
					ySizeOffset() ? (r1.position.y + r1.size.y) : r1.position.y
				};
			}
			//номер оси длины
			int lenght() const
			{
				return !horisontal;
			}
			//номер оси ширины
			int width() const
			{
				return horisontal;
			}
		};
		
		arrangeMode mode;
		float distance = 2;
		////////////////////////////////////////////////////////////////////////////////
		consistentGroup(nodePtr emptyExternalObject, const std::u16string& name, 
			glm::vec2 startPoint = glm::vec2(0,0), arrangeMode Mode = arrangeMode())
			:visualGroup(emptyExternalObject, name, startPoint), mode(Mode)
			//при удалении ноды, удалится и сей объект !!!только выделять через new!!!
		{}
		void setPositionByStartPoint(glm::vec2 pos)
		{
			frame.position.x = (mode.xSizeOffset()) ? pos.x - frame.size.x : pos.x;
			frame.position.y = (mode.ySizeOffset()) ? pos.y - frame.size.y : pos.y;
		}
		void resizeVithSaveStartPoint(glm::vec2 size)
		{
			if (mode.xSizeOffset())
				frame.position.x -= size.x - frame.size.x;
			if (mode.ySizeOffset())
				frame.position.y -= size.y - frame.size.y;
			frame.size = size;
		}
		//расставить ноды в группе согласно mode на расстоянии distance друг от друга
		virtual void update()
		{
			groupIterator gi(vNodeGroup());
			glm::vec2 startPos = mode.startPoint(frame);//точка установки первой ноды
			frame.size = glm::vec2{ 0, 0 };
			float lenghtPos{ startPos[mode.lenght()]};//точка установки
			if (!mode.table4)//с табличным отображением отдельная история
			{
				if (mode.reverseDirection)//реверс идёт с последней к первой
					gi.stepBack();
				do
				{
					glm::vec2 pos;//позиция установки ноды
					pos[mode.lenght()] = lenghtPos;
					pos[mode.width()] = startPos[mode.width()];
					glm::vec2 size = arrangeOne(gi, pos);
					lenghtPos += size[mode.lenght()] + distance;
					if (size[mode.width()] > frame.size[mode.width()])
					{
						if (mode.rightAlignment)
							frame.position[mode.width()] -=
							size[mode.width()] - frame.size[mode.width()];
						frame.size[mode.width()] = size[mode.width()];
					}

				} while ((mode.reverseDirection) ? gi.stepBack() : gi.stepForward());
			}
			else
			{
				std::array<float, 4> rPos =
					rowPos(startPos[mode.width()], distance);
				if (mode.reverseDirection)//реверс идёт с последней к первой
					gi.GoToPreviousHub();
				do
				{
					glm::vec2 size = arrangeRow(gi, rPos, lenghtPos);
					lenghtPos += size[mode.lenght()] + distance;
					if (size[mode.width()] > frame.size[mode.width()])
					{
						if (mode.rightAlignment)
							frame.position[mode.width()] -=
							size[mode.width()] - frame.size[mode.width()];
						frame.size[mode.width()] = size[mode.width()];
					}
				} while ((mode.reverseDirection) ? gi.GoToPreviousHub() : gi.GoToNextHub());
			}
			lenghtPos -= distance;
			if (lenghtPos > frame.size[mode.lenght()])
			{
				if (mode.rightAlignment)
					frame.position[mode.lenght()] -=
					lenghtPos - frame.size[mode.lenght()];
				frame.size[mode.lenght()] = lenghtPos;
			}
		}
	protected:
		void replaceRect(rect* r1, glm::vec2 pos) const
		{
			r1->position.x = (mode.xSizeOffset()) ? (pos.x - r1->size.x) : pos.x;
			r1->position.y = (mode.ySizeOffset()) ? (pos.y - r1->size.y) : pos.y;
		}
		glm::vec2 arrangeOne(groupIterator gi, glm::vec2 pos) const
		{
			rect* r1 = getRect(gi.get());
			if (r1)
			{
				replaceRect(r1, pos);
				return r1->size;
			}
			else
				return glm::vec2{ 0,0 };
		}
		glm::vec2 arrangeRow(groupIterator gi,
			std::array<float, 4>widthPos, float lenghtPos) const
		{
			glm::vec2 size{ 0,0 };
			for (int i = 0; i < 4; ++i)
			{
				gi.setLocalPos(i);
				rect* r1 = getRect(gi.get());
				if (r1)
				{
					glm::vec2 pos;
					pos[mode.lenght()] = lenghtPos;
					pos[mode.width()] = widthPos[i];
					replaceRect(r1, pos);
					if (r1->size[mode.lenght()] > size[mode.lenght()])
						size[mode.lenght()] = r1->size[mode.lenght()];
				}
			}
			size[mode.width()] += (mode.rightAlignment) ? maxSizeInRow(0) : maxSizeInRow(3);
			return size;
		}
		float maxSizeInRow(int rowNumber) const
		{
			groupIterator gi(vNodeGroup(), rowNumber);
			float maxRSize = 0;//поиск максимального размера
			do
			{
				rect* r1 = getRect(gi.get());
				if (r1)
				{
					float rSize = (mode.horisontal) ? r1->size.y : r1->size.x;
					if (rSize > maxRSize)
						maxRSize = rSize;
				}
			} while (gi.GoToNextHub());
			return maxRSize;
		}
		//получение стартовой точки каждого ряда для 4table
		std::array<float, 4> rowPos(float startPoint, float distance) const
		{
			std::array<float, 4> rowPos;
			//в каждом ряду, кроме последнего
			if (mode.rightAlignment)
			{
				rowPos[3] = startPoint;
				for (int i = 2; i >= 0; --i)
					rowPos[i] = rowPos[i + 1] - maxSizeInRow(i + 1) - distance;
			}
			else
			{
				rowPos[0] = startPoint;
				for (int i = 0; i < 3; ++i)
					rowPos[i + 1] = rowPos[i] + maxSizeInRow(i) + distance;
			}
			return rowPos;
		}
	public:
		
		
		/*получение указателя на consistentGroup по объекту.
		Возвращает nullptr при несоответствии*/
		static consistentGroup* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<consistentGroup*>(v1->getData<externalObject*>());
		}
		const static std::u16string typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::u16string& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}
	};
	const std::u16string consistentGroup::typeName = u"nechtoIde.consistentGroup";
	const connectionRule consistentGroup::cRule = connectionRule{};
	const staticNodeOperationSet consistentGroup::methodSet
	{
		/*visualGroup::methodSet,
		namedOperation(u"nothing", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				return true;
			}})*/
	};
}