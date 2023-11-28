#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "sdkconfig.h"

int ahihiCounter = 10;
int ihahaCounter = 5;

TimerHandle_t ahihiTimer;
TimerHandle_t ihahaTimer;

bool ahihiFlag = false;
bool ihahaFlag = false;

void ATimerCallback(TimerHandle_t xTimer)
{
    if ((int)pvTimerGetTimerID(xTimer) == 0 && ahihiCounter > 0)
    {
        ahihiFlag = true;
    }


    if ((int)pvTimerGetTimerID(xTimer) == 1 && ihahaCounter > 0)
    {
        ihahaFlag = true;
    }

}

void ahihi()
{
    while (1)
    {
        if (ahihiFlag)
        {
            printf("At %ldms: ahihi: %d times left\n", pdTICKS_TO_MS(xTaskGetTickCount()), --ahihiCounter);
            ahihiFlag = false;

            if (ahihiCounter <= 0)
            {
                printf("Stopping ahihiTimer\n");
                xTimerStop(ahihiTimer, 0);
            }
        }
        else if (ahihiCounter <= 0)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void ihaha()
{
    while (1)
    {
        if (ihahaFlag)
        {
            //xTaskGetTickCount() to milliseconds. It's used to display the current time in milliseconds.
            printf("At %ldms: ihaha: %d times left\n", pdTICKS_TO_MS(xTaskGetTickCount()), --ihahaCounter);
            ihahaFlag = false;

            if (ihahaCounter <= 0)
            {
                printf("Stopping ihahaTimer\n");
                xTimerStop(ihahaTimer, 0);
            }
        }
        else if (ihahaCounter <= 0)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void app_main(void)
{
    ahihiTimer = xTimerCreate("ahihiTimer",
                pdMS_TO_TICKS(2000), 
                pdTRUE,
                (void* const)0,
                ATimerCallback);
    ihahaTimer = xTimerCreate("ihahaTimer",
                pdMS_TO_TICKS(3000), 
                pdTRUE,
                (void * const)1,
                ATimerCallback);
    xTaskCreatePinnedToCore(ahihi, "ahihi", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(ihaha, "ihaha", 4096, NULL, 0, NULL, 1);
    xTimerStart(ahihiTimer, 0);
    xTimerStart(ihahaTimer, 0);
}