#include "../ili9341/ILI9341.h"

volatile uint32_t i;

sendCommand(0xEF);
sendData(0x03);
sendData(0x80);
sendData(0x02);

sendCommand(0xCF);
sendData(0x00);
sendData(0xC1);
sendData(0x30);

sendCommand(0xED);
sendData(0x64);
sendData(0x03);
sendData(0x12);
sendData(0x81);

sendCommand(0xE8);
sendData(0x85);
sendData(0x00);
sendData(0x78);

sendCommand(0xCB);
sendData(0x39);
sendData(0x2C);
sendData(0x00);
sendData(0x34);
sendData(0x02);

sendCommand(0xF7);
sendData(0x20);

sendCommand(0xEA);
sendData(0x00);
sendData(0x00);

//ST7735R Power Sequence
sendCommand(ILI9341_PWCTR1);     //Power control
sendData(0x23);
sendCommand(ILI9341_PWCTR2);     //Power control
sendData(0x10);

sendCommand(ILI9341_VMCTR1);     //VCM control
sendData(0x3E);
sendData(0x28);
sendCommand(ILI9341_VMCTR2);     //VCM control
sendData(0x86);

sendCommand(ILI9341_MADCTL);    //Memory Access Control
sendData(0x48);

//sendCommand(ILI9341_VSCRSADD);  //Vertical scroll zero
//sendData(0x00);

sendCommand(ILI9341_PIXFMT);
sendData(0x55);

sendCommand(ILI9341_FRMCTR1);
sendData(0x00);
sendData(0x18);

sendCommand(ILI9341_DFUNCTR);   //Display Function Control
sendData(0x08);
sendData(0x82);
sendData(0x27);

sendCommand(0xF2);              //3Gamma Function Disable
sendData(0x01);

sendCommand(ILI9341_GAMMASET);  //Gamma curve selected
sendData(0x01);

sendCommand(ILI9341_GMCTRP1);   //Set Gamma
sendData(0x0F);
sendData(0x31);
sendData(0x2B);
sendData(0x0C);
sendData(0x0E);
sendData(0x08);
sendData(0x4E);
sendData(0xF1);
sendData(0x37);
sendData(0x07);
sendData(0x10);
sendData(0x03);
sendData(0x0E);
sendData(0x09);
sendData(0x00);

sendCommand(ILI9341_GMCTRN1);   //Set Gamma
sendData(0x00);
sendData(0x0E);
sendData(0x14);
sendData(0x03);
sendData(0x11);
sendData(0x07);
sendData(0x31);
sendData(0xC1);
sendData(0x48);
sendData(0x08);
sendData(0x0F);
sendData(0x0C);
sendData(0x31);
sendData(0x36);
sendData(0x0F);

sendCommand(ILI9341_SLPOUT);    //Exit Sleep
for (i = 500; i > 0; i--);

sendCommand(ILI9341_DISPON);    //Main screen turn on
sendCommand(ILI9341_RAMWR);

