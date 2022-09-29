
/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <CARguru-Buch@web.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gustav Wostrack
 * ----------------------------------------------------------------------------
 */

#ifndef UTILS_H
#define UTILS_H

#include <serial2raspi.h>

enum statustype
{
  even = 0,
  odd,
  undef
};

statustype lastStatus;
statustype currStatus;

// timer
uint16_t secs = 0;
const uint8_t maxDevices = 20;

Ticker tckr;
#define tckrTime 1.0 / maxDevices

//CAN_FRAME_SIZE
// create UDP instance
//  EthernetUDP instances to let us send and receive packets over UDP

// events
//*********************************************************************************************************

// der Timer steuert das Scannen der Slaves, das Blinken der LED
// sowie das Absenden des PING
void timer1s()
{
/*  static uint8_t time2SendPing = 1;
  secs++;
  uint8_t slaveCnt = get_slaveCnt();
  if (slaveCnt > 0)
  {
    if ((secs / (maxDevices / slaveCnt)) % 2 == 0)
      currStatus = even;
    else
      currStatus = odd;
  }
  // periodic ping
  if (get_SYSseen())
  {
    if (secs % maxDevices == 0)
    {
      time2SendPing++;
    }
    if (time2SendPing % wait_for_ping == 0)
    {
      time2SendPing = 1;
      produceFrame(M_CAN_PING);
//      sendOutTCP(M_PATTERN);
      sendout2GW(M_PATTERN, toCAN);
    }
  }*/
}

// Start des Timers
void stillAliveBlinkSetup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  set_bAllSlavesOnboard(false);
  tckr.attach(tckrTime, timer1s); // each sec
  lastStatus = undef;
}

// hiermit wird die Aufforderung zum Blinken an die Decoder verschickt
// sowie einmalig die Aufforderung zur Sendung der Initialdaten an die Decoder
// Initialdaten sind bsplw. die anfängliche Stellung der Weichen, Signale oder
// Gleisbesetztmelder; anschließend gibt es eine Wartezeit, die für die einzelnen
// Decodertypen unterschiedlich lang ist
void stillAliveBlinking()
{
  static uint8_t slv = 0;
  uint8_t M_BLINK[] = {0x00, BlinkAlive, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t M_IDATA[] = {0x00, sendInitialData, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  if (get_bAllSlavesOnboard())
  {
    if (currStatus != lastStatus)
    {
      lastStatus = currStatus;
      M_BLINK[0x05] = currStatus;
      sendTheData(slv, M_BLINK, CAN_FRAME_SIZE);
      slv++;
      if (slv >= get_slaveCnt())
        slv = 0;
    }
  }
}

/*************************** End of file ****************************/
#endif
