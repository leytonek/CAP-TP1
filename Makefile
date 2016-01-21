CC	:= g++
CFLAGS	:= -O3 -std=c++0x
IFLAGS	:= -I./includes

obj/%.o: src/%.cpp includes/*.hpp
	mkdir -p obj
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

main: src/main.cpp  obj/api.o obj/object.o obj/user.o obj/aes.o
	$(CC) $(IFLAGS) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f obj/* main *~ */*~
