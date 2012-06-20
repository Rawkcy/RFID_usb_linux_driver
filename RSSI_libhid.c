#include <hid.h>
#include <stdio.h>
#include <string.h>

/* Unchanged constants */
#define EP_INTERRUPT_IN 0x81
#define PATHLEN 1
#define PATHIN 0xff000001


/* Prints out the hex on screen */
static void printHex(const void *data, int length)
{
  const unsigned char *byte = data;
  while (length>0)
  {
    length--;
    printf("%.2x ", *byte);
    byte++;
  }
}

static void RFIDCommunication(hid_return ret, HIDInterface** const hid)
{
    int const PATH_IN[] = { PATHIN };

    printf("\n/* * * * * * * * BEGIN COMMUNICATION WITH RFID DEVICE * * * * * * * */");

    // SET ANTENNA POWER to 18 dbm
    unsigned int SEND_PACKET_LEN1 = 3;
    unsigned int REPLY_PACKET_LENGTH1 = 3;
    char PACKET1[] = { 0xC0, 0x03, 0x12 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET1, SEND_PACKET_LEN1);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\n\n----->First sent: \n");
      printHex(PACKET1, SEND_PACKET_LEN1);
    }
    char reply1[REPLY_PACKET_LENGTH1+1];
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply1, REPLY_PACKET_LENGTH1, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\n\n----->First reply:\n");
      printHex(reply1, REPLY_PACKET_LENGTH1);
    }

    // TAG INVENTORY OPERATION
    // XXX: returns EPC Data
    unsigned int SEND_PACKET_LEN2 = 3;
    unsigned int REPLY_PACKET_LENGTH2 = 64;
    char PACKET2[] = { 0x43, 0x03, 0x01 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET2, SEND_PACKET_LEN2);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Second sent:\n");
      printHex(PACKET2, SEND_PACKET_LEN2);
    }
    char reply2[REPLY_PACKET_LENGTH2+1];
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply2, REPLY_PACKET_LENGTH2, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Second reply:\n");
      printHex(reply2, REPLY_PACKET_LENGTH2);
    }

    // SET ANTENNA POWER to 5 dbm
    unsigned int SEND_PACKET_LEN3 = 3;
    unsigned int REPLY_PACKET_LENGTH3 = 3;
    char PACKET3[] = { 0xC0, 0x03, 0x10 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET3, SEND_PACKET_LEN3);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\n\n----->First sent: \n");
      printHex(PACKET3, SEND_PACKET_LEN3);
    }
    char reply3[REPLY_PACKET_LENGTH3+1];
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply3, REPLY_PACKET_LENGTH3, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\n\n----->First reply:\n");
      printHex(reply3, REPLY_PACKET_LENGTH3);
    }

    // TAG INVENTORY OPERATION
    // XXX: returns EPC Data
    unsigned int SEND_PACKET_LEN6 = 3;
    unsigned int REPLY_PACKET_LENGTH6 = 64;
    char PACKET6[] = { 0x43, 0x03, 0x01 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET6, SEND_PACKET_LEN6);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Second sent:\n");
      printHex(PACKET6, SEND_PACKET_LEN6);
    }
    char reply6[REPLY_PACKET_LENGTH6+1];
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply6, REPLY_PACKET_LENGTH6, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Second reply:\n");
      printHex(reply6, REPLY_PACKET_LENGTH6);
    }

    // TAG SELECT OPERATION
    unsigned int SEND_PACKET_LEN4 = 15;
    unsigned int REPLY_PACKET_LENGTH4 = 3;
    char PACKET4[] = { 0x33, 0x0F, 0x0C, 0x30, 0x05, 0xFB, 0x63, 0xAC, 0x1F, 0x38, 0x41, 0xEC, 0x88, 0x04, 0x67 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET4, SEND_PACKET_LEN4);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Fourth sent: \n");
      printHex(PACKET4, SEND_PACKET_LEN4);
    }
    char reply4[REPLY_PACKET_LENGTH4+1];
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply4, REPLY_PACKET_LENGTH4, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Fourth reply: \n", reply4);
      printHex(reply4, REPLY_PACKET_LENGTH4);
    }

    // TAG READ EPC MEMORY OPERATION
    unsigned int SEND_PACKET_LEN5 = 9;
    unsigned int REPLY_PACKET_LENGTH5 = 64;
    char PACKET5[] = { 0x37, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET5, SEND_PACKET_LEN5);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Fifth sent: \n");
      printHex(PACKET5, SEND_PACKET_LEN5);
    }
    char reply5[REPLY_PACKET_LENGTH5+1];
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply5, REPLY_PACKET_LENGTH5, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\n\n----->Fifth reply: \n", reply5);
      printHex(reply5, REPLY_PACKET_LENGTH5);
    }
}


int main(int argc, char *argv[])
{
  HIDInterface* hid;
  int iface_num = 0;
  hid_return ret;

  unsigned short vendor_id  = 0x1325;
  unsigned short product_id = 0xc029;
  char *vendor, *product;
  
  // Match USB by VID and PID
  HIDInterfaceMatcher matcher = { vendor_id, product_id, NULL, NULL, 0 };

  // Enable debugging
//  hid_set_debug(HID_DEBUG_ALL);
//  hid_set_debug_stream(stderr);
//  hid_set_usb_debug(0);
  

  // Initialize HID
  ret = hid_init();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_init failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "*** hid_init success! ***\n");
  }

  hid = hid_new_HIDInterface();
  if (hid == 0) {
    fprintf(stderr, "hid_new_HIDInterface() failed, out of memory?\n");
    return 1;
  } else {
    fprintf(stderr, "*** hid_new_HIDInterface success! ***\n");
  }
 
  //TODO: Use hotplug or give permission to USB without sudo
  /* How to detach a device from the kernel HID driver:
   *
   * The hid.o or usbhid.ko kernel modules claim a HID device on insertion,
   * usually. To be able to use it with libhid, you need to blacklist the
   * device (which requires a kernel recompilation), or simply tell libhid to
   * detach it for you. hid_open just opens the device, hid_force_open will
   * try n times to detach the device before failing.
   *
   * To open the HID, you need permission to the file in the /proc usbfs
   * (which must be mounted -- most distros do that by default):
   *   mount -t usbfs none /proc/bus/usb
   * You can use hotplug to automatically give permissions to the device on
   * connection. Please see
   *   http://cvs.ailab.ch/cgi-bin/viewcvs.cgi/external/libphidgets/hotplug/
   * for an example. Try NOT to work as root!
   */
 

  // Detach kernel 
  ret = hid_force_open(hid, iface_num, &matcher, 25);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_force_open failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "*** hid_forcE_open success! ***\n");
  }
 
  ret = hid_write_identification(stdout, hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_write_identification failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "*** hid_write_identification success! ***\n");
  }
 
  ret = hid_dump_tree(stdout, hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_dump_tree failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "*** hid_dump_tree success! ***\n");
  }
 
  RFIDCommunication(ret, &hid);
  printf("\n\n/* * * * * * * * END COMMUNICATION WITH RFID DEVICE * * * * * * * */\n\n");

  ret = hid_close(hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_close failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "*** hid_close successful! ***\n");
  }
 
  hid_delete_HIDInterface(&hid);
 
  ret = hid_cleanup();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_cleanup failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "*** hid_cleanup successful! ***\n");
  }
  
  return 0;
}
