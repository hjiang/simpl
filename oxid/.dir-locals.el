((nil .
      (
       (eval . (setq compile-command "bazel test //oxid:all"))
       (eval . (setq flycheck-clang-include-path
                     (list (expand-file-name "..")
                           (expand-file-name "../bazel-oxid/external/gtest/googletest/include")))))))
