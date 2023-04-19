#include "tcpClient.h"

TcpClient::TcpClient(uint8_t type, char *name, char *addr, uint16_t port): port_number(port)
{
    dev_type = type;
    dev_name = new char[strlen(name) + 1];      // we should ensure that dev_name length does not exceed 254
    memset(dev_name, '\0', strlen(name) + 1);
    strcpy(dev_name, name);
    address = new char[strlen(addr)];   // allocate memory
    if(address == NULL){
        Serial.printf("Error on address alloc\n");
    }
    memset(address, '\0', strlen(addr));
    // should normally check if the alloc went well here

    stpcpy(address, addr);                 // and copy the address
    
    data_buf = new char[PACKET_SIZE + 1];
    if(data_buf == NULL){
        Serial.printf("Error on data_buf alloc\n");
    }
    // check if it went well

    address_length = sizeof(server_address);
}

TcpClient::~TcpClient()
{
    /* nothing to do */
}

void TcpClient::setAddress(char *addr)
{
    realloc(address, sizeof(addr));
    strcpy(address, addr);
}

void TcpClient::setPortNumber(uint16_t arg)
{
    port_number = arg;
}

int TcpClient::connectTo(){
    // Creates the socket
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor < 0){
        Serial.println("Socket creation error");
        while(1);
    }

    // config of server_address
    memset(&server_address, '\0', sizeof(address_length));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);   // port number assignment
    inet_aton(address, &(server_address.sin_addr));

    if(!connect(socket_descriptor, (struct sockaddr *)&server_address, address_length)){
        Serial.println("Successfully connected to server");
        // if connected succesfully
        pthread_create(&data_thread, NULL, TcpClient::sta_data_thread_func, this);  // then starts data thread
        setClientConnected(true);

        // From here we will try to send the device type and name to the server
        uint16_t tmp_size = strlen(dev_name) + 3;   // 1 byte for the length of the string, 1 for the client type; last byte is '\0'
        char *tmp = new char[tmp_size]; 
        if(tmp == NULL){
            Serial.println("error tmp");
            return 1;
        }
        memset(tmp, '\0', tmp_size);
        tmp[0] = tmp_size - 2;      // first byte = size; client_name + client_type
        tmp[1] = dev_type;         // second byte
        strcat(tmp, dev_name);    // concats the dev_name       

        write(socket_descriptor, tmp, tmp_size - 1);    // send tmp

        delete[] tmp;
        
        return 0;
    }

    Serial.printf("Failed to connect to server: \n");
    on_disconnected();  // executes the disconnection handler, expected to be a re-connection trial
    return 1;
}

int TcpClient::disconnect()
{
    return close(socket_descriptor);
}

int TcpClient::sendData(char *data, uint32_t size)
{
    return write(socket_descriptor, data, size);
}

void TcpClient::setReceptionHandler(void (*fn)(char*, uint32_t))
{
    on_packet_received = fn;
}
void TcpClient::setDisconnectHandler(void (*fn)(void))
{
    on_disconnected = fn;
}

void * TcpClient::sta_data_thread_func(void * arg)
{
    TcpClient *m_client = (TcpClient*) arg;     // retrieve object pointer from arg

    do{
        memset(m_client->data_buf, '\0', PACKET_SIZE + 1);
        switch(read(m_client->socket_descriptor, m_client->data_buf, PACKET_SIZE)){
        case -1:
                Serial.println("Socket read error");
                m_client->setClientConnected(false);
                break;
            case  0:
                Serial.println("Connection lost, may be closed by server");
                m_client->setClientConnected(false);
                break;
            default: 
                m_client->on_packet_received(m_client->data_buf, strlen(m_client->data_buf));
        }
    }while(m_client->isConnected());

    close(m_client->socket_descriptor); // close the connection
    m_client->setClientConnected(false);
    Serial.println("Data thread closed");
    m_client->on_disconnected();            // call the disconnection handler
    pthread_exit(NULL);

    return (void *)0;
}

void TcpClient::setClientConnected(bool arg)
{
    connected = arg;
}

bool TcpClient::isConnected()
{
    return connected;
}