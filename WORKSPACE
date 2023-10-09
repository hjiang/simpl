load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    urls = ["https://github.com/google/googletest/archive/2dd1c131950043a8ad5ab0d2dda0e0970596586a.zip"],
    strip_prefix = "googletest-2dd1c131950043a8ad5ab0d2dda0e0970596586a",
    sha256 = "ba96972e0aa8a1428596570ac573958c1c879483bd148a2b72994453f9dfa7c2"
)

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ac6411f8f347e5525038cb7858db4969db9e74f2.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-ac6411f8f347e5525038cb7858db4969db9e74f2",
    sha256 = "f03ed383f1093b5960b37648c4fc5ae2b3d98050509bc2e11719afc8b1fd3b3a"
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
