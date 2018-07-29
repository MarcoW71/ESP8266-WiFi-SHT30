
#ifndef __BUFFERED_PRINTER_HPP__
#define __BUFFERED_PRINTER_HPP__

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#define CHUNK_SIZE 512
#define PAGE_BUFFER_SIZE 6144

class BufferedPrinter : public Print {
private:
    size_t pos = 0;
    uint8_t pageBuffer[PAGE_BUFFER_SIZE];

public:
    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data);

    template <typename stringType>
    void write(const stringType &str);

    using Print::write;

    void reset();
    size_t getContentLength();
    size_t responseFiller(uint8_t *buffer, size_t maxLen, size_t index);
};


extern BufferedPrinter printer;

extern void sendPrinterPage( AsyncWebServerRequest *request );

#endif
