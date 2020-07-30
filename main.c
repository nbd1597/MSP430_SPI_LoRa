#include <msp430.h>
#include <stdint.h>
#include "mcu.h"
#include "uart.h"
#include "spi.h"
#include "sx1276.h"
#include "sx1276regs-fsk.h"
#include "base64.h"
#include "main.h"



uint8_t buffer[BUFFER_SIZE];
uint8_t buffer2[BUFFER_SIZE];

static radio_events_t radio_events;

int state = 0;

void SendPing() {
  buffer[0] = 1;
  buffer[1] = 0x10;
  buffer[2] = 8;
  buffer[3] = 1;
  buffer[4] = 'P';
  buffer[5] = 'I';
  buffer[6] = 'N';
  buffer[7] = 'G';

  uart_write("send_started\n\r");
  sx1276_send(buffer, 8);
//  sx1276_set_tx(10000);

}

void rf_init_lora();
void OnRxError();





void main(void) {
  mcu_init();

  P1DIR |= BIT0;

  uart_init();
  uart_write("\n\n Start \n\n");
  spi_init();
  P2IE &= ~BIT0;
  rf_init_lora();

  uart_write("$IND \n,");
  uart_printhex8(sx1276_read(REG_VERSION));
  uart_writec(',');
  uart_printhex32(RF_FREQUENCY);
  uart_writec(',');
  uart_printhex8(TX_OUTPUT_POWER);
  uart_writec(',');
  uart_printhex8(LORA_BANDWIDTH);
  uart_writec(',');
  uart_printhex8(LORA_SPREADING_FACTOR);
  uart_writec(',');
  uart_printhex8(LORA_CODINGRATE);
  uart_writec('\n');
  uart_write("1? \n");

  uart_printhex8(sx1276_read(0x1D));
  uart_write("\n2? \n");
  __delay_cycles(1000000);
//  sx1276_set_rx(100000);
  P2IE |= BIT0;
  __bis_SR_register(GIE);


  //while(1){ mcu_delayms(500); }
  state = 1;
  SendPing();
  __delay_cycles(1000000);
  while(1)
  {
//    if(state == 0)
//    {
////      state = 1;
//      SendPing();
//    } else if(state == 1)
//    {
//
//    }
//      SendPing();
//
  }

}

/*
 * INterrupt
 */
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(P2IFG == BIT0)
    {
//        if (state == 0)
//        {
//            OnTxDone();
//            state = 1;
//            sx1276_set_rx(10000);
//        }
//        else
//        {
//
//            OnRxDone(buffer2, 32, sx1276.Settings.LoRaPacketHandler.RssiValue, sx1276.Settings.LoRaPacketHandler.SnrValue);
//            state = 0;
//            SendPing();
//        }

        //else OnRxDone(payload, size, rssi, snr);
        uart_write("interrupt happens \r");
        sx1276_on_dio0irq();
//        OnTxDone();
//        SendPing();
//        OnRxDone(buffer2, 32, sx1276.Settings.LoRaPacketHandler.RssiValue, sx1276.Settings.LoRaPacketHandler.SnrValue);
//        sx1276_set_rx(100000);
    }
    P2IFG &= ~BIT0;
}


void OnTxDone() {
  uart_write("$TXS\n");

//  if(state == 1) sx1276_set_rx(0);

}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {

    uart_write("txdone_start");
    P1OUT |= BIT0;

  uart_write("$RXS,");

  uart_printhex32(rssi);
  uart_writec(',');

  uart_printhex8(snr);
  uart_writec(',');

  uart_printhex32(size);
  uart_writec(',');

  base64_encode(payload, size);
  uart_writec('\n');
  P1OUT &= ~BIT0;
  uart_write("????");

//  if(state == 1) SendPing();
}

void OnRxError() {
  uart_write("$RXE\n");
}
void rf_init_lora() {
  radio_events.TxDone = OnTxDone;
  radio_events.RxDone = OnRxDone;
  //radio_events.TxTimeout = OnTxTimeout;
  //radio_events.RxTimeout = OnRxTimeout;
  radio_events.RxError = OnRxError;

  sx1276_init(radio_events);
  sx1276_set_channel(RF_FREQUENCY);


  sx1276_set_txconfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

  sx1276_set_rxconfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

}


