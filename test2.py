#!/usr/bin/python

import sys
import usb.core
import usb.util

# bInterfaceNumber and bConfigurationValue
INTERFACE = 0
CONFIGURATION = 1
# idVendor and idProduct
VENDOR_ID = 0x0403
PRODUCT_ID = 0x6010

dev = usb.core.find(idVendor=VENDOR_ID, idProduct=PRODUCT_ID)
# was it found?
if dev is None:
    raise ValueError('Device not found')

if dev.is_kernel_driver_active(INTERFACE):
  try:
    dev.detach_kernel_driver(0)
    dev.detach_kernel_driver(1)
    print "Detached kernel driver"
  except usb.core.USBError as e:
    sys.exit("Failed to detach kernel driver: %s" % str(e))

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

ep = usb.util.find_descriptor(
    intf,
    # match the first OUT endpoint
    custom_match = \
    lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_OUT
)

assert ep is not None
import ipdb;ipdb.set_trace()

for breq in range(145):
  try:
    ret = dev.ctrl_transfer(0xC0, breq, 0, 0, 100)
    print "bRequest ",breq
    print ret
  except:
    pass

#for breq in range(145):
#  try:
#    ep.write(
#    print "bRequest ",breq
#    print ret
#  except:
#    pass

# write the data
#print ep.write([0xC0,0x03,0x12])
#print ep.read(ep.wMaxPacketSize)
