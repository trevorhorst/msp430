    .section .rodata
    .global font
    .type   font, %object
    .align  4
font:
    .incbin "font.txt"
font_end:
    .global font_size
    .type   font_size, %object
    .align  4
font_size:
    .int    font_end - font
