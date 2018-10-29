#include "des.h"

// 64 bit round key * 16 rounds

uint8_t roundKey[16][48];

// round key the frist repalce (left)

/* 
  t1 origin
  t2 result
  t3 result length
  t4 box
*/

void replaceBit(uint8_t *t1, uint8_t *t2, int t3, const uint8_t *t4) {

  uint8_t tmpMask = 0x80;

  for (int i = 0; i < t3; i++) {
      uint8_t tmp;

      tmp = t1[(t4[i] - 1) / 8] &  // no end
        (tmpMask >> ((t4[i] - 1) % 8));
      if(tmp != 0) {
        t2[i] = 1;
      }
      else {
        t2[i] = 0;
      }
    }
}


void replace(uint8_t *t1, uint8_t *t2, int t3, const uint8_t *t4) {

  uint8_t tmp;

  for(int i = 0; i < t3; i++) {
    tmp = t1[t4[i] - 1];
    if(tmp != 0) {
      t2[i] = 1;
    }
    else {
      t2[i] = 0;
    }
  }
}


/*
  t1  in1
  t2  in2
  t3  out
  l  length
*/

void XOR(uint8_t *t1, uint8_t *t2, uint8_t *t3, int l) {
  for(int i = 0; i < l; i++) {
    if(t1[i] != t2[i]) {
      t3[i] = 1;
    }
    else {
      t3[i] = 0;
    }
  }
}

/*
  t1  origin
  t2  origin length
  t3  step
*/

void moveLeft(uint8_t *t1, int t2, int t3) {
  uint8_t tmp;

  for(int i = 0; i < t3; i++) {
    tmp = t1[0];
    
    for(int j = 0; j < t2 - 1; j++) {
      t1[j] = t1[j + 1];
    }
    
    t1[t2 - 1] = tmp;
  }
}

/*
  l left
  t1 left length
  r right
  t2 right length
  b both
*/

void PasteLeftRight(uint8_t *l, int t1, uint8_t *r, int t2, uint8_t *b) {
  for(int i = 0; i < t1; i++) {
    b[i] = l[i];
  }
  for(int i = t1; i < t1 + t2; i++) {
    b[i] = r[i - t1];
  }
}


void geneRoundKey(uint8_t initKey[8]) {

  uint8_t tmpLeft[28];
  uint8_t tmpRight[28];
  uint8_t tmpBoth[56];

  // init 

  memset(tmpLeft, 0x00, sizeof(tmpLeft));

  memset(tmpRight, 0x00, sizeof(tmpRight));

  memset(roundKey, 0x00, sizeof(roundKey));

  // frist replace and choose

  replaceBit(initKey, tmpLeft, 28, replaceOneLeft);
  replaceBit(initKey, tmpRight, 28, replaceOneRight);

  // ROUND start! (16 rounds)

  for(int round = 0; round < 16; round++) {
    moveLeft(tmpLeft, 28, loopStep[round]);
    moveLeft(tmpRight, 28, loopStep[round]);
    PasteLeftRight(tmpLeft, 28, tmpRight, 28, tmpBoth);
    replace(tmpBoth, roundKey[round], 48, replaceTwo);
  }
}


/*
  dec to bin
  d decimal
  b binary
*/

void decTobin(uint8_t d[8], uint8_t b[32]) {
  for (int i = 0; i < 8; i++) {
    memcpy(b + i * 4 , dtob[d[i]], 4 * sizeof(uint8_t));
  }
}


/*
  t1  in
  t2  out
*/

void replaceS(uint8_t *t1, uint8_t *t2) {
  int tmp1, tmp2;
  uint8_t tmp[8];
  for(int i = 0; i < 8; i++) {
    tmp1 = t1[i * 6] * 32 + t1[i * 6 + 5] * 16;
    tmp2 = t1[i * 6 + 1] * 8 + t1[i * 6 + 2] * 4 + t1[i * 6 + 3] * 2 + 
    t1[i * 6 + 4] * 1;
    tmp1 = tmp1 + tmp2;
    tmp[i] = S[i][tmp1];
  }
  decTobin(tmp, t2);
}


/*
********************************************************************************
  p plaintext
  k key
  c ciphertext
********************************************************************************
*/

void enCipher(uint8_t p[8], uint8_t k[8], uint8_t c[8]) {

  uint8_t tmpLeft[32];
  uint8_t tmpRight[32];
  uint8_t tmpMid[32];
  uint8_t tmpMid1[32];
  uint8_t tmpBoth[64];
  uint8_t tmpBothMid[64];
  uint8_t tmpF[48];

  // geneary round keys
  geneRoundKey(k);
  // IP repalce
  replaceBit(p, tmpBoth, 64, replaceIP);
  memcpy(tmpLeft, tmpBoth, 32 * sizeof(uint8_t));
  memcpy(tmpRight, tmpBoth + 32, 32 * sizeof(uint8_t));

  // round start! (15 + 1)

  for(int round = 0; round < 15; round++) {

    replace(tmpRight, tmpF, 48, E);

    XOR(tmpF, roundKey[round], tmpF, 48);

    replaceS(tmpF, tmpMid);

    replace(tmpMid, tmpMid1, 32, P);

    XOR(tmpLeft, tmpMid1, tmpMid1, 32);

    memcpy(tmpLeft, tmpRight, 32);

    memcpy(tmpRight, tmpMid1, 32);

  }

  replace(tmpRight, tmpF, 48, E);
  XOR(tmpF, roundKey[15], tmpF, 48);
  replaceS(tmpF, tmpMid);
  replace(tmpMid, tmpMid1, 32, P);
  XOR(tmpLeft, tmpMid1, tmpLeft, 32);

  memcpy(tmpBoth, tmpLeft, 32);
  memcpy(tmpBoth + 32, tmpRight, 32);

  replace(tmpBoth, tmpBothMid, 64, replaceIPReverse);

  for (int i = 0; i < 64; i += 8)  {
    c[i / 8] =  tmpBothMid[i] * (1 << 7) 
    + tmpBothMid[i + 1] * (1 << 6) 
    + tmpBothMid[i + 2] * (1 << 5) 
    + tmpBothMid[i + 3] * (1 << 4) 
    + tmpBothMid[i + 4] * (1 << 3) 
    + tmpBothMid[i + 5] * (1 << 2) 
    + tmpBothMid[i + 6] * (1 << 1) 
    + tmpBothMid[i + 7] * (1 << 0);
  }

}
