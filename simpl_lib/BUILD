genrule(name = 'arrows',
        srcs = ['arrows.simpl'],
        outs = ['arrows.hh'],
        tools = ["//third_party/xxd:xxd"],
        cmd = "./$(location //third_party/xxd:xxd) -i $(location arrows.simpl) > $@",
        visibility = ["//visibility:public"])
