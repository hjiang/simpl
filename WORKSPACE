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
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/a14ad3a64e7bf398ab48105aaa0348e032ac87f8.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-a14ad3a64e7bf398ab48105aaa0348e032ac87f8",
    sha256 = "f01636585c3fb61c7c2dc74df511217cd5ad16427528ab33bc76bb34535f10a1"
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
