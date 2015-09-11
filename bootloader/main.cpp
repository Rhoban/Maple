#include <stdlib.h>
#include <wirish/wirish.h>
#include <servos.h>
#include <main.h>
/**
 * Robotis OpenCM9.04 bootloader fixer
 */

#include "bootloader.h"

typedef unsigned short       u16;
typedef volatile unsigned short       vu16;
typedef unsigned long       u32;
typedef volatile unsigned long       vu32;
typedef unsigned long       ui32;

#define BOOTLOADER_FLASH   ((u32)0x08000000)
#define PAGE_SIZE          1024

#define SET_REG(addr,val) do { *(vu32*)(addr)=val; } while(0)
#define GET_REG(addr)     (*(vu32*)(addr))


#define RCC   ((u32)0x40021000)
#define FLASH ((u32)0x40022000)

#define RCC_CR      RCC
#define RCC_CFGR    (RCC + 0x04)
#define RCC_CIR     (RCC + 0x08)
#define RCC_AHBENR  (RCC + 0x14)
#define RCC_APB2ENR (RCC + 0x18)
#define RCC_APB1ENR (RCC + 0x1C)

#define FLASH_ACR     (FLASH + 0x00)
#define FLASH_KEYR    (FLASH + 0x04)
#define FLASH_OPTKEYR (FLASH + 0x08)
#define FLASH_SR      (FLASH + 0x0C)
#define FLASH_CR      (FLASH + 0x10)
#define FLASH_AR      (FLASH + 0x14)
#define FLASH_OBR     (FLASH + 0x1C)
#define FLASH_WRPR    (FLASH + 0x20)

#define FLASH_KEY1     0x45670123
#define FLASH_KEY2     0xCDEF89AB
#define FLASH_RDPRT    0x00A5
#define FLASH_SR_BSY   0x01
#define FLASH_CR_PER   0x02
#define FLASH_CR_PG    0x01
#define FLASH_CR_START 0x40

typedef struct {
    vu32 CR; 
    vu32 CFGR;
    vu32 CIR;
    vu32 APB2RSTR;
    vu32 APB1RSTR;
    vu32 AHBENR;
    vu32 APB2ENR;
    vu32 APB1ENR;
    vu32 BDCR;
    vu32 CSR;
} RCC_RegStruct;
#define pRCC ((RCC_RegStruct *) RCC)


bool flashErasePage(u32 pageAddr) {
    u32 rwmVal = GET_REG(FLASH_CR);
    rwmVal = FLASH_CR_PER;
    SET_REG(FLASH_CR, rwmVal);

    while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}
    SET_REG(FLASH_AR, pageAddr);
    SET_REG(FLASH_CR, FLASH_CR_START | FLASH_CR_PER);
    while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}

    /* todo: verify the page was erased */

    rwmVal = 0x00;
    SET_REG(FLASH_CR, rwmVal);

    return true;
}

bool flashErasePages(u32 pageAddr, u16 n) {
    while (n-- > 0) {
        if (!flashErasePage(pageAddr + 0x400 * n)) {
            return false;
        }
    }

    return true;
}

bool flashWriteWord(u32 addr, u32 word) {
    vu16 *flashAddr = (vu16 *)addr;
    vu32 lhWord = (vu32)word & 0x0000FFFF;
    vu32 hhWord = ((vu32)word & 0xFFFF0000) >> 16;

    u32 rwmVal = GET_REG(FLASH_CR);
    SET_REG(FLASH_CR, FLASH_CR_PG);

    /* apparently we need not write to FLASH_AR and can
       simply do a native write of a half word */
    while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}
    *(flashAddr + 0x01) = (vu16)hhWord;
    while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}
    *(flashAddr) = (vu16)lhWord;
    while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}

    rwmVal &= 0xFFFFFFFE;
    SET_REG(FLASH_CR, rwmVal);

    /* verify the write */
    if (*(vu32 *)addr != word) {
        return false;
    }

    return true;
}

void flashLock() {
    /* take down the HSI oscillator? it may be in use elsewhere */

    /* ensure all FPEC functions disabled and lock the FPEC */
    SET_REG(FLASH_CR, 0x00000080);
}

void flashUnlock() {
    /* unlock the flash */
    SET_REG(FLASH_KEYR, FLASH_KEY1);
    SET_REG(FLASH_KEYR, FLASH_KEY2);
}

/* Minimum_Source*/

void setupFLASH() {
    /* configure the HSI oscillator */
    if ((pRCC->CR & 0x01) == 0x00) {
        u32 rwmVal = pRCC->CR;
        rwmVal |= 0x01;
        pRCC->CR = rwmVal;
    }   

    /* wait for it to come on */
    while ((pRCC->CR & 0x02) == 0x00) {}
}

bool writeChunk(u32 *ptr, int size, const char *data)
{
     flashErasePage((u32)(ptr));
     
     for (int i = 0; i<size; i = i + 4) {
       if (!flashWriteWord((u32)(ptr++), *((u32 *)(data + i)))) {
        return false; 
       }
     }
     
     return true;
}

void setup() {
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, LOW);
  
  setupFLASH();
  flashUnlock();
  
  int n = sizeof(bootloader);
  
  for (int i=0; i<n; i+=PAGE_SIZE) {
     int size = 0;
     u32* chunk = (u32 *)(BOOTLOADER_FLASH + i);
     
     size = n-i;
     if (size > PAGE_SIZE) size = PAGE_SIZE;
     
     if (!writeChunk(chunk, size, &bootloader[i])) {
      break;
     }
  }

  flashLock();
}

void loop() {
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(500);
  digitalWrite(BOARD_LED_PIN, HIGH);  
  delay(500);
}

