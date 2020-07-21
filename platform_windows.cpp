#include "binaryninjaapi.h"

using namespace BinaryNinja;
using namespace std;


class WindowsX86Platform: public Platform
{
public:
	WindowsX86Platform(Architecture* arch): Platform(arch, "windows-x86")
	{
		Ref<CallingConvention> cc;

		cc = arch->GetCallingConventionByName("cdecl");
		if (cc)
		{
			RegisterDefaultCallingConvention(cc);
			RegisterCdeclCallingConvention(cc);
		}

		cc = arch->GetCallingConventionByName("fastcall");
		if (cc)
			RegisterFastcallCallingConvention(cc);

		cc = arch->GetCallingConventionByName("stdcall");
		if (cc)
			RegisterStdcallCallingConvention(cc);

		cc = arch->GetCallingConventionByName("thiscall");
		if (cc)
			RegisterCallingConvention(cc);

		// Linux-style register convention is commonly used by Borland compilers
		cc = arch->GetCallingConventionByName("regparm");
		if (cc)
			RegisterCallingConvention(cc);
	}
};


class WindowsX64Platform: public Platform
{
public:
	WindowsX64Platform(Architecture* arch): Platform(arch, "windows-x86_64")
	{
		Ref<CallingConvention> cc;

		cc = arch->GetCallingConventionByName("win64");
		if (cc)
		{
			RegisterDefaultCallingConvention(cc);
			RegisterCdeclCallingConvention(cc);
			RegisterFastcallCallingConvention(cc);
			RegisterStdcallCallingConvention(cc);
		}
	}
};


class WindowsArmv7Platform: public Platform
{
public:
	WindowsArmv7Platform(Architecture* arch, const std::string& name): Platform(arch, name)
	{
		Ref<CallingConvention> cc;

		cc = arch->GetCallingConventionByName("cdecl");
		if (cc)
		{
			RegisterDefaultCallingConvention(cc);
			RegisterCdeclCallingConvention(cc);
			RegisterFastcallCallingConvention(cc);
			RegisterStdcallCallingConvention(cc);
		}
	}
};


class WindowsArm64Platform: public Platform
{
public:
	WindowsArm64Platform(Architecture* arch): Platform(arch, "windows-aarch64")
	{
		Ref<CallingConvention> cc;

		cc = arch->GetCallingConventionByName("cdecl");
		if (cc)
		{
			RegisterDefaultCallingConvention(cc);
			RegisterCdeclCallingConvention(cc);
			RegisterFastcallCallingConvention(cc);
			RegisterStdcallCallingConvention(cc);
		}
	}
};


extern "C"
{
#ifndef DEMO_VERSION
	BINARYNINJAPLUGIN void CorePluginDependencies()
	{
		AddOptionalPluginDependency("arch_x86");
		AddOptionalPluginDependency("arch_armv7");
		AddOptionalPluginDependency("arch_arm64");
	}
#endif

#ifdef DEMO_VERSION
	bool WindowsPluginInit()
#else
	BINARYNINJAPLUGIN bool CorePluginInit()
#endif
	{
		Ref<Architecture> x86 = Architecture::GetByName("x86");
		if (x86)
		{
			Ref<Platform> platform;

			platform = new WindowsX86Platform(x86);
			Platform::Register("windows", platform);
			BinaryViewType::RegisterDefaultPlatform("PE", x86, platform);
		}

		Ref<Architecture> x64 = Architecture::GetByName("x86_64");
		if (x64)
		{
			Ref<Platform> platform;

			platform = new WindowsX64Platform(x64);
			Platform::Register("windows", platform);
			BinaryViewType::RegisterDefaultPlatform("PE", x64, platform);
		}

		Ref<Architecture> armv7 = Architecture::GetByName("armv7");
		Ref<Architecture> thumb2 = Architecture::GetByName("thumb2");
		if (armv7 && thumb2)
		{
			Ref<Platform> armPlatform, thumbPlatform;

			armPlatform = new WindowsArmv7Platform(armv7, "windows-armv7");
			thumbPlatform = new WindowsArmv7Platform(thumb2, "windows-thumb2");
			armPlatform->AddRelatedPlatform(thumb2, thumbPlatform);
			thumbPlatform->AddRelatedPlatform(armv7, armPlatform);
			Platform::Register("windows", armPlatform);
			Platform::Register("windows", thumbPlatform);
			BinaryViewType::RegisterDefaultPlatform("PE", armv7, armPlatform);
		}

		Ref<Architecture> arm64 = Architecture::GetByName("aarch64");
		if (arm64)
		{
			Ref<Platform> platform;

			platform = new WindowsArm64Platform(arm64);
			Platform::Register("windows", platform);
			BinaryViewType::RegisterDefaultPlatform("PE", arm64, platform);
		}

		return true;
	}
}
