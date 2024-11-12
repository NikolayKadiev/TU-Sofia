#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"

#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#include "driver/gpio.h"
#include "driver/uart.h"

QueueHandle_t uart_queue, gpio_evt_queue;

void app_main(void){
    uart_event_t event;
    uint8_t buff_len = 5, buff_place = 0, iter = 0;
    uint8_t *temp = NULL;
    uint16_t *ring_buf = NULL;
    float comp_res = 0.0;

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_RTS,
        .rx_flow_ctrl_thresh = 20,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_0, 1024, 1024, 20, &uart_queue,0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    gpio_set_direction(23, GPIO_MODE_DEF_OUTPUT);
    gpio_set_level(23, 0);

    ring_buf = (uint16_t *)malloc(sizeof(uint16_t)*buff_len);
    // memset(ring_buf, 0, 5);
    for(iter = 0; iter < buff_len; iter++){
        ring_buf[iter] = 0x00;
    }

    while(1){
        if (xQueueReceive(uart_queue, (void * )&event, portMAX_DELAY)) {
            switch (event.type) {
                case UART_DATA:
                    temp = (uint8_t *)malloc(sizeof(uint8_t)*event.size);
                    memset(temp,0x0,event.size);
                    uart_read_bytes(UART_NUM_0,temp,event.size,portMAX_DELAY);
                break;
                default:
                break;
            }
            if((temp[0] == 'O') || (temp[0] == 0x4F)){
                    // memset(ring_buf, 0, 5);
                    for(iter = 0; iter < buff_len; iter++){
                        ring_buf[iter] = 0x00;
                    }
            }
            if((temp[0] == 'S') || (temp[0] == 0x53)){
                    gpio_set_level(23, 1);
                    comp_res = 0.0;
                    ring_buf[buff_place] = (temp[1] << 8) | temp[2];
                    for(iter = 0; iter < buff_len; iter++){
                        comp_res += (float)ring_buf[iter];
                    }
                    comp_res /= (float)buff_len;
                    buff_place ++;
                    if(buff_place == buff_len){
                        buff_place = 0;
                    }
                    printf("%f\n", comp_res);
                    gpio_set_level(23, 0);
            }
            free(temp);
        }
        else{
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    }
}