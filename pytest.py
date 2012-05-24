#!/usr/bin/env python
#uses pyusb

import usb
import sys
from time import sleep

def find_device(busses, idProduct, idVendor):
    for bus in busses:
        for dev in bus.devices:
            if dev.idProduct == idProduct and dev.idVendor == idVendor:
                return dev

def test_bulk(handle, msg):
    handle.bulkWrite(0x2, msg, 1000)
    data = handle.bulkRead(0x82, 1000, 1000)
    str = "".join([chr(i) for i in data]) # make a string
    if str != msg:
        print "Error in bulk test!!!"
        sys.exit(1)

# our endpoints are interrupts
def test_interrupt(handle, msg):
    handle.interruptWrite(0x02, msg, 1000)
    data = handle.interruptRead(0x81, 1000, 1000)
    str = "".join([chr(i) for i in data])
    if str != msg:
        print "Error in interrupt test!!!"
        sys.exit(1)


if __name__ == "__main__":
    print "********************************"
    print "PyUSB test script"
    print "********************************"
    print ""

    busses = usb.busses()

    print "enumeration device test..."
    dev = find_device(busses, 0xc029, 0x1325)
    if dev is None:
        print "enumeration test failed..."
        sys.exit(1)
    
    print "enumeration test ok..."
    
    print "opening device test..."
    handle = dev.open()
    print "opeing device ok..."

    # handle value changes here??
    handle.detachKernelDriver(0)
    print "configuration test..."
    handle.setConfiguration(1)
    print "configuration test ok..."

    print "claim interface test..."
    handle.claimInterface(0)
    print "claim interface test ok..."

    print "interface configuration test..."
    handle.setAltInterface(0)
    print "interface configuration ok..."

    print "I/O test..."

    # FAIL
    for x in range(40):
        test_interrupt(handle, "interrupt test 1")
        test_bulk(handle, "bulk test 1")
        test_interrupt(handle, "interrupt test 2")
        test_bulk(handle, "bulk test 2")

    print "I/O test ok..."

    print "reset endpoint test..."
    handle.resetEndpoint(2)
    print "reset endpoint ok..."

    print "string test..."
    print "string index 1:", handle.getString(1,100)
    print "string index 2:", handle.getString(2,100)
    print "string test ok..."

    print "descriptor test..."
    descr = handle.getDescriptor(1, 0, 18)
    if len(descr) != 18:
        print "descriptor test failed..."
        sys.exit(1)
    else:
        print "descriptor:", descr
    print "descriptor test ok..."

    print "control transfer test..."
    control_res = handle.controlMsg(0x80, 0, 2, 0, 0, 1000)
    if len(control_res) != 2:
        print "control transfer test failed..."
        sys.exit(1)
    else:
        print "control tranfer result: ", control_res
    print "control transfer ok..."

    print "reset device test..."
    handle.reset()

    del handle
    del dev
    del busses

    sleep(3)

    busses = usb.busses()

    dev = find_device(busses, 0xc029, 0x1325)
    if dev is None:
        print "reset device test failed..."
    else:
        print "reset device test ok..."

    del dev
    del busses
    del usb

    print "type enter key to exit"
    raw_input()
