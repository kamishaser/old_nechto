#pragma once
#include "standartConRule.h"
#include <array>

namespace nechto
{
	using ccrInterface = const conRuleInterface const*;
	class ruleGenerator
	{
		static std::vector<const std::unique_ptr<const conRuleInterface>> ruleSet;
	public:
		static size_t registerRule(ccrInterface rule)//регистрация нового правила извне
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
		constexpr static ccrInterface generateBasicRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			number = ruleSet.size();
			const conRuleInterface const* rule = new basicConnectionRule<elementaryRule(
				type, subtypeImportant, subtype, transitable)>();
			ruleSet.emplace_back(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<bool subtypeImportant = false,
			unsigned char subtype = 0, bool necessarilyExistingIterator = true,
			contentRule cr = contentRule::Any>
		constexpr static ccrInterface generateIteratorRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			number = ruleSet.size();
			const conRuleInterface const* rule = new basicConnectionRule<iteratorRule(
				subtypeImportant, subtype, necessarilyExistingIterator, cr)>();
			ruleSet.emplace_back(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<contentRule content = contentRule::Any, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		constexpr static ccrInterface generateNonTypedObjectRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			number = ruleSet.size();
			const conRuleInterface const* rule = new basicConnectionRule<nonTypedEntityRule(
				subtypeImportant, subtype, nodeT::Object, transitable, content>();
			ruleSet.emplace_back(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<contentRule content = contentRule::Any, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		constexpr static ccrInterface generateTextRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			number = ruleSet.size();
			const conRuleInterface const* rule = new basicConnectionRule < nonTypedEntityRule(
				subtypeImportant, subtype, nodeT::Text, transitable, content > ();
			ruleSet.emplace_back(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<contentRule content = contentRule::Any, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		constexpr static ccrInterface generateOperatorRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			number = ruleSet.size();
			const conRuleInterface const* rule = new basicConnectionRule < nonTypedEntityRule(
				subtypeImportant, subtype, nodeT::Operator, transitable, content > ();
			ruleSet.emplace_back(rule);
			return rule;
		}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		template<essT ECon, bool subtypeImportant = false,
			unsigned char subtype = 0, bool transitable = true>
		constexpr static ccrInterface generateObjectRule()
		{
			static size_t number = -1;
			if (number != -1)
				return ruleSet[number].get();
			number = ruleSet.size();
			const conRuleInterface const* rule = new basicConnectionRule < entityRule(
				subtypeImportant, subtype, transitable>, ECon ();
			ruleSet.emplace_back(rule);
			return rule;
		}
	};
	std::vector<const std::unique_ptr<const conRuleInterface>> ruleGenerator::ruleSet;
	
	namespace crSet
	{
		ccrInterface Variable = 
			ruleGenerator::generateBasicRule<nodeT::Variable>();
		ccrInterface I64 = 
			ruleGenerator::generateBasicRule<nodeT::Variable, true, variableT::I64>();
		ccrInterface F64 = 
			ruleGenerator::generateBasicRule<nodeT::Variable, true, variableT::F64>();

		ccrInterface Group =
			ruleGenerator::generateBasicRule<nodeT::Group>();
		ccrInterface WeakGroup =
			ruleGenerator::generateBasicRule<nodeT::Group, true, groupT::weak>();
		ccrInterface StrongGroup =
			ruleGenerator::generateBasicRule<nodeT::Group, true, groupT::strong>();

		ccrInterface Struct =
			ruleGenerator::generateBasicRule<nodeT::Struct>();

		ccrInterface Vector =
			ruleGenerator::generateBasicRule<nodeT::Vector>();

		ccrInterface anyObject =
			ruleGenerator::generateNonTypedObjectRule<contentRule::Any>();
		ccrInterface emptyObject =
			ruleGenerator::generateNonTypedObjectRule<contentRule::Empty>();

		ccrInterface anyIterator =
			ruleGenerator::generateIteratorRule<false, 0, false>();
		ccrInterface PortIterator =
			ruleGenerator::generateIteratorRule<true, iteratorT::PortIter, false>();
		ccrInterface PortIterator =
			ruleGenerator::generateIteratorRule<true, iteratorT::GroupIter, false>();

		ccrInterface anyExistingIterator =
			ruleGenerator::generateIteratorRule<false>();
		ccrInterface ExistingPortIterator =
			ruleGenerator::generateIteratorRule<true, iteratorT::PortIter>();
		ccrInterface ExistingGroupIterator =
			ruleGenerator::generateIteratorRule<true, iteratorT::GroupIter>();
		
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