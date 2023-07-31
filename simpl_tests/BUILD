sh_test(
    name = "arithmetic_test",
    size = "small",
    srcs = ["run_simpl_tests"],
    data = [
        "//simpl:simpli",
        "simpl_progs/arithmetic.simpl",
    ]
)

sh_test(
    name = "printing_test",
    size = "small",
    srcs = ["run_simpl_tests"],
    data = [
        "//simpl:simpli",
        "simpl_progs/printing.simpl",
    ]
)

sh_test(
    name = "repl_test",
    size = "small",
    flaky = True,
    srcs = ["run_repl_test"],
    data = [
        "//simpl:simpli",
    ]
)
