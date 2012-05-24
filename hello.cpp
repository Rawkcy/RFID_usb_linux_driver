// uses libusb
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <usb.h>
#include <usbpp.h>

#define DEBUG_LEVEL 0

#define PROTOCOL_MOUSE 2
#define PROTOCOL_KEYBOARD 1
#define PROTOCOL_RFID 2
#define ID_VENDOR 0x1325
#define ID_PRODUCT 0xc029

using namespace std;

struct usb_bus *busses,*bus,*dbus;

int findDevice(int PROTOCOL,struct usb_device *dev){
    bool found=false;
    //struct usb_device *dev;

    usb_find_busses();
    usb_find_devices();
    busses=usb_get_busses();
    for (bus=busses;bus && !found;bus=bus->next){
        for (dev=bus->devices;dev;dev=dev->next){
//            if (dev->config->interface->altsetting->bNumEndpoints==PROTOCOL) {dbus=bus;found=true;break;}
            if (dev->descriptor.idVendor==ID_VENDOR) {dbus=bus;found=true;break;}
        }//end of devices loop
    }//end of busses loop
    if (!found) return -1;
    return 1;
}

int main(int argc, char *argv[])
{
    struct usb_device dev;
    char *buf;
    int n,x,r;
    char string[50];
    bool found=false;
    usb_dev_handle *fdev;
    usb_set_debug(DEBUG_LEVEL);
    usb_init(); //initilize the usb library

    if (findDevice(PROTOCOL_RFID,&dev)<0){ //search for a USB mouse , you can change it to keyboard,joystick…etc
        printf("Unable to find the required device");
        exit(1);
    }

    printf("Now we are dealing with device from vendor ID : %d (%x) \n",dev.descriptor.idVendor,dev.descriptor.idVendor);
    printf("Trying to open the device…\n");
    if (fdev=usb_open(&dev)) printf("Device opened successfully\n"); //Here we open the device , just like fopen
    else { printf("Failed to open USB \n"); exit(1);}

    buf=(char*)calloc(1,100);
    if (usb_get_driver_np(fdev,0,buf,100)) printf("Kernel Using Driver : %s\n",buf); //Trying to get the kernel driver name (optional)
    else printf("Could not read the driver name  \n");
//    delete buf;

    if (usb_detach_kernel_driver_np(fdev,0)) printf("Device detached successfully from the kernel.\n"); //detach the driver from the kernel , seems to be just like rmmod , but it always returns errors , however , it wroks
    else printf("Error detaching the device \n");

    if (r=usb_claim_interface(fdev,0)) printf("Interface Claimed!! \n"); //reserving the device interface for our applicatoin , if another driver/software is using the device , it will return ‘interface busy’
    printf("Interface Claim Status : %d\n",r);

    if (r=usb_set_altinterface(fdev,0)) printf("Alternate Interface Claimed!! \n"); //reserving the device interface for our applicatoin , if another driver/software is using the device , it will return ‘interface busy’

    printf("Device Protocol : %d\n",dev.descriptor.bDeviceProtocol);
    printf("Report Length : %d\n",dev.descriptor.bLength);
    printf("Descriptor Type : %d\n",dev.descriptor.bDescriptorType);
    //printf("TESTING: %d\n",dev.config->interface->num_altsetting);
    //printf("End Points : %d\n",dev.config->interface->altsetting->bNumEndpoints);
    //printf("Interface Class : %d\n",dev.config->interface->altsetting->bInterfaceClass);
    //printf("Protocol : %d\n",dev.config->interface->altsetting->bInterfaceProtocol);
    //printf("Interface Number: %d\n",dev.config->interface->altsetting->bInterfaceNumber);
    printf("Device Filename : %s\n",dev.filename);
    printf("Bus Dir Name : %s\n",dbus->dirname);

    usb_get_string_simple(fdev,dev.descriptor.iManufacturer,string,sizeof(string));
    printf("Device Manfucaturer : %s\n",string);
    usb_get_string_simple(fdev,dev.descriptor.iProduct,string,sizeof(string));
    printf("Product Name : %s\n",string);
    usb_get_string_simple(fdev,dev.descriptor.iSerialNumber,string,sizeof(string));
    printf("Device Serial Number: %s\n",string);
    //printf("End point addresses : 0x%x\n",dev.config->interface->altsetting->endpoint->bEndpointAddress);

    //while (string[0]!=3){
    //    string[0]=string[1]=string[2]=string[3]=0;
    //    r=usb_interrupt_read(fdev,0x81,string,4,0); //I am reading 4 bytes using interrupt read , note that not every usb device supports interrupt read/write
    //    for (x=0;x<4 && r>0;x++) {
    //        printf("%d ",string[x]);
    //    }
    //    printf("\n");
    //    usb_clear_halt(fdev,0x81); //I need to reset the device node because of some devices (usually keyboards) keep sending the same bytes even after releasing the key !
    //}

    printf("Closing Device.\n");
    usb_release_interface(fdev,0);
    usb_close(fdev);
    delete buf;
    return EXIT_SUCCESS;
}
