sh_test(
    name = "arithmetic_test",
    size = "small",
    srcs = ["run_simpl_tests"],
    data = [
        "//simpl:simpl",
        "simpl_progs/arithmetic.simpl",
    ]
)

sh_test(
    name = "printing_test",
    size = "small",
    srcs = ["run_simpl_tests"],
    data = [
        "//simpl:simpl",
        "simpl_progs/printing.simpl",
    ]
)

sh_test(
    name = "complex_data_types_test",
    size = "small",
    srcs = ["run_simpl_tests"],
    data = [
        "//simpl:simpl",
        "simpl_progs/complex_data_types.simpl",
    ]
)

sh_test(
    name = "arrows_test",
    size = "small",
    srcs = ["run_simpl_tests"],
    data = [
        "//simpl:simpl",
        "simpl_progs/arrows.simpl",
    ]
)

sh_test(
    name = "repl_test",
    size = "small",
    flaky = True,
    srcs = ["run_repl_test"],
    data = [
        "//simpl:simpl",
    ]
)
