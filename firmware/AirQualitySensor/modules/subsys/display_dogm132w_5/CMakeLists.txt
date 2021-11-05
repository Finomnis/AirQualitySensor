
zephyr_library_named(subsys_display_dogm132w_5)
zephyr_library_sources_ifdef(CONFIG_SUBSYS_DISPLAY_DOGM132W_5 display_dogm132w_5.c)
zephyr_include_directories(.)

zephyr_library_sources_ifdef(CONFIG_SUBSYS_DISPLAY_DOGM132W_5
    ./ugfx/src/gfx_mk.c
    ./ugfx/drivers/gdisp/ST7565/gdisp_lld_ST7565.c
)
zephyr_include_directories(
    ./ugfx
    ./ugfx/drivers/gdisp/ST7565
)
