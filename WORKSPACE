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
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/4f28899228fb3ad0126897876f147ca15026151e.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-4f28899228fb3ad0126897876f147ca15026151e",
    sha256 = "658122cfb1f25be76ea212b00f5eb047d8e2adc8bcf923b918461f2b1e37cdf2"
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
