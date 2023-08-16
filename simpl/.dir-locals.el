((nil .
      (
       (eval . (setq compile-command "bazel test //simpl:all"))
       (eval . (setq flycheck-clang-include-path
                     (list (expand-file-name "..")
                           (expand-file-name "../bazel-simpl/external/gtest/googletest/include"))))
       (eval . (setq flycheck-cppcheck-include-path
                     (list (expand-file-name "..")
                           (expand-file-name "../bazel-simpl/external/gtest/googletest/include")))))))
