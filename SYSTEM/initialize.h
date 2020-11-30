#ifndef INITIALIZE_H
#define INITIALIZE_H

#define BANK0_SET				(INSCON &= (~BIT6))
#define BANK1_SET				(INSCON |= BIT6)

#define DEBUG_VERSION			0						//控制板调试版本号

#define RELEASE_VERSION			10						//控制板发行版本号

#define FLASH_CRL				0						//操作FLASH
#define EEPROM_CRL				1						//操作EEPROM

void InitOscilator(void);
void InitWdt(void);
void InitIo(void);
void InitTimer5(void);
void InitCharArrayData(unsigned char *buf,unsigned char len);
//void InitIntArrayData(unsigned int *buf,unsigned char len);
void SectorErase(unsigned int sectorAddr,bit sectorSelect);
void SectorWrite(unsigned int sectorAddr,unsigned char byteData,bit sectorSelect);
unsigned char SectorRead(unsigned int sectorAddr,bit sectorSelect);
void InitLPD(void);
void InitVariable(void);
void InitPriority(void);
void InitAll(void);

#endif