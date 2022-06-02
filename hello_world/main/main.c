#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_system.h"
#include "time.h"

static const char* TAG = "MainModule";
static QueueHandle_t queue;

void vTaskTimer(void *pvParameters) {
   struct tm *tm;
   int warning_frequency = (int) pvParameters;
   int count = 0;
   int param_queue;
   bool first_flag = true;

   for(;;) {
      time_t now = time(NULL);
      tm = localtime(&now);

      ESP_LOGI(TAG, "%s", asctime(tm));
      
      if (!(count % warning_frequency) && !first_flag) {
         ESP_LOGW(TAG, "%d seconds passed", warning_frequency);

         xQueueReceive(queue, &param_queue, portMAX_DELAY);
         ESP_LOGI(TAG, "value from queue: %d", param_queue);
      }

      vTaskDelay(1000 / portTICK_PERIOD_MS);
      first_flag = false;
      count++;
   }
}

void vTaskProducer(void *pvParameters) {
   int param = (int) pvParameters;

   for(;;) {
      xQueueSend(queue, &param, portMAX_DELAY);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }
}

void app_main(void) {
   TaskHandle_t task_ref;
   TaskHandle_t task_ref_core_1;

   //10second warning frequency
   /*int param_simple_timer = 10;
   xTaskCreate(vTaskTimer, "task_simple_timer", 4096, (void *)param_simple_timer, tskIDLE_PRIORITY + 5, &task_ref);*/

   //custom warning frequency
   /*int param_custom_timer = CONFIG_TIME_FREQUENCY_WARNING;
   xTaskCreate(vTaskTimer, "task_custom_timer", 4096, (void *)param_custom_timer, tskIDLE_PRIORITY + 5, &task_ref);*/

   queue = xQueueCreate(10, sizeof(int));

   //custom warning frequency core 0
   int param_custom_timer = CONFIG_TIME_FREQUENCY_WARNING;
   xTaskCreatePinnedToCore(vTaskTimer, "task_custom_timer_core_0", 4096, (void *) param_custom_timer, tskIDLE_PRIORITY + 5, &task_ref, 0);

   //task 2 on core 1
   xTaskCreatePinnedToCore(vTaskProducer, "task_producer_core_1", 4096, (void *) 42, tskIDLE_PRIORITY + 5, &task_ref, 1);

}