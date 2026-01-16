idf_component_register(SRCS "main.cpp" 
                       "hw/ssd1306_i2c.cpp" 
                       "sys/lvgl_port.cpp" 
                       "ui/hello_world.cpp"
                       PRIV_REQUIRES driver lvgl lvgl_cpp esp_timer freertos esp_driver_gpio esp_driver_i2c esp_lvgl_port esp_lcd
                       INCLUDE_DIRS "." "hw" "sys" "ui")
