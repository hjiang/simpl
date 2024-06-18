load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    urls = ["https://github.com/google/googletest/archive/1d17ea141d2c11b8917d2c7d029f1c4e2b9769b2.zip"],
    strip_prefix = "googletest-1d17ea141d2c11b8917d2c7d029f1c4e2b9769b2",
    sha256 = "e7b11076aae5d96aeaa628cc0767b1a4e665bf673dd7518b54e9c025a5c4094d"
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
