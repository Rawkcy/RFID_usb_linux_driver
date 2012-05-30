#include <hid.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> /* for getopt() */

// Unchanged constants
#define EP_INTERRUPT_IN 0x81
#define EP_CONTROL 0x00
#define PATHLEN 1


int main(int argc, char *argv[])
{
  HIDInterface* hid;
  int iface_num = 0;
  hid_return ret;

  unsigned short vendor_id  = 0x1325;
  unsigned short product_id = 0xc029;
  unsigned char const REPLY_PACKET_LENGTH = 3;
  unsigned char const SEND_PACKET_LEN = 3;
  char *vendor, *product;
  
  // Match USB by VID and PID
  HIDInterfaceMatcher matcher = { vendor_id, product_id, NULL, NULL, 0 };

  /* see include/debug.h for possible values */
  hid_set_debug(HID_DEBUG_ALL);
  hid_set_debug_stream(stderr);
  /* passed directly to libusb */
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
    fprintf(stderr, "hid_forcE_open success!\n");
  }
 
  ret = hid_write_identification(stdout, hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_write_identification failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "hid_write_identification success!\n");
  }
 
  ret = hid_dump_tree(stdout, hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_dump_tree failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "hid_dump_tree success!\n");
  }
 

  /* * * * * * * * BEGIN COMMUNICATION WITH RFID DEVICE * * * * * * * */
  //TODO: create a polling loop here
  //allow user to input the control transfer command
  //poll for interrupt
  //allow user to quit by hitting CR

  // control transfer HOST-TO-DEVICE
  int const PATH_IN[] = { 0xff000001 };
  //char const PACKET[] = { 0xC0, 0x03, 0x12 };
  char const PACKET[] = { 0x31, 0x03, 0x01 };
  
  ret = hid_set_output_report(hid, PATH_IN, PATHLEN, PACKET, SEND_PACKET_LEN);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
  } else {
    fprintf(stderr, "hid_set_output_report successful!\n");
  }

  // interrupt receive DEVICE-TO-HOST
  // TODO:REPLY_PACKET_LENGTH should be variable
  char reply[REPLY_PACKET_LENGTH];

  fprintf(stderr, "trying to read from RFID\n");
  ret = hid_interrupt_read(hid, EP_INTERRUPT_IN, reply, REPLY_PACKET_LENGTH, 100);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "interrupt read failed with return code %d\n", ret);
    return 1;
  } else {
    char * pch;
    fprintf(stderr, "interrupt read successful!\n");
    fprintf(stderr, "Parsing reply: %s\n", reply);
    pch = strtok (reply, "\\x");
    while (pch != NULL) {
      fprintf(stderr, "%s\n", pch);
      pch = strtok (NULL, "\\x");
    }
  }
  //TODO: Print reply
  /* * * * * * * * END COMMUNICATION WITH RFID DEVICE * * * * * * * */
  
 
  ret = hid_close(hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_close failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "hid_close successful!\n");
  }
 
  hid_delete_HIDInterface(&hid);
 
  ret = hid_cleanup();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_cleanup failed with return code %d\n", ret);
    return 1;
  } else {
    fprintf(stderr, "hid_cleanup successful!\n");
  }
  
  return 0;
}
