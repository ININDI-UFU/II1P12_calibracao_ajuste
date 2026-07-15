 #include <Arduino.h>

 #define pinANALOG 34 // GPIO34 (ADC1_CH6) - pino somente-entrada, ideal para leitura analógica no ESP32

 uint32_t timeDelayMS = 10;
 uint32_t expiresDelayMS = 0;

 void setup() {
     Serial.begin(115200);
     analogReadResolution(12);        // ADC de 12 bits (0-4095)
     analogSetAttenuation(ADC_11db);  // Faixa de leitura de 0 a ~3.3V
 }
 
 void loop() {
     if ((millis() - expiresDelayMS) >= timeDelayMS)
     {
       expiresDelayMS = millis();
       Serial.print(">graf:");
       Serial.print(expiresDelayMS);
       Serial.print(":");
       Serial.print(analogRead(pinANALOG));
       Serial.println("|g");
     }
 }