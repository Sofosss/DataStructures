CC = g++
CFLAGS = -std=c++11

PROGRAMS = PartI_A PartI_B PartI_C PartI_D

all: $(PROGRAMS)

%: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)