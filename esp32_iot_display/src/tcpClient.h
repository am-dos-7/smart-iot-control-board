#ifndef TCPCLIENT_H
#define TCPCLIENT_H

/* 
 * Début: 07/05/2021
 * Auteur: Am_Zirée
 * 
 * Implémentation d'un client TCP à partir des 'C socket'
 * 
 * Last edited: 31/08/2022
 */

#include <Arduino.h>
#include <lwip/sockets.h>
#include <esp_log.h>
#include <string.h>
#include <errno.h>
#include "sdkconfig.h"

#define DEFAULT_PORT 50885
#define PACKET_SIZE 254      // Default packet size

enum DEV_TYPE:uint8_t{
    DEFLT = 0,
    LAMP = 1,
    CAMERA = 2,
    DISP = 3        // Display, but DISPLAY is already used as a #define in some header files
};

class TcpClient
{
public:
    TcpClient(uint8_t type, char* name,  char *addr=NULL, uint16_t port = DEFAULT_PORT);
    ~TcpClient();
    void setAddress(char *addr);       // Définir l'adresse de la socket
    void setPortNumber(uint16_t port);   // Définir le numéro de port de la socket
    int connectTo();          // Se connecter à la socket
    int disconnect();       // Se déconnecter de la socket
    int sendData(char *data, uint32_t size);  // Ecrire des données sur la socket
    void setReceptionHandler(void (*fn)(char*, uint32_t));
    void setDisconnectHandler(void (*fn)(void));
    bool isConnected();

private:    // private function
    static void * sta_data_thread_func(void *);  // static function for data thread (client object as arg)
    void setClientConnected(bool);

private:
    int socket_descriptor;   // Descripteur de la socket
    uint16_t port_number;
    char *address;
    void (*on_packet_received)(char *data, uint32_t size) = NULL;   // function to be called on packet reception
    void (*on_disconnected)(void) = NULL;     // called on socket disconnect
    struct sockaddr_in server_address;
    socklen_t address_length;
    pthread_t data_thread;
    char *data_buf;
    bool connected = false;
    uint8_t dev_type;                // device type
    char *dev_name;                 // device name

};

#endif