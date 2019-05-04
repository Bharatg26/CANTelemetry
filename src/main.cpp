#include <mbed.h>
#include <inttypes.h>

CAN can0(D10, D2);   // CAN bus pins
Serial xbee(D1, D0); //Tx and Rx pins on stm
CANMessage Msg;
DigitalOut led(LED1); //led blinking on stm
void sendMsg(CANMessage Msg1);

int main()
{

  can0.frequency(250000);
  can0.mode(CAN::Normal);
  xbee.baud(9600);

  while (1)
  {
   
    if (can0.read(Msg))
    {

      if (Msg.id == 0x423)
      {

        sendMsg(Msg);
      }
    }
  }
}

void sendMsg(CANMessage Msg1) //uses logic to convert can byte array to serial message
{

  uint32_t fullmsg = 0;
  for (int i = 0; i <= 3; i++)
  {
    uint32_t byte = Msg1.data[i] << (24 - (8 * i));
    fullmsg = fullmsg + byte;
  }
  xbee.putc(fullmsg); //send the message here (bits 0-31)

  fullmsg = 0;
  for (int i = 4; i <= 7; i++)
  {
    uint32_t byte = Msg1.data[i] << (24 - (8 * (i - 4)));
    fullmsg = fullmsg + byte;
  }
  xbee.putc(fullmsg); //send message here (bits 32-63)
}