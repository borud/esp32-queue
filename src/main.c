#include <freertos/FreeRTOS.h>

#include <esp_log.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "errno.h"

#define QUEUE_ELEMENT_PAYLOAD_SIZE (256)
#define QUEUE_LENGTH               (100)

static const char *TAG = "queue";

// message_t is the message type we will be passing over the queue.
typedef struct {
	int counter;
	char payload[QUEUE_ELEMENT_PAYLOAD_SIZE];
} message_t;

// consumer_task is the task that consumes messages.
void consumer_task(void *arg) {
	QueueHandle_t queue = (QueueHandle_t)arg;
	message_t message;

	int counter = 0;
	while (true){
		if (! xQueueReceive(queue, &message, (1000 / portTICK_PERIOD_MS))) {
			continue;
		}

		// only log every 1000 messages.
		counter++;
		if (counter == 1000) {
			counter = 0;
			ESP_LOGI(TAG, "received: counter=%d payload=[%s]", message.counter, (char*)message.payload);
		}
	}
}

// producer is the task that produces messages.  In this example we just blast away
// with messages as quickly as we can.
void producer_task(void *arg) {
	QueueHandle_t queue = (QueueHandle_t)arg;
	message_t message = {
		.counter = 0,
	};

	while (true) {
		int n = snprintf(message.payload, QUEUE_ELEMENT_PAYLOAD_SIZE, "message %d",  message.counter++);
		if (n < 0) {
			ESP_LOGE(TAG, "error creating message %d", errno);
			continue;
		}

		BaseType_t ret = xQueueSend(queue, (void*)&message, (1000 / portTICK_PERIOD_MS));
		switch (ret) {
			case errQUEUE_FULL:
				ESP_LOGE(TAG, "queue was full, so we drop the message");
				break;

			case pdTRUE:
				break;

			default:
				ESP_LOGE(TAG, "unknown condition");
				break;
		}
	}
}

// we create a queue and then pass this as an argument to both the producer and the consumer.
void app_main() {
	ESP_LOGI(TAG, "Starting queue test");

	QueueHandle_t queue = xQueueCreate(QUEUE_LENGTH, sizeof(message_t));
	
	// Create the consumer task.
	TaskHandle_t consumer_task_handle = NULL;
	xTaskCreate(consumer_task, "consumer_task", 4096, queue, tskIDLE_PRIORITY, &consumer_task_handle);
	configASSERT(consumer_task_handle);

	// Create the producer task.
	TaskHandle_t producer_task_handle = NULL;
	xTaskCreate(producer_task, "producer_task", 4096, queue, tskIDLE_PRIORITY, &producer_task_handle);
	configASSERT(producer_task_handle);	
}