#!/usr/bin/python

import sys, os, time, logging
import usb.core
import usb.util
import usb.backend.libusb10 as libusb10
backend = libusb10.get_backend()

os.environ['PYUSB_DEBUG_LEVEL'] = 'debug'
os.environ['PYUSB_LOG_FILENAME'] = 'debug.log'
packet_len = 64

def pack_request(*arguments):
    packet = [0x0] * packet_len
    i = 0
    for arg in arguments:
        packet[i] = arg
        i = i + 1
#    return [c for c in packet]
    return ''.join([chr(c) for c in packet])

# bInterfaceNumber and bConfigurationValue
INTERFACE = 0
CONFIGURATION = 1
# idVendor and idProduct
VENDOR_ID = 0x1325
PRODUCT_ID = 0xC029

dev = usb.core.find(idVendor=VENDOR_ID, idProduct=PRODUCT_ID)
=======
PRODUCT_ID = 0xc029

dev = usb.core.find(idVendor=VENDOR_ID, idProduct=PRODUCT_ID, backend=backend)

>>>>>>> ae6d855f6593420626c5cd4ba32fe82e59b7a35f
# was it found?
if dev is None:
    raise ValueError('Device not found')

if dev.is_kernel_driver_active(INTERFACE):
  try:
    dev.detach_kernel_driver(INTERFACE)
    print "Detached kernel driver"
  except usb.core.USBError as e:
    sys.exit("Failed to detach kernel driver: %s" % strOUb(e))

# set the active configuration. With no arguments, the first
# configuration will be the active one
dev.set_configuration()

# get an endpoint instance
cfg = dev.get_active_configuration()
interface_number = cfg[(0,0)].bInterfaceNumber
alternate_setting = usb.control.get_interface(dev, interface_number)
intf = usb.util.find_descriptor(
    cfg, bInterfaceNumber = interface_number,
    bAlternateSetting = alternate_setting
)

ep_out = usb.util.find_descriptor(
    intf,
    # match the first OUT endpoint
    custom_match = \
    lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_OUT
)

assert ep is not None

ep_in = usb.util.find_descriptor(
    intf,
    # match the first IN endpoint
    custom_match = \
    lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_IN
)
raw = pack_request(0xc0,0x03,0x12)
dev.write(endpoint=0x02, interface=0, data=raw)
dev.read(endpoint=0x81, interface=0, size=64)
ep_out.write(raw)

# ep_out is HOST To DEVICE
# write the data
#print ep.write([0xC0,0x03,0x12])
#print ep.read(ep.wMaxPacketSize)
