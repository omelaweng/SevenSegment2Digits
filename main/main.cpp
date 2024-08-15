#include <stdio.h>
#include "SevenSegment.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

SevenSegment s1(0);
SevenSegment s2(4);

// create counter as global variable
uint8_t counter = 0;

// create task handle for LED display task
TaskHandle_t xSevenSegmentHandle = NULL;

// create task handle for counter task
TaskHandle_t xCounterHandle = NULL;

void vTaskScanSevenSegment(void *Parameters)
{
    while (1)
    {
        s1.DisplayNumber(counter / 10);
        s1.DisplayOn();
        vTaskDelay(10 / portTICK_PERIOD_MS);
        s1.DisplayOff();

        s2.DisplayNumber(counter % 10);
        s2.DisplayOn();
        vTaskDelay(10 / portTICK_PERIOD_MS);
        s2.DisplayOff();
    }
}

// add counter task, increment every 1000ms (1 second)
void vTaskCounter(void *Parameters)
{
    while (1)
    {
        if (counter++ > 99)
            counter = 0;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    xTaskCreate(vTaskScanSevenSegment, "Seven Seg",
                1024, NULL, 10, &xSevenSegmentHandle);

    // create counter task with priority 5 (Lower than scan display task)
    xTaskCreate(vTaskCounter, "Counter",
                1024, NULL, 5, &xCounterHandle);
}