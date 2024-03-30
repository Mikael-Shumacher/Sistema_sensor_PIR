#include<WiFi.h>
#include<HTTPClient.h>

#define sensorPIR 3
#define chaveAPI "Chave_da_sua_API"
#define canal "Numero_do_seu_canal"
#define token "Seu_token"

char rede[] = "Nome_da_rede";
char senha[] = "senha_da_rede";

int presenca = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPIR, INPUT);
  WiFi.begin(rede, senha);
}

void loop() {
  presenca = digitalRead(sensorPIR);
  HTTPClient http;
  if(digitalRead(0) == LOW){
    Serial.println("Botao de desligamento foi acionado, em 3 segundos o sistema ira desligar");
    delay(3000);
    desligarSistema();
    delay(5000);
  }
  if(presenca == HIGH){
    Serial.println("Presença detectada!!");
    enviarsite();
    enviarnotificacao();
    delay(5000);
  }
  String url = String("http://api.thingspeak.com/update?api_key=Chave_da_sua_API=" + presenca);
  http.begin(url);
  http.end();
  delay(1000);
}

void enviarsite(){
  HTTPClient http;
  String url1 = "http://api.thingspeak.com/update?api_key=47YYIYPKW47f03H4M&field1=1"; //Troque os valores para sua chave de API
  http.begin(url1);
  int codigo = http.GET();
  http.end();
  Serial.println("Solicitação enviada para o protocolo. Codigo de resposta: " + String(codigo));
}

void enviarnotificacao(){
  HTTPClient http2;
  String url2 = "https://api.pushbullet.com/v2/pushes";
  http2.begin(url2);
  http2.addHeader("Content-Type", "application/json");
  http2.addHeader("Access-Token", token);
  String payload = "{\"type\":\"note\",\"title\":\"Sistema de segurança\", \"body\":\"Presença detectada - o sensor de presença detectou algo!\"}";
  int codigo = http2.POST(payload);
  http2.end();

  Serial.println("Notificaçao enviada. Codigo de resposta: " + String(codigo));
}

void desligarSistema(){
  HTTPClient http2;
  String url2 = "https://api.pushbullet.com/v2/pushes";
  http2.begin(url2);
  http2.addHeader("Content-Type", "application/json");
  http2.addHeader("Access-Token", token);
  String payload = "{\"type\":\"note\",\"title\":\"Sistema de segurança\", \"body\":\"O sistema está sendo desligado....\"}";
  int codigo = http2.POST(payload);
  http2.end();
  digitalWrite(sensorPIR, LOW);
  WiFi.disconnect(true);
  Serial.println("Sistema desativado com sucesso!");
}
