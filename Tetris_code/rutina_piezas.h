bool pieza_activa = false;
int pieza_pos_j = 0;
int pieza_pos_i = 3;
int desplazamiento = 0;
int rotado = 0;
int palo_activo = 0;
int coordenadas = 0;

uint8_t pieza[3][3];
uint8_t pieza_[3][3];

struct piezas {
  uint8_t palo[3][3] = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
  uint8_t ele[3][3] = {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}};
  uint8_t ele_[3][3] = {{1, 1, 0}, {1, 0, 0}, {1, 0, 0}};
  uint8_t cuadrado[3][3] = {{1, 1, 0}, {1, 1, 0}, {0, 0, 0}};
  uint8_t te[3][3] = {{0, 1, 0}, {1, 1, 0}, {0, 1, 0}};
  uint8_t silla[3][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}};
  uint8_t silla_[3][3] = {{0, 1, 0}, {1, 1, 0}, {1, 0, 0}};

  uint8_t pieza_mostrada(int Id) {
    switch (Id) {
      case 0:
        for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)pieza[i][j] = palo[i][j];
        break;
      case 1:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = ele[i][j];
        break;
      case 2:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = ele_[i][j];
        break;
      case 3:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = cuadrado[i][j];
        break;
      case 4:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = te[i][j];
        break;
      case 5:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = silla[i][j];
        break;
      case 6:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = silla_[i][j];
        break;
      default:
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)pieza[i][j] = 0;
        break;
    }
  }

};


int coordenadas_anteriores[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void borrar_anterior(int matriz[8][16]) {

  matriz[coordenadas_anteriores[0]][coordenadas_anteriores[1]] = 0;
  matriz[coordenadas_anteriores[2]][coordenadas_anteriores[3]] = 0;
  matriz[coordenadas_anteriores[4]][coordenadas_anteriores[5]] = 0;
  matriz[coordenadas_anteriores[6]][coordenadas_anteriores[7]] = 0;

}

uint8_t ancho() {

  uint8_t num = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) if (pieza[i][j] == 1) {
        j = 3;
        num++;
      }
  }

  return num;

}

uint8_t alto() {

  uint8_t num = 0;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) if (pieza[i][j] == 1) {
        i = 3;
        num++;
      }
  }
  if (num == 1) num = 2;//El palo da problemas

  return num;

}

void rotar() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      pieza_[i][j] = pieza[i][j];
    }

  }
  if (palo_activo) {

    for (int i = 0; i < 3; i++) {
      pieza[0][i] = pieza_[i][0];
      pieza[1][i] = pieza_[i][1];
      pieza[2][i] = pieza_[i][2];
    }
    if (alto() == 3) pieza_pos_j++;
    if (alto() == 2) pieza_pos_j--;

    rotado = !rotado;

  }
  else {

    if (rotado == 0) {

      for (int i = 0; i < 3; i++) {
        pieza[1][i] = pieza_[i][0];
        pieza[0][i] = pieza_[i][1];
        pieza[2][i] = pieza_[i][2];
      }
      if (alto() == 3) pieza_pos_j++;
      rotado++;

    } else if (rotado == 1) {
      for (int i = 0; i < 3; i++) {
        pieza[2][i] = pieza_[i][0];
        pieza[1][i] = pieza_[i][1];
        pieza[0][i] = pieza_[i][2];
      }
      if (alto() == 2) pieza_pos_j--;
      rotado = 0;
    }
  }
}


bool comprobacion_bajar(int matriz[8][16]) {

  bool comprobacion = true;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      if (pieza[i][j]) {
        if ((matriz[i + pieza_pos_i][pieza_pos_j + j - (alto() - 2)] == 1)) comprobacion = false;
      }
    }
  }

  return comprobacion;
}
bool comprobacion_lateral(int matriz[8][16]) {

  bool comprobacion = true;
  if (pieza_pos_j <= 0) comprobacion = false;
  for (unsigned i = 0; i < alto(); i++) {
    if (desplazamiento == -1) if ((matriz[0 + pieza_pos_i - desplazamiento][pieza_pos_j - i] == 1) && (matriz[0 + pieza_pos_i][pieza_pos_j - i] == 1)) comprobacion = false;
    if (desplazamiento == 1) if ((matriz[ancho() - 1 + pieza_pos_i - desplazamiento][pieza_pos_j - i] == 1) && (matriz[ancho() - 1 + pieza_pos_i][pieza_pos_j - i] == 1)) comprobacion = false;

  }
  if (!comprobacion)  pieza_pos_i = pieza_pos_i - desplazamiento;

  return comprobacion;
}

bool comprobacion_rotar(int matriz[8][16]) {

  bool comprobacion = true;
  if (pieza_pos_j <= 0) comprobacion = false;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (pieza[i][j]) {
        if (matriz[i + pieza_pos_i][j + pieza_pos_j - (alto() - 1)]) comprobacion = false;
      }
    }
  }

  return comprobacion;
}


//NUEVAS-------------------------------------------------------------------------------

int excepciones_rotar() {

  int puede_rotar = 0;
  if (alto() == 3 || ancho() == 3)  //Si no es cuadrado
    if (pieza_pos_j != 14 || alto() != 2)  //Si no esta en el limite inferior
      if (pieza_pos_i != 6) //Si no esta en la derecha del todo
        if ((pieza_pos_i != -1) || (ancho() != 1)) //Si es el palo y no esta en la izquierda del todo
          if (pieza_pos_j > 0)
            puede_rotar = 1;

  return puede_rotar;

}

void escribir_pieza_rotada(int matriz[8][16]) {
  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {

      if (pieza[i][j]) {
        matriz[i + pieza_pos_i][j + pieza_pos_j - (alto() - 1)] = pieza[i][j];

        //guarda las coordenadas donde escribe la pieza
        coordenadas_anteriores[coordenadas] = i + pieza_pos_i;
        coordenadas++;
        coordenadas_anteriores[coordenadas] = j + pieza_pos_j - (alto() - 1); //-1 porque la posicion vertical la deja aumentada a la siguiente la funcion de bajar
        coordenadas++;

      }
    }
  }
  coordenadas = 0;
}

void escribir_pieza_no_rotada(int matriz[8][16]) {
  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {

      pieza[i][j] = pieza_[i][j];

      if (pieza[i][j]) {
        matriz[i + pieza_pos_i][j + pieza_pos_j - (alto() - 1)] = pieza[i][j];

        coordenadas_anteriores[coordenadas] = i + pieza_pos_i;
        coordenadas++;
        coordenadas_anteriores[coordenadas] = j + pieza_pos_j - (alto() - 1); //-1 porque la posicion vertical la deja aumentada a la siguiente la funcion de bajar
        coordenadas++;

      }
    }
  }
  coordenadas = 0;
  rotado = !rotado;
}

void escribir_desplazado(int matriz[8][16]) {
  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {

      if (pieza[i][j]) {
        matriz[i + pieza_pos_i][j + pieza_pos_j - (alto() - 1)] = pieza[i][j];

        coordenadas_anteriores[coordenadas] = i + pieza_pos_i;
        coordenadas++;
        coordenadas_anteriores[coordenadas] = j + pieza_pos_j - (alto() - 1); //-1 porque la posicion vertical la deja aumentada a la siguiente la funcion de bajar
        coordenadas++;

      }
    }
  }
  coordenadas = 0;
}

void escribir_pieza_bajada(int matriz[8][16]) {
  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {

      if (pieza[i][j]) {

        matriz[i + pieza_pos_i][j + pieza_pos_j - (alto() - 2)] = pieza[i][j];

        coordenadas_anteriores[coordenadas] = i + pieza_pos_i;
        coordenadas++;
        coordenadas_anteriores[coordenadas] = j + pieza_pos_j - (alto() - 2);
        coordenadas++;

      }
    }

  }
  coordenadas = 0;
}

void escribir_pieza_no_bajada(int matriz[8][16]) {
  matriz[coordenadas_anteriores[0]][coordenadas_anteriores[1]] = 1;
  matriz[coordenadas_anteriores[2]][coordenadas_anteriores[3]] = 1;
  matriz[coordenadas_anteriores[4]][coordenadas_anteriores[5]] = 1;
  if (!palo_activo) matriz[coordenadas_anteriores[6]][coordenadas_anteriores[7]] = 1;

}
