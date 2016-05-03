PREFIX  = /usr
CFLAGS  ?= -g Wall -Wno-deprecated-declarations -O2
LIBS    = -lSDL2 -lSDL2_image
OBJS    = onboard.o
TARGET  = onboard

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEFINES) -o $@ -c $<

install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	install -p -m 755 $(TARGET) $(PREFIX)/bin

clean:
	rm -f $(OBJS) $(TARGET) *~
