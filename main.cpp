#include <iostream>
#include <string.h>

using namespace std;


// IP repalce and reverse

const uint8_t replaceIP[64] = {
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4, 
  62, 54, 46, 38, 30, 22, 14, 6, 
  64, 56, 48, 40, 32, 24, 16, 8, 
  57, 49, 41, 33, 25, 17, 9, 1, 
  59, 51, 43, 35, 27, 19, 11, 3, 
  61, 53, 45, 37, 29, 21, 13, 5, 
  63, 55, 47, 39, 31, 23, 15, 7
};

const uint8_t replaceIPReverse[64] = {
  40, 8, 48, 16, 56, 24, 64, 32, 
  39, 7, 47, 15, 55, 23, 63, 31, 
  38, 6, 46, 14, 54, 22, 62, 30, 
  37, 5, 45, 13, 53, 21, 61, 29, 
  36, 4, 44, 12, 52, 20, 60, 28, 
  35, 3, 43, 11, 51, 19, 59, 27, 
  34, 2, 42, 10, 50, 18, 58, 26, 
  33, 1, 41, 9, 49, 17, 57, 25
};

// E
const uint8_t E[48] = {
  32, 1, 2, 3, 4, 5, 
  4, 5, 6, 7, 8, 9, 
  8, 9, 10, 11, 12, 13, 
  12, 13, 14, 15, 16, 17, 
  16, 17, 18, 19, 20, 21, 
  20, 21, 22, 23, 24, 25, 
  24, 25, 26, 27, 28, 29, 
  28, 29, 30, 31, 32, 1
};

// P
const uint8_t P[32] = {
  16, 7, 20, 21, 
  29, 12, 28, 17, 
  1, 15, 23, 26, 
  5, 18, 31, 10, 
  2, 8, 24, 14, 
  32, 27, 3, 9, 
  19, 13, 30, 6, 
  22, 11, 4, 25
};

// S boxs

const uint8_t S[8][64] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
      0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
      4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
      15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},

    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
      3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
      0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
      13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},

    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
      13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
      13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
      1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},

    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
      13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
      10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
      3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},

    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
      14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
      4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
      11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},

    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
      10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
      9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
      4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},

    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
      13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
      1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
      6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},

    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
      1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
      7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
      2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
};

// 64 bit round key * 16 rounds

uint8_t roundKey[16][48];

// round key the frist repalce (left)

const uint8_t replaceOneLeft[28] = {
  57, 49, 41, 33, 25, 17, 9, 
  1, 58, 50, 42, 34, 26, 18, 
  10, 2, 59, 51, 43, 35, 27, 
  19, 11, 3, 60, 52, 44, 36
};

// round key the frist repalce (right)

const uint8_t replaceOneRight[28] = {
  63, 55, 47, 39, 31, 33, 15,
  7, 62, 54, 46, 38, 30, 22,
  14, 6, 61, 53, 45, 37, 29,
  21, 13, 5, 28, 20, 12, 4
};

// replace 2 box

const uint8_t replaceTwo[48] = {
  14, 17, 11, 24, 1, 5,
  3, 28, 15, 6, 21, 10,
  23, 19, 12, 4, 26, 8,
  16, 7, 27, 20, 13, 2,
  41, 52, 31, 37, 47, 55,
  30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53,
  46, 42, 50, 36, 29, 32
};

// loop steps

const int loopStep[16] = {
  1, 1, 2, 2, 2, 2, 2, 2,
  1, 2, 2, 2, 2, 2, 2, 1
};


/*
  reverse decimal to binary
*/

const uint8_t dtob[16][4] = {
  {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1},
  {0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1},
  {1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1},
  {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1},
};


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

  const uint8_t tmpMask = 0x80;

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


int main(){
  uint8_t plaintext[8] = {0x12, 0x34, 0x56, 0xAB, 0xCD, 0x13, 0x25, 0x36};
  uint8_t key[8] = {0xAA, 0xBB, 0x09, 0x18, 0x27, 0x36, 0xCC, 0xDD};
  uint8_t ciphertext[8];

  for(int i = 0; i < 1000000; i++) {
    enCipher(plaintext, key, ciphertext);
  }

  for(int i = 0; i < 8; i++) {
    cout << hex << (int)ciphertext[i] << " ";
  }
  cout << endl;

  return 0;
}
