
#include <TelnetLogger.h>
#include <Arduino.h>

TelnetLogger::TelnetLogger(uint16_t port, const char *context, e_log_level level)
    : Logger(context, level), _server(port)
{
  _server.begin();
  _server.setNoDelay(true);
}


boolean TelnetLogger::isClientConnected()
{
  // Attempt to reconnect, if necessary
  if (_server.hasClient())
  {
    if (!_client || !_client.connected())
    {
      if (_client)
      {
        _client.stop();
      }
      _client = _server.available();
    }
    else
    {
      _server.available().stop();
    }
  }

  return _client && _client.connected();
}

void TelnetLogger::output(const char *log_buffer, size_t len)
{
  if (isClientConnected())
  {
    if (_buffered.length() > 0)
    {
      _client.print("\r\n<<< Buffered\r\n");
      _client.write(reinterpret_cast<const uint8_t *>(_buffered.c_str()), _buffered.length());
      _client.print("\r\n>>>\r\n");
      _buffered = String();
    }
    _client.write(reinterpret_cast<const uint8_t *>(log_buffer), len);
    _client.write('\r');
    _client.write('\n');
  }
  else
  {
    if (_buffered.length() < 1024)
    {
      _buffered += String(log_buffer);
      _buffered += "\r\n";
    }
  }
}

