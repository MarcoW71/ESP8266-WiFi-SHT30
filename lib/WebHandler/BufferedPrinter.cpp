#include <App.hpp>
#include "BufferedPrinter.hpp"

BufferedPrinter printer;

void sendPrinterPage( AsyncWebServerRequest *request )
{
  size_t contentLength = printer.getContentLength();
  // LOG1( "content length = %d\n", contentLength);
  char contentLengthBuffer[8];
  sprintf( contentLengthBuffer, "%d", contentLength );

  AsyncWebServerResponse *response = request->beginChunkedResponse("text/html", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
  {
    return printer.responseFiller( buffer, maxLen, index);
  }
                                                                   );
  response->addHeader( "Content-Length", contentLengthBuffer );
  response->addHeader("Cache-Control", "no-cache");
  request->send(response);
}

size_t BufferedPrinter::write(const uint8_t *data, size_t len)
{
  memcpy( pageBuffer + pos, data, len );
  pos += len;
  pageBuffer[pos] = 0;
  return len;
}

size_t BufferedPrinter::write(uint8_t data)
{
  return write( &data, 1 );
}

template <typename stringType>
void BufferedPrinter::write(const stringType &str)
{
  write(str.c_str(), str.length());
}

void BufferedPrinter::reset()
{
  memset( pageBuffer, 0, PAGE_BUFFER_SIZE );
  pos = 0;
}

size_t BufferedPrinter::getContentLength()
{
  return pos;
}

size_t BufferedPrinter::responseFiller(uint8_t *buffer, size_t maxLen, size_t index)
{
  size_t chunkSize = CHUNK_SIZE;

  if (( index + chunkSize ) > pos )
  {
    chunkSize = pos - index;
  }

  memcpy( buffer, pageBuffer+index, chunkSize );
  buffer[chunkSize] = 0;
  return chunkSize;
}
