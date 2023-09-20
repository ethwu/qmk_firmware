
flash-gmmk: flash-keymap

compile-keychron:
    qmk compile -kb keychron/q12/ansi_encoder -km ethwu

flash-keychron:
    qmk flash -kb keychron/q12/ansi_encoder -km ethwu

compile-keymap:
    qmk compile -kb gmmk/gmmk2/p65/ansi -km ethwu

flash-keymap:
    qmk flash -kb gmmk/gmmk2/p65/ansi -km ethwu
