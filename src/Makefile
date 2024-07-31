
CC = gcc


CFLAGS =


TARGET = cpq


LEX_SOURCE = cpq.lex
BISON_SOURCE = cpq.y
C_SOURCE = cpq.c utils.c stringHasher.c synMapper.c labelMapper.c idMapper.c


LEX_OBJECT = lex.yy.c
BISON_OBJECT = cpq.tab.c cpq.tab.h
C_OBJECT = $(C_SOURCE:.c=.o)


$(TARGET): $(LEX_OBJECT) $(BISON_OBJECT) $(C_OBJECT)
	$(CC) $(CFLAGS) -o $(TARGET) $(LEX_OBJECT) $(BISON_OBJECT) $(C_OBJECT)


$(LEX_OBJECT): $(LEX_SOURCE) $(BISON_OBJECT)
	flex $(LEX_SOURCE)


$(BISON_OBJECT): $(BISON_SOURCE)
	bison -d -t $(BISON_SOURCE)


$(C_OBJECT): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET) $(LEX_OBJECT) $(BISON_OBJECT) $(C_OBJECT) cpq.tab.h.gch

# Declare the "clean" target as a phony target
.PHONY: clean
