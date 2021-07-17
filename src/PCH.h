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
#include <xbyak/xbyak.h>
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = SKSE::log;
namespace numeric = SKSE::stl::numeric;
namespace string = SKSE::stl::string;

using namespace std::literals;

using RNG = SKSE::stl::RNG;

namespace stl
{
	using SKSE::stl::to_underlying;
	using SKSE::stl::is_in;

	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);

	template <class F>
	void asm_replace(std::uintptr_t a_from, std::size_t a_size, F a_newFunc)
	{
		asm_replace(a_from, a_size, reinterpret_cast<std::uintptr_t>(a_newFunc));
	}

	template <class F, std::size_t idx, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(idx, T::thunk);
	}
}

#include "Version.h"
