add_executable(MainCode)
pico_generate_pio_header(MainCode ${CMAKE_CURRENT_LIST_DIR}/pio_seq.pio)
target_sources(MainCode PRIVATE MainCode.c registers.c neopixel.c)



target_link_libraries(MainCode PRIVATE pico_stdlib hardware_pio hardware_dma)
pico_add_extra_outputs(MainCode)

pico_enable_stdio_usb(MainCode 1) # PICO SDK (USB)
pico_enable_stdio_uart(MainCode 0) # PICO SDK (USB)

# add url via pico_set_program_url
example_auto_set_url(MainCode)