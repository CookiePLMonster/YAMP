#pragma once

#include <map>
#include <cassert>

#include "wil/common.h"

enum class ImportSymbol; // Symbols are per-DLL

class Imports
{
public:
	Imports(std::initializer_list<std::pair<const ImportSymbol, void*>> symbols)
		: m_symbolMap(std::move(symbols))
	{
	}

	Imports(const Imports&) = delete;
	Imports(Imports&&) = default;

	void Add(ImportSymbol key, void* value)
	{
		m_symbolMap.emplace(key, value);
	}

	auto GetSymbol(ImportSymbol symbol) const
	{
		assert(m_symbolMap.count(symbol) == 1);
		return m_symbolMap.find(symbol)->second;
	}

	auto GetSymbolRange(ImportSymbol symbol) const
	{
		assert(m_symbolMap.count(symbol) != 0);
		auto range = m_symbolMap.equal_range(symbol);
		return wil::make_range(range.first, range.second);
	}

private:
	std::multimap<ImportSymbol, void*> m_symbolMap;
};