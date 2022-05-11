#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <ranges>

#pragma warning(push)
#include <spdlog/sinks/basic_file_sink.h>
#include <frozen/map.h>
#include <SimpleIni.h>
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = SKSE::log;
namespace numeric = SKSE::stl::numeric;
namespace string = SKSE::stl::string;

using namespace std::literals;

using RNG = SKSE::stl::RNG;

namespace stl
{
	using namespace SKSE::stl;
}

#include "Version.h"
