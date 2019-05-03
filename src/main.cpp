#include <mbed.h>
CAN can0(D10, D2);       // CAN bus pins
Serial xbee(PA_2, PA_3); //Tx and Rx pins on XBee
CANMessage Msg;
DigitalOut rst(PA_5); //digital reset for XBee
DigitalOut led(LED1); //led blinking on stm
void sendMsg();
void sendMsg() //uses logic to convert can byte array to serial message
{
  uint32_t fullmsg = 00000000000000000000000000000000;
  for (int i = 0; i <= 3; i++)
  {
    uint32_t byte = Msg.data[i] << (24 - 8 * i);
    fullmsg = fullmsg + byte;
  }
  xbee.putc(fullmsg); //send the message here (bits 0-31)
  for (int i = 4; i <= 7; i++)
  {
    uint32_t byte = Msg.data[i] << (24 - 8 * (i - 4));
    fullmsg = fullmsg + byte;
  }
  xbee.putc(fullmsg); //send message here (bits 32-63)
}
int main()
{

  can0.frequency(250000);
  can0.mode(CAN::Normal);

  while (1)
  {
    // put your main code here, to run repeatedly:
    if (can0.read(Msg))
    {

      if (Msg.id == 0x423)
      {
        sendMsg();
        led = !led;
      }
    }
  }
}