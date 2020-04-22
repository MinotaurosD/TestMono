#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

using namespace std;

MonoDomain *domain;

int main(int argc, char const *argv[])
{
  const char *managed_binary_path = "myapp.dll";
  domain = mono_jit_init(managed_binary_path);
  mono_set_dirs("/Library/Frameworks/Mono.framework/Home/lib", "/Library/Frameworks/Mono.framework/Home/etc");
  mono_config_parse(NULL);
  //加载程序集ManagedLibrary.dll
  MonoAssembly *assembly = mono_domain_assembly_open(domain, managed_binary_path);
  MonoImage *image = mono_assembly_get_image(assembly);
  //获取MonoClass
  MonoClass *main_class = mono_class_from_name(image, "MyNameSpace", "MyClass");
  //获取要调用的MonoMethodDesc
  MonoMethodDesc *entry_point_method_desc = mono_method_desc_new("MyNameSpace.MyClass:Main()", true);
  MonoMethod *entry_point_method = mono_method_desc_search_in_class(entry_point_method_desc, main_class);
  mono_method_desc_free(entry_point_method_desc);
  //调用方法
  mono_runtime_invoke(entry_point_method, NULL, NULL, NULL);
  //释放应用域
  mono_jit_cleanup(domain);

  return 0;
}
