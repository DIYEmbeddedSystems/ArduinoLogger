#include <UdpLogger.h>
#include <Arduino.h>

UdpLogger::UdpLogger(UDP &udp, IPAddress address, uint16_t port, const char *context, e_log_level level) :
    Logger(context, level), _udp(udp), _address(address), _port(port)
{
    debug("UdpLogger constructed");
}

// the actual output function
void UdpLogger::output(const char *log_buffer, size_t len)
{
  if (_udp.beginPacket(_address, _port)) 
  {
    _udp.write(reinterpret_cast<const uint8_t *>(log_buffer), len);
    _udp.write('\r');
    _udp.write('\n');
    _udp.endPacket();
  }
}