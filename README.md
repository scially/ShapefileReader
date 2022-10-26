# 开始
对于GIS开发者来说，GDAL是一个非常重要的库，可是GDAL编译非常麻烦，再加上UE又有自己的一套编译脚本，想要集成还是很困难的。
## C++包管理
在正式开始之前，先讲讲现在`C++`包管理。不像`Python`的`pip`、`Java`的`Maven`，C++标准是没有提供包管理的，于是就有了一些第三方的包管理软件，目前来说比较常用的就是微软提供的[vcpkg](https://github.com/microsoft/vcpkg)了，他是基于`CMake`进行包查找、编译，我用vcpkg这几年，没有发现哪些包是vcpkg没有包含的，而且vcpkg也提供了Visual Studio Nuget的整合，或者用`vcpkg+CMake`搞`C++`开发是完全没问题的。
## 准备
安装好vcpkg后，直接执行 `vcpkg install gdal:x64-windows`，时间还挺长的，安装完成之后，用[Dependencies](https://github.com/lucasg/Dependencies)找到所有的dll依赖，然后还有一个 `gdal_i.lib`，在加上头文件，gdal就准备好了。
我的路径是这样的：
```
|- ShapefileReader.uplugin
|- Resources
    |- GDAL_DATA
        |- GDAL Data
    |- PROJ_LIB
        |- proj.db
|- ThirdParty
    |- bin
       |- gdal相关dll
    |- include
        |-gdal相关头文件
    |- lib
        |- gdal_i.lib
```
### 和UE集成
这里讲最重要的3点
1. 通过`PublicAdditionalLibraries`把`gdal_i.lib`加进去
2. 通过`PublicRuntimeLibraryPaths`把`dll`所在路径加进去
3. 要把dll拷贝到UE编译后的路径，UE提供了`RuntimeDependencies`可以帮我们完成
综合下来，我们的`Build.cs`可以这么写：
```
using System;
using System.IO;
using UnrealBuildTool;

public class ShapefileReader : ModuleRules
{
	public ShapefileReader(ReadOnlyTargetRules Target) : base(Target){
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		string BasePath = Path.Combine(ModuleDirectory, "../../ThirdParty");
		bEnableUndefinedIdentifierWarnings = false;
		CppStandard = CppStandardVersion.Cpp17;
		bUseRTTI = true;
		bEnableExceptions = true;
	
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicIncludePaths.Add(Path.Combine(BasePath, "include"));
			string StaticLibraryPaths = Path.Combine(BasePath, "lib");
			string[] StaticLibraries = Directory.GetFiles(StaticLibraryPaths, "*.lib");
			foreach (string StaticLibrary in StaticLibraries)
			{
				Console.WriteLine(StaticLibrary);
				PublicAdditionalLibraries.Add(StaticLibrary);
			}
				
			string LibraryPaths = Path.Combine(BasePath, "bin");
			PublicRuntimeLibraryPaths.Add(LibraryPaths);
			string[] Libraries = Directory.GetFiles(LibraryPaths,  "*.dll");
			foreach (string LibraryPath in Libraries)
			{
				string LibraryName = Path.GetFileName(LibraryPath);
				Console.WriteLine(LibraryPath);
				RuntimeDependencies.Add(LibraryPath);
				RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)/", LibraryName), LibraryPath);
				RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)/", LibraryName), LibraryPath);
			}
				
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
			});
		}
		else
		{
			Console.Error.WriteLine("Only support Win64 Platform");
		}
	}
}
```
这样编译就没问题了，但是呢，proj和gdal都是有自己的数据要加载的，一般是通过设置`PROJ_LIB`和`GDLA_DATA`完成，为了这个可移植性，我们可以通过代码来做，在插件启动的时候，UE会调用`virtual void StartupModule() override;`，所以我们在这里写：
```
FShapefileReaderModule::FShapefileReaderModule()
{
        // gdal_dir 和 proj_dir 是两个成员变量
	gdal_dir_ = FPaths::Combine(FPaths::ProjectDir(), TEXT("Plugins/ShapefileReader/Resources"), TEXT("GDAL_DATA"));
	proj_dir_ = FPaths::Combine(FPaths::ProjectDir(), TEXT("Plugins/ShapefileReader/Resources"), TEXT("PROJ_LIB"));
}

void FShapefileReaderModule::StartupModule()
{
	CPLSetConfigOption("GDAL_DATA", TCHAR_TO_UTF8(*gdal_dir_));
	const char *const proj_lib_path[] = {TCHAR_TO_UTF8(*proj_dir_), nullptr};
	OSRSetPROJSearchPaths(proj_lib_path);
	GDALAllRegister();
}
```
### 和UE蓝图集成
通过继承`UBlueprintFunctionLibrary`来实现，我把常用的GDAL的类型都用UE封装了，具体使用可以直接看代码
```
UCLASS()
class SHAPEFILEREADER_API UShapefileReaderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable,  Category = "ShapefileLib")
	static UShapefileReaderRecord* ReadShapefile(const FString& Path);
};
```
