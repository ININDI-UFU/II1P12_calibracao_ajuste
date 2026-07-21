#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define GAIN 1             // Ganho para calibrar o valor do ADC (em unidades de leitura do ADC)
#define OFFSET 0           // Offset para calibrar o valor do ADC (em unidades de leitura do ADC)
#define pinANALOG A5       // Configura o pino de leitura
#define SAMPLE_PERIOD_MS 1 // período de amostragem da task (1 kHz)

static QueueHandle_t adcQueue = nullptr;
static uint32_t timeStampMS = 0;

// Task dedicada à amostragem do ADC: roda em paralelo ao loop(), lendo o valor do ADC a cada SAMPLE_PERIOD_MS e enviando para a fila adcQueue
static void adcSampleTask(void *pvParameters) {
    TickType_t lastWake = xTaskGetTickCount();
    for (;;) {
        uint16_t sample = analogRead(pinANALOG);
        xQueueSend(adcQueue, &sample, 0);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(SAMPLE_PERIOD_MS));
    }
}

void setup() {
    Serial.begin(115200);
    adcQueue = xQueueCreate(64, sizeof(uint16_t));
    xTaskCreate(adcSampleTask, "adcSampleTask", 2048, nullptr, 1, nullptr);
}

void loop() {
    uint16_t sample;
    if (xQueueReceive(adcQueue, &sample, 0) == pdTRUE) {
        Serial.print(">graf:");
        Serial.print(timeStampMS++);
        Serial.print(":");
        Serial.print(GAIN*sample+OFFSET);
        Serial.println("|g");
    }
}