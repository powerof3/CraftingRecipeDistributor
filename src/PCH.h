#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <ranges>

#include "RE/Skyrim.h"
#include "REX/REX.h"
#include "SKSE/SKSE.h"

#include <MergeMapperPluginAPI.h>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <frozen/map.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <CLibUtil/distribution.hpp>
#include <ClibUtil/editorID.hpp>

#include <SimpleIni.h>
#undef ERROR

using namespace clib_util;
using namespace std::literals;

// for visting variants
template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};

template <class K, class D, class H = boost::hash<K>, class KEqual = std::equal_to<K>>
using Map = boost::unordered_flat_map<K, D, H, KEqual>;

template <class K, class H = boost::hash<K>, class KEqual = std::equal_to<K>>
using Set = boost::unordered_flat_set<K, H, KEqual>;

struct string_hash
{
	using is_transparent = void;  // enable heterogeneous overloads

	std::size_t operator()(std::string_view str) const
	{
		return boost::hash<std::string_view>()(str);
	}
};

template <class D>
using StringMap = Map<std::string, D, string_hash, std::equal_to<>>;
using StringSet = Set<std::string, string_hash, std::equal_to<>>;

namespace Runtime
{
	inline constexpr REL::Version SSE_1_7_99(1, 7, 99, 0);
	inline constexpr REL::Version MIN_ADDRESS_LIBRARY_V5 = SSE_1_7_99;

	inline REL::Version version{};

	[[nodiscard]] inline bool IsAtLeast1_7_99() noexcept
	{
		static bool result = REX::FModule::GetExecutingModule().GetFileVersion() >= Runtime::SSE_1_7_99;
		return result;
	}
}

#include "Version.h"
