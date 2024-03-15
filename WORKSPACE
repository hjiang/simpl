load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    urls = ["https://github.com/google/googletest/archive/518387203b573f35477fa6872dd54620e70d2bdb.zip"],
    strip_prefix = "googletest-518387203b573f35477fa6872dd54620e70d2bdb",
    sha256 = "17d1d88665fb1812d496b97fc83cb71ed995b648397823d77a3d7b188263aec9"
)

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/5bcb0bd8a917b2b48fb5dc55818515f4be3b63ff.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-5bcb0bd8a917b2b48fb5dc55818515f4be3b63ff",
    sha256 = "5911a95b1e4e5a5d3c81ee01b4fc9c5ba63048bb2e9f0a623d9fdecfe8c46185"
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
