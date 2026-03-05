load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    urls = ["https://github.com/google/googletest/archive/a35bc7693c117a048152beeb34f6aac354b9423f.zip"],
    strip_prefix = "googletest-a35bc7693c117a048152beeb34f6aac354b9423f",
    sha256 = "01f3f5300e17b93d03a776a53cb139289d913252de9410875bb426c19761a576"
)

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/abb61a688167623088f8768cc9264798df6a9d10.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-abb61a688167623088f8768cc9264798df6a9d10",
    sha256 = "1b08abffbfbe89f6dbee6a5b33753792e8004f6a36f37c0f72115bec86e68724"
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
