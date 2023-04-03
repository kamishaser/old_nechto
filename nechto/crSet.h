#pragma once
#include "standartConRule.h"
#include <vector>
#include <array>

namespace nechto
{
	class ruleGenerator
	{
		static std::vector<std::unique_ptr<conRuleInterface>> ruleSet;
	public:
		static size_t registerRule(conRuleInterface* rule)//регистрация нового правила извне
		{
			size_t number = ruleSet.size();
			ruleSet.emplace_back(rule);
			return number;
		}
		static ccrInterface getRule(size_t number)//получение правила по регистрационному номеру
		{
			return ruleSet[number].get();
		}
		static size_t numberOfRegistredRules()
		{
			return ruleSet.size();
		}
		///////////////////////////////////////////////////////////////////////
		template<unsigned char type, bool subtypeImportant = false, 
			unsigned char subtype = 0, bool transitable = true>
		static ccrInterface generateBasicRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			conRuleInterface* rule = new basicConnectionRule<elementaryRule(
				type, subtypeImportant, subtype, transitable)>();
			number = registerRule(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<bool subtypeImportant = false,
			unsigned char subtype = 0, bool necessarilyExistingPointer = true,
			contentRule cr = contentRule::Any>
		static ccrInterface generatePointerRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			conRuleInterface* rule = new pointerConnectionRule<pointerRule(
				subtypeImportant, subtype, necessarilyExistingPointer, cr)>();
			number = registerRule(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<contentRule content = contentRule::Any, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		static ccrInterface generateNonTypedEntityRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			conRuleInterface* rule = new nonTypedEntityConnectionRule<nonTypedEntityRule(
				subtypeImportant, subtype, nodeT::Entity, transitable, content)>();
			number = registerRule(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<contentRule content = contentRule::Any, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		static ccrInterface generateTextRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			conRuleInterface* rule = new nonTypedEntityConnectionRule < nonTypedEntityRule(
				subtypeImportant, subtype, nodeT::Text, transitable, content )> ();
			number = registerRule(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<contentRule content = contentRule::Any, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		static ccrInterface generateOperatorRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			conRuleInterface* rule = new nonTypedEntityConnectionRule < nonTypedEntityRule(
				subtypeImportant, subtype, nodeT::Operator, transitable, content )> ();
			number = registerRule(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<essT ECon, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		static ccrInterface generateEntityRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			conRuleInterface* rule = new entityConnectionRule < entityRule(
				subtypeImportant, subtype, transitable), ECon>();
			number = registerRule(rule);
			return rule;
		}
	};
	std::vector<std::unique_ptr<conRuleInterface>> ruleGenerator::ruleSet;
	
	namespace cr
	{
		ccrInterface getNoneRule()
		{
			return ruleGenerator::generateBasicRule<0>();
		}//по некоторым причинам ccrInterface None инициализирован в "conRuleInterface.h"

		ccrInterface AnyVariable = 
			ruleGenerator::generateBasicRule<nodeT::Variable>();
		ccrInterface I64 = 
			ruleGenerator::generateBasicRule<nodeT::Variable, true, variableT::I64>();
		ccrInterface F64 = 
			ruleGenerator::generateBasicRule<nodeT::Variable, true, variableT::F64>();

		ccrInterface AnyGroup =
			ruleGenerator::generateBasicRule<nodeT::Group>();
		ccrInterface WeakGroup =
			ruleGenerator::generateBasicRule<nodeT::Group, true, groupT::weak>();
		ccrInterface StrongGroup =
			ruleGenerator::generateBasicRule<nodeT::Group, true, groupT::strong>();

		ccrInterface Struct =
			ruleGenerator::generateBasicRule<nodeT::Struct>();

		ccrInterface Vector =
			ruleGenerator::generateBasicRule<nodeT::Vector>();

		ccrInterface AnyEntity =
			ruleGenerator::generateNonTypedEntityRule<contentRule::Any>();
		ccrInterface EmptyEntity =
			ruleGenerator::generateNonTypedEntityRule<contentRule::Empty>();

		ccrInterface AnyPointer =
			ruleGenerator::generatePointerRule<false, 0, false>();
		ccrInterface PortPointer =
			ruleGenerator::generatePointerRule<true, pointerT::PortPointer, false>();
		ccrInterface GroupPointer =
			ruleGenerator::generatePointerRule<true, pointerT::GroupPointer, false>();

		ccrInterface anyExistingPointer =
			ruleGenerator::generatePointerRule<false>();
		ccrInterface ExistingPortPointer =
			ruleGenerator::generatePointerRule<true, pointerT::PortPointer>();
		ccrInterface ExistingGroupPointer =
			ruleGenerator::generatePointerRule<true, pointerT::GroupPointer>();
		
		ccrInterface Text =
			ruleGenerator::generateTextRule<contentRule::Any>();
		ccrInterface ExisingText = 
			ruleGenerator::generateTextRule<contentRule::NonEmpty>();

		ccrInterface Operator =
			ruleGenerator::generateOperatorRule<contentRule::Any>();
		ccrInterface ExisingOperator =
			ruleGenerator::generateOperatorRule<contentRule::NonEmpty>();
	}
}
