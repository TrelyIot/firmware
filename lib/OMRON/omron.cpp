#include <Arduino.h>
#include <Wire.h>

/* defines */
#define D6F_ADDR 0x6C  // D6F-PH I2C client address at 7bit expression
#define NUM_DATOS 30

void ordenarDatos(float datos[], int n) {
  // Función para ordenar un arreglo de datos flotantes usando el algoritmo de selección
  for (int i = 0; i < n - 1; i++) {
    int minIdx = i;
    for (int j = i + 1; j < n; j++) {
      if (datos[j] < datos[minIdx]) {
        minIdx = j;
      }
    }
    if (minIdx != i) {
      // Intercambiar los valores
      float temp = datos[i];
      datos[i] = datos[minIdx];
      datos[minIdx] = temp;
    }
  }
}

float calcularMediana(float datos[], int n) {
  // Función para calcular la mediana de un arreglo ordenado de datos flotantes
  if (n % 2 == 1) {
    // Si la cantidad de datos es impar, la mediana es el valor del medio
    return datos[n / 2];
  } else {
    // Si la cantidad de datos es par, la mediana es el promedio de los dos valores del medio
    return (datos[n / 2 - 1] + datos[n / 2]) / 2.0;
  }
}

uint8_t conv16_u8_h(int16_t a) {
  return (uint8_t)(a >> 8);
}

uint8_t conv16_u8_l(int16_t a) {
  return (uint8_t)(a & 0xFF);
}

uint16_t conv8us_u16_be(uint8_t* buf) {
  return (uint16_t)(((uint32_t)buf[0] << 8) | (uint32_t)buf[1]);
}

/** <!-- i2c_write_reg16 {{{1 --> I2C write bytes with a 16bit register.
 */
bool i2c_write_reg16(uint8_t slave_addr, uint16_t register_addr, uint8_t *write_buff, uint8_t len) {
  Wire.beginTransmission(slave_addr);

  Wire.write(conv16_u8_h(register_addr));
  Wire.write(conv16_u8_l(register_addr));

  if (len != 0) {
      for (uint8_t i = 0; i < len; i++) {
          Wire.write(write_buff[i]);
      }
  }
  Wire.endTransmission();
  return false;
}

/** <!-- i2c_read_reg8 {{{1 --> I2C read bytes with a 8bit register.
 */
bool i2c_read_reg8(uint8_t slave_addr, uint8_t register_addr, uint8_t *read_buff, uint8_t len) {
  Wire.beginTransmission(slave_addr);
  Wire.write(register_addr);
  Wire.endTransmission();

  // TODO Control error 
  Serial.print("Check: ");
  Serial.println(Wire.requestFrom(slave_addr, len));

  if (Wire.available() != len) {
    return true;
  }
  for (uint16_t i = 0; i < len; i++) {
    read_buff[i] = Wire.read();
  }
  return false;
}

float get_diff_press() {

  float datos[NUM_DATOS]; // Arreglo para almacenar los datos flotantes
  float valor;

  // Capturar 20 datos flotantes del pin A0
  for (int i = 0; i < NUM_DATOS; i++) {
    // 2. Trigger getting data (00h, D0h, 40h, 18h, 06h)
    uint8_t send0[] = {0x40, 0x18, 0x06};
    i2c_write_reg16(D6F_ADDR, 0x00D0, send0, 3);

    delay(50);  // wait 50ms
    
    // 3. Read data (00h, D0h, 51h, 2Ch) (07h)
    uint8_t send1[] = {0x51, 0x2C};
    i2c_write_reg16(D6F_ADDR, 0x00D0, send1, 2);
    uint8_t rbuf[2];
    
    if (i2c_read_reg8(D6F_ADDR, 0x07, rbuf, 2)) {  // read from [07h]
      return 0.00;
    }
    uint16_t rd_flow = conv8us_u16_be(rbuf);
    float flow_rate;
    
    // calculation for +/-500[Pa] range
    // flow_rate = ((float)rd_flow - 1024.0) * 1000.0 / 60000.0 - 500.0;
    flow_rate = (float)rd_flow - 31006.00;

    valor = fabs(flow_rate) * 2100 / 3800; // Leer valor analógico y convertir a voltaje
    datos[i] = valor;
    delay(100); // Esperar 100 ms entre cada lectura (ajusta según sea necesario)
  }

  // Ordenar el arreglo de datos
  ordenarDatos(datos, NUM_DATOS);

  // Calcular la mediana
  float mediana = calcularMediana(datos, NUM_DATOS);

  // Imprimir la mediana
  
  // Serial.print(mediana, 2); // Imprimir con 4 decimales
  // Serial.println(";");

  return mediana;
}