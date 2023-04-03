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
			arrangeMode(nodePtr cgs)
			{
				assert(cgs >> *this);
			}
			friend bool operator>>(nodePtr cgs, arrangeMode& gr)
			{
				return
					sPack::vGroup::consistent::horisontal / cgs >> gr.horisontal &&
					sPack::vGroup::consistent::rightAlignment / cgs >> gr.rightAlignment &&
					sPack::vGroup::consistent::reverseDirection / cgs >> gr.reverseDirection;
			}
			friend bool operator<<(nodePtr cgs, const arrangeMode& gr)
			{
				return
					sPack::vGroup::consistent::horisontal / cgs << gr.horisontal &&
					sPack::vGroup::consistent::rightAlignment / cgs << gr.rightAlignment &&
					sPack::vGroup::consistent::reverseDirection / cgs << gr.reverseDirection;
			}
		};
		
		arrangeMode mode;
		float distance = 20;//сохранять пока не планирую
		////////////////////////////////////////////////////////////////////////////////
		consistentGroup(nodePtr node, ideFactory& fact)
			:visualGroup(node, fact), mode(sPack::vGroup::data / node)
		{}
		void setPositionByStartPoint(glm::vec2 pos)
		{
			rect frame{ frameNode() };
			frame.position.x = (mode.xSizeOffset()) ? pos.x - frame.size.x : pos.x;
			frame.position.y = (mode.ySizeOffset()) ? pos.y - frame.size.y : pos.y;
			frameNode() << frame;
		}
		void resizeVithSaveStartPoint(glm::vec2 size)
		{
			rect frame{ frameNode() };
			if (mode.xSizeOffset())
				frame.position.x -= size.x - frame.size.x;
			if (mode.ySizeOffset())
				frame.position.y -= size.y - frame.size.y;
			frame.size = size;
			frameNode() << frame;
		}
		//расставить ноды в группе согласно mode на расстоянии distance друг от друга
		virtual void update()
		{
			groupPointer gi(vNodeGroup());
			rect frame{ frameNode() };
			glm::vec2 startPos = mode.startPoint(frame);//точка установки первой ноды
			float oldLenght = frame.size[mode.lenght()];
			frame.size = glm::vec2{ 0, 0 };
			float lenghtPos{ startPos[mode.lenght()]};//точка установки
			if (mode.reverseDirection)//реверс идёт с последней к первой
				gi.stepBack();
			do
			{
				if (!gi.get().exist())
					continue;
				glm::vec2 pos;//позиция установки ноды
				pos[mode.lenght()] = lenghtPos;
				pos[mode.width()] = startPos[mode.width()];
				glm::vec2 size = arrangeOne("0210"_np / gi.get(), pos);
				lenghtPos += size[mode.lenght()] + distance;
				if (size[mode.width()] > frame.size[mode.width()])
				{
					frame.size[mode.width()] = size[mode.width()];
				}

			} while ((mode.reverseDirection) ? gi.stepBack() : gi.stepForward());
			lenghtPos -= distance;
			frame.size[mode.lenght()] = lenghtPos - startPos[mode.lenght()];
			frame.position[mode.lenght()] = mode.reverseDirection ?
				startPos[mode.lenght()] - frame.size[mode.lenght()] + oldLenght : 
				startPos[mode.lenght()];
			frame.position[mode.width()] = mode.rightAlignment ?
				startPos[mode.width()] - frame.size[mode.width()] :
				startPos[mode.width()];
			frameNode() << frame;
		}
	protected:
		void replaceRect(nodePtr rNode, glm::vec2 pos) const
		{
			rect r1(rNode);
			r1.position.x = (mode.xSizeOffset()) ? (pos.x - r1.size.x) : pos.x;
			r1.position.y = (mode.ySizeOffset()) ? (pos.y - r1.size.y) : pos.y;
			rNode << r1;
		}
		glm::vec2 arrangeOne(nodePtr rNode, glm::vec2 pos) const
		{
			if (sPack::rect::match(rNode))
			{
				rect r1(rNode);
				replaceRect(rNode, pos);
				return r1.size;
			}
			else
				return glm::vec2{ 0,0 };
		}
	};
}