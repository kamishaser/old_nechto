#pragma once

#include "ideDisplay.h"
#include "nodeBoard.h"
#include "spreadHandler.h"
#include "autoExpandHandler.h"
#include "userH.h"
#include "vnDataUpdateH.h"
#include "checkAndStep.h"


namespace nechto::ide
{
	class client
	{
	public:
		nodePtr nBoardNode;
		nodePtr ideDisplayNode;
		spreadHandler sph;
		vnDataUpdateH vduh;
		userH uh;

		client(/*externalFunction::shEFS exFunSet*/)
			:nBoardNode((new nodeBoard)->get()), 
			ideDisplayNode((new ideDisplay(nBoardNode))->get()),
			sph(nBoardNode, glm::vec2(500, 500), 4, 4, 0.02),
			uh(nBoardNode, ideDisplayNode),
			vduh(nBoardNode)
		{
			nodeBoard* nBoard = nodeBoard::getByNode(nBoardNode);
			IterIterConnect(group::firstEmptyPort(nBoard->taggedGroup()),
				connectionIterator(uh.cursored.get(), 1));

			auto* vn1 = nBoard->addNode(nBoard->vNodeGroup());
			nBoard->addNode(nBoardNode);

			nodePtr v = newNode(node::Variable, 1);
			v->setData<i64>(124);
			auto* vn2 = nBoard->addNode(v);
			nBoard->addConnection(vn1, vn2);
			nodePtr test = groupIterator(nBoard->vNodeGroup()).get();
			assert(visualNode::getByNode(test));
		}

		bool update()
		{
			sph.update();
			vduh.update();
			uh.update();
			auto* display = ideDisplay::getByNode(ideDisplayNode);
			assert(display);
			return display->update();
		}
	};

}

