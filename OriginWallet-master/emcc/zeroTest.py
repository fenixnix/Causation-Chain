#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
import hashlib
import base64
import math
import ctypes
import http.client

def init(linuxlib):
	emcc = linuxlib("./libuECC.so")
	emcc.getkey()
	emcc.getPublicKey.restype=ctypes.c_char_p
	emcc.getPrivateKey.restype=ctypes.c_char_p
	emcc.getSign.restype=ctypes.c_char_p
	emcc.verify.restype=ctypes.c_int

	emcc.sign.argtypes = [ctypes.c_char_p,ctypes.c_char_p]
	emcc.verify.argtypes = [ctypes.c_char_p,ctypes.c_char_p,ctypes.c_char_p]

	return emcc

def test(emcc):
	pub = emcc.getPublicKey()
	pri = emcc.getPrivateKey()[0:64]
	print (pub,len(pub))
	print (pri,len(pri))

	msg = hashlib.sha256('hello').hexdigest()
	emcc.sign(pri,msg)
	sig = emcc.getSign()

	print (sig,len(sig))
	print (emcc.verify(pub,msg,sig))

def getBlock(emcc,ptype,pubkey,prikey,name,func,arg):
	data = ptype+"$"+str(pubkey)+"$"+name+"$"+func+"$"+arg
	msg = hashlib.sha256(data.encode("utf8")).hexdigest()
	print (msg)
	#emcc.sign(prikey,msg)
	#sig = emcc.getSign()
    #data = (ptype+"$"+pubkey+"$"+name+"$"+func+"$"+arg)
    #block = sig+'`'+data

linuxlib = ctypes.cdll.LoadLibrary  
emcc=init(linuxlib)
pub = emcc.getPublicKey()
pri = emcc.getPrivateKey()[0:64]
getBlock(emcc,'method',pub,pri,'ex3','getSymbolList','null')
'''
block = '/'+getBlock(emcc,'method',pub,pri,'ex3','getSymbolList','null')

con = http.client.HTTPConnection('118.178.127.35:3000')
con.request("GET", "/listContract")
resu = con.getresponse()
print(resu.status,resu.reason,resu.info())
print (resu.read())
'''