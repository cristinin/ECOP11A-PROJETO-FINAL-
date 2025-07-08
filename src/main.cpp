const int pinoChave = 2;
const int pinoBotaoAbrir = 3;
const int pinoBotaoSaida = 4;
const int pinoSensorPorta = 5;
const int pinoSensorPresenca1 = 6;
const int pinoSensorPresenca2 = 7;
const int pinoMotor = 8;
const int pinoLedBranco = 9;

// --- Estados auxiliares ---
bool portaAberta = false;
bool motorLigado = false;

void setup() {
  Serial.begin(9600);

  pinMode(pinoChave, INPUT_PULLUP);
  pinMode(pinoBotaoAbrir, INPUT_PULLUP);
  pinMode(pinoBotaoSaida, INPUT_PULLUP);
  pinMode(pinoSensorPorta, INPUT_PULLUP);
  pinMode(pinoSensorPresenca1, INPUT_PULLUP);
  pinMode(pinoSensorPresenca2, INPUT_PULLUP);

  pinMode(pinoMotor, OUTPUT);
  pinMode(pinoLedBranco, OUTPUT);

  desligarMotor();
  digitalWrite(pinoLedBranco, LOW);
}

void loop() {
  // Verifica se a câmara está ligada
  if (digitalRead(pinoChave) == LOW) {
    Serial.println("Câmara ligada");

    // Verifica se botão abrir foi pressionado
    if (digitalRead(pinoBotaoAbrir) == LOW) {
      abrirPorta();
    }

    // Verifica se botão de saída foi pressionado
    if (digitalRead(pinoBotaoSaida) == LOW) {
      abrirPorta();
    }

    // Verificação contínua de sensores
    bool portaFechada = digitalRead(pinoSensorPorta) == LOW;
    bool presenca1 = digitalRead(pinoSensorPresenca1) == LOW;
    bool presenca2 = digitalRead(pinoSensorPresenca2) == LOW;

    if (!portaFechada && !presenca1 && !presenca2) {
      Serial.println("Nenhum sensor detectado. Esperando 5s para fechar...");
      delay(5000);
      fecharPorta();
      digitalWrite(pinoLedBranco, LOW);
    } else if (!portaFechada && presenca1 && presenca2) {
      Serial.println("Dois sensores detectando. Porta permanece aberta por segurança.");
      desligarMotor();
      digitalWrite(pinoLedBranco, HIGH);
    } else if (!portaFechada && (presenca1 || presenca2)) {
      Serial.println("Presença parcial detectada. Aguardando liberação total.");
    }

  } else {
    Serial.println("Câmara desligada.");
    desligarMotor();
    digitalWrite(pinoLedBranco, LOW);
    delay(1000);
  }

  delay(200);
}

void abrirPorta() {
  Serial.println("Abrindo porta...");
  ligarMotor();
  portaAberta = true;
}

void fecharPorta() {
  Serial.println("Fechando porta...");
  // Simula fechamento 70%
  delay(1000); // tempo simbólico
  desligarMotor();
  digitalWrite(pinoLedBranco, HIGH); // LED acende indicando quase fechado
  delay(5000); // espera mais 5s para simular fechamento total
  portaAberta = false;
}

void ligarMotor() {
  digitalWrite(pinoMotor, HIGH);
  motorLigado = true;
  Serial.println("Motor ligado.");
}

void desligarMotor() {
  digitalWrite(pinoMotor, LOW);
  motorLigado = false;
  Serial.println("Motor desligado.");
}
