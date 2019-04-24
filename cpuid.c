#include <stdio.h>
#include <stdint.h>

//Get Bender Name
char* getBender(char* target) {
  uint32_t ebx, ecx, edx;
  asm volatile ("movl $0, %%eax\n"
                "cpuid\n"
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                "movl %%edx, %2\n"
               :"=m"(ebx), "=m"(ecx), "=m"(edx)
               :
               :"%eax", "%ebx", "%ecx", "%edx");
  for (int32_t i = 0; i < 4; i++) {
    target[3-i] = ebx << (8*i) >> 24;
  }
  for (int32_t i = 0; i < 4; i++) {
    target[7-i] = edx << (8*i) >> 24;
  }
  for (int32_t i = 0; i < 4; i++) {
    target[11-i] = ecx << (8*i) >> 24;
  }
  return target;
}

//Test to get Brand
int isSupportBrand(void) {
  uint32_t ext_max = 0;
  uint32_t ope = 0x80000000;
  asm volatile ("movl %1, %%eax\n"
                "cpuid\n"
                "movl %%eax, %0\n"
               :"=m"(ext_max)
               :"m"(ope)
               :"%eax", "%ebx", "%ecx", "%edx");
  return (ext_max >= 0x80000004);
}
//Get Processor Brand String
char* getBrand(char* target) {
  uint32_t string[12] = {};
  for (uint32_t i = 0; i < 3; i++) {
    uint32_t ope = 0x80000002 + i;
    asm volatile ("movl %4, %%eax\n"
                  "cpuid\n"
                  "movl %%eax, %0\n"
                  "movl %%ebx, %1\n"
                  "movl %%ecx, %2\n"
                  "movl %%edx, %3\n"
                  :"=m"(string[i*4]), "=m"(string[i*4+1]), "=m"(string[i*4+2]), "=m"(string[i*4+3])
                  :"m"(ope)
                  :"%eax", "%ebx", "%ecx", "%edx");
    for (uint32_t j = 0; j < 4; j++) {
      target[i*16+(3-j)]    = string[i*4]   << (j*8) >> 24;
      target[i*16+(3-j)+4]  = string[i*4+1] << (j*8) >> 24;
      target[i*16+(3-j)+8]  = string[i*4+2] << (j*8) >> 24;
      target[i*16+(3-j)+12] = string[i*4+3] << (j*8) >> 24;
    }
  }
  return target;
}

int main(void) {
  char bender[13] = {};
  getBender(bender);
  printf("Bender: %s\n", bender);
  if (isSupportBrand()) {
    char brand[49] = {};
    getBrand(brand);
    printf("Processor: %s\n", brand);
  } else {
    printf("Can't get Processor Brand String.\n");
  }
  return 0;
}