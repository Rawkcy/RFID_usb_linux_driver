#include <hid.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> /* for getopt() */

// Unchanged constants
#define EP_INTERRUPT_IN 0x81
#define EP_CONTROL 0x00
#define PATHLEN 1


static void printHex(const void *data, int length)
{
  const char *byte = data;
  while (length>0)
  {
    length--;
    printf("%.2x ", *byte);
    byte++;
  }
}

static void RFIDCommunication(hid_return ret, HIDInterface** const hid)
{
  //void append(char* str, char c)
  //{
  //  int len = strlen(str);
  //  str[len] = c;
  //  str[len+1] = '\0';
  //}

  int const PATH_IN[] = { 0xff000001 };
  unsigned int SEND_PACKET_LEN1 = 3;
  unsigned int REPLY_PACKET_LENGTH1 = 3;

  printf("\n/* * * * * * * * BEGIN COMMUNICATION WITH RFID DEVICE * * * * * * * */\n\n");
  
  // Looping the communication channel
  //while (true) {
    //XXX Exiting if user input is not a number
    //printf("\nSize of packet to be sent: ");
    //if (scanf("%d", &SEND_PACKET_LEN) != 1) { return; }
    //printf("Size of expected packet reply: ");
    //if (scanf("%d", &REPLY_PACKET_LENGTH) != 1) { return; }

    char PACKET1[] = { 0xC0, 0x03, 0x12 };
    //char PACKET[SEND_PACKET_LEN];
    //printf("\nPlease input packet data\n");
    //char input_hex[] = { };
    //int num_of_inputs;
    //num_of_inputs = 0;
    //do {
      //printf("Input %d: ", num_of_inputs+1);
      // TODO:
      // append the user input into PACKET[]
      //
//      gets(input_hex);
//      strcpy(PACKET, input_hex);
      //append(PACKET, input_hex);
      //++num_of_inputs;
    //} while (num_of_inputs < SEND_PACKET_LEN);
    
    // control transfer OUT from HOST-TO-DEVICE
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET1, SEND_PACKET_LEN1);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\nhid_set_output_report successful!\n");
      printf("\n----->First sent: %s\n", PACKET1);
    }

    // interrupt IN from DEVICE-TO-HOST
    char reply1[REPLY_PACKET_LENGTH1+1];

    fprintf(stderr, "trying to read from RFID\n");
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply1, REPLY_PACKET_LENGTH1, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      //TODO: Print reply
      //char * pch;
      printf("\ninterrupt read successful!\n");
      fprintf(stderr, "\n----->First reply: %s\n", reply1);
      //pch = strtok (reply, "\\x");
      //while (pch != NULL) {
      //  fprintf(stderr, "%s\n", pch);
      //  pch = strtok (NULL, "\\x");
      //}
    }
  //}
    unsigned int SEND_PACKET_LEN2 = 3;
    unsigned int REPLY_PACKET_LENGTH2 = 64;
    char PACKET2[] = { 0x31, 0x03, 0x01 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET2, SEND_PACKET_LEN2);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\nhid_set_output_report successful!\n");
      printf("\n----->Second sent: %s\n", PACKET2);
    }
    char reply2[REPLY_PACKET_LENGTH2+1];
    fprintf(stderr, "trying to read from RFID\n");
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply2, REPLY_PACKET_LENGTH2, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\ninterrupt read successful!\n");
      fprintf(stderr, "\n----->Second reply: %s\n", reply2);
    }

    unsigned int SEND_PACKET_LEN3 = 3;
    unsigned int REPLY_PACKET_LENGTH3 = 64;
    char PACKET3[] = { 0x31, 0x03, 0x02 };
    ret = hid_set_output_report(*hid, PATH_IN, PATHLEN, PACKET3, SEND_PACKET_LEN3);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else {
      printf("\nhid_set_output_report successful!\n");
      printf("\n----->Third sent: \n");
      printHex(PACKET3, SEND_PACKET_LEN3);
    }
    char reply3[REPLY_PACKET_LENGTH3+1];
    fprintf(stderr, "trying to read from RFID\n");
    ret = hid_interrupt_read(*hid, EP_INTERRUPT_IN, reply3, REPLY_PACKET_LENGTH3, 100);
    if (ret != HID_RET_SUCCESS) {
      fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    } else {
      printf("\ninterrupt read successful!\n");
      fprintf(stderr, "\n----->Third reply: \n", reply3);
      printHex(reply3, REPLY_PACKET_LENGTH3);
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
  hid_set_debug(HID_DEBUG_ALL);
  hid_set_debug_stream(stderr);
  hid_set_usb_debug(0);
  

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
  printf("\n/* * * * * * * * END COMMUNICATION WITH RFID DEVICE * * * * * * * */\n\n");

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
