import sys
import xaut
import time
import threading

mouse = xaut.mouse()
sensitivity = 5
keyboard = xaut.keyboard()
pipe = open('/dev/input/js0','r')
action = []

class mousemanipulator(threading.Thread):
	def run(self):
		self.reset()
		global mouse
		while True:
			time.sleep(.02) # Just so we don't hog CPU waiting around
			if self.coords != (0, 0):
				mouse.move(mouse.x()+self.coords[0],mouse.y()+self.coords[1])

	def updatex(self, coord): self.coords = (coord, self.coords[1])
	def updatey(self, coord): self.coords = (self.coords[0], coord)
	def reset(self): self.coords = (0, 0)

 # Which joypad actions should affect which keys/mouseactions (joypadaction:computeraction)
keymap = {'00':26, '01':None, '02':27, '03': None, '04':65, # <- Buttons
	  '05':'MSBTN1','06':'MSSCRLDOWN','07':'MSSCRLUP',  
	  '08':23,'09':9, '0A':None, '0B':None,
	  'DPADUP':None, 'DPADDOWN':None, 'DPADLEFT':None, 'DPADRIGHT':None, # <- Dpad
	  'LJSUP':25, 'LJSDOWN':39, 'LJSLEFT':38, 'LJSRIGHT':40} #<- Left Joystick

def pressactivate(key): # Press down keys/buttons

	global mouse
	global action
	global keyboard

	if type(key) is int: keyboard.down(key)
	elif key == 'MSBTN1': mouse.btn_down(1)
	elif key == 'MSBTN2': mouse.btn_down(2)
	elif key == 'MSSCRLDOWN': mouse.btn_down(5)
	elif key == 'MSSCRLUP': mouse.btn_down(4)
	else: pass

def keyupactivate(key): # Release buttons

	global mouse
	global action
	global keyboard

	if type(key) is int: keyboard.up(key)
	elif key == 'MSBTN1': mouse.btn_up(1)
	elif key == 'MSBTN2': mouse.btn_up(2)
	elif key == 'MSSCRLDOWN': mouse.btn_up(5)
	elif key == 'MSSCRLUP': mouse.btn_up(4)
	else: pass

mousecontrol = mousemanipulator()
mousecontrol.start()
mousecontrol.reset()

while 1:
	for character in pipe.read(1):
		action += ['%02X' % ord(character)]
		if len(action) == 8:

			num = int(action[5], 16) # Translate back to integer form
			percent254 = str(((float(num)-128.0)/126.0)-100)[4:6] # Calculate the percentage of push
			percent128 = str((float(num)/127.0))[2:4]

			if num >= 128: # Convert % of push back into an integer
				if percent254 == '.0': percent254 = 100
				else: percent254 = int(percent254)
			else:
				if percent128 == '0': percent128 = 100
				else: percent128 = int(percent128)

			if action[6] == '01': # Button
				key = keymap[action[7]] # Find the equivalent keyboard key to each button

				if action[4] == '01': pressactivate(key)
				else: keyupactivate(key)

			elif action[7] == '00': # D-pad left/right
				if action[4] == 'FF': pressactivate(keymap['DPADRIGHT'])
				elif action[4] == '01': pressactivate(keymap['DPADLEFT'])
				else: 
					keyupactivate(keymap['DPADRIGHT'])
					keyupactivate(keymap['DPADLEFT'])

			elif action[7] == '01': # D-pad up/down
				if action[4] == 'FF': pressactivate(keymap['DPADDOWN'])
				elif action[4] == '01': pressactivate(keymap['DPADUP'])
				else: 
					keyupactivate(keymap['DPADDOWN'])
					keyupactivate(keymap['DPADUP'])

			elif action[7] == '04': # Left Joystick left/right
				if action[4] == 'FF': pressactivate(keymap['LJSRIGHT'])
				elif action[4] == '01': pressactivate(keymap['LJSLEFT'])
				else: 
					keyupactivate(keymap['LJSRIGHT'])
					keyupactivate(keymap['LJSLEFT'])

			elif action[7] == '05': # Left Joystick up/down
				if action[4] == 'FF': pressactivate(keymap['LJSDOWN'])
				elif action[4] == '01': pressactivate(keymap['LJSUP'])
				else: 
					keyupactivate(keymap['LJSDOWN'])
					keyupactivate(keymap['LJSUP'])

			elif action[7] == '02': # Right Joystick left/right
				if num >= 128:
					if percent254 < 20: mousecontrol.updatex(sensitivity*-1)
					elif percent254 < 40: mousecontrol.updatex((sensitivity*-1)*2)
					elif percent254 < 60: mousecontrol.updatex((sensitivity*-1)*3)
					elif percent254 < 80: mousecontrol.updatex((sensitivity*-1)*4)
					elif percent254 < 90: mousecontrol.updatex((sensitivity*-1)*5)
					else: mousecontrol.updatex((sensitivity*-1)*6)
				elif num <= 127 \
				and num != 0:
					if percent128 < 20: mousecontrol.updatex(sensitivity)
					elif percent128 < 40: mousecontrol.updatex(sensitivity*2)
					elif percent128 < 60: mousecontrol.updatex(sensitivity*3)
					elif percent128 < 80: mousecontrol.updatex(sensitivity*4)
					elif percent128 < 90: mousecontrol.updatex(sensitivity*5)
					else: mousecontrol.updatex(sensitivity*6)
				else:
					mousecontrol.updatex(0)

			elif action[7] == '03': # Right Joystick up/ down
				if num >= 128:
					if percent254 < 20: mousecontrol.updatey(sensitivity*-1)
					elif percent254 < 40: mousecontrol.updatey((sensitivity*-1)*2)
					elif percent254 < 60: mousecontrol.updatey((sensitivity*-1)*3)
					elif percent254 < 80: mousecontrol.updatey((sensitivity*-1)*4)
					elif percent254 < 90: mousecontrol.updatey((sensitivity*-1)*5)
					else: mousecontrol.updatey((sensitivity*-1)*6)
				elif num <= 127 \
				and num != 0:
					if percent128 < 20: mousecontrol.updatey(sensitivity)
					elif percent128 < 40: mousecontrol.updatey(sensitivity*2)
					elif percent128 < 60: mousecontrol.updatey(sensitivity*3)
					elif percent128 < 80: mousecontrol.updatey(sensitivity*4)
					elif percent128 < 90: mousecontrol.updatey(sensitivity*5)
					else: mousecontrol.updatey(sensitivity*6)
				else:
					mousecontrol.updatey(0)
			action = []
