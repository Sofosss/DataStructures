CC = g++
CFLAGS = -std=c++11

PROGRAMS = PartII_A_AVL PartII_A_RB PartII_B_AVL PartII_B_RB PartII_AB_AVL PartII_AB_RB PartII_C PartII_ABC_AVL PartII_ABC_RB 

all: $(PROGRAMS)

%: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)