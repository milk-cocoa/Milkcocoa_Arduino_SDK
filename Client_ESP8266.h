/*
 Copyright (C) 2015 Embedded and Real-Time Systems Laboratory
              Graduate School of Information Science, Nagoya Univ., JAPAN
  
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _CLIENT_ESP8366_H_
#define _CLIENT_ESP8366_H_
#include "Print.h"
#include "Stream.h"
#include "IPAddress.h"
#include "ESP8266.h"

#define ESP8266CLIENT_BUFFER_LEN  512

class ESP8266Client: public Client, public ESP8266 {

  public:
    /*
     * @retval 0 : faild
     * @retval 1 : success
     */
    int connect(IPAddress ip, uint16_t port) {
        String ip_str;
        bufferlen= 0;
        ip_str.concat(ip[0]);
        ip_str.concat(".");
        ip_str.concat(ip[1]);
        ip_str.concat(".");
        ip_str.concat(ip[2]);
        ip_str.concat(".");
        ip_str.concat(ip[3]);
        _connected = (createTCP(ip_str, port))? 1 : 0;        
        return (int)_connected;
    };
    
    int connect(const char *host, uint16_t port) {
        String ip_str(host);
        bufferlen= 0;
        _connected = (createTCP(ip_str, port))? 1 : 0;        
        return (int)_connected;        
    };
    
    size_t write(uint8_t data) {
        if(send(&data, 1)){
            return 1;
        }
        else {
            return 0;
        }
    };
    size_t write(const uint8_t *buf, size_t size){
        if(send(buf, size)){
            return size;
        }
        else {
            return 0;
        }
    };
    int available(){
        if (bufferlen > 0) {
            return 1;
        }
        if (dataAvailable()) {
            bufferlen = recv(buffer, ESP8266CLIENT_BUFFER_LEN);
            pbuffer = buffer;
            //In case of that data is available but it is not valid data(not +IPD,...),
            //connection is closed by server.(send "CLOSED")
            if(bufferlen == 0){
                _connected = 0;
                releaseTCP();
                return 0;
            }
            return 1;
        }
        return 0;
    };
    int read(){
        if (bufferlen == 0) {
            bufferlen = recv(buffer, ESP8266CLIENT_BUFFER_LEN);
            pbuffer = buffer;
        }
        if (bufferlen > 0){
            bufferlen--;
            return *pbuffer++;
        }
        else {
            return (int)-1;
        }            
    };
    int read(uint8_t *buf, size_t size){return recv(buf, size);};
    int peek(){return 1;};
    void flush(){};
    void stop(){releaseTCP();};
    uint8_t connected(){return _connected;};
    operator bool(){return (bool)(_connected == 1);};
  private:
    uint8_t _connected;
    uint8_t buffer[ESP8266CLIENT_BUFFER_LEN];
    uint8_t bufferlen;
    uint8_t *pbuffer;
};

#endif /* _CLIENT_ESP8366_H_ */
