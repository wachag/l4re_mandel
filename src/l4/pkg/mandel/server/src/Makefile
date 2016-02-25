PKGDIR		?= ../..
L4DIR		?= $(PKGDIR)/../..

TARGET		= mandelserver mandelclient

# list your .c or .cc files here
#SRC_C		= main.c
SRC_CC_mandelserver          = server.cc
SRC_CC_mandelclient          = client.cc

# list requirements of your program here
REQUIRES_LIBS   = libstdc++ libevent l4re_c-util

include $(L4DIR)/mk/prog.mk
