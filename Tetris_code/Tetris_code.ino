#include <LedControl.h>
#include <TimerOne.h>
#include "rutina_piezas.h"

#define Boton_A 4
#define Boton_B 5
#define up 6
#define down 7
#define right 9
#define left 8

#define MAX_i 8
#define MAX_j 16

int dato[MAX_i][MAX_j] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

unsigned long previousMillis;
unsigned long interval = 300;

int puntuacion = 0;
LedControl lc = LedControl(12, 11, 10, 3);

void muestra_puntuacion(int digit) {
  puntuacion = digit;
  lc.setDigit(2, 3, digit % 10, false);
  lc.setDigit(2, 2, (digit / 10) % 10, false);
  lc.setDigit(2, 1, (digit / 100) % 10, false);
  lc.setDigit(2, 0, (digit / 1000) % 10, false);
}


double datoToHEX(int i) {
  double datoHEX = 0;
  int incremento = 0;
  for (int j = MAX_i - 1; j >= 0; j--) {
    datoHEX = datoHEX + dato[j][i] * pow(2, incremento);
    incremento++;
  }
  if (datoHEX > 3) datoHEX++;
  return datoHEX;
}

void refresh_(double Matrix[16]) {

  for (unsigned i = 0; i < MAX_j; i++) {
    if (i < 8) lc.setColumn(0, i, Matrix[i]);
    else  lc.setColumn(1, i - 8, Matrix[i]);
  }
}


void Clear_matriz() {
  for (unsigned i = 0; i < MAX_i; i++) {
    for (unsigned j = 0; j < MAX_j; j++) {
      dato[i][j] = 0;
    }
  }
}


void linea() {

  int lineas[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int j = MAX_j - 1; j >= 0; j--) {
    for (int i = 0; i < MAX_i; i++) {
      if (dato[i][j] == 1) {
        lineas[j]++;
        if (lineas[j] == 8) {
          for (int k = j; k >= 0; k--) {
            for (int l = 0; l < MAX_i; l++) {
              dato[l][k] = dato[l][k - 1];
              if (k == 0) dato[l][k] = 0;
            }
          }
          j = MAX_j; i = 0;
          for (int i = 0; i < 16; i++) lineas[i] = 0;
          if (interval > 50) interval = interval - 10;
          puntuacion = puntuacion + 25;
        }
      }
    }
  }
  muestra_puntuacion(puntuacion);

}

long T_setup = 150;
volatile unsigned long last_micros = 0;
volatile char letra_leida;
void botonera_derecha(byte boton) {
  switch (boton) {
    case Boton_A:
      letra_leida = 'w';
      break;
    case Boton_B:

      break;
  }

}
void botonera_izquierda(byte boton) {


  switch (boton) {
    case up:
      break;
    case down:
      break;
    case right:
      letra_leida = 'a';

      break;
    case left:
      letra_leida = 'd';

      break;

  }
}

void shifting_2() {

  if ((long)(micros() - last_micros) > (T_setup * 1000)) {
    if (digitalRead(Boton_A) == LOW)botonera_derecha(Boton_A);
    else if (digitalRead(Boton_B) == LOW)botonera_derecha(Boton_B);
    last_micros = micros();

  }
}


void shifting_1() {
  if ((long)(micros() - last_micros) > (T_setup * 1000)) {
    if (digitalRead(left) == LOW)botonera_izquierda(left);
    else if (digitalRead(right) == LOW)botonera_izquierda(right);
    else if (digitalRead(up) == LOW)botonera_izquierda(up);
    else if (digitalRead(down) == LOW)botonera_izquierda(down);
    last_micros = micros();
  }
}


void setup() {
  for (unsigned i = 0; i < 8; i++) pinMode(i, INPUT);
  pinMode(A0, OUTPUT);

  for (int i = 0; i < 3; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
  }
  for (int i = 0; i < 4; i++) lc.setDigit(2, i, 0, false);

  digitalWrite(A0, LOW);
  attachInterrupt(digitalPinToInterrupt(2), shifting_2, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), shifting_1, FALLING);
  /* Timer1.initialize(150000);
    Timer1.attachInterrupt(music);*/

  Serial.begin(9600);

}

int thisNote = 0;
void music() {
  int noteDuration = 1000 / noteDurations[thisNote];
  tone(A0, melody[thisNote], noteDuration);

  /*
    to distinguish the notes, set a minimum time between them.
    the note's duration + 30% seems to work well:
  */

  if ( thisNote > 300)thisNote = 0;
  //stop the tone playing:
  thisNote++;


}

void loop() {
  double matrixToMax[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  bool comprobacion;
  int Tid;

  if (!pieza_activa) {//Si no hay ninguna cayendo crea otra
    coordenadas = 0;
    linea();

    for (unsigned i = 0; i < MAX_j; i++)matrixToMax[i] = datoToHEX(i);
    refresh_(matrixToMax);

    if (dato[3][0] || dato[4][0] || dato[5][0] || dato[3][1] || dato[4][1] || dato[5][1]) {
      interval = 300;
      puntuacion = 0;
      Clear_matriz();
      puntuacion = 0;
      muestra_puntuacion(puntuacion);

    }

    pieza_pos_j = 0;
    rotado = 0;

    Tid = random(7);
    struct piezas piezas_1;
    piezas_1.pieza_mostrada(Tid);
    if (Tid == 0) palo_activo = 1;
    else palo_activo = 0;
    pieza_activa = true;
    previousMillis = millis();
    for (int i = 0; i < 8; i++) coordenadas_anteriores[i] = 0;
    pieza_pos_i = 3;
    desplazamiento = 0;

  }



  //ROTAR------------------------------------------------------------------------------------------------
  noInterrupts();
  if (letra_leida == 'w' ) {

    if (excepciones_rotar()) {

      rotar();
      borrar_anterior(dato);
      comprobacion = comprobacion_rotar(dato);

      if (comprobacion)  escribir_pieza_rotada(dato);//Si pasa la comprobacion la roto
      else escribir_pieza_no_rotada(dato);//La dejo donde estaba

    }
    letra_leida = 0;

  }
  interrupts();

  for (unsigned i = 0; i < MAX_j; i++)matrixToMax[i] = datoToHEX(i);
  refresh_(matrixToMax);
  delay(10);




  //LATERAL-----------------------------------------------------------------------------------------------------
  noInterrupts();
  if ((letra_leida == 'a' || letra_leida == 'd')) {

    if (palo_activo && rotado) {
      switch (letra_leida) {
        case 'a':
          if (pieza_pos_i < 6) {
            pieza_pos_i++;
            desplazamiento++;
          }
          break;
        case 'd':
          if (pieza_pos_i > -1) {
            pieza_pos_i--;
            desplazamiento--;
          }
          break;

      }
    }
    else {
      switch (letra_leida) {
        case 'a':
          if (pieza_pos_i < (8 - ancho())) {
            pieza_pos_i++;
            desplazamiento++;
          }
          break;
        case 'd':
          if (pieza_pos_i > 0) {
            pieza_pos_i--;
            desplazamiento--;
          }
          break;
      }
    }
    letra_leida = 0;
    comprobacion = comprobacion_lateral(dato);

    if (comprobacion) {
      borrar_anterior(dato);
      escribir_desplazado(dato);
    }

    desplazamiento = 0;

  }
  interrupts();
  for (unsigned i = 0; i < MAX_j; i++)matrixToMax[i] = datoToHEX(i);
  refresh_(matrixToMax);



  //VERTICAL--------------------------------------------------------------------------------
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    borrar_anterior(dato);//Borro su posicion actual

    comprobacion = comprobacion_bajar(dato);

    if (comprobacion) escribir_pieza_bajada(dato);//Si puede bajar lo escribe en dato
    else escribir_pieza_no_bajada(dato);//Si no puede bajar la dejo donde estaba

    pieza_pos_j++;
    desplazamiento = 0;
    coordenadas = 0;

    if (pieza_pos_j == 15 || pieza_activa == false || comprobacion == 0) {//esta pieza ya ha terminado de bajar, pasamos a la siguiente
      pieza_activa = false;
      pieza_pos_j = 0;//reseteamos  la posicion para la nueva pieza
      puntuacion++;
      muestra_puntuacion(puntuacion);
    }

  }
  for (unsigned i = 0; i < MAX_j; i++)matrixToMax[i] = datoToHEX(i);
  refresh_(matrixToMax);


}

