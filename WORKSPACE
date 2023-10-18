load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    urls = ["https://github.com/google/googletest/archive/9bb354fa8325fa31faa1e12627b25ab445e6eed3.zip"],
    strip_prefix = "googletest-9bb354fa8325fa31faa1e12627b25ab445e6eed3",
    sha256 = "29bc37662bbf75b48937ced4d5c41cdbe9acf3c9935f8be5142b29307d838bf6"
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
