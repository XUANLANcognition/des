#include <iostream>
#include <string.h>
#include "des.h"

using namespace std;

int main(){
  // 明文
  uint8_t plaintext[8] = {0x12, 0x34, 0x56, 0xAB, 0xCD, 0x13, 0x25, 0x36};
  // 密钥
  uint8_t key[8] = {0xAA, 0xBB, 0x09, 0x18, 0x27, 0x36, 0xCC, 0xDD};
  // 密文
  uint8_t ciphertext[8];

  enCipher(plaintext, key, ciphertext);

  for(int i = 0; i < 8; i++) {
    cout << hex << (int)ciphertext[i] << " ";
  }
  cout << endl;

  return 0;
}
