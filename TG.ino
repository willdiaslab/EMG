const int analogInPin = A0;   // Saída da Placa EMG
const int ledCaptura = 12;    // LED de indicação de captura
const int baudRate = 9600;

bool isCapturing = false;
bool ledState = false;

unsigned long previousAdcMillis = 0;
unsigned long previousLedMillis = 0;
const long adcInterval = 10;   // 10ms -> 100 Hz de amostragem
const long ledInterval = 500;  // 500ms -> Pisca 2 vezes por segundo

void setup() {
  pinMode(ledCaptura, OUTPUT);
  digitalWrite(ledCaptura, LOW); // Garante que começa desligado
  Serial.begin(baudRate);
}

void loop() {
  // Verifica se chegou comando do Python
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      isCapturing = true;
    } else if (command == '0') {
      isCapturing = false;
      digitalWrite(ledCaptura, LOW); // Desliga o LED ao parar
    }
  }

  unsigned long currentMillis = millis();

  // Se estiver no modo de captura, executa as tarefas
  if (isCapturing) {
    
    // 1. Envia dados do ADC a cada 10ms
    if (currentMillis - previousAdcMillis >= adcInterval) {
      previousAdcMillis = currentMillis;
      int sensorValue = analogRead(analogInPin);
      float voltage = sensorValue * (5.0 / 1023.0);
      Serial.println(voltage);
    }

    // 2. Pisca o LED a cada 500ms
    if (currentMillis - previousLedMillis >= ledInterval) {
      previousLedMillis = currentMillis;
      ledState = !ledState; // Inverte o estado do LED
      digitalWrite(ledCaptura, ledState);
    }
    
  }
}