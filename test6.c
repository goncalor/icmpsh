#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char **argv) {
    HANDLE hIcmpFile;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;


    ReplySize = sizeof(ICMP6_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    if (ReplyBuffer == NULL) {
        printf("\tUnable to allocate memory\n");
        return 1;
    }    


    hIcmpFile = Icmp6CreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        printf("\tUnable to open handle.\n");
        printf("Icmp6Createfile returned error: %ld\n", GetLastError() );
        return 1;
    }
    else {
        printf("\tHandle created.\n");
    }

    struct sockaddr_in6 src_addr, dst_addr;

    src_addr.sin6_family = AF_UNSPEC;
    src_addr.sin6_port = htons(0);
    src_addr.sin6_flowinfo = 0;
    src_addr.sin6_scope_id = 0;
    src_addr.sin6_addr = in6addr_any;

    dst_addr.sin6_family = AF_INET6;
    dst_addr.sin6_port = htons(0);
    dst_addr.sin6_flowinfo = 0;
    dst_addr.sin6_scope_id = 0;
    inet_pton(AF_INET6, "fe80::bde0:a6c2:e0c9:7354", &dst_addr.sin6_addr);

    dwRetVal = Icmp6SendEcho2(
            hIcmpFile,  // HANDLE                 IcmpHandle,
            NULL,  // HANDLE                 Event,
            NULL,  // PIO_APC_ROUTINE        ApcRoutine,
            NULL,  // PVOID                  ApcContext,
            &src_addr,  // sockaddr_in6           *SourceAddress,
            &dst_addr,  // sockaddr_in6           *DestinationAddress,
            SendData,  // LPVOID                 RequestData,
            sizeof(SendData),  // WORD                   RequestSize,
            NULL,  // PIP_OPTION_INFORMATION RequestOptions,
            ReplyBuffer,  // LPVOID                 ReplyBuffer,
            ReplySize,  // DWORD                  ReplySize,
            1000   // DWORD                  Timeout
            );

    if (dwRetVal == 0) {
        printf("\tCall to IcmpSendEcho failed.\n");
        printf("\tIcmpSendEcho returned error: %ld\n", GetLastError() );
        return 1;
    }
}
