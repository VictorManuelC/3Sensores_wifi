#include <DHT.h>
#include <ESP8266WiFi.h>

char ssid[] = "Demonic";
char password[] = "1234cinco678nueve";
 
// CONSTRUCTOR DEL OBJETO DHT RECIBE EL PIN EN EL QUE SE CONECTA EL SENSOR
// Y TAMBIEN RECIBE EL TIPO DE SENSOR QUE VAMOS A CONECTAR
DHT dht(D7, DHT11);

const int LEDPin = D1;        // pin para el LED
const int PIRPin = D8;         // pin de entrada (for PIR sensor)
 
int pirState = LOW;           // de inicio no hay movimiento
int val = 0;  

 
void setup() {
   Serial.begin(9600);
  Serial.println("Wifi test!");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  
  // CONFIGURAR PINES DE ENTRADA Y SALIDA
  pinMode(LEDPin, OUTPUT); 
  pinMode(PIRPin, INPUT);
  // PREPARAR LA COMUNICACION SERIAL
  Serial.begin(9600);
  
  
  // PREPARAR LA LIBRERIA PARA COMUNICARSE CON EL SENSOR
  dht.begin();
}
 
void loop() { 
  temperatura();

  movimiento();

  suelohumedad();
}

void temperatura(){
   // ESPERAR ENTRE MEDICIONES, NECESARIO PARA EL BUEN FUNCIONAMIENTO
  delay(2000);
  
  // LEER LA HUMEDAD USANDO EL METRODO READHUMIDITY
  float h = dht.readHumidity();
  // LEER LA TEMPERATURA USANDO EL METRODO READTEMPERATURE
  float t = dht.readTemperature();
 
  // REVISAR QUE LOS RESULTADOS SEAN VALORES NUMERICOS VALIDOS, INDICANDO QUE LA COMUNICACION ES CORRECTA
  if (isnan(h) || isnan(t)) {
    Serial.println("Falla al leer el sensor DHT11!");
    return;
  }
 
  // IMPRIMIR RESULTADO AL MONITOR SERIAL
  Serial.print("Humedad ambiente: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");
}


void movimiento(){
  val = digitalRead(PIRPin);
   if (val == HIGH)   //si está activado
   { 
      digitalWrite(LEDPin, HIGH);  //LED ON
      if (pirState == LOW)  //si previamente estaba apagado
      {
        Serial.println("Sensor activado");
        pirState = HIGH;
      }
   } 
   else   //si esta desactivado
   {
      digitalWrite(LEDPin, LOW); // LED OFF
      if (pirState == HIGH)  //si previamente estaba encendido
      {
        Serial.println("Sensor parado");
        pirState = LOW;
      }
   }
}

void suelohumedad(){
   int lectura = analogRead(A0);
   
     delay(1000);

   //Convirtiendo a Porcentaje
   int lecturaPorcentaje = map(lectura, 1023, 0, 0, 100);

   Serial.print("La Humedad del suelo es del: ");
   Serial.print(lecturaPorcentaje);
   Serial.println("%");
}
