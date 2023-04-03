#pragma once
#include "path.h"
#include "nodePtr.h"
#include "conRuleInterface.h"
#include "crSet.h"
#include "variablePtr.h"
#include "text.h"
#include "entity.h"
#include "pExchange.h"
#include <typeinfo>
#include <map>
//по возможности переименовать

namespace nechto
{
  template<class TCon>
  concept nodeTranslatable =
    requires(TCon t, nodePtr node)
  {
    {node << t };
  } &&
    requires(TCon t, nodePtr node)
  {
    {node >> t };
  } &&
    requires(TCon t, nodePtr node)
  {
    {node ^ t };
  } &&
    requires(TCon t, nodePtr node)
  {
    { node % t};
  };
}