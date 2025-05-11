import lit.formats

config.name = "MarkUnusedPluginTest"

config.llvm_tools_dir = "/PATH/TO/LLVM/BUILD/bin"
config.llvm_libs_dir = "/PATH/TO/PLUGIN/build"

config.test_format = lit.formats.ShTest(execute_external=True)
