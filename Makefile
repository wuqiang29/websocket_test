TARGET = websocket

INCLUDES  += -I/usr/local/include

CFLAGS	 += -lpthread -lwebsockets -lm -lssl

#LDFLAGS := -L$(STAGING_DIR)/usr/lib

#LIBsss			= -lpthread -lwebsockets -lm -lssl

SOURCES := cJSON.c websocket_main.c xun_action.c mesg_com.c server_client.c timer_mana.c


OBJS := $(patsubst %.c,%.o,$(SOURCES))

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -g -o $@ $^  $(INCLUDES) $(LDFLAGS) $(CFLAGS) 
#	echo *************$(CFLAGS)***************
#	echo *************$(LDFLAGS)***************	
$(OBJS):%.o:%.c
	$(CC) -o $@ -c $< $(INCLUDES) $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJS)

