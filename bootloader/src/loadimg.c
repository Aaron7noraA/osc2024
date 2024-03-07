#include "uart0.h"
#include "utli.h"

// long long address_input() {
//     uart_printf("\rPlease input kernel load address (default: 0x80000): 0x");

//     int idx = 0, end = 0, i;
//     char cmd[128];
//     char c;
//     while ((c = uart_read()) != '\n') {
//         if (end == 8) {
//             continue;
//         }
//         // CTRL-C
//         if (c == 3) {
//             uart_printf("\n");
//             return -1;
//         }
//         // Backspace
//         else if (c == 8 || c == 127) {
//             if (idx > 0) {
//                 idx--;
//                 // left shift command
//                 for (i = idx; i < end; i++) {
//                     cmd[i] = cmd[i + 1];
//                 }
//                 cmd[--end] = '\0';
//             }
//         } else {
//             // right shift command
//             if (idx < end) {
//                 for (i = end; i > idx; i--) {
//                     cmd[i] = cmd[i - 1];
//                 }
//             }
//             cmd[idx++] = c;
//             cmd[++end] = '\0';
//         }
//         uart_printf("\rPlease input kernel load address (default: 0x80000): 0x%s \r\e[%dC", cmd, idx + 55);
//     }

//     long long address = 0;
//     for (i = 0; i < end && cmd[i] != 0; i++) {
//         address *= 16;
//         address += cmd[i] - '0';
//     }

//     if (address == 0) {
//         uart_printf("\rPlease input kernel load address (default: 0x80000): 0x80000");
//         address = 0x80000;
//     }

//     uart_printf("\n");

//     return address;
// }

void loadimg() {
    uart_init();
    uart_flush();
    // long long address = address_input();
    // if (address == -1) {
    //     uart_printf("invaild address!\n");
    //     return;
    // }
    uart_printf("\n[%f] Init PL011 UART done", get_timestamp());
    uart_printf("\n ____              _     _                    _           \n");
    uart_printf("| __ )  ___   ___ | |_  | |    ___   __ _  __| | ___ _ __ \n");
    uart_printf("|  _ \\ / _ \\ / _ \\| __| | |   / _ \\ / _` |/ _` |/ _ \\ '__|\n");
    uart_printf("| |_) | (_) | (_) | |_  | |__| (_) | (_| | (_| |  __/ |   \n");
    uart_printf("|____/ \\___/ \\___/ \\__| |_____\\___/ \\__,_|\\__,_|\\___|_|   \n\n");
    wait_usec(2000000);
    uart_printf("Send image via UART now!\n");

    // big endian
    int img_size = 0, i;
    for (i = 0; i < 4; i++) {
        img_size <<= 8;
        img_size |= (int)uart_read_raw();
    }
    uart_printf("Sucessfully get img_size!\n");
    uart_printf("img_size: %d\n", img_size);

    // big endian
    int img_checksum = 0;
    for (i = 0; i < 4; i++) {
        img_checksum <<= 8;
        img_checksum |= (int)uart_read_raw();
    }
    uart_printf("Sucessfully get img_checksum!\n");
    uart_printf("img_checksum: %d\n", img_checksum);

    char *kernel = (char *)0x80000;

    for (i = 0; i < img_size; i++) {
        char b = uart_read_raw();
        *(kernel + i) = b;
        img_checksum -= (int)b;
        // if (i % 1000 == 0)
        //     uart_printf("\rreceived byte#: %d", i);
    }

    uart_write('\n');
    if (img_checksum != 0) {
        uart_printf("checksum check failed!\n");
        uart_printf("Please reupload..\n");
        asm volatile(
            "mov x30, 0x60000;"
            "ret;");
    } else {
        uart_printf("checksum correct!\n");
        asm volatile(
            "mov x30, 0x80000;"
            "ret;");
    }
}