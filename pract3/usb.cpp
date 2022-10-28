#include <cstdio>
#include <iostream>
#include <libusb-1.0/libusb.h>

void PrintHead(){
    printf("===================================\n");
    printf("|* класс устройства\n");
    printf("|  | * идентификатор производителя\n");
    printf("|  |    | * идентификатор устройства\n");
    printf("|  |    |    | * серийный номер\n");
    printf("+--+----+----+--------------------+\n");
}

void printDevices(libusb_device *dev){
    libusb_device_descriptor desc{}; // дескриптор устройства
    libusb_device_handle *handle = nullptr; // хэндл устройства
    unsigned char str[256]; // строка для хранения серийного номера

    int r = libusb_get_device_descriptor(dev, &desc); // получить дескриптор
    if (r < 0){
        fprintf(stderr,"Ошибка: дескриптор устройства не получен, код: %d.\n", r);
        return;
    }

    printf("%.2x %.4x %.4x ",
        desc.bDeviceClass,
        desc.idVendor,
        desc.idProduct);

    libusb_open(dev, &handle);
    if (handle && desc.iSerialNumber){
        r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, str, sizeof(str));
        printf("%s", str);
    } 
    else
        printf("null");
    std::cout << "\n";
}

int main(){
    libusb_device **devs; // указатель на указатель на устройство, используется для получения списка устройств
    libusb_context *ctx = nullptr; // контекст сессии libusb
    int r; // для возвращаемых значений
    size_t cnt; // число найденных USB-устройств

    r = libusb_init(&ctx); // инициализировать библиотеку libusb, открыть сессию работы с libusb
    if (r < 0){
        fprintf(stderr,"Ошибка: инициализация не выполнена, код: %d.\n", r);
        return 1;
    }

    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0){
        fprintf(stderr,
                "Ошибка: список USB устройств не получен. Код: %d\n", r);
        return 1;
    }

    PrintHead();

    for (size_t i = 0; i < cnt; i++){ // цикл перебора всех устройств
        printf("Устройство %ld\\%ld\n", i + 1, cnt);
        printDevices(devs[i]); // печать параметров устройства
    }
    printf("===================================\n");

    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1); 

    // завершить работу с библиотекой libusb, закрыть сессию работы с libusb
    libusb_exit(ctx);
    return 0;
}
