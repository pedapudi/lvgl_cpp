idf_component_register(SRCS "main.cpp" 
                       "hw/gc9a01.cpp" 
                       "hw/chsc6x.cpp" 
                       "sys/lvgl_port.cpp"
                       "ui/hello_world.cpp"
                       "ui/select_hello.cpp"
                       INCLUDE_DIRS "." "hw" "sys" "ui"
                       PRIV_REQUIRES esp_lcd lvgl lvgl_cpp esp_lcd_gc9a01 esp_timer esp_driver_i2c driver)
